#ifndef D2D_CFILECACHE_H
#define D2D_CFILECACHE_H 1

#include <unordered_map>
#include "CFile.h"

using FileId = std::size_t;

class CFileCache : public std::unordered_map <FileId, CFile>
{
public:
    CFileCache (bool available = false);
};

#endif // D2D_CFILECACHE_H
