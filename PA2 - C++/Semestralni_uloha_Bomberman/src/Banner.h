#ifndef BANNER_H
#define BANNER_H
#include <vector>
#include <string>

/**
 * @class CBanner
 * @brief Passive class representing banner
 */
class CBanner
{
public:

    /**
    * Default CBanner constructor
    */
    CBanner() = default;

    /**
     * Default CBanner destructor
     */
    ~CBanner() = default;

    /**
    * @brief Prints ASCII art BOMBERMAN (Font Slant)
    * Credits to: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
    */
    void PrintBannerBomberman ();

    /**
     * @brief Prints ASCII art HALLOFFAME
     * Credits to: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
     */
    void PrintBannerHallOfFame ();

    /**
     * @brief Prints ASCII art GAME SETTINGS (Font Slant)
     * Credits to: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
     */
    void PrintBannerGameSettings ();

    /**
    * @brief Prints ASCII art HELP (Font Slant)
    * Credits to: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
    */
    void PrintBannerHelp ();
};
#endif //BANNER_H
