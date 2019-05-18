#ifndef D2D_CFILESOURCE_H
#define D2D_CFILESOURCE_H 1

#include "../transfer/CFileCache.h"
#include "../messages/DataPacket_m.h"

#include "INode.h"
#include "IProcessor.h"

#include <omnetpp.h>

class CFileSource : public IProcessor
{
public:
    CFileSource (INode * pNode, CFileCache * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_processRequest  (DataPacket * pDataPacket);
    void do_processFeedback (DataPacket * pDataPacket);

private:
    INode * m_pNode;
    CFileCache * m_pCache;
};

#endif // D2D_CFILESOURCE_H
