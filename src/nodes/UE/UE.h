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

#ifndef __FILECACHINGD2D_UE_H_
#define __FILECACHINGD2D_UE_H_

#include <omnetpp.h>

#include <memory>

#include "../../messages/DataPacket_m.h"
#include "../../transfer/CFileCache.h"

#include "../abstraction/INode.h"

#include "CMobility.h"

using namespace omnetpp;

class UE : public cSimpleModule, public INode
{
public:
    virtual void sendInternal   (omnetpp::cMessage * pMsg, omnetpp::simtime_t offset) override;
    virtual void sendOut (omnetpp::cMessage * pMsg, int nodeId) override;

    virtual omnetpp::cDisplayString& getDisplay () override;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

private:
    CFileCache m_cache;

    std::unique_ptr <CMobility> m_pMobility;
};

#endif
