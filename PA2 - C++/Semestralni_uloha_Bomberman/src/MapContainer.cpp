#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>

#include "MapContainer.h"
#include "PlayerStats.h"
#include "AIPlayer.h"
#include "Block.h"

using namespace std;

CMapContainer::CMapContainer(vector<vector<char>> & mapData, int gamemode)
{
    copy(mapData.begin(),mapData.end(), back_inserter(m_pureMapData));
    m_gamemode = gamemode;
}

void CMapContainer::LoadObjectives(string & mainPlayerNick, string & secondaryPlayerNick)
{
    //Creating stats for players AI/Normal it doesn't matter
    CPlayerStats mainPlayerStats(3,3,mainPlayerNick);
    m_playerStats.push_back(mainPlayerStats);
    CPlayerStats enemyPlayerStats(3, 3, secondaryPlayerNick);
    m_playerStats.push_back(enemyPlayerStats);


    //Initializing players
    CPlayer mainPlayer(CPosition(1,1),3,CBlock::GetBlockChar(CBlock::PLAYER),m_playerStats.at(0));
        //Position cannot be out of map. This check is here, because it is map-related, player don't care about that
    if(mainPlayer.CheckPosition().GetXPosition() > (int)m_pureMapData.at(0).size()-2 || mainPlayer.CheckPosition().GetYPosition() > (int)m_pureMapData.size())
        throw runtime_error("Wrong position of player! Player cannot be spawned out of map! (P PLAYER ERROR)");
    m_entities.push_back(mainPlayer.CreatePtr());

    //Create enemy based on gamemode
    if(m_gamemode == 1)
    {
        CPlayer enemyPlayer(CPosition((int)m_pureMapData.at(m_pureMapData.size()-2).size()-2,(int)m_pureMapData.size()-2),3,CBlock::GetBlockChar(CBlock::ENEMY_PLAYER),m_playerStats.at(1));
        if(enemyPlayer.CheckPosition().GetXPosition() > (int)m_pureMapData.at(0).size()-2 || enemyPlayer.CheckPosition().GetYPosition() > (int)m_pureMapData.size())
            throw runtime_error("Wrong position of player! Player cannot be spawned out of map! (E PLAYER ERROR)");
        m_entities.push_back(enemyPlayer.CreatePtr());
    }
    else if(m_gamemode == 2)
    {
        CAIPlayer enemyPlayer(CPosition((int)m_pureMapData.at(m_pureMapData.size()-2).size()-2,(int)m_pureMapData.size()-2),3,CBlock::GetBlockChar(CBlock::ENEMY_PLAYER),m_playerStats.at(1));
        if(enemyPlayer.CheckPosition().GetXPosition() > (int)m_pureMapData.at(0).size()-2 || enemyPlayer.CheckPosition().GetYPosition() > (int)m_pureMapData.size())
            throw runtime_error("Wrong position of player! Player cannot be spawned out of map! (E PLAYER ERROR)");
        m_entities.push_back(enemyPlayer.CreatePtr());
    }
    else
        throw runtime_error("Gamemode specified is out of range. Please choose 1 or 2.");


    //     pair<ghost range of motion>
    vector<vector<CPosition>> ghostsPositions;
    //Here I'm finding all blocks that Ghost can walk on and save them in temp vector
    //After that I'm searching for longest (must be longer than 5 blocks) sequence of whitespaces/: that I place the ghost to
    //Since this method is called once (only in game initialization) it is alright that it has higher time complexity
    for (size_t i = 0; i < m_pureMapData.size(); ++i)
    {
        vector<CPosition> tmpGhostBlocks;
        //Load positions of blocks, that may be suitable for ghost
        for (size_t j = 0; j < m_pureMapData.at(0).size(); ++j) //Rows -> I should get better check of row length than obj.m_pureMapData.at(0).size()
        {
            if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::SPACE) ||
               m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL) ||
               m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::SCORE_BOOST))
                tmpGhostBlocks.emplace_back(j,i);
        }


        //Search for the best place for spawning ghost and save the best sequence to ghostPositions (If the sequence is longer than 5)
        if(tmpGhostBlocks.size() > 5)
        {
            vector<CPosition> tmpLongestSubsequence = LongestSubsequence(tmpGhostBlocks);
            if(!tmpLongestSubsequence.empty())
                ghostsPositions.push_back(tmpLongestSubsequence);
        }

        tmpGhostBlocks.clear();
    }

    //Generating X ghosts to the map
    for (size_t i = 0; i < ghostsPositions.size(); ++i)
    {
        //To let ghosts spawn on both ends of range of motion so they don't move all the same direction at start
        //If it is last iteration, I have to make sure ghost will spawn on left end (not players side)
        if(i % 2 == 0 && i != ghostsPositions.size()-1)
        {
            CGhost g1(ghostsPositions.at(i).at(ghostsPositions.at(i).size()-1), 1, CBlock::GetBlockChar(CBlock::GHOST), ghostsPositions.at(i),(int)ghostsPositions.at(i).size()-1);
            m_entities.push_back(g1.CreatePtr());
        }
        else
        {
            CGhost g1(ghostsPositions.at(i).at(0), 1, CBlock::GetBlockChar(CBlock::GHOST), ghostsPositions.at(i),0);
            m_entities.push_back(g1.CreatePtr());
        }
    }
}

