#ifndef CPLAYERSTATS_H
#define CPLAYERSTATS_H
#include <string>

/**
 * @class CPlayerStats
 * @brief Represents stats of player
 */
class CPlayerStats
{
public:
    /**
     * @brief Constructor for creating new stats
     * @param bombsAvailable How many bombs can player plant at the moment
     * @param maxBombs Maximum number of bombs to plant
     * @param nickname User's nickname
     */
    CPlayerStats(int bombsAvailable, int maxBombs, std::string & nickname);

    /**
     * Default CPlayerStats destructor
     */
    ~CPlayerStats() = default;

    /**
     * @return Number of available bombs
     */
    int CheckBombsAvailable() const;

    /**
     * @return True if bonus is active
     * @return False if bonus isn't active
     */
    bool CheckBiggerBlowBonus() const;

    /**
     * @return Player's score
     */
    int CheckScore() const;

    /**
     * @return Player's nickname
     */
    std::string CheckNick();

    /**
     * @brief Returns AvailableBombs--, subtracts one bomb when user placed bomb
     */
    void SubBomb();

    /**
     * @brief Returns AvailableBombs++, adding bombs back after explosion
     */
    void AddBomb();

    /**
     * @brief Changing m_biggerBlowBonus to true - permanent effect
     */
    void AddBiggerBlowBonus();

    /**
     * @brief +1 bomb to m_maxBombs and also m_bombsAvailable
     */
    void AddBombBonus();

    /**
     * @brief Function changing score
     * @param value How much to add to the score
     */
    void AddToScore(int value);

private:
    //Player specific atributes
    int m_maxBombs;
    int m_score;
    std::string m_nickname;
    int m_bombsAvailable;
    bool m_biggerBlowBonus;
};


#endif //CPLAYERSTATS_H
