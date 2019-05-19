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

void
CFileCache::recalculatePriorities ()
{
//    std::sort(begin(), end());
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
