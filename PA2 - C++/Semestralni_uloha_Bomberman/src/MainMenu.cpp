#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <chrono>
#include <thread>
#include <curses.h>

#include "Banner.h"
#include "MainMenu.h"
#include "GameSettingsMenu.h"
#include "UserInputMenu.h"
#include "MapContainer.h"
using namespace std;

void CMainMenu::Run()
{
    bool endOfGame = false;
    while(!endOfGame)
    {
        clear(); //Clears inside initialization of ncurses
        MainMenu(endOfGame);
    }
}

void CMainMenu::MainMenu(bool & endOfGame)
{
    bool wrongInput = false;
    vector<string> options = {"Start Game", "Highest scores", "Help / Controls", "End"};
    CUserMenu userMenu(options);
    CBanner banner;
    CGameSettingsMenu gameSettings;

    while (true)
    {
        int userInput = -1;
        while (userInput == -1)
        {
            cout << u8"\033[2J\033[1;1H"; //Equivalent of system("clear"), but with ANSI Escape Sequences
            banner.PrintBannerBomberman();
            userInput = userMenu.HandleMenu(wrongInput, userInput);
        }
        switch (userInput)
        {
            case 1:
                cout << "Starting game settings..." << endl;
                gameSettings.LoadMapMenu(endOfGame);

                if (gameSettings.IsMapReady())
                {
                    cout << "Starting game..." << endl;
                    vector<vector<char>> tmpMapData = gameSettings.GetMapData();
                    string mainPlayerNick = gameSettings.GetMainNickname();
                    string secondaryPlayerNick = gameSettings.GetSecondaryNickname();
                    CMapContainer playersMap(tmpMapData, gameSettings.GetGamemode());
                    playersMap.LoadObjectives(mainPlayerNick, secondaryPlayerNick);
                    //END OF ONE-TIME INICIALIZATION

                    while(!endOfGame)
                    {
                        playersMap.RenderMap(endOfGame);
                        playersMap.MoveEntities();
                        this_thread::sleep_for(chrono::milliseconds(150)); //Maybe change position of this sleep
                    }
                    //EndOfGame flag was set true to exit the game-loop, but actually I want to return to mainMenu
                    endOfGame = false;
                }
                wrongInput = false;
                break;
            case 2:
                cout << "Showing table of scores..." << endl;
                ScoreMenu(endOfGame);
                wrongInput = false;
                break;
            case 3:
                HelpMenu(endOfGame);
                wrongInput = false;
                break;
            case 4:
                cout << "THANK YOU FOR PLAYING!..." << endl;
                endOfGame = true;
                break;
            default:
                throw runtime_error("Received unexpected value, that shouldn't be possible.");
        }
        //If end was pressed on menu, we would want to completely end game
        //Second case is when game ended and we are returning to new menu
        if(endOfGame || (userInput == 1 && !endOfGame))
            break;
    }
}

void CMainMenu::ScoreMenu(bool & endOfGame)
{
    int userInput = -1;
    bool wrongInput = false;
    vector<string> options = {"Back to Main Menu" , "End"};
    CUserMenu scoreMenu(options);
    CBanner scoreBanner;

    while(userInput == -1)
    {
        //Clearing terminal + banner printing, basic initialization
        cout << u8"\033[2J\033[1;1H";
        scoreBanner.PrintBannerHallOfFame();

        multimap<int, string, greater<>> database; //Keeping database descending
        auto it = database.begin();
        ifstream scoreFile;
        scoreFile.open("assets/scores.txt");

        //File couldn't be opened
        if(!scoreFile)
        {
            throw runtime_error("ERROR: Can't open file assets/scores.txt - check if exists!");
        }

        //Getting data from file to my database (map)
        string tmpNickname;
        int tmpScore;
        while(scoreFile >> tmpNickname >> tmpScore)
        {
            database.insert(it, pair<int, string>(tmpScore,tmpNickname));
        }

        //Header printing
        cout << "Pos";
        cout << setw(28);
        cout << "Nickname | Score" << endl;

        //If database is empty, tell user, that scores file is empty, else show scores
        if(database.empty())
        {
            cout << "No scores in file assets/scores.txt, but file is present!" << endl;
        }
        else
        {
            int position = 1;
            for (it=database.begin(); it!=database.end(); ++it)
            {
                if(position >= 10) //Prevention for endless/too long output (only TOP 9 scores)
                    break;
                cout << position++ << ". ";
                cout << setw(20);
                cout << it->second << " | " << it->first << endl;
            }
        }

        //Printing options for user, handling user's input
        cout << endl << endl;
        userInput = scoreMenu.HandleMenu(wrongInput,userInput);
    }

    switch (userInput)
    {
        case 1:
            cout << "Returning to MAIN MENU" << endl;
            break;
        case 2:
            cout << "THANK YOU FOR PLAYING!..." << endl;
            endOfGame = true;
            break;
        default:
            throw runtime_error("Received unexpected value, that shouldn't be possible.");
    }
}

void CMainMenu::HelpMenu(bool & endOfGame)
{
    int userInput = -1;
    bool wrongInput = false;
    vector<string> options = {"Back to Main Menu" , "End"};
    CUserMenu helpMenu(options);
    CBanner scoreBanner;

    while(userInput == -1)
    {
        cout << u8"\033[2J\033[1;1H";
        scoreBanner.PrintBannerHelp();
        cout << "- Bomberman game made by Martin Zvelebil (zvelemar) as project at FIT CVUT" << endl << endl;
        cout << "- Game can be played in 2 gamemodes:" << endl;
        cout << "  1. Player vs Player" << endl;
        cout << "  2. Player vs PC" << endl << endl;
        cout << "- Controls:" << endl;
        cout << "  P1: WASD (moving) + C (bomb planting)" << endl;
        cout << "  P2: ARROWS (moving) + M (bomb planting)" << endl << endl;
        cout << "- Map objects:" << endl;
        cout << "  P - Player one" << endl;
        cout << "  E - Player two / AI" << endl;
        cout << "  # - Unbreakable wall" << endl;
        cout << "  : - Breakable wall" << endl;
        cout << "  @ - Ghost" << endl;
        cout << "  a - Adding one bomb to maximum number of bombs" << endl;
        cout << "  O - Permanent boost of bomb explosion range, single one on the map" << endl;
        cout << "  P - Bomb" << endl;
        cout << "  * - Bomb effect" << endl << endl << endl;
        userInput = helpMenu.HandleMenu(wrongInput,userInput);
    }

    switch (userInput)
    {
        case 1:
            cout << "Returning to MAIN MENU" << endl;
            break;
        case 2:
            cout << "THANK YOU FOR PLAYING!..." << endl;
            endOfGame = true;
            break;
        default:
            throw runtime_error("Received unexpected value, that shouldn't be possible.");
    }
}
