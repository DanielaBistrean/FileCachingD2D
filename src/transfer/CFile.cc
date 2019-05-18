#include "CFile.h"

CFile::CFile ()
: m_bytes {0}
{}

CFile::CFile (std::size_t bytes, bool available)
: m_blocks (bytes / 1500 + (bytes % 1500 != 0), available)
, m_bytes  {bytes}
{}

bool
CFile::hasBlock (std::size_t index)
{
    return m_blocks.at (index);
}

void
CFile::setBlock (std::size_t index)
{
    m_blocks.at (index) = true;
}

void
CFile::unsetBlock (std::size_t index)
{
    m_blocks.at (index) = false;
}

