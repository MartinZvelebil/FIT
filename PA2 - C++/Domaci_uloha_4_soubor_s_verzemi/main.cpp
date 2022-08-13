#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;

#endif /* __PROGTEST__ */
struct Vector
{
    Vector() = default;
    Vector(const Vector&) = default;

    ~Vector()
    {
        delete[] m_data;
    }

    void push_back(const uint8_t& value)
    {
        if (m_alocated_space <= m_size)
        {
            //Realocation of space, because array isn't big enough atm
            size_t new_alocated_space = m_alocated_space * 2 + 10;
            auto * new_data = new uint8_t[new_alocated_space];

            for(size_t i = 0; i < m_size; i++)
            {
                new_data[i] = m_data[i];
            }

            delete[] m_data;
            m_data = new_data;
            m_alocated_space = new_alocated_space;
        }
        m_data[m_size++] = value;
    }

    uint8_t pop_back()
    {
        if(m_size <= 0)
        {
            throw invalid_argument("Vector is empty.");
        }
        else
        {
            //Stačí jen zmenšit size arraye, ale prvek nemusím nutně mazat, zničí ho destruktor (viz Láďa proseminář)
            return m_data[--m_size];
        }
    }

    uint8_t& at(size_t index)
    {
        if(index >= m_size) //Možná bez = ?
        {
            throw invalid_argument("Index of array is out of range (bigger than max arr).");
        }
        else if (index < 0)
        {
            throw invalid_argument("Index of array is negative number.");
        }
        else
        {
            return m_data[index];
        }
    }

    size_t size() const
    {
        return m_size;
    }

    Vector & operator = (const Vector & v)
    {
        //Self-assignment protection
        if(this == &v)
        {
            return *this;
        }

        if(this->size() != v.size())
        {
            delete[] m_data;
            m_data = nullptr;
            //Edge-case zero check (don't want to initalize array of zero members)
            if(v.size() == 0)
            {
                m_data = nullptr;
            }
            else
            {
                m_data = new uint8_t[v.size()];
            }
            m_size = v.size();
            //Have to copy the alocated space variable
            m_alocated_space = v.size();
        }
        copy(v.m_data,v.m_data + v.m_size,m_data);
        return *this;
    }

    bool operator == (const Vector & Rdate)
    {
        if(*this->m_data == *Rdate.m_data)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    uint8_t& operator[](size_t index)
    {
        return at(index);
    }

private:
    size_t m_size = 0;
    size_t m_alocated_space = 0;
    uint8_t * m_data = nullptr;
};

//Struct|Node definition
struct CVersions_Node;

//Methods definitions for CFile class
class CFile {
public:
    CFile();
    CFile(const CFile& c);
    ~CFile();
    CFile & operator = (CFile & c);
    bool seek(uint32_t offset);
    uint32_t read(uint8_t *dst, uint32_t bytes);
    uint32_t write ( const uint8_t * src, uint32_t bytes );
    uint32_t fileSize () const;
    void addVersion ();
    bool undoVersion ();
    void truncate ();
private:
    //Vector m_data;
    //size_t m_position;
    CVersions_Node * m_current;
};

//Struct|Node implementation
struct CVersions_Node
{
    //CFile save;
    //vector<uint8_t>m_data;
    Vector m_data;
    size_t m_position = 0;
    int cnt = 1;
    struct CVersions_Node * next = nullptr;
};

//---------
//CFILE METHODS IMPLEMENTATION START
//---------

    //Implicit constructor
    CFile::CFile()
    {
        auto * newVersion = new CVersions_Node;
        m_current = newVersion;
    }

    //Deep copy constructor
    CFile::CFile(const CFile& c)
    {
        //Prvek nového spojáku
        CVersions_Node * source = c.m_current;
        CVersions_Node * newList = nullptr;
        CVersions_Node * tail = nullptr;

        while(source != nullptr)
        {
            //Začátek spojáku
            if (newList == nullptr)
            {
                newList = new CVersions_Node;
                newList->m_position = source -> m_position;
                newList->m_data = source->m_data;
                newList->cnt = source->cnt;
                newList->next = nullptr;
                tail = newList;
            }
            else
            {
               tail -> next = new CVersions_Node;
               tail = tail -> next;
               tail -> m_position = source -> m_position;
               tail -> m_data = source->m_data;
               tail -> cnt = source->cnt;
               tail -> next = nullptr;
            }
            source = source -> next;
        }
        this -> m_current = newList;
    }

    //Operator = overload
    CFile & CFile::operator = (CFile & c)
    {
        //Self-assign protection
        if(this == &c)
        {
            return *this;
        }

        CFile a(c);
        //swap(a,*this);
        swap(a.m_current,c.m_current);
        return *this;
    }

    //Destructor
    CFile::~CFile()
    {
        while(m_current != nullptr)
        {
            auto a = this -> m_current;
            m_current = m_current -> next;
            delete a;
        }
    }

