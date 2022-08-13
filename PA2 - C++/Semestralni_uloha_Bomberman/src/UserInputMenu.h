#ifndef USERMENU_H
#define USERMENU_H
#include <vector>
#include <string>

/**
 * @class CUserMenu
 * @brief Class responsible for printing user options and handling user's choice
 */
class CUserMenu
{
public:
    /**
     * @brief Initialize object CUserMenu, with m_options -> options user have
     * @param options Options that user have
     */
    CUserMenu(std::vector<std::string> & options);

    /**
     * Default CUserMenu destructor
     */
    ~CUserMenu() = default;

    /**
     * @brief Prints menu and check, if user's input is valid
     * @param wrongInput
     * @param userInput
     * @return -1 if user's choice was valid
     * @return between 1 and options.size() if user's choice was valid (within range)
     */
    int HandleMenu(bool & wrongInput, int & userInput);

private:
    /**
     * @brief Options to display for user
     */
    std::vector<std::string> m_options;
};

#endif //USERMENU_H