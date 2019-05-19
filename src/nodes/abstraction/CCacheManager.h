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
#include "../transfer/CFileCache.h"

#include "INode.h"
#include "IProcessor.h"

#include "CFileSink.h"

struct FileInfo
{
    std::size_t bytes;
    std::size_t blocks;
    std::vector <std::size_t> availableBlocks;
};

class CCacheManager : public IProcessor
{
public:
    CCacheManager(INode * pNode, CFileSink * pFileSink, CFileCache * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

public:
    bool selectFileForDownload (FileId &fileId);
    bool getFileInfo (FileId fileId, FileInfo &fileInfo);

    CacheState getCacheState (FileId fileId);
    void setCacheState (FileId fileId, CacheState state);

private:
    void do_processSelfMessages (omnetpp::cMessage * pSelfMessage);

private:
    void do_recalculatePriorities ();

private:
    INode * m_pNode;
    CFileSink * m_pFileSink;

    CFileCache * m_pCache;
};

#endif /* NODES_ABSTRACTION_CCACHEMANAGER_H_ */
