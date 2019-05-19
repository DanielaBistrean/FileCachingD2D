#ifndef D2D_CFILESINK_H
#define D2D_CFILESINK_H 1

#include "../transfer/CFileCache.h"
#include "../messages/DataPacket_m.h"

#include "INode.h"
#include "IProcessor.h"

#include <omnetpp.h>

class CFileSink : public IProcessor
{
public:
    CFileSink (INode * pNode, CFileCache * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

public:
    void requestFile (FileId fileId, int nodeId = -1);

    bool isDownloading () { return m_bDownloading; };

private:
    void do_processData  (DataPacket * pDataPacket);
    void do_processEOF   (DataPacket * pDataPacket);
    void do_processError (DataPacket * pDataPacket);

private:
    INode * m_pNode;
    CFileCache * m_pCache;

    bool m_bDownloading;
};

#endif // D2D_CFILESINK_H
