#ifndef D2D_CFILE_H
#define D2D_CFILE_H 1

#include <vector>
#include <cstdlib>

class CFile
{
public:
    CFile ();
    CFile (std::size_t bytes, bool available = false);

public:
    bool hasBlock   (std::size_t index);
    void setBlock   (std::size_t index);
    void unsetBlock (std::size_t index);

public:
    std::size_t size   () { return m_bytes;          };
    std::size_t blocks () { return m_blocks.size (); };

private:
    std::vector <bool> m_blocks;
    std::size_t        m_bytes;
};

#endif // D2D_CFILE_H