//Comparator for LongestSubsequence
bool Longest(const vector<CPosition> &A, const vector<CPosition> &B)
{
    return A.size() < B.size();
}

vector<CPosition> CMapContainer::LongestSubsequence(vector<CPosition> tmpGhostBlocks)
{
    vector<vector<CPosition>> allSequences;
    vector<CPosition> tmpSequence;
    bool startOfSequence = true;

    for (size_t j = 1; j < tmpGhostBlocks.size(); ++j)
    {
        //Check 2 consecutives element if their values are different by 1, if so, push it to the vector
        //When it is first time we find two consecutive vals, push both, after that only push one at the time
        if(tmpGhostBlocks.at(j - 1).GetXPosition() == (tmpGhostBlocks.at(j).GetXPosition() - 1))
        {
            if(startOfSequence)
            {
                tmpSequence.push_back(tmpGhostBlocks.at(j - 1));
                tmpSequence.push_back(tmpGhostBlocks.at(j));
                startOfSequence = false;
            }
            else
            {
                tmpSequence.push_back(tmpGhostBlocks.at(j));
            }
        }
        //If 2 consecutives elements don't have value differencial by one, just check the queue (tmpSequence) if we want that (>5)
        //Anyway, clear our tmpSequence and reset startOfSequence flag
        else
        {
            if(tmpSequence.size() > 5)
            {
                allSequences.push_back(tmpSequence);
            }
            tmpSequence.clear();
            startOfSequence = true;
        }
    }

    //The last sequence won't be pushed if it's last element was last element of the whole block vector
    //So push it now, if we want that sequence (bigger than 5)
    if(tmpSequence.size() > 5)
    {
        if (tmpSequence.at(tmpSequence.size() - 1).GetXPosition() == tmpGhostBlocks.at(tmpGhostBlocks.size() - 1).GetXPosition())
        {
            allSequences.push_back(tmpSequence);
            tmpSequence.clear();
        }
    }

    vector<CPosition> bestSequence;
    //If any sequence longer than 5 was found, find me the longest one and copy that to bestSequence, otherwise return empty vector
    if(!allSequences.empty())
    {
        auto max_itr = max_element(allSequences.begin(), allSequences.end(), Longest);
        copy(max_itr->begin(), max_itr->end(), back_inserter(bestSequence));
    }
    return bestSequence;
}

void CMapContainer::MoveEntities()
{
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        //At this point ghosts can move hovever they want (they have given trajectory), but I have to validate user's choice over the map borders
        m_entities.at(i)->Move(m_pureMapData, m_bombs, m_entities.at(0)->CheckPosition());
    }
}

void CMapContainer::SaveScoreToFile(int score, string nickname)
{
    ofstream scoreFile;
    //std::ios::app stands for appending
    scoreFile.open("assets/scores.txt", std::ios::app);

    //File couldn't be opened
    if(!scoreFile)
    {
        throw runtime_error("ERROR: Can't open file score/scores.txt");
    }
    else
    {
        string wholeLine = nickname + ' ' + to_string(score);
        scoreFile << wholeLine << '\n';
        scoreFile.close();
    }
}

void CMapContainer::EndScreenHandler(string winnerQuote)
{
    SaveScoreToFile(m_playerStats.at(0).CheckScore(), m_playerStats.at(0).CheckNick());
    SaveScoreToFile(m_playerStats.at(1).CheckScore(), m_playerStats.at(1).CheckNick());
    cout << endl << winnerQuote << endl;
    this_thread::sleep_for(chrono::seconds (7));
}

