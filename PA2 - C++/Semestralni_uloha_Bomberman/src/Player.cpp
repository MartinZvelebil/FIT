#include <iostream>

#include "Player.h"
#include "curses.h"
#include "Block.h"

using namespace std;

CPlayer::CPlayer(CPosition pos, int hearts, char charval, CPlayerStats & playerStats) : CEntity(pos, hearts, charval), m_playerStats(playerStats)
{
    //Everything is happening in sub-constructor of entity + playerStats
}

char CPlayer::ToString() const
{
    return m_char;
}

int CPlayer::GetChar(int lineOfInput)
{
    //  Zdroj: https://jbwyatt.com/ncurses.html#input

    //NCurses reading char
    initscr();
    //move(lineOfInput, 0);
    flushinp();
    timeout(200);
    cbreak(); //No buffering
    noecho();
    keypad(stdscr, TRUE);

    //nodelay(stdscr, TRUE);
    int ch = getch();
    endwin();

    if(ERR == ch)
        return -1;
    else
        return ch;
}

void CPlayer::HandleMove(char playerChar, int userVal, vector<vector<char>> & m_pureMapData, vector<unique_ptr<CBomb>> & m_bombs)
{
    vector <int> playerControls = {0};
    if(playerChar == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER))
    {
        //Doubled because of CAPS-LOCK/non-caps needed for the other player (wasd/WASD), and I need the same length for both
        playerControls = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_RIGHT, KEY_RIGHT, KEY_LEFT, KEY_LEFT, 'M', 'm'};
    }
    else if(playerChar == CBlock::GetBlockChar(CBlock::PLAYER))
    {
        playerControls = {'W', 'w', 'S', 's', 'D', 'd', 'A', 'a', 'C', 'c'};
    }
    else
        throw runtime_error("Unexpected handleMove player Char value!");

    //UP
    if (playerControls.at(0) == userVal || playerControls.at(1) == userVal) {
        if(m_pureMapData.at(m_position.GetYPosition()-1).at(m_position.GetXPosition()) != CBlock::GetBlockChar(CBlock::WALL)
           && m_pureMapData.at(m_position.GetYPosition()-1).at(m_position.GetXPosition()) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL))
            m_position.ChangePosition(0, -1);
    }
    //DOWN
    else if (playerControls.at(2) == userVal || playerControls.at(3) == userVal) {
        if(m_pureMapData.at(m_position.GetYPosition()+1).at(m_position.GetXPosition()) != CBlock::GetBlockChar(CBlock::WALL)
           && m_pureMapData.at(m_position.GetYPosition()+1).at(m_position.GetXPosition()) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL))
            m_position.ChangePosition(0, 1);
    }
    //RIGHT
    else if (playerControls.at(4) == userVal || playerControls.at(5) == userVal) {
        if(m_pureMapData.at(m_position.GetYPosition()).at(m_position.GetXPosition()+1) != CBlock::GetBlockChar(CBlock::WALL)
           && m_pureMapData.at(m_position.GetYPosition()).at(m_position.GetXPosition()+1) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL))
            m_position.ChangePosition(1, 0);
    }
    //LEFT
    else if (playerControls.at(6) == userVal || playerControls.at(7) == userVal) {
        if(m_pureMapData.at(m_position.GetYPosition()).at(m_position.GetXPosition()-1) != CBlock::GetBlockChar(CBlock::WALL)
           && m_pureMapData.at(m_position.GetYPosition()).at(m_position.GetXPosition()-1) != CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL))
            m_position.ChangePosition(-1, 0);
    }
    //BOMB
    else if (playerControls.at(8) == userVal || playerControls.at(9) == userVal)
    {
        //Already placed bomb on that place handler
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
}

void CPlayer::Move(vector<vector<char>> & m_pureMapData, vector<unique_ptr<CBomb>> & m_bombs, CPosition playerPosition)
{
    int whereToScanInput = (int)(m_pureMapData.size()-1) + 2;

    int userVal = GetChar(whereToScanInput);

    //If reading had Error flag
    if(userVal == -1)
        return;

    //This is move-specific function declaring it's
    HandleMove(m_char,userVal,m_pureMapData,m_bombs);
}

unique_ptr<CEntity> CPlayer::CreatePtr() const
{
    return make_unique<CPlayer>(*this);
}
