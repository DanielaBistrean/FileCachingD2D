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
{}

void
CCacheManager::process (omnetpp::cMessage * pMsg)
{
    if (pMsg->isSelfMessage ())
    {
        do_processSelfMessages (pMsg);
        return;
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
    default:
        break;
    }
}

bool
CCacheManager::selectFileForDownload (FileId &fileId)
{
    EV_STATICCONTEXT

    if (m_pFileSink->isDownloading ())
        return false;

    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    std::vector <FileId> candidates;
    for (auto & entry : *m_pCache)
        if (entry.second.state == NOTPRESENT) candidates.push_back (entry.first);

    if (candidates.empty ())
        return false;

    std::size_t winner = random->intRand (candidates.size ());
    fileId = candidates [winner];

    setCacheState (fileId, ENQUIRY);
    return true;
}

void
CCacheManager::do_recalculatePriorities ()
{
    m_pCache->recalculatePriorities ();
}

bool
CCacheManager::getFileInfo (FileId fileId, FileInfo &fileInfo)
{
    FileInfo info;

    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return false;

    for (std::size_t i = 0; i < it->second.file.blocks (); ++i)
        if (it->second.file.hasBlock (i)) info.availableBlocks.push_back (i);

    info.blocks = it->second.file.blocks ();
    info.bytes = it->second.file.size ();

    fileInfo = info;
    return true;
}

CacheState
CCacheManager::getCacheState (FileId fileId)
{
    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return ERROR;

    return it->second.state;
}

void
CCacheManager::setCacheState (FileId fileId, CacheState state)
{
    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return;

    it->second.state = state;
}
