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

#ifndef NODES_ABSTRACTION_CCACHEMANAGER_H_
#define NODES_ABSTRACTION_CCACHEMANAGER_H_

#include <vector>
#include <queue>
#include <algorithm>

#include "../messages/ControlPacket_m.h"
#include "../transfer/CFileStore.h"
#include "INode.h"
#include "IProcessor.h"

struct FileInfo
{
    std::size_t bytes;
    std::size_t blocks;
    std::vector <std::size_t> availableBlocks;
};

enum CacheState
{
    ERROR = -2,
    ENQUIRY = -1,
    NOTPRESENT = 0,
    DOWNLOADING = 1,
    FULL = 2
};

struct CacheData
{
    CacheState state;
    int score;

    CacheData (bool available = false)
    : state {available ? FULL : NOTPRESENT}, score {1} {};

    friend bool operator<  (const CacheData &lhs, const CacheData &rhs);
    friend bool operator== (const CacheData &lhs, const CacheData &rhs);
};

using CacheEntry = std::pair <FileId, CacheData>;
bool operator<  (const CacheEntry &lhs, const CacheEntry &rhs);
bool operator== (const CacheEntry &lhs, const CacheEntry &rhs);

class CCacheManager : public IProcessor
{
public:
    CCacheManager(INode * pNode, CFileStore * pStore);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

public:
    bool selectFileForDownload (FileId &fileId);
    bool getFileInfo (FileId fileId, FileInfo &fileInfo);

    void setFileData  (FileId fileId, int blockId);
    bool getFileData  (FileId fileId, int blockId);
    bool isValidFile  (FileId fileId);
    bool isValidBlock (FileId fileId, int blockId);

    int  getNumBlocks (FileId fileId);

    CacheState getCacheState (FileId fileId);
    void setCacheState (FileId fileId, CacheState state);

private:
    void do_processSelfMessages (omnetpp::cMessage * pSelfMessage);

private:
    void do_recalculatePriorities ();

    int do_findCacheEntry (FileId fileId);

private:
    INode * m_pNode;
    CFileStore * m_pStore;

    std::vector <CacheEntry> m_cache;
};

#endif /* NODES_ABSTRACTION_CCACHEMANAGER_H_ */
