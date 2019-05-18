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

#include "UE.h"

Define_Module(UE);

#include "../abstraction/NetworkAbstraction.h"

void UE::initialize()
{
    NetworkAbstraction::getInstance ().registerUser (this);

    m_pMobility = std::unique_ptr <CMobility> (new CMobility (this));
}

void UE::handleMessage(cMessage *msg)
{
    m_pMobility->process (msg);
}

void UE::finish()
{
    NetworkAbstraction::getInstance ().deregisterUser (this);
}

void UE::sendInternal (omnetpp::cMessage * pMsg, omnetpp::simtime_t offset)
{
    scheduleAt (simTime () + offset, pMsg);
}

void UE::sendOut (omnetpp::cMessage * pMsg, int nodeId)
{
    cGate * out = nullptr;

    if (nodeId == -1)
        out = NetworkAbstraction::getInstance ().getBaseGate ();
    else
        out = NetworkAbstraction::getInstance ().getUserGate (nodeId);

    if (! out)
        return;

    sendDirect(pMsg, out);
}

omnetpp::cDisplayString& UE::getDisplay ()
{
    return getDisplayString();
}
