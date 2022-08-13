#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
//Product handle -> rozpracovaná nechutnost
/*while(!product_match -> second.empty())
            {
                if(rm_shop_l.front().second == 0) //Už jsem všechno zboží nalezl
                    break;
                if(product_match -> second.empty()) //Pokud už v tom zboží s tím jménem není žádné zboží
                    break;
                //Dokud nemám produkt prázdný NEBO už jsem ho nevyprodal dostatečně, tak budu postupně po datumech vyhazovat to zboží
                //Pokud ho vyprodám a shopping list ještě to zboží obsahuje, nesmím ho pop_frontnout, musím dát jen continue a v další iteraci ho zkusit
                //najít znovu

                //Case, kdy mám
                if(product_match -> second.begin() -> second >= rm_shop_l.front().second)
                {
                    product_match -> second.begin() -> second -= rm_shop_l.front().second;
                    rm_shop_l.front().second = 0;
                    cout << product_match -> second.begin()->first.toString() << " | " << product_match -> second.begin()->second << endl; //Pocet kusu
                }
                else if(product_match -> second.begin() -> second < rm_shop_l.front().second)
                {
                    rm_shop_l.front().second -= product_match -> second.begin() -> second;
                    cout << product_match -> second.begin()->first.toString() << " | " << product_match -> second.begin()->second << endl; //Pocet kusu
                    product_match -> second.erase(product_match -> second.begin());
                }
                //product_match -> second.begin()->first; //CDate
                //product_match -> second.erase(product_match -> second.begin());
            } */
class CDate
{
  public:
     CDate ( int year, int month, int day)
     {
         m_year = year;
         m_month = month;
         m_day = day;
     }
    //Function for nulling hours, minutes and seconds in tm structure
    static void NullHours(tm & Rdate)
    {
        Rdate.tm_hour = 0;
        Rdate.tm_min = 0;
        Rdate.tm_sec = 0;
    }

    bool operator == (CDate RightDate) const
    {
         if(this->m_year == RightDate.m_year && this ->m_month == RightDate.m_month && this -> m_day == RightDate.m_day)
             return true;
         else
             return false;
    }
    bool operator > (CDate RightDate) const
    {
        tm right_date = {0};
        right_date.tm_year = RightDate.m_year-1900; right_date.tm_mon = RightDate.m_month-1; right_date.tm_mday = RightDate.m_day;
        NullHours(right_date);

        tm this_date = {0};
        this_date.tm_year = this->m_year-1900; this_date.tm_mon = this->m_month-1; this_date.tm_mday = this->m_day;
        NullHours(this_date);

        time_t right_date_sec = mktime(&right_date);
        time_t this_date_sec = mktime(&this_date);
        if(this_date_sec > right_date_sec)
            return true;
        else
            return false;
    }
    bool operator < (CDate RightDate) const
    {
        tm right_date = {0};
        right_date.tm_year = RightDate.m_year-1900; right_date.tm_mon = RightDate.m_month-1; right_date.tm_mday = RightDate.m_day;
        NullHours(right_date);

        tm this_date = {0};
        this_date.tm_year = this->m_year-1900; this_date.tm_mon = this->m_month-1; this_date.tm_mday = this->m_day;
        NullHours(this_date);

        time_t right_date_sec = mktime(&right_date);
        time_t this_date_sec = mktime(&this_date);
        if(this_date_sec < right_date_sec)
            return true;
        else
            return false;
    }
    string toString() const
    {
         string a = to_string(m_year) + "/" + to_string(m_month) + "/" + to_string(m_day);
         return a;
    }
  private:
    int m_year;
    int m_month;
    int m_day;
};

class CSupermarket {
public:
    CSupermarket() = default;

    //Must return CSupermarket so I can use it with tests
    CSupermarket & store (const string & name, CDate date, int count);
    vector<string> OCRMatch( list<pair<string,int>> & shop_list );
    void sell ( list<pair<string,int>> & shop_list );
    list<pair<string,int>> expired ( CDate );

  private:
    //unordered_map<string, priority_queue<pair<CDate,int>>> products;
    //map<string, map<CDate, int>> products;
    unordered_map<string, map<CDate, int>> products;
};


