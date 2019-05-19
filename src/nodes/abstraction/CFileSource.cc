#include "CFileSource.h"

#include "NetworkAbstraction.h"

CFileSource::CFileSource (INode * pNode, CCacheManager * pCache)
: m_pNode  {pNode}
, m_pCache {pCache}
{}

void
CFileSource::process (omnetpp::cMessage * pMsg)
{
    DataPacket *pDataPacket = dynamic_cast <DataPacket *> (pMsg);

    if (! pDataPacket)
        return;

    switch (pDataPacket->getType())
    {
    case DP_REQ:
        do_processRequest (pDataPacket);
        break;
    case DP_FEEDBACK:
        do_processFeedback (pDataPacket);
        break;
    default:
        break;
    }
}

void
CFileSource::do_processRequest (DataPacket * pDataPacket)
{
    EV_STATICCONTEXT

    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    EV_WARN << "Processing data packet from node " << sId << std::endl;

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    RequestDataPacket * pRequest = dynamic_cast <RequestDataPacket *> (pPacket);

    if (! pRequest)
        return;

    int fileId = pRequest->getFileId ();
    int startBlockId = pRequest->getStartBlockId ();

    if (! m_pCache->getFileData (fileId, startBlockId))
        do_respondWithError(fileId, sId, DP_ERR_NOTFOUND);
    else
        do_respondWithData (fileId, sId, startBlockId);
}

void
CFileSource::do_processFeedback (DataPacket * pDataPacket)
{
    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    FeedbackDataPacket * pFeedback = dynamic_cast <FeedbackDataPacket *> (pPacket);

    if (! pFeedback)
        return;

    int fileId = pFeedback->getFileId ();
    int blockId = pFeedback->getBlockId ();
    bool ack = pFeedback->getAck ();
    int nextBlockId = pFeedback->getNextBlockId ();

    if (! m_pCache->isValidFile (fileId))
        return;

    bool bInRange = NetworkAbstraction::getInstance ().isGateInRange(m_pNode->getNode ()->getId (), sId);

    if (! m_pCache->getFileData(fileId, blockId))
        do_respondWithError(fileId, sId, DP_ERR_NOTFOUND);
    else if (ack && (nextBlockId == -1 && (m_pCache->getNumBlocks (fileId) == (blockId + 1))))
        do_respondWithEOF (fileId, sId);
    else if (bInRange)
    {
        if (ack)
            // TODO: check if block is available
            do_respondWithData (fileId, sId, (nextBlockId == -1) ? (blockId + 1) : nextBlockId);
        else
            do_respondWithData (fileId, sId, blockId);
    }
    else
        do_respondWithError (fileId, sId, DP_ERR_OUTOFRANGE);
}

void
CFileSource::do_respondWithError (FileId fileId, int destId, int error)
{
    DataPacket * pResponse = new DataPacket ();
    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (destId);
    pResponse->setType (DP_ERROR);

    ErrorDataPacket * pError = new ErrorDataPacket ();
    pError->setFileId (fileId);
    pError->setError(error);

    pResponse->encapsulate (pError);

    m_pNode->sendOut (pResponse, destId);
}

void
CFileSource::do_respondWithData (FileId fileId, int destId, int blockId)
{
    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (destId);
    pResponse->setType (DP_DATA);

    FileDataPacket * pFileDataPacket = new FileDataPacket ();

    pFileDataPacket->setFileId (fileId);
    pFileDataPacket->setBlockId (blockId);

    pResponse->encapsulate(pFileDataPacket);

    m_pNode->sendOut (pResponse, destId);
}

void
CFileSource::do_respondWithEOF (FileId fileId, int destId)
{
    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (m_pNode->getNode ()->getId ());
    pResponse->setDestinationId (destId);
    pResponse->setType (DP_EOF);

    EOFDataPacket * pEOFDataPacket = new EOFDataPacket ();
    pEOFDataPacket->setFileId (fileId);

    pResponse->encapsulate (pEOFDataPacket);

    m_pNode->sendOut (pResponse, destId);
}
