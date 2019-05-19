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

#ifndef NODES_UE_CMAINLOOP_H_
#define NODES_UE_CMAINLOOP_H_

#include "../abstraction/INode.h"
#include "../abstraction/IProcessor.h"
#include "../abstraction/CCacheManager.h"
#include "../abstraction/CNetworkManager.h"

class CMainLoop : public IProcessor
{
public:
    CMainLoop(INode * pNode, CCacheManager * pCacheManager, CNetworkManager * pNetworkManager);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_attemptFileDownload ();
    void do_scheduleNextAttempt ();

    void do_scheduleNextRecache ();


private:
    INode * m_pNode;
    CCacheManager * m_pCacheManager;
    CNetworkManager * m_pNetworkManager;
};

#endif /* NODES_UE_CMAINLOOP_H_ */
