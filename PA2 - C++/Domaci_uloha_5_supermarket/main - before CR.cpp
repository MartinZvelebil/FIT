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
        return tie(m_year,m_month,m_day) > tie(RightDate.m_year,RightDate.m_month,RightDate.m_day);
    }
    bool operator < (CDate RightDate) const
    {
        return tie(m_year,m_month,m_day) < tie(RightDate.m_year,RightDate.m_month,RightDate.m_day);
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
    //Equivalent of lambda function
    struct CustomCompare
    {
        bool operator()(const pair<CDate,int> l, const pair<CDate,int> r) {
            return l.first > r.first;
        }
    };

    //Must return CSupermarket so I can use it with tests
    CSupermarket & store (const string & name,
                          CDate date,
                          int count);
    vector<string> OCRMatch( list<pair<string,int>> & shop_list,
                             const list<pair<string,int>>::iterator & it) const;
    void sell ( list<pair<string,int>> & shop_list );
    void manageProducts (list<pair<string,int>>::iterator & it,
                         unordered_map<string, priority_queue <pair <CDate,int>, vector<pair<CDate,int>>, CustomCompare>>::iterator & product_match,
                         list<pair<string,int>> & shop_list);
    list<pair<string,int>> expired ( CDate ) const;

  private:
    unordered_map<string, priority_queue <pair <CDate,int>, vector<pair<CDate,int>>, CustomCompare> > products;
};

CSupermarket & CSupermarket::store (const string & name, CDate date, int count)
{
    //Performance abilities may be better with iterator returning the position of the product
    //If key already exists

    if(products.count(name) == 1)
    {
        products.at(name).push(make_pair(date,count));
    }
    else
    {
        //Create new product with new map
        //priority_queue<pair<CDate,int>> temp;
        priority_queue<pair<CDate,int>,vector<pair<CDate,int>>, CustomCompare> temp;
        temp.push(make_pair(date, count));
        products.insert(make_pair(name,temp));
    }
    return *this;
}

vector<string> CSupermarket::OCRMatch( list<pair<string,int>> & shop_list,
                                       const list<pair<string,int>>::iterator & it) const
{
    //Arr for right words save
    vector<string> matches = {};
    //OCR correction
    for (auto const& [key,val] : products) //Outer map
    {
        //If they don't have same length we don't have to check the OCR
        if(key.length() == it->first.length())
        {
            //misstypes = 0 shouldn't happen -> that would be strict match
            //misstypes = 1 means I found exactly one letter wrong, It is alright
            //misstypes = 2+ means I found more letters wrong, don't check further

            int misstypes = 0;
            for (unsigned int i = 0; i < key.length(); ++i) //Char by char iteration
            {
                if(misstypes > 2)
                    break;
                if(key[i] != it->first[i])
                    misstypes++;
            }
            if(misstypes == 1)
                matches.push_back(key); //Ulož si to slovo, je dobry, když byl překlep max v jednom písmenu
        }
        if(matches.size() > 1)
            return matches;

    }
    return matches;
}

void CSupermarket::manageProducts(list<pair<string,int>>::iterator & it,
                                  unordered_map<string, priority_queue <pair <CDate,int>, vector<pair<CDate,int>>, CustomCompare>>::iterator & product_match,
                                  list<pair<string,int>> & shop_list)
{

    while(it -> second > 0) //Dokud jsem nevydal dostatek položek k danému jménu
    {
        if(product_match->second.empty()) //Pokud priority queue produktu je empty, tak smolík, break
            break;

        //Nejnižší datum a jeho zboží
        if(product_match->second.top().second > it->second) //Pokud mám více zboží, než je žádáno, tak easiest case
        {
            //cout << " ENOUGH IN DATE | Have: " << product_match->first << " | Earliest date: " << product_match->second.top().first.toString() << " | Num: " <<  product_match->second.top().second ;
            //Musím si nahrát k sobě dočasně ten pair, abych ho mohl modifikovat, pak ho zase vrátím
            auto tmp = product_match->second.top();
            product_match->second.pop();
            tmp.second -= it->second;
            product_match->second.push(tmp);
            it->second = 0;
            //cout << " -> " << product_match->second.top().second << endl;
        }
        else //Produktu tam je s datem spotřeby málo, takže ho vyprodám a vyhodím z queue
        {
            //cout << " NOT ENOUGH IN DATE | Have: " << product_match->first << " | Earliest date: " << product_match->second.top().first.toString() << " | Num: " <<  product_match->second.top().second ;
            it->second -= product_match->second.top().second;
            //cout << " -> " << '0' << " ------- Left: " << it -> second << endl;
            product_match->second.pop();
        }
    }
    if(it -> second == 0)
    {
        it = shop_list.erase(it); //Zpracoval jsem daný produkt, takže ho vyhodím ze svého seznamu
    }
    else
    {
        it++;
    }
}

