#ifndef D2D_CFILESOURCE_H
#define D2D_CFILESOURCE_H 1

#include "../messages/DataPacket_m.h"

#include "INode.h"
#include "IProcessor.h"

#include <omnetpp.h>
#include "../transfer/CFileStore.h"

class CFileSource : public IProcessor
{
public:
    CFileSource (INode * pNode, CFileStore * pCache);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

private:
    void do_processRequest  (DataPacket * pDataPacket);
    void do_processFeedback (DataPacket * pDataPacket);

    void do_respondWithError (FileId fileId, int destId, int error);
    void do_respondWithData (FileId fileId, int destId, int blockId);
    void do_respondWithEOF (FileId, int destId);

private:
    INode * m_pNode;
    CFileStore * m_pStore;
};

#endif // D2D_CFILESOURCE_H
