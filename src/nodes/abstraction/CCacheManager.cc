//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "CCacheManager.h"

#include "../messages/ControlPacket_m.h"
#include "../messages/UENotification_m.h"
#include "../../configuration/CGlobalConfiguration.h"

CCacheManager::CCacheManager(INode * pNode, CFileSink * pFileSink, CFileCache * pCache)
: m_pNode {pNode}
, m_pFileSink {pFileSink}
, m_pCache {pCache}
{
    int numFiles = CGlobalConfiguration::getInstance ().get ("numFiles");

    if (m_pNode->getNode ()->getId () == 2)
        do_scheduleNextEnquiry ();
}

bool
operator< (const CCacheManager::ConfirmationData &lhs, const CCacheManager::ConfirmationData &rhs)
{
    return (lhs.quality * lhs.availableBlocks) < (rhs.quality * rhs.availableBlocks);
}

bool
operator== (const CCacheManager::ConfirmationData &lhs, const CCacheManager::ConfirmationData &rhs)
{
    return (lhs.quality * lhs.availableBlocks) == (rhs.quality * rhs.availableBlocks);
}

void
CCacheManager::process (omnetpp::cMessage * pMsg)
{
    if (pMsg->isSelfMessage ())
    {
        do_processSelfMessages (pMsg);
        return;
    }

    ControlPacket *pControlPacket = dynamic_cast <ControlPacket *> (pMsg);

    if (! pControlPacket)
        return;

    switch (pControlPacket->getType())
    {
    case CP_CONFIRM:
        do_processConfirmation (pControlPacket);
        break;
    case CP_BROADCAST:
        do_processBroadcast (pControlPacket);
        break;
    default:
        break;
    }
}

void
CCacheManager::do_processSelfMessages (omnetpp::cMessage * pSelfMessage)
{
    EV_STATICCONTEXT

    UENotification * pNotification = dynamic_cast <UENotification *> (pSelfMessage);

    if (! pNotification)
    {
        EV_WARN << "Not a UE notification. Ignoring" << std::endl;
        return;
    }

    FileId fileId = pNotification->getFileId ();

    switch (pNotification->getType ())
    {
    case N_SCHEDULE:
        do_prepareEnquiry ();
        break;
    case N_TIMEOUT:
        do_processTimeout (fileId);
        break;
    default:
        break;
    }
}

void
CCacheManager::do_processTimeout (FileId fileId)
{
    EV_STATICCONTEXT

    auto it = m_enquires.find (fileId);
    if (it == m_enquires.end ())
    {
        EV_WARN << "Invalid file id. Not really interested in that file. Ignoring..." << std::endl;
        return;
    }

    auto queue = it->second;
    m_pCache->setCacheState(fileId, DOWNLOADING);

    if (queue.empty ())
    {
        m_pFileSink->requestFile (fileId);
    }
    else
    {
        ConfirmationData winner = queue.top ();
        m_pFileSink->requestFile (fileId, winner.nodeId);
    }

    m_enquires.erase (fileId);
}

void
CCacheManager::do_prepareEnquiry ()
{
    EV_STATICCONTEXT

    if (m_pFileSink->isDownloading ())
    {
        do_scheduleNextEnquiry ();
        return;
    }

    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    std::vector <FileId> candidates;
    for (auto & entry : *m_pCache)
        if (entry.second.state == NOTPRESENT) candidates.push_back (entry.first);

    if (candidates.empty ())
        return;

    std::size_t winner = random->intRand (candidates.size ());
    FileId fileId = candidates [winner];

    m_pCache->setCacheState (fileId, ENQUIRY);
    m_enquires [fileId] = {};

    ControlPacket * pResponse = new ControlPacket ();
    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (-1);
    pResponse->setType (CP_BROADCAST);

    BroadcastControlPacket * pBroadcast = new BroadcastControlPacket ();
    pBroadcast->setFileId (fileId);
    pBroadcast->setStartBlockId (0); // TODO: check first missing block

    pResponse->encapsulate (pBroadcast);

    m_pNode->sendBroadcast(pResponse);

    EV << "Sending broadcast for file " << fileId << std::endl;

    do_scheduleTimeout (fileId);
    do_scheduleNextEnquiry ();
}

void
CCacheManager::do_scheduleNextEnquiry ()
{
    UENotification * pNotification = new UENotification ();
    pNotification->setType (N_SCHEDULE);

    // TODO: make it random
    m_pNode->sendInternal (pNotification, 10);
}

void
CCacheManager::do_scheduleTimeout (FileId fileId)
{
    static double timeout = CGlobalConfiguration::getInstance ().get ("D2DEnquiryTimeout");

    UENotification * pNotification = new UENotification ();
    pNotification->setType (N_TIMEOUT);
    pNotification->setFileId (fileId);

    m_pNode->sendInternal (pNotification, timeout);
}

void
CCacheManager::do_recalculatePriorities ()
{
    m_pCache->recalculatePriorities ();
}

void
CCacheManager::do_processConfirmation (ControlPacket * pControlPacket)
{
    EV_STATICCONTEXT

    auto sId = pControlPacket->getSourceId ();
    auto dId = pControlPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pControlPacket->decapsulate ();

    if (! pPacket)
    {
        EV_WARN << "Could not decapsulate packet" << std::endl;
        return;
    }

    ConfirmationControlPacket * pConfirmation = dynamic_cast <ConfirmationControlPacket *> (pPacket);

    if (! pConfirmation)
    {
        EV_WARN << "Not a confirmation packet. Something bad happened." << std::endl;
        return;
    }

    FileId fileId = pConfirmation->getFileId ();
    std::size_t blockId = pConfirmation->getStartBlockId ();
    std::size_t numBlocks = pConfirmation->getNumBlocks ();

    auto it = m_enquires.find (fileId);
    if (it == m_enquires.end ())
    {
        EV_WARN << "Did not request a discovery/query on the file. Ignoring..." << std::endl;
        return;
    }

    ConfirmationData confirmation;
    confirmation.nodeId = sId;
    confirmation.quality = 1;
    confirmation.availableBlocks = numBlocks;

    it->second.push (confirmation);
    EV << "Saved confirmation from node " << sId << "for analysis" << std::endl;
}

void
CCacheManager::do_processBroadcast (ControlPacket * pControlPacket)
{
    EV_STATICCONTEXT

    auto sId = pControlPacket->getSourceId ();
    auto dId = pControlPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pControlPacket->decapsulate ();

    if (! pPacket)
    {
        EV_WARN << "Could not decapsulate packet" << std::endl;
        return;
    }

    BroadcastControlPacket * pBroadcast = dynamic_cast <BroadcastControlPacket *> (pPacket);

    if (! pBroadcast)
    {
        EV_WARN << "Not a broadcast packet. Something bad happened" << std::endl;
        return;
    }

    FileId fileId = pBroadcast->getFileId ();
    int startBlockId = pBroadcast->getStartBlockId ();

    int available = m_pCache->getAvailability (fileId, startBlockId);
    if (available <= 0)
        return;

    ControlPacket * pResponse = new ControlPacket ();
    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (sId);
    pResponse->setType (CP_CONFIRM);

    ConfirmationControlPacket * pConfirmation = new ConfirmationControlPacket ();
    pConfirmation->setFileId (fileId);
    pConfirmation->setStartBlockId (startBlockId);
    pConfirmation->setNumBlocks (available);

    pResponse->encapsulate (pConfirmation);
    m_pNode->sendOut (pResponse, sId);
}
