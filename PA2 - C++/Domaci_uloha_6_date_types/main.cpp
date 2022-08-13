#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;

#endif /* __PROGTEST__ */

//Děděné třídy, tak při konstruktoru volají konstruktor té nad třídy
//Stejně tak s destruktorem, takže v tom potomkovi vytváříme konstruktor/destruktor pouze pro prvky, které jsou tam nové
/**
 * Main class for inheritance
 * */
class CDataType
{
public:
    CDataType() = default;
    virtual ~CDataType() = default;
    virtual bool operator == (const CDataType& RdataType) const
    {
        return m_type == RdataType.m_type;
    }
    virtual bool operator != (const CDataType& RdataType) const
    {
        return m_type != RdataType.m_type;
    }
    virtual size_t getSize() const
    {
        return m_size;
    }
    virtual string toString() const = 0;
    virtual unique_ptr<CDataType> createPtr() const = 0;
protected:
    size_t m_size;
    //string m_name;
    string m_type;
    friend ostream& operator << (ostream& out, const CDataType & dataType)
    {
        out << dataType.toString();
        return out;
    }
};

class CDataTypeInt : public CDataType
{
public:
    CDataTypeInt()
    {
        m_type = "int";
        m_size = sizeof(int);
        //m_name = "";
    }
    size_t getSize() const override
    { return this -> m_size;}
    string toString() const override
    {return this -> m_type;}
    unique_ptr<CDataType> createPtr() const override
    {
        return make_unique<CDataTypeInt>(*this);
    }
};

class CDataTypeDouble : public CDataType
{
public:
    CDataTypeDouble()
    {
        m_type = "double";
        m_size = sizeof(double);
        //m_name = "";
    }
    size_t getSize() const override
    { return this -> m_size;}
    string toString() const override
    {return this -> m_type;}
    unique_ptr<CDataType> createPtr() const override
    {
        return make_unique<CDataTypeDouble>(*this);
    }
};

class CDataTypeEnum : public CDataType
{
public:
    CDataTypeEnum()
    {
        m_size = 4;
        m_type = "enum";
        //m_name = "";
    }
    size_t getSize() const override
    { return this->m_size;}
    bool operator == (const CDataType& RdataType) const override
    {
        return this->toString() == RdataType.toString();
    }
    bool operator != (const CDataType& RdataType) const override
    {
        return this->toString() != RdataType.toString();
    }
    string toString() const override
    {
        string out = "";
        out += this->m_type;
        out += '{';
        for (unsigned int i = 0; i < m_Enum.size(); ++i)
        {
            out += m_Enum.at(i);
            if(i != m_Enum.size()-1)
                out += ',';
        }
        out += '}';
        return out;
    }
    CDataTypeEnum & add(const char * name)
    {
        string tmp = name; //Just for the exception
        for (unsigned int i = 0; i < m_Enum.size(); ++i)
        {
            if(m_Enum.at(i) == name)
                throw invalid_argument("Duplicate enum value: " + tmp);
        }
        m_Enum.emplace_back(name);
        return *this;
    }
    unique_ptr<CDataType> createPtr() const override
    {
        return make_unique<CDataTypeEnum>(*this);
    }
private:
    vector<string> m_Enum;
};

class CDataTypeStruct : public CDataType
{
public:
    CDataTypeStruct(CDataTypeStruct const & strct) : CDataType(strct)
    {
        this -> m_size = strct.m_size;
        this -> m_type = strct.m_type;

        for (unsigned int i = 0; i < strct.m_Struct.size(); ++i)
        {
            this -> m_Struct.emplace_back(strct.m_Struct.at(i).first,strct.m_Struct.at(i).second->createPtr());
        }
    }
    CDataTypeStruct()
    {
        m_size = 0;
        m_type = "struct";
        //m_name = "";
    }
    size_t getSize() const override
    {
        size_t sum = 0;
        for (unsigned int i = 0; i < m_Struct.size(); ++i)
        {
            sum += m_Struct.at(i).second->getSize();
        }
        return sum;
    }
    CDataTypeStruct & addField(const string& name, const CDataType & a)
    {
        for (unsigned int i = 0; i < m_Struct.size(); ++i)
        {
            if(m_Struct.at(i).first == name)
                throw invalid_argument("Duplicate field: " + name);
        }
        m_Struct.emplace_back(make_pair(name, a.createPtr()));
        return *this;
    }
    CDataType & field(const string& name) const
    {
        for (const auto & i : m_Struct)
        {
            if(i.first == name)
                return *i.second;
        }
        throw invalid_argument("Unknown field: " + name);
    }
    bool operator == (const CDataType& RdataType) const override
    {
        auto rdata_dyn = dynamic_cast<const CDataTypeStruct*>(&RdataType);
        if(rdata_dyn == nullptr || (this->getSize() != RdataType.getSize()))
            return false;

        for (unsigned int i = 0; i < this->m_Struct.size(); ++i)
        {
            if(this->m_Struct.at(i).second->toString() != rdata_dyn->m_Struct.at(i).second->toString())
                return false;
        }
        return true;
    }
    bool operator != (const CDataType& RdataType) const override
    {
        return !(*this == RdataType);
    }
    string toString() const override //Předělat
    {
        string out = "";
        out += this->m_type;
        out += "{";
        for (unsigned int i = 0; i < m_Struct.size(); ++i)
        {
            out += m_Struct.at(i).second->toString();
            out += m_Struct.at(i).first;
            out += ";";
        }
        out += "}";
        return out;
    }
    unique_ptr<CDataType> createPtr() const override
    {
        return make_unique<CDataTypeStruct>(*this);
    }
protected:
    vector<pair<string, unique_ptr<CDataType>>> m_Struct;
};

