#include "Position.h"


CPosition::CPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

CPosition::CPosition(void)
{
    m_x = -1;
    m_y = -1;
}

int CPosition::GetXPosition() const
{
    return m_x;
}

int CPosition::GetYPosition() const
{
    return m_y;
}

bool CPosition::operator == (CPosition Rpos) const
{
    if(this->m_x == Rpos.m_x && this->m_y == Rpos.m_y)
        return true;
    else
        return false;
}

void CPosition::ChangePosition(int x, int y)
{
    this->m_x += x;
    this->m_y += y;
}