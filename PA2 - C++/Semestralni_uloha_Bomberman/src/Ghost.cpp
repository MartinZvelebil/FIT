#include "Ghost.h"
using namespace std;

CGhost::CGhost(CPosition pos, int hearts, char charval, vector<CPosition> rangeOfMotion, int RangeOfMotionIndex)
: CEntity(pos, hearts, charval)
{
    m_rangeOfMotion = rangeOfMotion;
    m_directionR = true; //By default I will setup to go right always
    m_rangeOfMotionIndex = RangeOfMotionIndex;
}

char CGhost::ToString() const
{
    return m_char;
}

void CGhost::Move(vector<vector<char>> & m_pureMapData, vector<unique_ptr<CBomb>> & m_bombs, CPosition playerPosition)
{
    //If I'm at the end of my range motion, change direction
    if(m_position == m_rangeOfMotion.at(m_rangeOfMotion.size()-1))
        m_directionR = false;
    else if(m_position == m_rangeOfMotion.at(0))
        m_directionR = true;

    //If I'm going left
    if(!m_directionR)
    {
        m_rangeOfMotionIndex--;
        m_position = m_rangeOfMotion.at(m_rangeOfMotionIndex);
    }
    //If I'm going right
    else
    {
        m_rangeOfMotionIndex++;
        m_position = m_rangeOfMotion.at(m_rangeOfMotionIndex);
    }
}

unique_ptr<CEntity> CGhost::CreatePtr() const
{
    return make_unique<CGhost>(*this);
}
