#include <iostream>
#include "Bomb.h"
#include "Block.h"
using namespace std;

CBomb::CBomb(const CPosition & position, int radius, char owner) //: m_Owner(owner)
{
    m_position = position;
    m_radius = radius;
    m_owner = owner;
    m_representingChar = CBlock::GetBlockChar(CBlock::BOMB);
    m_ticks = 8;
}

unique_ptr<CBomb> CBomb::CreatePtr() const
{
    return make_unique<CBomb>(*this);
}

CPosition CBomb::CheckPosition() const
{
    return m_position;
}

char CBomb::CheckChar() const
{
    return m_representingChar;
}

char CBomb::CheckOwner() const
{
    return m_owner;
}

bool CBomb::MakeTick()
{
    m_ticks--;
    if(m_ticks == 0)
        return true;
    else
        return false;
}

vector<CPosition> CBomb::FlameEffect(std::vector<std::vector<char>> & m_pureMapData)
{
    vector<CPosition> flame;
    flame.push_back(m_position); //Center point

    //Creation of the flame positions
    //1 = TOP, 2 = RIGHT, 3 = BOTTOM, 4 = LEFT
    int maxY = (int)m_pureMapData.size()-1;
    int maxX = (int)m_pureMapData.at(0).size()-1;
    int direction = 1;
    int repetetions = (m_radius - 1) / 2;
    while (direction != 5)
    {
        CPosition tmp = m_position;
        switch(direction)
        {
            case 1: //TOP
                for (int i = 0; i < repetetions; ++i) {
                    tmp.ChangePosition(0, -1);
                    if(m_pureMapData.at(tmp.GetYPosition()).at(tmp.GetXPosition()) == CBlock::GetBlockChar(CBlock::WALL)) //If bomb is firing at wall, stop generating flame that way
                        break;
                    if(tmp.GetXPosition() <= maxX && tmp.GetYPosition() <= maxY && tmp.GetXPosition() >= 0 && tmp.GetYPosition() >= 0) //So I don't get out of map with the flame
                        flame.push_back(tmp);
                }
                break;
            case 2: //RIGHT
                for (int i = 0; i < repetetions; ++i) {
                    tmp.ChangePosition(1, 0);
                    if(m_pureMapData.at(tmp.GetYPosition()).at(tmp.GetXPosition()) == CBlock::GetBlockChar(CBlock::WALL))
                        break;
                    if(tmp.GetXPosition() <= maxX && tmp.GetYPosition() <= maxY && tmp.GetXPosition() >= 0 && tmp.GetYPosition() >= 0) //So I don't get out of map with the flame
                        flame.push_back(tmp);
                }
                break;
            case 3: //BOTTOM
                for (int i = 0; i < repetetions; ++i) {
                    tmp.ChangePosition(0, 1);
                    if(m_pureMapData.at(tmp.GetYPosition()).at(tmp.GetXPosition()) == CBlock::GetBlockChar(CBlock::WALL))
                        break;
                    if(tmp.GetXPosition() <= maxX && tmp.GetYPosition() <= maxY && tmp.GetXPosition() >= 0 && tmp.GetYPosition() >= 0) //So I don't get out of map with the flame
                        flame.push_back(tmp);
                }
                break;
            case 4: //LEFT
                for (int i = 0; i < repetetions; ++i) {
                    tmp.ChangePosition(-1, 0);
                    if(m_pureMapData.at(tmp.GetYPosition()).at(tmp.GetXPosition()) == CBlock::GetBlockChar(CBlock::WALL))
                        break;
                    if(tmp.GetXPosition() <= maxX && tmp.GetYPosition() <= maxY && tmp.GetXPosition() >= 0 && tmp.GetYPosition() >= 0) //So I don't get out of map with the flame
                        flame.push_back(tmp);
                }
                break;
            default:
                throw runtime_error("Unexpected value");
        }
        direction++;
    }
    return flame;
}
