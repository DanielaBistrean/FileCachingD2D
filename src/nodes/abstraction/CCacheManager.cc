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

#include "../messages/UENotification_m.h"
#include "../../configuration/CGlobalConfiguration.h"

CCacheManager::CCacheManager(INode * pNode, CFileStore * pStore)
: m_pNode {pNode}
, m_pStore {pStore}
{
    for (auto & file : *m_pStore)
        m_cache.push_back ({file.first, CacheData (file.second.available () == file.second.blocks ())});
}

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
    UENotification * pNotification = dynamic_cast <UENotification *> (pSelfMessage);

    if (! pNotification)
        return;

    FileId fileId = pNotification->getFileId ();

    switch (pNotification->getType ())
    {
    case N_RECACHE:
        do_computeCache ();
        break;
    default:
        break;
    }
}

void
CCacheManager::do_computeCache ()
{
    EV_STATICCONTEXT
    do_recalculatePriorities ();

    static double cacheLimit = CGlobalConfiguration::getInstance ().get ("cacheLimit");
    std::size_t limit = m_cache.size () * (1 - cacheLimit);

    EV << "----------------------------------------------------------------------\n";
    EV << '\n';
    EV << "    RECACHING IN PROGRESS WITH LIMIT: " << limit << "\n";
    EV << '\n';
    EV << "----------------------------------------------------------------------\n";
    EV << '\n';
    for (std::size_t i = 0; i < m_cache.size (); ++i)
    {
        double score = m_cache [i].second.score * (float) m_cache [i].second.state;
        EV << "    " << m_cache [i].first << "\t[" << score << "] => SCORE: " << m_cache [i].second.score << "; STATE=" << m_cache [i].second.state << '\n';
    }
    EV << '\n';
    for (std::size_t i = 0; i < limit; ++i)
    {
        m_cache [i].second.state = NOTPRESENT;
        m_pStore->removeFile (m_cache [i].first);
    }
    EV << '\n';
    for (std::size_t i = 0; i < m_cache.size (); ++i)
    {
        double score = m_cache [i].second.score * (float) m_cache [i].second.state;
        EV << "    " << m_cache [i].first << "\t[" << score << "] => SCORE: " << m_cache [i].second.score << "; STATE=" << m_cache [i].second.state << '\n';
    }
    EV << '\n';
    EV << '\n';
    EV << '\n';
    EV << "----------------------------------------------------------------------\n";
}

bool
CCacheManager::selectFileForDownload (FileId &fileId)
{
//    EV_STATICCONTEXT

//    if (m_pFileSink->isDownloading ())
//        return false;

    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    std::vector <FileId> candidates;
    for (auto & entry : m_cache)
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
    // Perform normalization of scores
//    float maxScore = 0.0f;
//    for (auto & entry : m_cache)
//        if (entry.second.score > maxScore) maxScore = entry.second.score;
//
//    for (auto & entry : m_cache)
//        entry.second.score /= maxScore;

    std::sort (m_cache.begin(), m_cache.end (), sortEntries);
}

bool
CCacheManager::getFileInfo (FileId fileId, FileInfo &fileInfo)
{
    FileInfo info;

    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return false;

    for (std::size_t i = 0; i < it->second.blocks (); ++i)
        if (it->second.hasBlock (i)) info.availableBlocks.push_back (i);

    info.blocks = it->second.blocks ();
    info.bytes = it->second.size ();

    fileInfo = info;
    return true;
}

CacheState
CCacheManager::getCacheState (FileId fileId)
{
    std::size_t idx = do_findCacheEntry (fileId);
    if (idx < 0)
        return ERROR;

    return m_cache [idx].second.state;
}

void
CCacheManager::setCacheState (FileId fileId, CacheState state)
{
    std::size_t idx = do_findCacheEntry (fileId);
    if (idx < 0)
        return;

    m_cache [idx].second.state = state;
}

void
CCacheManager::setFileData (FileId fileId, int blockId)
{
    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return;

    it->second.setBlock (blockId);
}

bool
CCacheManager::getFileData (FileId fileId, int blockId)
{
    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return false;

    return it->second.hasBlock (blockId);
}

bool
CCacheManager::isValidFile (FileId fileId)
{
    auto it = m_pStore->find (fileId);
    return (it != m_pStore->end ());
}

bool
CCacheManager::isValidBlock (FileId fileId, int blockId)
{
    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return false;

    return (it->second.blocks() > blockId);
}

int
CCacheManager::getNumBlocks (FileId fileId)
{
    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return -1;

    return it->second.blocks();
}

int
CCacheManager::getFirstMissingBlock (FileId fileId, int startBlockId)
{
    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return -1;

    for (std::size_t i = startBlockId; i < it->second.blocks (); ++i)
        if (! it->second.hasBlock (i)) return i;

    return -1;
}

void
CCacheManager::updateCacheCounter (FileId fileId)
{
    std::size_t idx = do_findCacheEntry (fileId);
    if (idx < 0)
        return;

    m_cache [idx].second.score ++;
}

int
CCacheManager::do_findCacheEntry (FileId fileId)
{
    for (std::size_t i = 0; i < m_cache.size (); ++i)
        if (m_cache [i].first == fileId) return i;

    return -1;
}
