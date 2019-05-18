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

class CCacheManager : public IProcessor
{
public:
    CCacheManager(INode * pNode, CFileSink * pFileSink, CFileCache * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_processConfirmation (ControlPacket * pDataPacket);
    void do_processBroadcast    (ControlPacket * pDataPacket);
    void do_processSelfMessages (omnetpp::cMessage * pSelfMessage);

    void do_processTimeout (FileId fileId);
    void do_processRequest (FileId fileId);

    void do_prepareEnquiry ();

    void do_scheduleNextEnquiry ();
    void do_scheduleTimeout (FileId fileId);

private:
    struct ConfirmationData
    {
        int nodeId;
        double quality;
        int availableBlocks;
    };

    friend bool operator<  (const ConfirmationData &lhs, const ConfirmationData &rhs);
    friend bool operator== (const ConfirmationData &lhs, const ConfirmationData &rhs);

    void do_recalculatePriorities ();

private:
    INode * m_pNode;
    CFileSink * m_pFileSink;

    bool m_bDownloading;

    CFileCache * m_pCache;
    std::map <FileId, std::priority_queue <ConfirmationData> > m_enquires;
};

#endif /* NODES_ABSTRACTION_CCACHEMANAGER_H_ */
