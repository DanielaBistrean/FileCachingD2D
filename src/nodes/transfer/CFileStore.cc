#include "../../configuration/CGlobalConfiguration.h"
#include "CFileStore.h"

CFileStore::CFileStore (bool available)
{
    int numFiles = CGlobalConfiguration::getInstance ().get ("numFiles");

    for (std::size_t id = 0; id < numFiles; ++id)
    {
        insert({id, CFile (16000, available)});
    }
}

void
CFileStore::removeFile (FileId fileId)
{
    auto it = find (fileId);
    if (it == end ())
        return;

    it->second.unsetAll ();
}
