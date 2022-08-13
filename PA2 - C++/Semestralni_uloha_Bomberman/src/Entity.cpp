#include <stdexcept>

#include "Entity.h"
using namespace std;

CEntity::CEntity(CPosition pos, int hearts, char charval)
{
    m_position = pos;
    if(m_position.GetXPosition() <= 0 || m_position.GetYPosition() <= 0)
        throw runtime_error("POSITION OF ENTITY CANNOT BE NEGATIVE!");
    m_hearts = hearts;
    if(m_hearts <= 0)
        throw runtime_error("HEARTS OF ENTITY CANNOT BE <= 0 IN INITIALIZATION!");
    m_char = charval;
}

void CEntity::DeleteHeart()
{
    m_hearts--;
}

CPosition CEntity::CheckPosition()
{
    return m_position;
}

int CEntity::CheckHearts() const
{
    return m_hearts;
}