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

#include "CNetworkManager.h"

#include "../../configuration/CGlobalConfiguration.h"
#include "../messages/UENotification_m.h"

CNetworkManager::CNetworkManager (INode * pNode, CFileSink * pFileSink, CCacheManager * pCacheManager)
: m_pNode         {pNode}
, m_pFileSink     {pFileSink}
, m_pCacheManager {pCacheManager}
{}

void
CNetworkManager::process (omnetpp::cMessage * pMsg)
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
CNetworkManager::do_processSelfMessages (omnetpp::cMessage * pSelfMessage)
{
    UENotification * pNotification = dynamic_cast <UENotification *> (pSelfMessage);

    if (! pNotification)
        return;

    FileId fileId = pNotification->getFileId ();

    switch (pNotification->getType ())
    {
    case N_TIMEOUT:
        do_processTimeout (fileId);
        break;
    case N_REQUERY:
        do_processRequery (fileId);
    default:
        break;
    }
}

void
CNetworkManager::do_processRequery (FileId fileId)
{
    requestFile (fileId);
}

void
CNetworkManager::do_processConfirmation (ControlPacket * pControlPacket)
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
    EV << "Saved confirmation from node " << sId << " for analysis" << std::endl;
}

void
CNetworkManager::do_processBroadcast (ControlPacket * pControlPacket)
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

    FileInfo fileInfo;
    bool bAvailable = m_pCacheManager->getFileInfo (fileId, fileInfo);

    if (! bAvailable)
        return;

    auto it = std::find (fileInfo.availableBlocks.begin(), fileInfo.availableBlocks.end(), startBlockId);
    if (it == fileInfo.availableBlocks.end ())
        return;

    ControlPacket * pResponse = new ControlPacket ();
    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (sId);
    pResponse->setType (CP_CONFIRM);

    ConfirmationControlPacket * pConfirmation = new ConfirmationControlPacket ();
    pConfirmation->setFileId (fileId);
    pConfirmation->setStartBlockId (fileInfo.availableBlocks [0]);
    pConfirmation->setNumBlocks (fileInfo.availableBlocks.size ());

    pResponse->encapsulate (pConfirmation);
    m_pNode->sendOut (pResponse, sId);
}

void
CNetworkManager::requestFile (FileId fileId)
{
    EV_STATICCONTEXT

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
}

void
CNetworkManager::do_processTimeout (FileId fileId)
{
    EV_STATICCONTEXT

    EV << " -------------> processing timeout for file" << fileId << std::endl;

    auto it = m_enquires.find (fileId);
    if (it == m_enquires.end ())
    {
        EV_WARN << "Invalid file id. Not really interested in that file. Ignoring..." << std::endl;
        return;
    }

    auto queue = it->second;
    m_pCacheManager->setCacheState (fileId, DOWNLOADING);

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
CNetworkManager::do_scheduleTimeout (FileId fileId)
{
    static double timeout = CGlobalConfiguration::getInstance ().get ("D2DEnquiryTimeout");

    UENotification * pNotification = new UENotification ();
    pNotification->setType (N_TIMEOUT);
    pNotification->setFileId (fileId);

    m_pNode->sendInternal (pNotification, timeout);
}

bool
operator< (const CNetworkManager::ConfirmationData &lhs, const CNetworkManager::ConfirmationData &rhs)
{
    return (lhs.quality * lhs.availableBlocks) < (rhs.quality * rhs.availableBlocks);
}

bool
operator== (const CNetworkManager::ConfirmationData &lhs, const CNetworkManager::ConfirmationData &rhs)
{
    return (lhs.quality * lhs.availableBlocks) == (rhs.quality * rhs.availableBlocks);
}
