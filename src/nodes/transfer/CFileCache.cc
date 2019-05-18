#include "CFileCache.h"

#include "../../configuration/CGlobalConfiguration.h"

CFileCache::CFileCache (bool available)
{
    int numFiles = CGlobalConfiguration::getInstance ().get ("numFiles");

    for (std::size_t id = 0; id < numFiles; ++id)
    {
        insert({id, CacheData (16000, available)});
    }
}

CacheState
CFileCache::getCacheState (FileId fileId)
{
    auto it = find (fileId);
    if (it == end ())
        return ERROR;

    return it->second.state;
}

void
CFileCache::setCacheState (FileId fileId, CacheState state)
{
    auto it = find (fileId);
    if (it == end ())
        return;

    it->second.state = state;
}

void
CFileCache::recalculatePriorities ()
{
//    std::sort(begin(), end());
}

int
CFileCache::getAvailability (FileId fileId, int blockId)
{
    auto it = find (fileId);
    if ((it == end ()) || (blockId >= it->second.file.blocks ()) || (! it->second.file.hasBlock (blockId)))
        return -1;

    return it->second.file.available ();
}

bool
operator< (const CacheData &lhs, const CacheData &rhs)
{
    return (lhs.score * lhs.state) < (rhs.score * lhs.state);
}

bool
operator== (const CacheData &lhs, const CacheData &rhs)
{
    return (lhs.score * lhs.state) < (rhs.score * lhs.state);
}