void CSupermarket::sell ( list<pair<string,int>> & shop_list)
{
    auto it = shop_list.begin();
    vector<decltype(products.begin())> toDelete;

    while(it != shop_list.end())
    {
        auto product_match = products.find(it->first); //Vrátí iterátor na místo, kde byl produkt nalezen

        if(product_match != products.end()) //We found pure match
        {
            //cout << "Wanted: " << it->first << " x" << it->second << endl;
            manageProducts(it, product_match, shop_list);
            if(products.at(product_match->first).empty())
                toDelete.push_back(product_match);
        }
        //If we couldn't find any strict match, let's try OCR correction method
        else
        {
            vector<string> matches = OCRMatch(shop_list, it);
            if(matches.size() == 1) //Když jsem nalezl jen jedno slovo s jedním překlepem
            {
                product_match = products.find(matches.front());
                //cout << "Wanted: " << it->first << " x" << it->second << " | Have: " << product_match->first << endl ;
                manageProducts(it, product_match, shop_list);
                if(products.at(product_match->first).empty())
                    toDelete.push_back(product_match);
            }
            else //Musím zvýšit iterátor, pokud bylo nalezeno více matches -> nedostal bych se do manageProducts
                it++;
        }
    }

    //Sort them for duplicities
    sort(toDelete.begin(),toDelete.end(), [](unordered_map<string, priority_queue <pair <CDate,int>, vector<pair<CDate,int>>, CustomCompare>>::iterator & left,
                                                            unordered_map<string, priority_queue <pair <CDate,int>, vector<pair<CDate,int>>, CustomCompare>>::iterator & right)
    {
        return left->first < right->first;
    });

    //Deleting products from map, if they are 0x
    for (size_t i = 0; i < toDelete.size(); ++i)
    {
        if(toDelete.size() - 1 == i)
        {
            products.erase(toDelete.at(i));
        }
        //Skip duplicities
        else if(toDelete.at(i)->first != toDelete.at(i+1)->first)
        {
            products.erase(toDelete.at(i));
        }
    }
}

list<pair<string,int>> CSupermarket::expired ( CDate expirationDate ) const
{
    //Using vector temporarily, because of weird error with sort on List
    vector<pair<string,int>> vect_output;

    //O(n * n) - WEAK IMPLEMENTATION
    for (auto & [key,val] : products) //Outer map
    {
        //Have to copy the priority_queue, because im poping from it
        auto tmpQueue = val;

        bool firstIterationWithName = false;
        while(!tmpQueue.empty())
        {
            auto & tmp = tmpQueue.top();
            if (tmp.first < expirationDate)
            {
                //If this is the first find of the given key, just create new pair
                if (!firstIterationWithName)
                {
                    vect_output.emplace_back(key, tmp.second); //Not good, duplicates present under strings
                    firstIterationWithName = true;
                }
                    //If the pair with given key already exists and expired was found, just add number of products to the second element of pair
                else
                {
                    vect_output.at(vect_output.size() -
                                   1).second += tmp.second; //MAY ERROR (when size is zero, but it shouldn't happen)
                }
            }
            else
                break;
            tmpQueue.pop();
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

    s . expired ( CDate ( 209, 224, 333 ) );

  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

    s . expired ( CDate ( 209, 224, 333 ) );

  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
  cout << "List 1 done" << endl << endl;

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );
  cout << "List 2 done"<< endl << endl;

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );
  cout << "List 3 done" << endl << endl;

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );
  cout << "List 4 done" << endl << endl;

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );
  cout << "List 5 done" << endl << endl;

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );
  cout << "List 6 done" << endl << endl;

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );
  cout << "List 7 done" << endl << endl;

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );
    cout << "List 8 done" << endl << endl;

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );
    cout << "List 9 done" << endl << endl;

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );
    cout << "List 10 done" << endl << endl;

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );
    cout << "List 11 done" << endl << endl;

    //PROČ JE TADY PRÁZDNEJ cake DOPÍČI
  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );
    cout << "List 12 done" << endl << endl;

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );
    cout << "List 13 done" << endl << endl;

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );
    cout << "List 14 done" << endl << endl;

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );
    cout << "List 15 done" << endl << endl;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
