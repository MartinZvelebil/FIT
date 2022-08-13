#ifndef GAMESETTINGSMENU_H
#define GAMESETTINGSMENU_H
#include <string>
#include <vector>
#include "Banner.h"
#include "UserInputMenu.h"
//#include <MANY OTHER INCLUDES>

/**
 *  @class CGameSettingsMenu
 *  @brief Accepting users input and his preferences about map settings. Class is responsible for reading pure data from
 *         files and pass them in raw, non-changed form to MapContainer
 */
class CGameSettingsMenu
{
public:
    /**
    * @brief From fixed folder read all available maps when creating CLoadMap object
    */
    CGameSettingsMenu();

    /**
     * Default CGameSettingsMenu destructor
     */
    ~CGameSettingsMenu() = default;

    /**
    *  @brief Create and handle user's nickname
    *  @returns Nickname of user
    *  @param TextForUser specify, if this nickname is for second or first user
    */
    static std::string LoadNickNameMenu(CBanner & settingsBanner, std::string TextForUser);

    /**
    *  @brief Create and handle user's nickname
    */
    void LoadGameModeMenu(CBanner & settingsBanner);

    /**
     *  @brief Create and handle menu for map creation
     *  @param endPressed Is set true, if user chose to end game in menu creation
     */
     void LoadMapMenu(bool & endPressed);

    /**
     * @brief Decides for user, which map he is going to play
     */
    void ChooseRandomMap();

    /**
     * @brief Load data from chosen file to our map array, that will be retrieved by MapContainer in a future
     * @param mapNum is number representing index of the map path in m_knownMaps vector
     */
    void LoadMapData(int mapNum);

    /**
     * @brief Called from MapContainer to retrieve pure, not changed data. At this point, we will never have modified map,
     *        because we only got the data based on users preferences
     * @return 2D array of ASCII chars, that is accepted from MapContainer.cpp
     */
    std::vector<std::vector<char>> GetMapData();

    /**
     * @return true if map is well loaded and end wasn't chosen
     */
    bool IsMapReady() const;

    /**
     * @return User choice of game (AI/Player vs Player)
     */
    int GetGamemode() const;

    /**
     * @return Nickname of P1 (P)
     */
    std::string GetMainNickname();

    /**
     * @return Nickname of P2 (E)
     */
    std::string GetSecondaryNickname();

private:
    std::vector<std::string> m_knownMaps;
    std::vector<std::vector<char>> m_pureMapData;
    bool m_mapReady = false;
    std::string m_mainPlayerNickname;
    std::string m_secondaryPlayerNickname;
    //2 = AI Player (Against PC)
    //1 = Multiplayer with friend
    int m_gamemode;
};

#endif //GAMESETTINGSMENU_H
