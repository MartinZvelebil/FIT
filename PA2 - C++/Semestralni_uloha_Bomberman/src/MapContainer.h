#ifndef MAP_CONTAINER_H
#define MAP_CONTAINER_H
#include <string>
#include <vector>

#include "Player.h"
#include "Ghost.h"
#include "Bomb.h"

/**
 *  @class CMapContainer
 *  @brief Based on action refreshes data of the map and render changed map. Responsible for actions on the map and handling objects on map
 */
class CMapContainer
{
public:
    /**
     * @brief Constructor for MapContainer -> new object should allways have initialized everything
     * @param mapData RAW map data to load
     * @param gamemode is 1 if player vs player was set, is 2 if player vs AI was set
     */
    CMapContainer(std::vector<std::vector<char>> & mapData, int gamemode);

    /**
     * Default CMapContainer destructor
     */
    ~CMapContainer() = default;

    /**
     *  @brief Method responsible for polymorphic moving of every entity
     */
    void MoveEntities();

    /**
     *  @brief Decide, where will players, ghosts, enemy etc. spawn based on map structure (clever spawning of ghosts)
     *  @param mainPlayerNick Nickname of player 1 ('P')
     *  @param secondaryPlayerNick Nickname of player 2 ('E')
     */
    void LoadObjectives(std::string & mainPlayerNick, std::string & secondaryPlayerNick);

    /**
     *  @brief Adding monsters to the map + handling collisions of players and ghosts
     *  @param endOfGame If ghost kills one of players, than endOfGame flag is set true
     */
    void AddAndHandleEntitiesOnMap(bool & endOfGame);

    /**
     *  @brief Adding bombs to the map + controlling the explosion
     *  @param endOfGame If bomb kills one of players, than endOfGame flag is set true
     */
    void AddAndHandleBombOnMap(bool & endOfGame);

    /**
     *  @brief Handling bonuses on the map, based on their visibility at the moment (it means you can't pickup bonus, while you are being hitted by ghost)
     *  @param i index of the line/vector in map (y axis)
     *  @param j index of the element in the line in map (x axis)
     */
    void HandleBonusesOnMap(size_t i, size_t j);

    /**
     * @brief Clears console and render map with changed data, also is responsible for colisions, adding monsters etc
     * @param endOfGame If bomb/ghost kills one of players, than endOfGame flag is set true
     */
    void RenderMap(bool & endOfGame);

    /**
     * @brief Printing winner and sleeps for 7 seconds to see scores etc.
     * @param winnerQuote Quote to print for user, who is the winner
     */
    void EndScreenHandler(std::string winnerQuote);

    /**
     * @brief Generating the flame + destroying things around
     * @param flame vector of positions, where the flame is
     * @param endOfGame If bomb killed player
     */
    void HandleBombExplosion(std::vector<CPosition> flame, bool & endOfGame);

    /**
     * @brief Method called at the end of the game, responsible for writing to the scores file
     * @param score Score of the player
     * @param nickname Nickname of the player
     */
    static void SaveScoreToFile(int score, std::string nickname);
private:

    /**
     * @brief Private method for finding longest subsequence in given vector of positions
     * @return Vector of positions, that will be used for ghosts's range of motion
     * @param ghostBlocks Vector of positions, where ' ' or ':' blocks are located
     */
    static std::vector<CPosition> LongestSubsequence(std::vector<CPosition> ghostBlocks);

    /**
     * @brief m_WholeMapData is representing game with everything (entities, bombs etc.)
     *        In every tick I copy whole m_PureMapData to m_WholeMapData and and generate entities on the map
     */
    std::vector<std::vector<char>> m_wholeMapData;

    /**
     * @brief m_PureMapData is representing game without entities and bombs
     */
    std::vector<std::vector<char>> m_pureMapData;

    /**
     * @brief All my entities together in one vector of unique pointers for polymorphic behaviour
     */
    std::vector<std::unique_ptr<CEntity>> m_entities;
    std::vector<CPlayerStats> m_playerStats; //index 0 = Player P, index 1 = Player E (same as m_entities)
    std::vector<std::unique_ptr<CBomb>> m_bombs;
    int m_gamemode;
};

#endif //MAP_CONTAINER_H