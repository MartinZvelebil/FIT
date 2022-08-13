#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
#include <memory>

#include "Position.h"
#include "Entity.h"
#include "Bomb.h"
#include "PlayerStats.h"

/**
 * @class CPlayer
 * @brief Represents Players
 */
class CPlayer : public CEntity
{
public:
    /**
     * @brief Constructor, that will create & initialize new Player with basic parameters
     * @param pos Position of the player on the map
     * @param hearts Number of hearts player has
     * @param charval Character that will be displayed on map
     * @param playerStats Reference on player stats (player-specific statistics like score). Handled in different object for better object behaviour.
     */
    CPlayer(CPosition pos, int hearts, char charval, CPlayerStats & playerStats);

    /**
     * Default CPlayer destructor
     */
    virtual ~CPlayer() = default;

    /**
     * @brief Function responsible for ncurses library (scanning user's input)
     * @return Scanned value (returns -1 if ERR)
     * @param lineOfInput Line number, where scanning of input should be
     */
    static int GetChar(int lineOfInput);

    /**
     * @brief Function reading user input based on which user wanted to move. Creating controls vector based on player and then validating user's move.
     * @param playerChar Char representing player
     * @param userVal Value read by NCurses (getChar()) to process
     * @param m_pureMapData Map without objects to validate borders etc.
     * @param m_bombs MapContainer vector keeping active bombs inside. Adding new bomb, if user decided to plant one
     */
    void HandleMove(char playerChar, int userVal, std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs);

    /**
    * @brief When user changes position, this method manage it + is responsible for checking if the move is possible
    * @brief Also the method manages placing of bombs.
    */
    void Move(std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs, CPosition playerPosition) override;

    /**
     * @return Unique pointer representing Player
     */
    std::unique_ptr<CEntity> CreatePtr() const override;

    /**
     * @return Char representing player
     */
    char ToString() const override;

private:
    //Player specific atributes inside CPlayer Class
    CPlayerStats & m_playerStats;
};

#endif //PLAYER_H
