#ifndef AIPLAYER_H
#define AIPLAYER_H
#include <vector>
#include <memory>
#include <deque>

#include "Entity.h"
#include "PlayerStats.h"

/**
 * @brief Class representing AI player
 * @class CAIPlayer
 */
class CAIPlayer : public CEntity
{
public:
    /**
    * @brief Constructor, that will create & initialize new Player with basic parameters
    * @param pos Position of the player on the map
    * @param hearts Number of hearts AI player has
    * @param charval Character that will be displayed on map
    * @param playerStats Reference on AI player stats (player-specific statistics like score). Handled in different object for better object behaviour.
    */
    CAIPlayer(CPosition pos, int hearts, char charval, CPlayerStats & playerStats);

    /**
    * Default CAIPlayer destructor
    */
    virtual ~CAIPlayer() = default;

    /**
     * @brief Function is used, when BFS Couldn't find any possible way between players
     */
    void RandomizeMove(std::vector<std::vector<char>> & m_pureMapData);

    /**
     *  @brief Handle if planting bomb is possible and if so creates new bomb object, plants bomb
     */
    void PlantBomb(std::vector<std::unique_ptr<CBomb>> & m_bombs);

    /**
     * @brief Calculate BFS and move the best direction possible. It is working in different direction (finding path from end to start)
     * @brief That's because it is way more efficient, because my next step is the last step calculated before finding finish.
     * @param start Starting position (allways position of AI player)
     * @param end End position (allways position of other player)
     * @param m_pureMapData Map data used for validation of steps
     * @param m_bombs If player's positions are equal, new bomb is planted and unique pointer is added to MapContainer's m_bombs
     */
    void BFSPath(CPosition start, CPosition end, std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs);

    /**
    * @brief When AI changes position, this method manage it + is responsible for checking if the move is possible
    * @brief Also the method manages placing of bombs.
    */
    void Move(std::vector<std::vector<char>> & m_pureMapData, std::vector<std::unique_ptr<CBomb>> & m_bombs, CPosition playerPosition) override;

    /**
     * @return Char representing player
     */
    char ToString() const override;

    /**
     * @return Unique pointer representing AI Player
     */
    std::unique_ptr<CEntity> CreatePtr() const override;

private:
    CPlayerStats & m_playerStats;
};


#endif //AIPLAYER_H
