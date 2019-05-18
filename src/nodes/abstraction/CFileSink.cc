#include "CFileSink.h"

#include "NetworkAbstraction.h"

CFileSink::CFileSink (INode * pNode, CFileCache * pCache)
: m_pNode  {pNode}
, m_pCache {pCache}
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
    default:
        break;
    }
}

void
CFileSink::requestFile (FileId fileId, int nodeId)
{
    if (nodeId == -1)
        nodeId = NetworkAbstraction::getInstance ().getBaseId ();

    DataPacket * pResponse = new DataPacket ();

    pResponse->setSourceId (m_pNode->getNode ()->getId());
    pResponse->setDestinationId (nodeId);
    pResponse->setType (DP_REQ);

    RequestDataPacket * pRequestDataPacket = new RequestDataPacket ();

    pRequestDataPacket->setFileId (fileId);
    pRequestDataPacket->setStartBlockId (0);

    pResponse->encapsulate (pRequestDataPacket);

    m_pNode->sendOut (pResponse, nodeId);
}

void
CFileSink::do_processData  (DataPacket * pDataPacket)
{
    EV_STATICCONTEXT

    auto sId = pDataPacket->getSourceId ();
    auto dId = pDataPacket->getDestinationId ();

    EV << "Processing payload from node " << sId << std::endl;

    omnetpp::cPacket * pPacket = pDataPacket->decapsulate ();
    if (! pPacket)
    {
        EV_WARN << "Could not decapsulate packet" << std::endl;
        return;
    }

    FileDataPacket * pData = dynamic_cast <FileDataPacket *> (pPacket);

    if (! pData)
    {
        EV_WARN << "Not a payload packet. Something bad happened" << std::endl;
        return;
    }

    int fileId = pData->getFileId ();
    int blockId = pData->getBlockId ();

    auto it = m_pCache->find (fileId);
    if (it == m_pCache->end ())
    {
        EV_WARN << "File not found!" << std::endl;
        return;
    }

    CFile file = it->second;

    if (file.blocks () <= blockId)
    {
        EV_WARN << "Block " << blockId << " outside of file range (" << file.blocks () << ")" << std::endl;
        return;
    }

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