void CMapContainer::HandleBombExplosion(vector<CPosition> flame, bool & endOfGame)
{
    bool pDead = false;
    bool eDead = false;
    for (size_t k = 0; k < flame.size(); ++k)
    {
        //Damaging players + ghosts
        for (size_t j = 0; j < m_entities.size(); ++j)
        {
            if(m_entities.at(j)->CheckPosition() == flame.at(k))
                m_entities.at(j)->DeleteHeart();

            if(m_entities.at(j)->CheckHearts() <= 0) //If the entity should be dead, delete it from the entities
            {
                if(m_entities.at(j)->ToString() == CBlock::GetBlockChar(CBlock::PLAYER))
                    pDead = true;
                if(m_entities.at(j)->ToString() == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER))
                    eDead = true;
                auto elementToRemove = m_entities.begin() + (int)j;
                if(elementToRemove != m_entities.end())
                    m_entities.erase(elementToRemove);
            }
        }

        //Destroying walls + generating flame over players (only for that one tick, that's the reason behind m_wholeMapData changing only to *)
        if(m_wholeMapData.at(flame.at(k).GetYPosition()).at(flame.at(k).GetXPosition()) != CBlock::GetBlockChar(CBlock::WALL))
        {
            m_wholeMapData.at(flame.at(k).GetYPosition()).at(flame.at(k).GetXPosition()) = CBlock::GetBlockChar(CBlock::BOMB_EFFECT);
            m_pureMapData.at(flame.at(k).GetYPosition()).at(flame.at(k).GetXPosition()) = CBlock::GetBlockChar(CBlock::SPACE);
        }
    }

    //EndScreens if players died
    if(!pDead && eDead)
    {
        string playerChar(1,CBlock::GetBlockChar(CBlock::PLAYER));
        EndScreenHandler("END OF GAME ! END OF GAME !\n" + playerChar + " player is the winner !");
        endOfGame = true;
    }
    else if (pDead && !eDead)
    {
        string playerChar(1,CBlock::GetBlockChar(CBlock::ENEMY_PLAYER));
        EndScreenHandler("END OF GAME ! END OF GAME !\n" + playerChar + " player is the winner !");
        endOfGame = true;
    }
    else if(pDead && eDead)
    {
        EndScreenHandler("END OF GAME ! END OF GAME !\nNobody won, both players are DEAD!");
        endOfGame = true;
    }
}

void CMapContainer::AddAndHandleEntitiesOnMap(bool & endOfGame)
{
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        auto position = m_entities.at(i)->CheckPosition();

        //This is the case, when E/P player have same position as some of the ghosts (ghosts are placed later, so allways player will be refreshed on the map at the moment)
        //We have to subtract heart of players and check, if they don't have zero at that moment
        if(m_wholeMapData.at(position.GetYPosition()).at(position.GetXPosition()) == CBlock::GetBlockChar(CBlock::PLAYER) &&
           m_entities.at(i)->ToString() == CBlock::GetBlockChar(CBlock::GHOST))
        {
            m_entities.at(0)->DeleteHeart();
            if(m_entities.at(0)->CheckHearts() <= 0)
            {
                string playerChar(1,CBlock::GetBlockChar(CBlock::ENEMY_PLAYER));
                EndScreenHandler("END OF GAME ! END OF GAME !\n" + playerChar + " player is the winner !");
                endOfGame = true;
                return;
            }
        }
        if(m_wholeMapData.at(position.GetYPosition()).at(position.GetXPosition()) == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER) &&
           m_entities.at(i)->ToString() == CBlock::GetBlockChar(CBlock::GHOST))
        {
            m_entities.at(1)->DeleteHeart();
            if(m_entities.at(1)->CheckHearts() <= 0)
            {
                string playerChar(1,CBlock::GetBlockChar(CBlock::PLAYER));
                EndScreenHandler("END OF GAME ! END OF GAME !\n" + playerChar + " player is the winner !");
                endOfGame = true;
                return;
            }
        }

        //Anyway place the character on the map
        m_wholeMapData.at(position.GetYPosition()).at(position.GetXPosition()) = m_entities.at(i)->ToString();
    }
}

