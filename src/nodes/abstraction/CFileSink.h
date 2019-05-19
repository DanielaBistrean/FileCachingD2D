#ifndef D2D_CFILESINK_H
#define D2D_CFILESINK_H 1

#include <omnetpp.h>

#include "INode.h"
#include "IProcessor.h"
#include "CCacheManager.h"

#include "../messages/DataPacket_m.h"

class CCacheManager;

class CFileSink : public IProcessor
{
public:
    CFileSink (INode * pNode, CCacheManager * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

public:
    void requestFile (FileId fileId, int nodeId = -1);

    bool isDownloading () { return m_bDownloading; };

private:
    void do_processData  (DataPacket * pDataPacket);
    void do_processEOF   (DataPacket * pDataPacket);
    void do_processError (DataPacket * pDataPacket);

    void do_sendFileRequest (FileId fileId, int destId, int startBlockId);
    void do_sendFileFeedback (FileId fileId, int destId, int blockId, bool ack, int nextBlockId);

private:
    INode * m_pNode;
    CCacheManager * m_pCache;

    bool m_bDownloading;
};

#endif // D2D_CFILESINK_H
