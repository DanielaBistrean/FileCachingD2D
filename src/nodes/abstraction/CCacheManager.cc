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

#include "../messages/DataPacket_m.h"
#include "../messages/UENotification_m.h"
#include "../../configuration/CGlobalConfiguration.h"

CCacheManager::CCacheManager(INode * pNode, CFileSink * pFileSink)
: m_pNode {pNode}
, m_pFileSink {pFileSink}
, m_bDownloading {false}
{
    int numFiles = CGlobalConfiguration::getInstance ().get ("numFiles");

    for (std::size_t id = 0; id < numFiles; ++id)
    {
        m_files.push_back ({id, CacheData ()});
    }

    do_scheduleNextRequest ();
}

bool
operator< (const CCacheManager::CacheEntry &lhs, const CCacheManager::CacheEntry &rhs)
{
    return (lhs.second.score * lhs.second.status) < (rhs.second.score * lhs.second.status);
}

bool
operator== (const CCacheManager::CacheEntry &lhs, const CCacheManager::CacheEntry &rhs)
{
    return (lhs.second.score * lhs.second.status) < (rhs.second.score * lhs.second.status);
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

    DataPacket *pDataPacket = dynamic_cast <DataPacket *> (pMsg);

    if (! pDataPacket)
        return;

    switch (pDataPacket->getType())
    {
    case DP_CONFIRM:
        do_processConfirmation (pDataPacket);
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
    case N_REQUEST:
        do_processRequest ();
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
    do_setCacheState(fileId, DOWNLOADING);
    m_bDownloading = true;

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
CCacheManager::do_processRequest ()
{
    if (m_bDownloading)
    {
        do_scheduleNextRequest ();
        return;
    }

    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    std::vector <FileId> candidates;
    for (auto & entry : m_files)
        if (entry.second.status == NOTPRESENT) candidates.push_back (entry.first);

    if (candidates.empty ())
        return;

    std::size_t winner = random->intRand (candidates.size ());
    FileId fileId = candidates [winner];

    do_setCacheState (fileId, ENQUIRY);
    m_enquires [fileId] = {};

    DataPacket * pResponse = new DataPacket ();
    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (-1);
    pResponse->setType (DP_BROADCAST);

    RequestDataPacket * pRequest = new RequestDataPacket ();
    pRequest->setFileId (fileId);
    pRequest->setStartBlockId (0); // TODO: check first missing block

    pResponse->encapsulate (pRequest);

    m_pNode->sendBroadcast(pResponse);

    do_scheduleTimeout (fileId);
    do_scheduleNextRequest ();
}

void
CCacheManager::do_scheduleNextRequest ()
{
    UENotification * pNotification = new UENotification ();
    pNotification->setType (N_REQUEST);

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

CCacheManager::CacheStatus
CCacheManager::do_getCacheState (FileId fileId)
{
    for (auto entry : m_files)
        if (entry.first == fileId) return entry.second.status;

    return ERROR;
}

void
CCacheManager::do_setCacheState (FileId fileId, CCacheManager::CacheStatus status)
{
    for (auto entry : m_files)
        if (entry.first == fileId) { entry.second.status = status; return; }
}

void
CCacheManager::do_recalculatePriorities ()
{
    std::sort(m_files.begin(), m_files.end());
}

void
CCacheManager::do_processConfirmation (DataPacket * pDataPacket)
{
    EV_STATICCONTEXT

    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();

    if (! pPacket)
    {
        EV_WARN << "Could not decapsulate packet" << std::endl;
        return;
    }

    ConfirmationDataPacket * pConfirmation = dynamic_cast <ConfirmationDataPacket *> (pPacket);

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
}
