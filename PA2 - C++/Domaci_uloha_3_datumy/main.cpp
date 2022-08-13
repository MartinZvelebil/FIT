#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
    InvalidDateException ( )
            : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
public:
    //Konstruktor
    CDate(int year, int month, int day)
    {
        date.tm_year = year;
        date.tm_mon = month;
        date.tm_mday = day;
        if(!validateDate(date))
        {
            throw InvalidDateException();
        }
        //Must convert the actual date to output, that would be accepted by mktime etc.
        date.tm_year -= 1900;
        date.tm_mon -= 1;
    }
    CDate operator + (int num);
    CDate operator - (int num);
    int operator - (CDate date1);
    bool operator == (CDate Rdate);
    bool operator != (CDate Rdate);
    bool operator >= (CDate Rdate);
    bool operator <= (CDate Rdate);
    bool operator < (CDate Rdate);
    bool operator > (CDate Rdate);
    CDate &operator ++ ();
    CDate operator ++ (int);
    CDate &operator -- ();
    CDate operator -- (int);

    //Friend enable access for methods to private parts of class
    friend ostream & operator << ( ostream & os, const CDate & x );
    friend istream & operator >> ( istream & is, CDate & x );
    friend void NullHours(CDate & Rdate);

private:
    tm date = {0};
    static bool validateDate(tm Rdate)
    {
        //Initial basic check
        if(Rdate.tm_year > 2030 || Rdate.tm_year < 2000 || Rdate.tm_mon > 12 || Rdate.tm_mon < 1 || Rdate.tm_mday < 1 || Rdate.tm_mday > 31)
            return false;

        //Months with 30 days
        if(Rdate.tm_mon == 4 || Rdate.tm_mon == 6 || Rdate.tm_mon == 9 || Rdate.tm_mon == 11) {
            if (Rdate.tm_mday <= 30)
                return true;
            else
                return false;
        }

        //Months with 31 days
        if(Rdate.tm_mon == 1 || Rdate.tm_mon == 3 || Rdate.tm_mon == 5 || Rdate.tm_mon == 7 || Rdate.tm_mon == 8 || Rdate.tm_mon == 10 || Rdate.tm_mon == 12) {
            if (Rdate.tm_mday <= 31)
                return true;
            else
                return false;
        }

        //Leap year - 2nd month
        bool isLeap = (Rdate.tm_year % 4 == 0 && Rdate.tm_year % 100 != 0) || (Rdate.tm_year % 400 == 0);
        if(isLeap && Rdate.tm_mon == 2)
        {
            if(Rdate.tm_mday <= 29)
                return true;
            else
                return false;
        }
        else if (!isLeap && Rdate.tm_mon == 2)
        {
            if(Rdate.tm_mday <= 28)
                return true;
            else
                return false;
        }

        //Unexpected behaviour
        return false;
    }
};

//Just method for nulling arguments i don't need in comparing
void NullHours(CDate & Rdate)
{
    Rdate.date.tm_hour = 0;
    Rdate.date.tm_min = 0;
    Rdate.date.tm_sec = 0;
}
//+- Operators
CDate CDate::operator + (int num)
{
    //Dřív výpis takhle - tm funguje retardovaně
    //cout << this -> date.tm_year << "-"  << setw(2) << setfill('0') << this -> date.tm_mon << "-"  << setw(2) << setfill('0') << this -> date.tm_mday << endl;

    date.tm_mday += num;
    time_t dateSec = mktime(&this -> date);
    this -> date = *localtime(&dateSec);
    return *this;
}
CDate CDate::operator - (int num)
{
    date.tm_mday -= num;
    time_t dateSec = mktime(&this -> date);
    this -> date = *localtime(&dateSec);
    return *this;
}
int CDate::operator - (CDate Rdate)
{
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    return abs((date1_sec - date_sec) / 86400);
}
//Comparing functions package
bool CDate::operator == (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);

    //cout << put_time(&this -> date,"%c") << endl;
    //cout << put_time(&Rdate.date,"%c") << endl;

    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);

    //cout << date_sec << " | " << date1_sec << endl;

    if(date_sec == date1_sec)
        return true;
    else
        return false;
}
bool CDate::operator != (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    if(date_sec == date1_sec)
        return false;
    else
        return true;
}
bool CDate::operator >= (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    if(date_sec >= date1_sec)
        return true;
    else
        return false;
}
bool CDate::operator <= (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    if(date_sec <= date1_sec)
        return true;
    else
        return false;
}
bool CDate::operator < (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    if(date_sec < date1_sec)
        return true;
    else
        return false;
}
bool CDate::operator > (CDate Rdate)
{
    NullHours(*this);
    NullHours(Rdate);
    time_t date_sec = mktime(&this -> date);
    time_t date1_sec = mktime(&Rdate.date);
    if(date_sec > date1_sec)
        return true;
    else
        return false;
}
//Preincrement (++a)
CDate & CDate::operator ++ ()
{
    this -> date.tm_mday += 1;
    time_t dateSec = mktime(&this -> date);
    this -> date = *localtime(&dateSec);
    return *this;
}
//Postincrement (a++)
CDate CDate::operator ++ (int)
{
    CDate tmp(*this);
    operator++();
    return tmp;
}
//Predecrement (--a)
CDate & CDate::operator -- ()
{
    this -> date.tm_mday -= 1;
    time_t dateSec = mktime(&this -> date);
    this -> date = *localtime(&dateSec);
    return *this;
}
//Postdecrement (a++)
CDate CDate::operator -- (int)
{
    CDate tmp(*this);
    operator--();
    return tmp;
}
ostream & operator << ( ostream & os, const CDate & d )
{
    os << put_time(&d.date,"%F");
    return os;
}
//Missing the case, when conversion gets wrong format
istream & operator >> ( istream & is, CDate & x )
{
    string b;
    size_t pos = 0;
    tm tmp = {0};

    //Load from istream
    is >> b;

    //Check if is.fail() isnt present (if failbit)
    if(is.fail())
    {
        is.setstate(ios::failbit);
        return is;
    }

    //Converting parts of string to ints
    //Then erasing the part i just converted + the delimiter (-)
    tmp.tm_year = stoi(b,&pos);
    b=b.erase(0,pos+1);
    tmp.tm_mon = stoi(b,&pos);
    b=b.erase(0,pos+1);
    tmp.tm_mday = stoi(b,&pos);
    b=b.erase(0,pos);

    //Try if the date is valid
    if (!CDate::validateDate(tmp))
    {
        //cout << "Couldn't validate" << endl;
        is.setstate ( ios::failbit );
        return is;
    }

    //Declaring the year & month to the tm struct as it wants
    x.date.tm_year = tmp.tm_year - 1900;
    x.date.tm_mon = tmp.tm_mon - 1;
    x.date.tm_mday = tmp.tm_mday;

    return is;
}

