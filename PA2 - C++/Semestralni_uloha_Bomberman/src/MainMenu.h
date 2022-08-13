#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
//#include <MANY OTHER INCLUDES>

/**
 *  @class CMainMenu
 *  @brief Creating menu, handling menu, nearly nothing related to actual game or game settings
 */
class CMainMenu
{
public:

    /**
     * Default CMainMenu constructor
     */
    CMainMenu() = default;

    /**
     * Default CMainMenu destructor
     */
    ~CMainMenu() = default;

    /**
     *  @brief Entry point for application, manages everything before starting gameplay settings
     */
    void Run();
private:

    /**
     *  @brief Render main menu (start of game)
     *  @param endOfGame flag is true, if the game should end
     */
    void MainMenu(bool & endOfGame);

    /**
     * @brief Hall of fame printing
     * @param endOfGame Is true, if user wanted to end the game in ScoreMenu
     */
    static void ScoreMenu(bool & endOfGame);

    /**
     * @brief Printing help/controls for user
     * @param endOfGame Is true, if user wanted to end the game in Help
     */
    static void HelpMenu(bool & endOfGame);
};

#endif //GAME_MANAGER_H