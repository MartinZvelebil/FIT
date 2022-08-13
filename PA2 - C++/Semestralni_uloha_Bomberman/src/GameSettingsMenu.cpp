#include <iostream>
#include <fstream>
#include <filesystem>

#include "GameSettingsMenu.h"
#include "UserInputMenu.h"
#include "Block.h"
using namespace std;

CGameSettingsMenu::CGameSettingsMenu()
{
    //CLION is starting in different location, so for CLion debug purposes I have the absolute path here
    //string pathToMaps = "/mnt/c/Users/marti/CLionProjects/Semestralni_Prace_Bombermann/examples/maps";
    string pathToMaps = (string)filesystem::current_path() + "/examples/maps";
    m_gamemode = 1;
    for (const auto & dir_entry : filesystem::directory_iterator(pathToMaps))
        m_knownMaps.push_back(dir_entry.path());
}

string CGameSettingsMenu::LoadNickNameMenu(CBanner & settingsBanner, string TextForUser)
{
    int userInput = -1;
    bool wrongInput = false;
    string tmpNickname;
    while(userInput == -1)
    {
        if(cin.eof())
        {
            throw runtime_error("CTRL+D was pressed.");
        }
        userInput = 1;
        cout << u8"\033[2J\033[1;1H";
        settingsBanner.PrintBannerGameSettings();
        if(wrongInput)
            cout << "Please keep in mind: Your nickname has to be only with letters or numbers. Length of name must be within 3-20 characters." << endl;
        cout << TextForUser;
        getline(cin,tmpNickname);
        if(tmpNickname.length() > 20 || tmpNickname.length() < 3)
        {
            userInput = -1;
            wrongInput = true;
        }
        else
        {
            for (auto i : tmpNickname)
            {
                if(isalnum(i) == false || i == ' ')
                {
                    userInput = -1;
                    wrongInput = true;
                }
            }
        }
    }
    return tmpNickname;
}

void CGameSettingsMenu::LoadGameModeMenu(CBanner & settingsBanner)
{
    int userInput = -1;
    bool wrongInput = false;
    vector <string> options = {"Play with friend", "Play against computer"};
    CUserMenu userMenu(options);
    while(userInput == -1)
    {
        cout << u8"\033[2J\033[1;1H";
        settingsBanner.PrintBannerGameSettings();
        userInput = userMenu.HandleMenu(wrongInput,userInput);
    }
    m_gamemode = userInput;
}

void CGameSettingsMenu::LoadMapMenu(bool & endPressed)
{
    int userInput = -1;
    bool wrongInput = false;
    //Maybe I will add option for loading from different folders (relative, absolute paths)
    vector <string> options = {"Choose map from default folder", "Choose random map from default folder", "End"};
    CUserMenu userMenu(options);
    CBanner settingsBanner;

    //Getting nickname from user before any game settings
    m_mainPlayerNickname = LoadNickNameMenu(settingsBanner, "Enter nickname for P1: ");
    //Getting usermode from user
    LoadGameModeMenu(settingsBanner);
    //If gamemode is playing with friends, than get also his username
    if(m_gamemode == 1)
        m_secondaryPlayerNickname = LoadNickNameMenu(settingsBanner, "Enter nickname for P2: ");
    else
        m_secondaryPlayerNickname = "AIPlayerNick:)"; //Nobody can choose this username since it has special characters

    while(userInput == -1)
    {
        cout << u8"\033[2J\033[1;1H";
        settingsBanner.PrintBannerGameSettings();
        userInput = userMenu.HandleMenu(wrongInput,userInput);
    }

    CUserMenu mapOptions(m_knownMaps);
    int userInput_mapchoice = -1;
    switch(userInput)
    {
        case 1:
            //Showing user options for maps and let him choose and load data after that
            while (userInput_mapchoice == -1)
            {
                cout << u8"\033[2J\033[1;1H";
                settingsBanner.PrintBannerGameSettings();
                if(m_knownMaps.empty())
                    throw runtime_error("ERROR: No maps found, check maps folder!");
                cout << "Map options..." << endl;
                userInput_mapchoice = mapOptions.HandleMenu(wrongInput, userInput_mapchoice);
            }
            LoadMapData(userInput_mapchoice-1); //-1, because we need index of the map
            break;
        case 2:
            cout << "Choosing random map..." << endl;
            ChooseRandomMap();
            break;
        case 3:
            cout << "THANK YOU FOR PLAYING!..." << endl;
            endPressed = true;
            break;
        default:
            throw runtime_error("Received unexpected value, that shouldn't be possible.");
    }

    //Check m_mapReady flags (also checking if end of game wasn't chosen)
    if(m_mapReady)
        cout << "Let's play!" << endl;
    else if(!m_mapReady && userInput != 3)
    {
        throw runtime_error("Something bad happend with loading map, please check format validity of the map!");
    }
}

