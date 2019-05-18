#include "INode.h"

#include "NetworkAbstraction.h"

void INode::sendInternal (omnetpp::cMessage * pMsg, omnetpp::simtime_t offset)
{
    getNode ()->scheduleAt (simTime () + offset, pMsg);
}

void INode::sendOut (omnetpp::cMessage * pMsg, int nodeId)
{
    cGate * out = NetworkAbstraction::getInstance ().getNodeGate (nodeId);
    getNode ()->sendDirect(pMsg, out);
}

void INode::sendBroadcast (omnetpp::cMessage * pMsg)
{
    auto gates = NetworkAbstraction::getInstance ().getBroadcastGates (getNode ()->getId ());

    for (auto & gate : gates)
    {
        cMessage *tmp = pMsg->dup ();
        getNode ()->sendDirect (tmp, gate);
    }

    delete pMsg;
}
