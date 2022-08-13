#include <vector>
#include <iostream>
#include <string>
#include <limits>

#include "UserInputMenu.h"
using namespace std;

CUserMenu::CUserMenu(vector<string> & options)
{
    string tmp;
    for (size_t i = 0; i < options.size(); ++i)
    {
        tmp = to_string(i+1) + ") " + options.at(i);
        m_options.push_back(tmp);
    }
}

int CUserMenu::HandleMenu(bool & wrongInput, int & userInput)
{
    for (size_t i = 0; i < m_options.size(); ++i)
    {
        cout << m_options.at(i) << endl;
    }

    if(wrongInput)
        cout << "WRONG CHOICE! Choose number 1-" << m_options.size() << endl;

    cout << endl << "Your choice: ";

    cin >> userInput;
    if(userInput >= 1 && userInput <= (int)m_options.size())
        return userInput;
    else
    {
        if(cin.eof())
        {
            throw runtime_error("CTRL+D was pressed!");
        }
        wrongInput = true;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear cin buffer, to prevent infinite loop
        return -1;
    }
}
