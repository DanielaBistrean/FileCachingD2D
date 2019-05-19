#include "CFileSink.h"

#include "NetworkAbstraction.h"
#include "../messages/UENotification_m.h"

CFileSink::CFileSink (INode * pNode, CCacheManager * pCache)
: m_pNode  {pNode}
, m_pCache {pCache}
, m_bDownloading {false}
, m_bRequery {false}
{}

void
CFileSink::process (omnetpp::cMessage * pMsg)
{
    DataPacket *pDataPacket = dynamic_cast <DataPacket *> (pMsg);

    if (! pDataPacket)
        return;

    switch (pDataPacket->getType())
    {
    case DP_DATA:
        do_processData (pDataPacket);
        break;
    case DP_EOF:
        do_processEOF (pDataPacket);
        break;
    case DP_ERROR:
        do_processError (pDataPacket);
    default:
        break;
    }
}

void
CFileSink::requestFile (FileId fileId, int nodeId)
{
    if (m_bDownloading && ! m_bRequery)
        return;

    if (nodeId == -1)
        nodeId = NetworkAbstraction::getInstance ().getBaseId ();

    m_pCache->setCacheState (fileId, DOWNLOADING);

    m_bDownloading = true;
    m_bRequery = false;

    do_sendFileRequest(fileId, nodeId, 0);
}

void
CFileSink::do_processData  (DataPacket * pDataPacket)
{
    EV_STATICCONTEXT

    if (! m_bDownloading)
        return;

    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    EV << "Processing payload from node " << sId << std::endl;

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    FileDataPacket * pData = dynamic_cast <FileDataPacket *> (pPacket);

    if (! pData)
        return;

    int fileId = pData->getFileId ();
    int blockId = pData->getBlockId ();

    // TODO: handle errors
    if (! m_pCache->isValidFile (fileId))
    {
        EV_WARN << "File not found!" << std::endl;
        return;
    }

    if (! m_pCache->isValidBlock (fileId, blockId))
    {
        EV_WARN << "Block " << blockId << " outside of file range " << std::endl;
        return;
    }

    m_pCache->setFileData(fileId, blockId);
    int nextBlockId = m_pCache->getFirstMissingBlock(fileId);

    do_sendFileFeedback(fileId, sId, blockId, true, nextBlockId);
}

void
CFileSink::do_sendFileRequest (FileId fileId, int destId, int startBlockId)
{
    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (m_pNode->getNode ()->getId());
    pResponse->setDestinationId (destId);
    pResponse->setType (DP_REQ);

    RequestDataPacket * pRequestDataPacket = new RequestDataPacket ();

    pRequestDataPacket->setFileId (fileId);
    pRequestDataPacket->setStartBlockId (startBlockId);

    pResponse->encapsulate (pRequestDataPacket);

    m_pNode->sendOut (pResponse, destId);
}

void
CFileSink::do_sendFileFeedback (FileId fileId, int destId, int blockId, bool ack, int nextBlockId)
{
    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (m_pNode->getNode ()->getId());
    pResponse->setDestinationId (destId);
    pResponse->setType (DP_FEEDBACK);

    FeedbackDataPacket * pFeedbackDataPacket = new FeedbackDataPacket ();

    pFeedbackDataPacket->setFileId (fileId);
    pFeedbackDataPacket->setBlockId (blockId);
    pFeedbackDataPacket->setAck (ack);
    pFeedbackDataPacket->setNextBlockId (nextBlockId);

    pResponse->encapsulate (pFeedbackDataPacket);

    m_pNode->sendOut (pResponse, destId);
}

void
CFileSink::do_processEOF (DataPacket * pDataPacket)
{
    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    EOFDataPacket * pEOF = dynamic_cast <EOFDataPacket *> (pPacket);

    if (! pEOF)
        return;

    m_bDownloading = false;
    m_pCache->setCacheState(pEOF->getFileId (), FULL);
}

void
CFileSink::do_processError (DataPacket * pDataPacket)
{
    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    ErrorDataPacket * pError = dynamic_cast <ErrorDataPacket*> (pPacket);

    if (! pError)
        return;

    m_bRequery = true;

    FileId fileId = pError->getFileId ();

    UENotification * pNotification = new UENotification ();
    pNotification->setFileId (fileId);
    pNotification->setType(N_REQUERY);

    m_pNode->sendInternal(pNotification, 0);
}
