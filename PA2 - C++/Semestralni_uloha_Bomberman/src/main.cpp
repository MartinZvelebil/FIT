#include <iostream>
#include <exception>

#include "MainMenu.h"

int main()
{
    try
    {
        CMainMenu Bomberman;
        Bomberman.Run();
    }
    catch (std::runtime_error & exception)
    {
        std::cout << "Error during game: "<< exception.what() << std::endl;
    }

    return 0;
}
