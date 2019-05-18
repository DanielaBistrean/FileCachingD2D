#ifndef D2D_INODE_H
#define D2D_INODE_H 1

#include <omnetpp.h>

struct INode
{
    virtual void sendInternal  (omnetpp::cMessage * pMsg, omnetpp::simtime_t offset);
    virtual void sendOut       (omnetpp::cMessage * pMsg, int nodeId);
    virtual void sendBroadcast (omnetpp::cMessage * pMsg);

    virtual omnetpp::cSimpleModule * getNode () = 0;
};

#endif // D2D_INODE_H
