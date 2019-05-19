#include "CFileSource.h"

#include "NetworkAbstraction.h"

CFileSource::CFileSource (INode * pNode, CFileStore * pCache)
: m_pNode  {pNode}
, m_pStore {pCache}
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
    {
        EV_WARN << "Could not decapsulate packet" << std::endl;
        return;
    }

    RequestDataPacket * pRequest = dynamic_cast <RequestDataPacket *> (pPacket);

    if (! pRequest)
    {
        EV_WARN << "Not a request packet. Something bad happened" << std::endl;
        return;
    }

    int fileId = pRequest->getFileId ();
    int startBlockId = pRequest->getStartBlockId ();

    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
    {
        EV_WARN << "No file found with id " << fileId << std::endl;
        return;
    }

    CFile file = it->second;
    if (! file.hasBlock (startBlockId))
        return;

    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (dId);
    pResponse->setDestinationId (sId);
    pResponse->setType (DP_DATA);

    FileDataPacket * pFileDataPacket = new FileDataPacket ();

    pFileDataPacket->setFileId (fileId);
    pFileDataPacket->setBlockId (startBlockId);

    pResponse->encapsulate (pFileDataPacket);

    m_pNode->sendOut (pResponse, sId);
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

    auto it = m_pStore->find (fileId);
    if (it == m_pStore->end ())
        return;

    CFile file = it->second;

    // TODO: rework branches
    if (! ack)
    {
        if (! file.hasBlock (blockId))
            return; // ERROR;

        DataPacket * pResponse = new DataPacket ();

        pResponse->setSourceId (dId);
        pResponse->setDestinationId (sId);
        pResponse->setType (DP_DATA);

        FileDataPacket * pFileDataPacket = new FileDataPacket ();

        pFileDataPacket->setFileId (fileId);
        pFileDataPacket->setBlockId (blockId);

        pResponse->encapsulate (pFileDataPacket);

        m_pNode->sendOut (pResponse, sId);
    }
    else
    {
        if (nextBlockId == -1 && (file.blocks() == (blockId + 1)))
        {
            DataPacket * pResponse = new DataPacket ();

            pResponse->setSourceId (dId);
            pResponse->setDestinationId (sId);
            pResponse->setType (DP_EOF);

            EOFDataPacket * pEOFDataPacket = new EOFDataPacket ();
            pEOFDataPacket->setFileId (fileId);

            pResponse->encapsulate (pEOFDataPacket);

            m_pNode->sendOut (pResponse, sId);
        }
        else
        {
            nextBlockId = (nextBlockId == -1) ? (blockId + 1) : nextBlockId;

            if (! file.hasBlock (nextBlockId))
            {
                do_RespondWithError (fileId, sId, DP_ERR_NOTFOUND);
                return;
            }

            if (! NetworkAbstraction::getInstance ().isGateInRange(m_pNode->getNode ()->getId (), sId))
            {
                do_RespondWithError (fileId, sId, DP_ERR_OUTOFRANGE);
                return;
            }

            DataPacket * pResponse = new DataPacket ();

            pResponse->setSourceId (dId);
            pResponse->setDestinationId (sId);
            pResponse->setType (DP_DATA);

            FileDataPacket * pFileDataPacket = new FileDataPacket ();

            pFileDataPacket->setFileId (fileId);
            pFileDataPacket->setBlockId (nextBlockId);

            pResponse->encapsulate(pFileDataPacket);

            m_pNode->sendOut (pResponse, sId);
        }
    }
}

void
CFileSource::do_RespondWithError (FileId fileId, int destId, int error)
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

