#include <iostream>
#include <algorithm>

#include "AIPlayer.h"
#include "Block.h"

using namespace std;

CAIPlayer::CAIPlayer(CPosition pos, int hearts, char charval, CPlayerStats & playerStats) : CEntity(pos, hearts, charval), m_playerStats(playerStats)
{
    //Everything is happening in sub-constructor of entity + playerStats
}

char CAIPlayer::ToString() const
{
    return m_char;
}

void CAIPlayer::RandomizeMove(vector<std::vector<char>> & m_pureMapData)
{
                 //UP, DOWN,  RIGHT,  LEFT
    int x[4] = {0, 0, 1, -1};
    int y[4] = {-1, 1, 0, 0};

    //Random number generator for maps
    int direction;
    srand(time(nullptr));
    direction = rand() % 4;
    CPosition tmpPos = m_position;
    tmpPos.ChangePosition(x[direction], y[direction]);

    if (m_pureMapData.at(tmpPos.GetYPosition()).at(tmpPos.GetXPosition()) != CBlock::GetBlockChar(CBlock::WALL) &&
        m_pureMapData.at(tmpPos.GetYPosition()).at(tmpPos.GetXPosition()) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL))
            m_position = tmpPos;

}

void CAIPlayer::PlantBomb(vector<unique_ptr<CBomb>> & m_bombs)
{
    bool sameBomb = false;
    for (auto &m_bomb: m_bombs)
    {
        if (m_bomb->CheckPosition() == m_position)
            sameBomb = true;
    }
    if(!sameBomb)
    {
        //Planting bomb based on the biggerBlowBonus, owner char and position
        if (m_playerStats.CheckBiggerBlowBonus() && m_playerStats.CheckBombsAvailable() > 0){
            CBomb b1(m_position, 5, m_char);
            m_bombs.push_back(b1.CreatePtr());
            m_playerStats.SubBomb();
        }
        else if (!m_playerStats.CheckBiggerBlowBonus() && m_playerStats.CheckBombsAvailable() > 0){
            CBomb b1(m_position, 3, m_char);
            m_bombs.push_back(b1.CreatePtr());
            m_playerStats.SubBomb();
        }
    }
}

void CAIPlayer::BFSPath(CPosition start, CPosition end, vector<std::vector<char>> & m_pureMapData, vector<unique_ptr<CBomb>> & m_bombs)
{
                 //UP, DOWN,  RIGHT,  LEFT
    int x[4] = {0, 0, 1, -1};
    int y[4] = {-1, 1, 0, 0};
    bool endWasFound = false;
    deque<CPosition> visited;
    deque<CPosition> queue;
    CPosition BFSPosition = end;
    queue.push_back(BFSPosition);
    visited.push_back(BFSPosition);
    CPosition whereToMove;

    //While it makes sense to search through a graph (still some ways exists)
    while(!queue.empty())
    {
        BFSPosition = queue.front();

        //Exploring neighbourghs (4 ways)
        for (int i = 0; i < 4; ++i)
        {
            CPosition tmpPos = BFSPosition;
            tmpPos.ChangePosition(x[i], y[i]);
            //Validate, if the move is possible and if wasn't made before
            if (m_pureMapData.at(tmpPos.GetYPosition()).at(tmpPos.GetXPosition()) != CBlock::GetBlockChar(CBlock::WALL) &&
                m_pureMapData.at(tmpPos.GetYPosition()).at(tmpPos.GetXPosition()) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL) &&
                count(visited.begin(), visited.end(), tmpPos) == 0)
            {
                visited.push_back(tmpPos);
                queue.push_back(tmpPos);
                //If we reached the desired position, we take the BFSPosition (step before finish)
                if(tmpPos == start)
                {
                    whereToMove = BFSPosition;
                    endWasFound = true;
                    break;
                }
            }
        }
        //Get rid of the element we just searched neighbours for
        queue.pop_front();
    }

    if(endWasFound)
        m_position = whereToMove;
    else //If there is no way between Players, randomized moves are on
        RandomizeMove(m_pureMapData);

    if(m_position == end)
        PlantBomb(m_bombs);
}

void CAIPlayer::Move(vector<vector<char>> & m_pureMapData, vector<unique_ptr<CBomb>> & m_bombs, CPosition playerPosition)
{
    BFSPath(m_position,playerPosition, m_pureMapData, m_bombs);
}

unique_ptr<CEntity> CAIPlayer::CreatePtr() const
{
    return make_unique<CAIPlayer>(*this);
}

