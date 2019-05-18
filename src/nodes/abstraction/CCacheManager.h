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

#include "../messages/DataPacket_m.h"
#include "../transfer/CFileCache.h"

#include "INode.h"
#include "IProcessor.h"

#include "CFileSink.h"

class CCacheManager : public IProcessor
{
public:
    CCacheManager(INode * pNode, CFileSink * pFileSink);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_processConfirmation (DataPacket * pDataPacket);
    void do_processSelfMessages (omnetpp::cMessage * pSelfMessage);

    void do_processTimeout (FileId fileId);
    void do_processRequest ();

    void do_scheduleNextRequest ();
    void do_scheduleTimeout (FileId fileId);

private:
    enum CacheStatus
    {
        ERROR = -2,
        ENQUIRY = -1,
        NOTPRESENT = 0,
        DOWNLOADING = 1,
        FULL = 2
    };

    struct CacheData
    {
        CacheStatus status;
        int score;

        CacheData () : status {NOTPRESENT}, score {0} {};
    };

    struct ConfirmationData
    {
        int nodeId;
        double quality;
        int availableBlocks;
    };

    using CacheEntry = std::pair <FileId, CacheData>;

    friend bool operator<  (const CacheEntry &lhs, const CacheEntry &rhs);
    friend bool operator== (const CacheEntry &lhs, const CacheEntry &rhs);

    friend bool operator<  (const ConfirmationData &lhs, const ConfirmationData &rhs);
    friend bool operator== (const ConfirmationData &lhs, const ConfirmationData &rhs);

    CacheStatus do_getCacheState (FileId fileId);
    void do_setCacheState (FileId fileId, CacheStatus status);

    void do_recalculatePriorities ();

private:
    INode * m_pNode;
    CFileSink * m_pFileSink;

    bool m_bDownloading;

    std::vector <CacheEntry> m_files;
    std::map <FileId, std::priority_queue <ConfirmationData> > m_enquires;
};

#endif /* NODES_ABSTRACTION_CCACHEMANAGER_H_ */
