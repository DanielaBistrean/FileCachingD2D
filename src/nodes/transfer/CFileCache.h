#ifndef D2D_CFILECACHE_H
#define D2D_CFILECACHE_H 1

#include <unordered_map>
#include "CFile.h"

using FileId = std::size_t;

enum CacheState
{
    ERROR = -2,
    ENQUIRY = -1,
    NOTPRESENT = 0,
    DOWNLOADING = 1,
    FULL = 2
};

struct CacheData
{
    CacheState state;
    int score;
    CFile file;

    CacheData (std::size_t bytes, bool available = false)
    : state {available ? FULL : NOTPRESENT}, score {0}, file {bytes, available} {};

    friend bool operator<  (const CacheData &lhs, const CacheData &rhs);
    friend bool operator== (const CacheData &lhs, const CacheData &rhs);
};

class CFileCache : public std::unordered_map <FileId, CacheData>
{
public:
    CFileCache (bool available = false);

public:
    CacheState getCacheState (FileId fileId);
    void setCacheState (FileId fileId, CacheState state);

    void recalculatePriorities ();

public:
    int getAvailability (FileId fileId, int blockId = 0);
};

#endif // D2D_CFILECACHE_H