    // copy cons, dtor, op=
    bool CFile::seek(uint32_t offset)
    {
        //cout << offset << " | " << m_data.size() << endl;
        if(offset >= 0 && offset <= m_current->m_data.size())
        {
            m_current->m_position = offset;
            return true;
        }
        else
            return false;
    }
    uint32_t CFile::read(uint8_t *dst, uint32_t bytes)
    {
        int index = 0;
        uint32_t numOfReadBytes = 0;
        while(bytes > 0)
        {
            if(m_current->m_position >= m_current->m_data.size()) //Možná -1
            {
                break;
            }
            else {
                //cout << (int)m_data[m_position] << " ";
                dst[index] = m_current->m_data[m_current->m_position];
                m_current->m_position++; numOfReadBytes++; bytes--; index++;
            }
        }
        //cout << endl << "Num of read bytes: " << numOfReadBytes << endl;
        return numOfReadBytes;
    }
    uint32_t CFile::write ( const uint8_t * src, uint32_t bytes )
    {
        //Asi potřeba check, jestli nejsem na konci souboru, jestli se vše povedlo zapsat
        int srcIndex = 0;
        uint32_t howManyToReturn = bytes;
        while (bytes > 0)
        {
            if(m_current->m_position >= m_current->m_data.size())
            {
                m_current->m_data.push_back(src[srcIndex]);
            }
            else {
                m_current->m_data.at(m_current->m_position) = src[srcIndex];
            }
            m_current->m_position++; srcIndex++; bytes--;
        }
        return howManyToReturn;
    }
    uint32_t CFile::fileSize () const
    {
        return m_current->m_data.size();
    }
    void CFile::truncate ()
    {
        while(m_current->m_data.size() > m_current->m_position)
        {
            m_current->m_data.pop_back();
        }
    }
    void CFile::addVersion()
    {
        //Bude chtít asi shared_ptr
        //shared_ptr<CVersions_Node> newVersion( new CVersions_Node );
        //Pokud je nějaká další node s kterou to porovnávat, tak ověř, jestli nejsou aktuální data úplně stejný jako z předchozí zálohy. Pokud ano, tak nech zálohu být a jen přičti 1 do cnt referen
        if((this -> m_current -> next != nullptr) && ((this -> m_current -> m_data) == (this -> m_current -> next -> m_data)) && ((this -> m_current -> m_position) == (this -> m_current -> next -> m_position)))
        {
            this -> m_current -> cnt += 1;
        }
        else
        {
            auto *newVersion = new CVersions_Node;
            newVersion->m_data = this->m_current->m_data;
            newVersion->m_position = this->m_current->m_position;
            newVersion->cnt = this->m_current->cnt;
            newVersion->next = this->m_current;
            this->m_current = newVersion;
        }
        //Creating deep copy of the actual CFile
        //CFile C (*this);
        /*if(C.m_current == nullptr)
        {
            //newVersion -> save = C;
            newVersion -> m_data = C.m_data;
            newVersion -> m_position = C.m_position ;
            newVersion -> next = nullptr; //Neexistuje předchůdce
            //Add the created version as new Version
            this -> m_current = newVersion;
        }
        else
        {
            newVersion -> m_data = C.m_data;
            newVersion -> m_position = C.m_position ;
            newVersion -> next = C.m_current;
            //Add the created version as new Version
            this -> m_current = newVersion;
        } */
    }
    bool CFile::undoVersion()
    {
        //Pokud už není odkaz na žádnou verzi
        if(this -> m_current -> next == nullptr && this -> m_current -> cnt == 1) //Nemám zádnej další next node a zároveň na této node není přítomná další záloha
        {
            return false;
        }
        else if(this -> m_current -> cnt > 1) //Záloha je furt přítomná na jednom node
        {
            this -> m_current -> cnt -= 1;
            return true;
        }
        else if(this -> m_current -> next != nullptr && this -> m_current -> cnt == 1) //Case, kdy chci jít s verzí na další node
        {
            //Přehraju si sám do sebe ze zálohované verze ten CFile
            //auto a = this -> m_current;
            //*this = this->m_current->save;
            //this->m_current = this->m_current->next;
            auto a = this->m_current; //Odkaz na původní první prvek spojáku
            this->m_current = this -> m_current -> next;
            delete a;
            return true;
        }
        return true;
    }

//---------
//CFILE METHODS IMPLEMENTATION END
//---------

#ifndef __PROGTEST__
bool writeTest  ( CFile & x,
                  const initializer_list<uint8_t> & data,
                  uint32_t wrLen )
{
    return x . write ( data . begin (), data . size () ) == wrLen;
}

bool readTest ( CFile & x,
                const initializer_list<uint8_t> & data,
                uint32_t rdLen )
{
    uint8_t  tmp[100];
    uint32_t idx = 0;
    //cout << "Data size assert: " << data.size() << endl;

    if ( x . read ( tmp, rdLen ) != data . size ())
    {
        cout << " | " << x . read ( tmp, rdLen ) << endl;
        return false;
    }
    for ( auto v : data )
    {
        if (tmp[idx++] != v)
        {
            //cout << "ERROR = Original: " << (int)v << " | " << "What return said: " << (int)tmp[idx++] << endl;
            return false;
        }
    }
    return true;
}

int main ( void )
{
    //cout << "test" << endl;
    CFile f0;
    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    assert ( f0 . fileSize () == 3 );
    assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 2 ));
    assert ( writeTest ( f0, { 5, 4 }, 2 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 1 ));
    assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
    assert ( f0 . seek ( 3 ));
    f0 . addVersion();
    assert ( f0 . seek ( 6 ));
    assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
    f0 . addVersion();
    assert ( f0 . seek ( 5 ));

    //cout << "test" << endl;
    CFile f1 ( f0 );
    f0 . truncate ();
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
    //cout << "hh" << endl;
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    //cout << "hh" << endl;
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
    //cout << "hh" << endl;
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
   //cout << "hh" << endl;
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
    assert ( !f0 . seek ( 100 ));
    assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
    assert ( f1 . seek ( 0 ));
    assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
    assert ( f1 . undoVersion () );
    assert ( f1 . undoVersion () );
    assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
    assert ( !f1 . undoVersion () );
    //cout << "aaa" << endl;
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
