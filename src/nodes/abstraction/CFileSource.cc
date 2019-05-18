#include "CFileSource.h"

CFileSource::CFileSource (INode * pNode, CFileCache * pCache)
: m_pNode  {pNode}
, m_pCache {pCache}
{}

void
CFileSource::process (omnetpp::cMessage * pMsg)
{
    DataPacket *pDataPacket = omnetpp::check_and_cast <DataPacket *> (pMsg);

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
    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    RequestDataPacket * pRequest = dynamic_cast <RequestDataPacket *> (pPacket);

    if (! pRequest)
        return;

    int fileId = pRequest->getFileId ();
    int startBlockId = pRequest->getStartBlockId ();

    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return;

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

    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return;

    CFile file = it->second;

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
                return; // ERROR

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