CSupermarket & CSupermarket::store (const string & name, CDate date, int count)
{
    //Performance abilities may be better with iterator returning the position of the product
    //If key already exists
    if(products.count(name) == 1)
    {
        //If there is product with name and also the same expiracy date, just += count to the second map value
        if(products.at(name).count(date) == 1)
        {
             products.at(name).at(date) += count;
        }
        //If there is product with name, but without this expiracy date, create new in second map
        else
        {
            products.at(name).insert(make_pair(date,count)); //Where is my product located(index of the map i want insert to)
        }
    }
    else
    {
        //Create new product with new map
        products.insert(make_pair(name,map<CDate,int>{make_pair(date,count)}));
    }
    return *this;
}

vector<string> CSupermarket::OCRMatch( list<pair<string,int>> & shop_list )
{
    //Arr for right words save
    vector<string> matches = {};
    //OCR correction
    for (auto const& [key,val] : products) //Outer map
    {
        //If they don't have same length we don't have to check the OCR
        if(key.length() == shop_list.front().first.length())
        {
            //misstypes = 0 shouldn't happen -> that would be strict match
            //misstypes = 1 means I found exactly one letter wrong, It is alright
            //misstypes = 2+ means I found more letters wrong, don't check further

            int misstypes = 0;
            for (int i = 0; i < key.length(); ++i) //Char by char iteration
            {
                if(misstypes > 2)
                    break;
                if(key[i] != shop_list.front().first[i])
                    misstypes++;
            }
            if(misstypes == 1)
                matches.push_back(key); //Ulož si to slovo, je dobry, když byl překlep max v jednom písmenu
        }
    }
    return matches;
}

void CSupermarket::sell ( list<pair<string,int>> & shop_list)
{
    list<pair<string,int>> rm_shop_l = shop_list;

    while(!rm_shop_l.empty())
    {
        auto product_match = products.find(rm_shop_l.front().first); //Vrátí iterátor na místo, kde byl produkt nalezen
        //We found pure match
        if(product_match != products.end())
        {
            cout << "Wanted: " << rm_shop_l.front().first << " x" << rm_shop_l.front().second << " | Have: " << product_match->first << endl ;
            //HAVE THE POSITION OF THE RIGHT WORD (product_match)

            rm_shop_l.pop_front(); //Pouze, pokud jsem dosáhl toho, že jsem to plně vyprodal
        }
        //If we couldn't find any strict match, let's try OCR correction method
        else
        {
            vector<string> matches = OCRMatch(rm_shop_l);
            if(matches.size() == 1) //Když jsem nalezl jen jedno slovo s jedním překlepem
            {
                product_match = products.find(matches.front());
                cout << "Wanted: " << rm_shop_l.front().first << " x" << rm_shop_l.front().second << " | Have: " << product_match->first << endl ;
                //HAVE THE POSITION OF THE RIGHT WORD - AFTER OCD (product_match)

                rm_shop_l.pop_front(); //Pouze, pokud jsem dosáhl toho, že jsem to plně vyprodal, pokud jsem plně nevyprodal, tak musím projet další iterací toho stejného itemu
            }
            else
            {
                rm_shop_l.pop_front(); //Pouze, pokud jsem nenašel match
            }
        }
    }
}
list<pair<string,int>> CSupermarket::expired ( CDate expirationDate )
{
    //Using vector temporarily, because of weird error with sort on List
    vector<pair<string,int>> vect_output;

    //O(n * n) - WEAK IMPLEMENTATION
    for (auto const& [key,val] : products) //Outer map
    {
        bool firstIterationWithName = false;
        for (auto const& [key2,val2] : val) //Inner map <DATE, INT>
        {
            if(key2 < expirationDate)
            {
                //If this is the first find of the given key, just create new pair
                if(!firstIterationWithName)
                {
                    vect_output.emplace_back(key, val2); //Not good, duplicates present under strings
                    firstIterationWithName = true;
                }
                //If the pair with given key already exists and expired was found, just add number of products to the second element of pair
                else
                {
                    vect_output.at(vect_output.size()-1).second += val2; //MAY ERROR (when size is zero, but it shouldn't happen)
                }
            }
        }
    }

    //Sorting based on second parameter (num of items)
    sort(vect_output.begin(),vect_output.end(), [](const pair<string,int> &a,
                                                                  const pair<string,int> &b)
         {
             return (a.second > b.second);
         }
    );

    //Converting to List here, because some random things don't let me sort list, but only vector
    list<pair<string,int>> output(vect_output.begin(),vect_output.end());

    return output;
}

#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
