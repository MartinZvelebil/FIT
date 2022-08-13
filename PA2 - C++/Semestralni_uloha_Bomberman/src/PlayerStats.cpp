#include "PlayerStats.h"
#include <memory>
using namespace std;


CPlayerStats::CPlayerStats(int bombsAvailable, int maxBombs, std::string &nickname)
{
    m_maxBombs = maxBombs;
    m_bombsAvailable = bombsAvailable;
    m_nickname = nickname;
    m_score = 0;
    m_biggerBlowBonus = false;
}

int CPlayerStats::CheckBombsAvailable() const
{
    return m_bombsAvailable;
}

int CPlayerStats::CheckScore() const
{
    return m_score;
}

bool CPlayerStats::CheckBiggerBlowBonus() const
{
    return m_biggerBlowBonus;
}

string CPlayerStats::CheckNick()
{
    return m_nickname;
}

void CPlayerStats::SubBomb()
{
    m_bombsAvailable = m_bombsAvailable - 1;
}

void CPlayerStats::AddBomb()
{
    m_bombsAvailable++;
}

void CPlayerStats::AddBiggerBlowBonus()
{
    m_biggerBlowBonus = true;
}

void CPlayerStats::AddBombBonus()
{
    m_bombsAvailable++;
    m_maxBombs++;
}

void CPlayerStats::AddToScore(int value)
{
    m_score += value;
}