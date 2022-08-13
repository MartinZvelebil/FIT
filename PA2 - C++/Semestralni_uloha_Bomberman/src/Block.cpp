#include "Block.h"

char CBlock::GetBlockChar(CBlock::TypeOfBlock type)
{
    return m_characters[type];
}
