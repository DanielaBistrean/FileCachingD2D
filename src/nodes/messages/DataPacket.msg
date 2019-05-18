//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


enum DataPacketType
{
    DP_ERROR = -1;
    DP_DATA = 0;
    DP_FEEDBACK = 1;
    DP_REQ = 2;
    DP_EOF = 3;
};

enum ErrorType
{
    DP_ERR_NOTFOUND = 0;
    DP_ERR_OUTOFRANGE = 1;
    DP_ERR_UNKNOWN = 2;
}

packet ErrorDataPacket
{
    int fileId;
    int error @enum(ErrorType);
}

packet FileDataPacket
{
    int fileId;
    int blockId;
};

packet RequestDataPacket
{
    int fileId;
    int startBlockId;
};

packet ConfirmationDataPacket
{
    int fileId;
    int numBlocks;
    int startBlockId;
};

packet FeedbackDataPacket
{
    int fileId;
    int blockId;
    bool ack;
    int nextBlockId;
};

packet EOFDataPacket
{
    int fileId;
};

packet DataPacket
{
	int sourceId;
	int destinationId;
	int type @enum(DataPacketType);
};
