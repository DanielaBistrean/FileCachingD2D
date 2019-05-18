#include "CFileSink.h"

CFileSink::CFileSink (INode * pNode, CFileCache * pCache)
: m_pNode  {pNode}
, m_pCache {pCache}
{}

void
CFileSink::process (omnetpp::cMessage * pMsg)
{
    DataPacket *pDataPacket = omnetpp::check_and_cast <DataPacket *> (pMsg);

    switch (pDataPacket->getType())
    {
    case DP_DATA:
        do_processData (pDataPacket);
        break;
    case DP_EOF:
        do_processEOF (pDataPacket);
        break;
    default:
        break;
    }
}

void
CFileSink::do_processData  (DataPacket * pDataPacket)
{
    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
        return;

    FileDataPacket * pData = dynamic_cast <FileDataPacket *> (pPacket);

    if (! pData)
        return;

    int fileId = pData->getFileId ();
    int blockId = pData->getBlockId ();

    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
        return;

    CFile file = it->second;

    if (file.blocks () >= blockId)
        return;

    file.setBlock (blockId);

    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (dId);
    pResponse->setDestinationId (sId);
    pResponse->setType (DP_FEEDBACK);

    FeedbackDataPacket * pFeedbackDataPacket = new FeedbackDataPacket ();

    pFeedbackDataPacket->setFileId (fileId);
    pFeedbackDataPacket->setBlockId (blockId);
    pFeedbackDataPacket->setAck (true);
    pFeedbackDataPacket->setNextBlockId (-1);

    pResponse->encapsulate (pFeedbackDataPacket);

    m_pNode->sendOut (pResponse, sId);
}

void
CFileSink::do_processEOF (DataPacket * pDataPacket)
{}