#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
    string nowhite_a = "";
    string nowhite_b = "";
    for (unsigned int i = 0; i < a.length(); ++i)
    {
        if(!isspace(a[i]))
            nowhite_a += a[i];
    }
    for (unsigned int i = 0; i < b.length(); ++i)
    {
        if(!isspace(b[i]))
            nowhite_b += b[i];
    }
    return nowhite_a==nowhite_b;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
    ostringstream oss;
    //cout << x;
    oss << x;
    return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{

   /* CDataTypeStruct  a = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct b = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "READY" ) ).
            addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct c =  CDataTypeStruct () .
            addField ( "m_First", CDataTypeInt () ) .
            addField ( "m_Second", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Third", CDataTypeDouble () );

    CDataTypeStruct  d = CDataTypeStruct () .
            addField ( "m_Length", CDataTypeInt () ) .
            addField ( "m_Status", CDataTypeEnum () .
            add ( "NEW" ) .
            add ( "FIXED" ) .
            add ( "BROKEN" ) .
            add ( "DEAD" ) ).
            addField ( "m_Ratio", CDataTypeInt () );

    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "}") );

    assert ( whitespaceMatch ( c, "struct\n"
                                  "{\n"
                                  "  int m_First;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Second;\n"
                                  "  double m_Third;\n"
                                  "}") );

    assert ( whitespaceMatch ( d, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  int m_Ratio;\n"
                                  "}") );

    assert ( a != b );
    assert ( a == c );
    assert ( a != d );
    assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
    assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
    assert ( a != CDataTypeInt() );
    assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
                                                         "{\n"
                                                         "  NEW,\n"
                                                         "  FIXED,\n"
                                                         "  BROKEN,\n"
                                                         "  DEAD\n"
                                                         "}") );

    CDataTypeStruct aOld = a;
    b . addField ( "m_Other", CDataTypeDouble ());

    a . addField ( "m_Sum", CDataTypeInt ());

    assert ( a != aOld );
    assert ( a != c );
    assert ( aOld == c );
    assert ( whitespaceMatch ( a, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  int m_Sum;\n"
                                  "}") );

    assert ( whitespaceMatch ( b, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    READY\n"
                                  "  } m_Status;\n"
                                  "  double m_Ratio;\n"
                                  "  double m_Other;\n"
                                  "}") );

    c . addField ( "m_Another", a . field ( "m_Status" ));

    assert ( whitespaceMatch ( c, "struct\n"
                                  "{\n"
                                  "  int m_First;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Second;\n"
                                  "  double m_Third;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Another;\n"
                                  "}") );

    d . addField ( "m_Another", a . field ( "m_Ratio" ));

    assert ( whitespaceMatch ( d, "struct\n"
                                  "{\n"
                                  "  int m_Length;\n"
                                  "  enum\n"
                                  "  {\n"
                                  "    NEW,\n"
                                  "    FIXED,\n"
                                  "    BROKEN,\n"
                                  "    DEAD\n"
                                  "  } m_Status;\n"
                                  "  int m_Ratio;\n"
                                  "  double m_Another;\n"
                                  "}") );

    assert ( a . getSize () == 20 );
    assert ( b . getSize () == 24 );
    try
    {
        a . addField ( "m_Status", CDataTypeInt () );
        assert ( "addField: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate field: m_Status"sv );
    }

    try
    {
        cout << a . field ( "m_Fail" ) << endl;
        assert ( "field: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Unknown field: m_Fail"sv );
    }

    try
    {
        CDataTypeEnum en;
        en . add ( "FIRST" ) .
                add ( "SECOND" ) .
                add ( "FIRST" );
        assert ( "add: missing exception!" == nullptr );
    }
    catch ( const invalid_argument & e )
    {
        assert ( e . what () == "Duplicate enum value: FIRST"sv );
    } */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