#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;
    istringstream iss;

    CDate a ( 2000, 1, 2 );
    CDate b ( 2010, 2, 3 );
    CDate c ( 2004, 2, 10 );

    oss . str ("");
    oss << a;
    assert ( oss . str () == "2000-01-02" );
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2010-02-03" );
    oss . str ("");
    oss << c;
    assert ( oss . str () == "2004-02-10" );

    a = a + 1500;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2004-02-10" );
    b = b - 2000;
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2004-08-13" );
    assert ( b - a == 185 );
    assert ( ( b == a ) == false );
    assert ( ( b != a ) == true );
    assert ( ( b <= a ) == false );
    assert ( ( b < a ) == false );
    assert ( ( b >= a ) == true );
    assert ( ( b > a ) == true );
    assert ( ( c == a ) == true );
    assert ( ( c != a ) == false );
    assert ( ( c <= a ) == true );
    assert ( ( c < a ) == false );
    assert ( ( c >= a ) == true );
    assert ( ( c > a ) == false );
    a = ++c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-11" );
    a = --c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-10" );
    a = c++;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-11" );
    a = c--;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-10" );
    iss . clear ();
    iss . str ( "2015-09-03" );
    assert ( ( iss >> a ) );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-09-03" );
    a = a + 70;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-11-12" );

    CDate d ( 2000, 1, 1 );
    try
    {
        CDate e ( 2000, 32, 1 );
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( ... )
    {
    }
    iss . clear ();
    iss . str ( "2000-12-33" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-11-31" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-02-29" );
    assert ( ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
    iss . clear ();
    iss . str ( "2001-02-29" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );

    /*
    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------
    CDate f ( 2000, 5, 12 );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    oss . str ("");
    oss << date_format ( "%Y/%m/%d" ) << f;
    assert ( oss . str () == "2000/05/12" );
    oss . str ("");
    oss << date_format ( "%d.%m.%Y" ) << f;
    assert ( oss . str () == "12.05.2000" );
    oss . str ("");
    oss << date_format ( "%m/%d/%Y" ) << f;
    assert ( oss . str () == "05/12/2000" );
    oss . str ("");
    oss << date_format ( "%Y%m%d" ) << f;
    assert ( oss . str () == "20000512" );
    oss . str ("");
    oss << date_format ( "hello kitty" ) << f;
    assert ( oss . str () == "hello kitty" );
    oss . str ("");
    oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
    assert ( oss . str () == "121212121212050505200020002000%%%%%" );
    oss . str ("");
    oss << date_format ( "%Y-%m-%d" ) << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-1" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-1-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-001-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-02" );
    assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2001-01-02" );
    iss . clear ();
    iss . str ( "05.06.2003" );
    assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2003-06-05" );
    iss . clear ();
    iss . str ( "07/08/2004" );
    assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2004-07-08" );
    iss . clear ();
    iss . str ( "2002*03*04" );
    assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2002-03-04" );
    iss . clear ();
    iss . str ( "C++09format10PA22006rulez" );
    assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2006-09-10" );
    iss . clear ();
    iss . str ( "%12%13%2010%" );
    assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2010-12-13" );

    CDate g ( 2000, 6, 8 );
    iss . clear ();
    iss . str ( "2001-11-33" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "29.02.2003" );
    assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "14/02/2004" );
    assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2002-03" );
    assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "hello kitty" );
    assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2005-07-12-07" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "20000101" );
    assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-01-01" );
    */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