void CMapContainer::AddAndHandleBombOnMap(bool & endOfGame)
{
    for (size_t i = 0; i < m_bombs.size(); ++i)
    {
        auto position = m_bombs.at(i)->CheckPosition();
        m_wholeMapData.at(position.GetYPosition()).at(position.GetXPosition()) = m_bombs.at(i)->CheckChar();
        bool exploded = m_bombs.at(i)->MakeTick();
        if(exploded)
        {
            //flameEffects returns flame of positions, where destroying will be
            vector<CPosition> flame = m_bombs.at(i)->FlameEffect(m_pureMapData);

            //Generating the flame + destroying things arround
            HandleBombExplosion(flame, endOfGame);
            if(endOfGame)
                return;

            //Add bomb for the player, who owned bomb, that just exploded, also give some points for that
            if(m_bombs.at(i)->CheckOwner() == CBlock::GetBlockChar(CBlock::PLAYER))
            {
                m_playerStats.at(0).AddBomb();
                m_playerStats.at(0).AddToScore(5);
            }
            else if(m_bombs.at(i)->CheckOwner() == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER))
            {
                m_playerStats.at(1).AddBomb();
                m_playerStats.at(1).AddToScore(5);
            }

            //Erasing the bomb after explosion from my bombs array
            auto elementToRemove = m_bombs.begin() + (int)i;
            if(elementToRemove != m_bombs.end())
                m_bombs.erase(elementToRemove);
        }
    }
}

void CMapContainer::HandleBonusesOnMap(size_t i, size_t j)
{
    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::PLAYER)) {
        m_playerStats.at(0).AddBombBonus();
        m_playerStats.at(0).AddToScore(25);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }
    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB_RANGE) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::PLAYER)){
        m_playerStats.at(0).AddBiggerBlowBonus();
        m_playerStats.at(0).AddToScore(100);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }
    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER)){
        m_playerStats.at(1).AddBombBonus();
        m_playerStats.at(1).AddToScore(25);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }
    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB_RANGE) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER)){
        m_playerStats.at(1).AddBiggerBlowBonus();
        m_playerStats.at(1).AddToScore(100);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }

    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::SCORE_BOOST) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::PLAYER)){
        m_playerStats.at(0).AddToScore(20);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }
    if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::SCORE_BOOST) && m_wholeMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::ENEMY_PLAYER)){
        m_playerStats.at(1).AddToScore(20);
        m_pureMapData.at(i).at(j) = CBlock::GetBlockChar(CBlock::SPACE);
    }
}

void CMapContainer::RenderMap(bool & endOfGame)
{
    //Copy pure map to the whole map and than copy entities to the map
    m_wholeMapData.clear();
    copy(m_pureMapData.begin(),m_pureMapData.end(), back_inserter(m_wholeMapData));

    //Add monsters to the map + handle colisions of players and ghosts
    AddAndHandleEntitiesOnMap(endOfGame);

    //Add bombs to the map + handle destroying of walls, damaging entities etc.
    AddAndHandleBombOnMap(endOfGame);
    if(endOfGame)
         return;

    cout << u8"\033[2J\033[1;1H";
    for (size_t i = 0; i < m_wholeMapData.size(); ++i) //Lines
    {
        for (size_t j = 0; j < m_wholeMapData.at(0).size(); ++j) //Rows -> I should get better check of row length than obj.m_pureMapData.at(0).size()
        {
            //Bonus handling (only if it even makes sense to call that function
            if(m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB) ||
               m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::BONUS_BOMB_RANGE) ||
               m_pureMapData.at(i).at(j) == CBlock::GetBlockChar(CBlock::SCORE_BOOST))
            {
                HandleBonusesOnMap(i,j);
            }

            //Rendering the whole map
            cout << m_wholeMapData.at(i).at(j);
        }
        cout << "\n";
    }

    cout << CBlock::GetBlockChar(CBlock::PLAYER) << ": Score: " << m_playerStats.at(0).CheckScore();
    cout << "\t" << "Hearts: " << m_entities.at(0)->CheckHearts(); //It is strict, that m_entity on index 0 is Normal player
    cout << "\t" << "Bombs: " << m_playerStats.at(0).CheckBombsAvailable() << endl;

    cout << CBlock::GetBlockChar(CBlock::ENEMY_PLAYER) << ": Score: " << m_playerStats.at(1).CheckScore();
    cout << "\t" << "Hearts: " << m_entities.at(1)->CheckHearts(); //It is strict, that m_entity on index 1 is Enemy player
    cout << "\t" << "Bombs: " << m_playerStats.at(1).CheckBombsAvailable() << endl;
}
