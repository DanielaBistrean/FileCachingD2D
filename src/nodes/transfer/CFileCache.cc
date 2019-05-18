#include "CFileCache.h"

#include "../../configuration/CGlobalConfiguration.h"

CFileCache::CFileCache (bool available)
{
    int numFiles = CGlobalConfiguration::getInstance ().get ("numFiles");

    for (std::size_t id = 0; id < numFiles; ++id)
    {
        insert({id, CFile (16000, available)});
    }
}
