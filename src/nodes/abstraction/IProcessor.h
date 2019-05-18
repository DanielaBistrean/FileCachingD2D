#ifndef D2D_IPROCESSOR_H
#define D2D_IPROCESSOR_H 1

#include <omnetpp.h>

struct IProcessor
{
    virtual void process (omnetpp::cMessage * pMsg) = 0;
};

#endif // D2D_IPROCESSOR_H
