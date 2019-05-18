#ifndef D2D_INODE_H
#define D2D_INODE_H 1

#include <omnetpp.h>

struct INode
{
    virtual void sendInternal   (omnetpp::cMessage * pMsg, omnetpp::simtime_t offset) = 0;
    virtual void sendUpperLayer (omnetpp::cMessage * pMsg) = 0;
    virtual void sendLowerLayer (omnetpp::cMessage * pMsg) = 0;

    virtual omnetpp::cDisplayString& getDisplay () = 0;
};

#endif // D2D_INODE_H