void CGameSettingsMenu::LoadMapData(int mapNum)
{
    ifstream mapFile;
    mapFile.open(m_knownMaps.at(mapNum));
    if(!mapFile.is_open())
    {
        throw runtime_error("ERROR: Can't open file" + m_knownMaps.at(mapNum));
    }
    else //Convert lines of file to 2D Vector array
    {
        string line;
        int countline = 0;
        while(getline(mapFile,line))
        {
            vector<char> tmp;
            for (size_t i = 0; i < line.length()-1; ++i) //-1 because last char is newline
            {
                if(line[i] == CBlock::GetBlockChar(CBlock::WALL) || line[i] == CBlock::GetBlockChar(CBlock::DESTRUCTABLE_WALL) ||
                   line[i] == CBlock::GetBlockChar(CBlock::SCORE_BOOST) || line[i] == CBlock::GetBlockChar(CBlock::SPACE) ||
                   line[i] == CBlock::GetBlockChar(CBlock::BONUS_BOMB_RANGE) || line[i] == CBlock::GetBlockChar(CBlock::BONUS_BOMB))
                    {tmp.push_back(line[i]);}
                else
                {
                    throw runtime_error("Map contains unwanted characters!");
                    m_mapReady = false;
                }
            }
            m_pureMapData.push_back(tmp);
            countline++;
        }
    }
    mapFile.close();

    //Checking, if the file wasn't empty, less than 5 lines or less than 20 rows or more than 25 lines or 100 rows
    if(m_pureMapData.empty() || m_pureMapData.size() < 5 || m_pureMapData.at(0).size() < 20
    || m_pureMapData.size() > 25 || m_pureMapData.at(0).size() > 100)
    {
        m_mapReady = false;
        return;
    }

    //Map must have newline at the end of the file, otherwise fail will happen
    //Checking, if every line is the same length, if isn't, map is not valid!
    size_t linesize = m_pureMapData.at(0).size(); //First line length
    for (size_t i = 1; i < m_pureMapData.size(); ++i) //Lines
    {
        if(linesize != m_pureMapData.at(i).size())
        {
            m_mapReady = false;
            return;
        }
    }

    //For each vector check, if first and last vector is only made from # and other vectors start and end with #
    for (size_t i = 0; i < m_pureMapData.size(); ++i)
    {
        //This ugly for will happen only twice (first and last line) -> checking if whole lane is border
        if(i == 0 || i == m_pureMapData.size()-1)
        {
            for (char j : m_pureMapData.at(i))
            {
                if(j != CBlock::GetBlockChar(CBlock::WALL))
                {
                    m_mapReady = false;
                    return;
                }
            }
        }
        else if( m_pureMapData.at(i).at(0) != CBlock::GetBlockChar(CBlock::WALL) ||
                 m_pureMapData.at(i).at(m_pureMapData.at(i).size() - 1) != CBlock::GetBlockChar(CBlock::WALL))
        {
            m_mapReady = false;
            return;
        }
    }

    //At this point, our map should be valid
    m_mapReady = true;
}

void CGameSettingsMenu::ChooseRandomMap()
{
    //If maps folder is empty
    if(m_knownMaps.empty())
    {
        throw runtime_error("ERROR: No maps found, check maps folder!");
    }

    //Random number generator for maps, don't choose maps with ERR/EXA Prefix - choose only OK prefixes
    bool errMap = true;
    int mapNum;
    srand(time(nullptr));
    while(errMap)
    {
        mapNum = rand() % m_knownMaps.size() + 1;
        mapNum--;
        if(m_knownMaps.at(mapNum).find("OK") != string::npos)
            errMap = false;
    }
    cout << endl << "Randomly chosen map is: " << m_knownMaps.at(mapNum) << endl;
    CGameSettingsMenu::LoadMapData(mapNum);
}

vector<vector<char>> CGameSettingsMenu::GetMapData()
{
    return m_pureMapData;
}

bool CGameSettingsMenu::IsMapReady() const
{
    return m_mapReady;
}

int CGameSettingsMenu::GetGamemode() const
{
    return m_gamemode;
}

std::string CGameSettingsMenu::GetMainNickname()
{
    return m_mainPlayerNickname;
}

std::string CGameSettingsMenu::GetSecondaryNickname()
{
    return m_secondaryPlayerNickname;
}