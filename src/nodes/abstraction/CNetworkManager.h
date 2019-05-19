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

#ifndef NODES_ABSTRACTION_CNETWORKMANAGER_H_
#define NODES_ABSTRACTION_CNETWORKMANAGER_H_

#include <vector>
#include <queue>
#include <algorithm>

#include "INode.h"
#include "IProcessor.h"

#include "../messages/ControlPacket_m.h"
#include "../abstraction/CCacheManager.h"
#include "../abstraction/CFileSink.h"

class CNetworkManager : public IProcessor
{
public:
    CNetworkManager(INode * pNode, CFileSink * pFileSink, CCacheManager * pCacheManager);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_processSelfMessages (omnetpp::cMessage * pSelfMessage);

    void do_processConfirmation (ControlPacket * pDataPacket);
    void do_processBroadcast    (ControlPacket * pDataPacket);

    void do_scheduleTimeout (FileId fileId);
    void do_processTimeout (FileId fileId);

public:
    void requestFile (FileId fileId);

private:
    struct ConfirmationData
    {
        int nodeId;
        double quality;
        int availableBlocks;
    };

    friend bool operator<  (const ConfirmationData &lhs, const ConfirmationData &rhs);
    friend bool operator== (const ConfirmationData &lhs, const ConfirmationData &rhs);


private:
    INode * m_pNode;
    CFileSink * m_pFileSink;
    CCacheManager * m_pCacheManager;

    std::map <FileId, std::priority_queue <ConfirmationData> > m_enquires;
};

#endif /* NODES_ABSTRACTION_CNETWORKMANAGER_H_ */
