#ifndef D2D_CFILECACHE_H
#define D2D_CFILECACHE_H 1

#include <unordered_map>
#include "CFile.h"

using FileId = std::size_t;

class CFileStore : public std::unordered_map <FileId, CFile>
{
public:
    CFileStore (std::size_t fileSize, bool available = false);

    void removeFile (FileId fileId);
};

#endif // D2D_CFILECACHE_H
