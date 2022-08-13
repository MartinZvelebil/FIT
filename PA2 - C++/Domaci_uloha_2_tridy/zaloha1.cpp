#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
// Předpokládejte, že vytvoření a likvidace firmy jsou řádově méně časté než ostatní operace, tedy zde je lineární složitost akceptovatelná.
// Častá jsou volání invoice a audit, jejich časová složitost musí být lepší než lineární (např. logaritmická nebo amortizovaná konstantní).
// Dále, v povinných testech se metoda medianInvoice volá málo často, tedy nemusí být příliš efektivní (pro úspěch v povinných testech stačí
// složitost lineární nebo n log n, pro bonusový test je potřeba složitost lepší než lineární).

class CVATRegister
{
public:
    //Implicitně bude bezparametrický konstruktor
    CVATRegister ( )= default;

    //Destruktor
    //~CVATRegister  ( void );

    // Přidá do existující databáze další záznam. Parametry name a addr reprezentují jméno a adresu, parametr id udává daňový identifikátor.
    // Metoda vrací hodnotu true, pokud byl záznam přidán, nebo hodnotu false, pokud přidán nebyl
    // (protože již v databázi existoval záznam se stejným jménem a adresou, nebo záznam se stejným id).
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID )
    {
        auto NewCompany = new company;
        NewCompany -> _addr = addr;
        NewCompany -> _taxID = taxID;
        NewCompany -> _name = name;
        NewCompany -> _money = 0;

        if(CompanyUnique(*NewCompany))
        {
            database.push_back(*NewCompany);
            return true;
        }
        else
        {
            cout << "Company isn't unique!" << endl;
            return false;
        }
    }
    // Odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru
    // (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací),
    // vrátí metoda hodnotu false.
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr )
    {

    }
    bool          cancelCompany  ( const string    & taxID )
    {

    }
    // Zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou,
    // nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount )
    {

    }
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount )
    {

    }
    //Vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem.
    //Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const
    {

    }
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const
    {

    }
    // Slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména.
    // Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda firstCompany nalezne první firmu.
    // Pokud je seznam firem prázdný, vrátí metoda hodnotu false. V opačném případě vrátí metoda hodnotu true a vyplní výstupní parametry name a addr.
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const
    {

    }
    // Metoda nextCompany funguje obdobně, nalezne další firmu, která v seznamu následuje za firmou určenou parametry.
    // Pokud za name a addr již v seznamu není další firma, metoda vrací hodnotu false. V opačném případě metoda vrátí true a přepíše parametry
    // name a addr jménem a adresou následující firmy.
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const
    {

    }
    // Vyhledá medián hodnoty faktury. Do vypočteného mediánu se započtou všechny úspěšně zpracované faktury zadané voláním invoice.
    // Tedy nezapočítávají se faktury, které nešlo přiřadit (volání invoice selhalo), ale započítávají se všechny dosud registrované faktury,
    // tedy při výmazu firmy se neodstraňují její faktury z výpočtu mediánu. Pokud je v systému zadaný sudý počet faktur, vezme se vyšší ze dvou
    // prostředních hodnot. Pokud systém zatím nezpracoval žádnou fakturu, bude vrácena hodnota 0.
    unsigned int  medianInvoice  ( void ) const
    {

    }
private:
    struct company
    {
        string _name;
        string _addr;
        string _taxID;
        int _money;
    };
    vector<company> database;
    vector<double> _invoices;

    //Check if company TaxID is unique && addres and name unique together
    //False if not unique, True if unique
    bool CompanyUnique(const company & C) //const - mby?
    {
        bool returnVal = true;
        for (auto & i : database)
        {
            if((C._taxID == i._taxID) || (i._name == C._name && i._addr == C._addr))
            {
                returnVal = false;
                break;
            }
        }
        return returnVal;
    }
};

#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( ! b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
   /* assert ( b1 . invoice ( "666/666", 2000 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . audit ( "1234567", sumIncome ) );
    assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) ); */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */