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

//Zavolani next company pred first company, zavolani audit a median pred invoice, zavolat first company a pak tu prvni spolecnost smazat, pak by ti ta first company mela dat tu druhou
class CVATRegister
{
public:
    //Implicitně bude bezparametrický konstruktor
    CVATRegister ( )= default;

    //Destruktor
    ~CVATRegister  ( )= default; //Není třeba, shared ptr pořešily

    // Přidá do existující databáze další záznam. Parametry name a addr reprezentují jméno a adresu, parametr id udává daňový identifikátor.
    // Metoda vrací hodnotu true, pokud byl záznam přidán, nebo hodnotu false, pokud přidán nebyl
    // (protože již v databázi existoval záznam se stejným jménem a adresou, nebo záznam se stejným id).
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID )
    {
        shared_ptr<company> NewCompany( new company ); //Chytrá alokace, která se destruktne sama
        //auto NewCompany = new company;
        NewCompany -> _addr = addr;
        NewCompany -> _taxID = taxID;
        NewCompany -> _name = name;
        NewCompany -> _money = 0;
        int addr_whereToInsert = 0;
        int taxID_whereToInsert = 0;

        if(CompanyUnique(*NewCompany, addr_whereToInsert, taxID_whereToInsert))
        {
            //Insert do name+addr database
            if(addr_whereToInsert == -1) //Pokud mám pushnout na konec, tak se vrátilo -1
            {
                //cout << "ano" << endl;
                database.push_back(*NewCompany);
            }
            else
            {
                database.insert(database.begin() + addr_whereToInsert,*NewCompany); //Insertnu na pozici, kde mi je řečeno metodou
            }
            //cout << "ADDR :: Inserted " << NewCompany -> _name << " to index: " << addr_whereToInsert << endl;
            if(taxID_whereToInsert == -1)
            {
                taxID_sorted_database.push_back(*NewCompany);
            }
            else
            {
                taxID_sorted_database.insert(taxID_sorted_database.begin() + taxID_whereToInsert,*NewCompany);
            }
            //cout << "taxID :: Inserted " << NewCompany -> _name << "|" << NewCompany -> _taxID << " to index: " << taxID_whereToInsert << endl << endl;
            return true;
        }
        else
        {
            //cout << "Company isn't unique!" << endl;
            return false;
        }
    }
    // Odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru
    // (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu true. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací),
    // vrátí metoda hodnotu false.
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr )
    {
        int index = findCompany(name, addr); //První najdu index v databázi sorted podle name & addr
        if(index == -1)
        {
            return false;
        }
        else
        {
            //Odstranění z databáze, která sortovala dle taxID
            string taxID = database.at(index)._taxID;
            for (unsigned int i = 0; i < taxID_sorted_database.size(); ++i)
            {
                if(taxID_sorted_database.at(i)._taxID == taxID)
                {
                    taxID_sorted_database.erase(taxID_sorted_database.begin()+i);
                    break;
                }
            }

            //Odstranění z databáze, která sortovala podle name a addr
            database.erase(database.begin()+index);

            return true;
        }
    }
    bool          cancelCompany  ( const string    & taxID )
    {
        int index = findCompany(taxID);
        if(index == -1)
        {
            return false;
        }
        else
        {
            //Odstranění z databáze, která sortovala dle name a addr
            string taxIDL = taxID_sorted_database.at(index)._taxID;
            for (unsigned int i = 0; i < database.size(); ++i)
            {
                if(database.at(i)._taxID == taxIDL)
                {
                    database.erase(database.begin()+i);
                    break;
                }
            }
            //Odstranění z databáze, která sortovala podle taxID
            taxID_sorted_database.erase(taxID_sorted_database.begin()+index);

            return true;
        }
    }
    // Zaznamenají příjem ve výši amount. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou,
    // nebo daňovým identifikátorem. Pokud metoda uspěje, vrací true, pro neúspěch vrací false (neexistující firma).
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount )
    {
        int index = findCompany(taxID);
        //cout << "Index: " << index << endl;
        if(index == -1)
        {
            return false;
        }
        else
        {
            string name = taxID_sorted_database.at(index)._name;
            string addr = taxID_sorted_database.at(index)._addr;
            int addr_index = findCompany(name, addr);
            database.at(addr_index)._money += amount;

            taxID_sorted_database.at(index)._money += amount;

            _invoices.push_back(amount);
            //cout << "TAXID_INVOICE Adding to: " << name << " | " << addr << " ----- " << taxID << endl << endl;
            return true;
        }
    }
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount )
    {
        int index = findCompany(name, addr);
        if(index == -1)
        {
            return false;
        }
        else
        {
            string taxID = database.at(index)._taxID;
            int taxID_index = findCompany(taxID);
            taxID_sorted_database.at(taxID_index)._money += amount;

            database.at(index)._money += amount;

            _invoices.push_back(amount);
            //cout << "NAME & ADDR_INVOICE Adding to: " << taxID << " ----- " << name << " | " << addr << endl << endl;
            return true;
        }
    }
    //Vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem.
    //Nalezený součet uloží do výstupního parametru sum. Metoda vrátí true pro úspěch, false pro selhání (neexistující firma).
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const
    {
        int index = findCompany(name, addr);
        if(index == -1){
            return false;
        }
        else{
            sumIncome = database.at(index)._money;
            return true;
        }
    }
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const
    {
        int index = findCompany(taxID);
        if(index == -1)
        {
            return false;
        }
        else
        {
            sumIncome = taxID_sorted_database.at(index)._money;
            return true;
        }
        //bsearch(taxID, taxID_sorted_database.data(), taxID_sorted_database.size(), sizeof(company), SortComparator);
    }
    // Slouží k procházení existujícího seznamu firem v naší databázi. Firmy jsou procházené v abecedním pořadí podle jejich jména.
    // Pokud mají dvě firmy stejná jména, rozhoduje o pořadí jejich adresa. Metoda firstCompany nalezne první firmu.
    // Pokud je seznam firem prázdný, vrátí metoda hodnotu false. V opačném případě vrátí metoda hodnotu true a vyplní výstupní parametry name a addr.
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const
    {
        if(!database.empty())
        {
            name = database.at(0)._name;
            addr = database.at(0)._addr;
            return true;
        }
        else
        {
            return false;
        }
    }
    // Metoda nextCompany funguje obdobně, nalezne další firmu, která v seznamu následuje za firmou určenou parametry.
    // Pokud za name a addr již v seznamu není další firma, metoda vrací hodnotu false. V opačném případě metoda vrátí true a přepíše parametry
    // name a addr jménem a adresou následující firmy.
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const
    {
        //auto lower = lower_bound(database.begin(),database.end(),name,[](auto & x, auto & y){return x._name < y._name && x._addr < y._addr;});
        int index = findCompany(name, addr);
        if(index == -1)
        {
            return false;
        }
        else
        {
            if((index + 1) <= (signed)database.size()-1)
            {
                name = database.at(index + 1)._name;
                addr = database.at(index + 1)._addr;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    // Vyhledá medián hodnoty faktury. Do vypočteného mediánu se započtou všechny úspěšně zpracované faktury zadané voláním invoice.
    // Tedy nezapočítávají se faktury, které nešlo přiřadit (volání invoice selhalo), ale započítávají se všechny dosud registrované faktury,
    // tedy při výmazu firmy se neodstraňují její faktury z výpočtu mediánu. Pokud je v systému zadaný sudý počet faktur, vezme se vyšší ze dvou
    // prostředních hodnot. Pokud systém zatím nezpracoval žádnou fakturu, bude vrácena hodnota 0.
    unsigned int  medianInvoice  ( ) const
    {
        size_t indexOfInvoice = _invoices.size();
        //Pokud prázdný seznam, vracím rovnou false
        if(indexOfInvoice == 0)
        {
            return 0;
        }
        else if(indexOfInvoice == 1)
        {
            return _invoices.front();
        }

        sort(_invoices.begin(), _invoices.end(), SortComparator);

        //Pokud lichý, tak vrátím přímo prostřední
        if(indexOfInvoice % 2 != 0)
        {
            return _invoices.at(indexOfInvoice / 2);
        }
        //Pokud sudý, tak větší z prostředních dvou
        else
        {
            if (_invoices.at(indexOfInvoice / 2) > _invoices.at((indexOfInvoice / 2) - 1))
            {
                return _invoices.at(indexOfInvoice / 2);
            }
            else
            {
                return _invoices.at((indexOfInvoice / 2) - 1);
            }
        }
    }
private:
    struct company
    {
        string _name;
        string _addr;
        string _taxID;
        unsigned int _money{};
    };

    vector<company> database;
    vector<company> taxID_sorted_database;
    mutable vector<unsigned int> _invoices;

    //Just sort comparator for sorting invoices
    static bool SortComparator(double x, double y)
    {
        return x > y;
    }

    //Check if company TaxID is unique && addres and name unique together
    //False if not unique, True if unique
    //Find, where to insert the company. If returns -1 it means to the end of vector (push_back)
    bool CompanyUnique(const company & C, int & addr_whereToInsert, int & taxID_whereToInsert) //const - mby?
    {
        bool returnVal = true;
        bool addr_insertingFound = false;
        bool taxID_insertingFound = false;
        unsigned int addr_sizeOfDatabase = database.size();
        unsigned int taxID_sizeOfDatabase = taxID_sorted_database.size();
        //Pokud je databáze prázdná, rovnou nastavím, že insertuju na 0
        if(addr_sizeOfDatabase == 0)
            {addr_whereToInsert = 0;addr_insertingFound = true;}
        if(taxID_sizeOfDatabase == 0)
            {taxID_whereToInsert = 0;taxID_insertingFound = true;}

        for (unsigned int i = 0; i < addr_sizeOfDatabase; ++i)
        {
            int cmpr_name = strcasecmp(database.at(i)._name.c_str(), C._name.c_str());
            int cmpr_addr = strcasecmp(database.at(i)._addr.c_str(), C._addr.c_str());
            if(!addr_insertingFound)
            {
                //Pokud jsem na začátku prohledávání (na vrcholu db)
                if(i == 0)
                {
                    if(cmpr_name > 0) //A rovnou je jmeno dáno, jakože má být dříve
                    {
                        addr_whereToInsert = 0;
                        addr_insertingFound = true;
                        //cout << "   ADDRINSERT: Found on the start, insert on first place | " << C._name << endl;
                        goto hihi;
                    }
                }

                //Pokud mají stejné jméno, tak rozhoduje o pořadí abecední pořadí adres
                if (cmpr_name == 0)
                {
                    int indexator = i;
                    if (cmpr_addr > 0) //Pokud aktuální záznam addresy v databázi, je později v abecedě, tak to šoupnu před něj
                    {
                        addr_whereToInsert = i;
                        addr_insertingFound = true;
                    }
                    else //Pokud je později, tak musím hledat, dokud jsou stejná jména, tu pozici, kde adresa zleva a zprava bude správná (zleva abecedně dřív, zprava později)
                    {
                        if((unsigned)(indexator + 1) > database.size()-1) //Pokud zjistím, že index, na který bych se dotazoval, bude neplatný
                        {
                            goto sorryZaGoto;
                        }
                        while(strcasecmp(database.at(indexator)._name.c_str(), C._name.c_str()) == 0 && strcasecmp(database.at(indexator+1)._name.c_str(), C._name.c_str()) == 0)
                        {
                            if(strcasecmp(database.at(indexator)._addr.c_str(), C._addr.c_str()) < 0 && strcasecmp(database.at(indexator+1)._addr.c_str(), C._addr.c_str()) > 0)
                            {
                                addr_whereToInsert = indexator + 1;
                                addr_insertingFound = true;
                            }
                            indexator ++;
                            if((unsigned)(indexator + 1) > database.size()-1) //Pokud zjistím, že index, na který bych se dotazoval, bude neplatný
                            {
                                goto sorryZaGoto;
                            }
                        }
                    }
                    sorryZaGoto:
                    if(!addr_insertingFound)
                    {
                        addr_whereToInsert = indexator + 1; //mby +1
                        addr_insertingFound = true;
                    }
                    //cout << "   ADDRINSERT: Inserting via addreses | " << C._name << endl;
                }
                //Pokud mám jen jeden prvek v databázi, tak insert bude před nebo po něm (check je jinej)
                else if (addr_sizeOfDatabase == 1) {
                    if (cmpr_name < 0) {
                        addr_whereToInsert = 1;
                    } else {
                        addr_whereToInsert = 0;
                    }
                    //cout << "   ADDRINSERT: Found position for inserting - one prvek is present | " << C._name << endl;
                    addr_insertingFound = true;
                }
                //Pokud jsem už na konci databáze, nemůžu vykonat spodní checking podmínku (hodila by error)
                else if (i == addr_sizeOfDatabase - 1)
                {
                    if (cmpr_name < 0)
                    {
                        //cout << "   ADDRINSERT: Found position for inserting - last position -1 | " << C._name << endl;
                        addr_whereToInsert = -1; //Push back
                    }
                    else //Pokud je dříve námi vkládaný string, tak nahradím na pozici i, ten aktuální
                    {
                        addr_whereToInsert = i;
                        //cout << "   ADDRINSERT: Found position for inserting - last position I | " << C._name << endl;
                    }
                    //cout << "   Found position for inserting - last position" << endl;
                    addr_insertingFound = true;
                }
                //Pokud aktuální záznam je abecedně dřív, než ten co insertuju a zároveň ten potom je později, tak insert na místo
                else if (cmpr_name < 0 && strcasecmp(database.at(i+1)._name.c_str(), C._name.c_str()) > 0)
                {
                    addr_whereToInsert = i + 1;
                    //cout << "   ADDRINSERT: Found position of inserting inside complex method | " << C._name << endl;
                    //cout << cmpr_addr << " | " << database.at(i)._name << " | " << database.at(i+1)._name << endl;
                    addr_insertingFound = true;
                }
            }

            hihi:
            //ROZLIŠUJU MALÝ VELKÝ PÍSMENA!
            if(!taxID_insertingFound)
            {
                //Pokud jsem na začátku prohledávání
                if(i == 0)
                {
                    if(taxID_sorted_database.at(i)._taxID.compare(C._taxID) > 0)
                    {
                        taxID_whereToInsert = 0;
                        taxID_insertingFound = true;
                        //cout << "   TAXID: Found on the start, insert on first place" << endl;
                        goto hihi2;
                    }
                }

                //Pokud mám jen jeden prvek v databázi, tak insert bude před nebo po něm (check je jinej)
                if (taxID_sizeOfDatabase == 1)
                {
                    if (taxID_sorted_database.at(i)._taxID.compare(C._taxID) < 0){
                        taxID_whereToInsert = 1;

                    } else {
                        taxID_whereToInsert = 0;
                    }
                    //cout << "   TAXID: Found position for inserting - one prvek is present" << endl;
                    taxID_insertingFound = true;
                }
                //Pokud jsem už na konci databáze, nemůžu vykonat spodní checking podmínku (hodila by error)
                else if (i == taxID_sizeOfDatabase - 1)
                {
                    if (taxID_sorted_database.at(i)._taxID.compare(C._taxID) < 0)
                    {
                        taxID_whereToInsert = -1; //Push back
                    }
                    else //Pokud je dříve námi vkládaný string, tak nahradím na pozici i, ten aktuální
                    {
                        taxID_whereToInsert = i;
                    }
                    //cout << "   TAXID: Found position for inserting - last position" << endl;
                    taxID_insertingFound = true;
                }
                //Pokud aktuální záznam je abecedně dřív, než ten co insertuju a zároveň ten potom je později, tak insert na místo
                else if (taxID_sorted_database.at(i)._taxID.compare(C._taxID) < 0 && taxID_sorted_database.at(i + 1)._taxID.compare(C._taxID) > 0)
                {
                    taxID_whereToInsert = i + 1;
                    //cout << "   TAXID: Found position of inserting inside complex method" << endl;
                    taxID_insertingFound = true;
                }
            }

            hihi2:
            //Pokud je duplicitní záznam v databázi
            if((C._taxID == database.at(i)._taxID) || (cmpr_name == 0 && cmpr_addr == 0))
            {
                returnVal = false;
                addr_whereToInsert = -2; //Jen aby tam nebyla legit value
                break;
            }
        }
        /*if(!taxID_insertingFound)
        {
            cout << "TAXID: Unexpected behaviour: " << taxID_whereToInsert << endl;
        }
        else
        {
            cout << "TAXID: " << taxID_whereToInsert << endl;
        }
        if(!addr_insertingFound)
        {
            cout << "ADDRINSERT: Unexpected behaviour: " << addr_whereToInsert << endl;
        }
        else
        {
            cout << "ADDRINSERT: " << addr_whereToInsert << endl << endl;
        }*/
        return returnVal;
    }

    //TODO: BONUS IMPLEMENTATION - MAKE INVOICES ALLWAYS SORTED -> BINARY SEARCH IN THEM

    //Selects where to insert invoice, so the array is allways sorted
/*
    int binarySearch(int left, int right, int amount)
     {
        if(left == right)
        {
            return 0;
        }
        if (left <= right)
        {
           int mid = (left + right)/2;
           if (_invoices.at(mid) < amount && _invoices.at(mid) > amount)
              return mid;
           if (_invoices.at(mid) > amount)
              return binarySearch(left,mid-1, amount);
           if (_invoices.at(mid) < amount)
              return binarySearch(mid+1, right, amount);
        }
       return -1;
    }
*/

    //Finding company index via binary search with desired parameters - přetěžování
    //Returns index of company
    //If company wasn't found, returns -1
    int findCompany(const string & name, const string & addr) const
    {
        return findArroundFoundName(name,addr,findName(name,addr));
    }
    int findCompany(const string & taxID) const
    {
        return findName(taxID);
    }

    //Finding first possible index of the same name as provided
    //Returns index of that
    int findName(const string & name, const string & addr) const
    {
        int left = 0 ;
        int right = database.size() - 1;
        while (left <= right)
        {
            int middle = left + (right - left) / 2;
            int cmpr = strcasecmp(name.c_str(),database.at(middle)._name.c_str());
            int res = -112131;   //Random hodnota, aby to nevracelo 0 rovnou

            if (cmpr == 0){
                res = 0;
            }

            // Pokud je hodnota právě na prostředku teď
            if (res == 0){
                return middle;
            }

            // Pokud hledané jméno je v abecedě dříve, tak zahazuju pravou část pole
            if (cmpr < 0) {
                right = middle - 1;
            }

            // Pokud hledané jméno je v abecedě později, tak zahazuju levou část pole
            else{
                left = middle + 1;
            }
        }
        return -1;
    }
    int findName(const string & taxID) const
    {
        int left = 0 ;
        int right = taxID_sorted_database.size() - 1;
        while (left <= right)
        {
            int middle = left + (right - left) / 2;
            int res = -112131;   //Random hodnota, aby to nevracelo bordel
            if (taxID == taxID_sorted_database.at(middle)._taxID){
                res = 0;
            }

            // Pokud je hodnota právě na prostředku teď
            if (res == 0) {
                return middle;
            }

            // Pokud hledané jméno je v abecedě dříve, tak zahazuju pravou část pole
            if (taxID.compare(taxID_sorted_database.at(middle)._taxID) < 0) {
                right = middle - 1;
            }
            // Pokud hledané jméno je v abecedě později, tak zahazuju levou část pole
            else {
                left = middle + 1;
            }
        }
        return -1;
    }

    //Finding the right index, because the method findCompany found only right name, not the address
    //Returns index of the right name, -1 returned if wasn't found
    int findArroundFoundName(const string & name, const string & addr, int index) const
    {
        //cout << "Find arround found name: " << database.size() << " | " << index << endl;
        //Pokud přišlo -1, vím, že už bylo něco špatně v findName (Neexistuje ani jmeno v DB)
        if(index == -1)
        {
            return -1;
        }
        //cout << "Index from findName: " << index << " Db.size: " << database.size() << endl;
        bool UpTooMuch = false;
        bool DownTooMuch = false; //Jen abych si ušetřil pár interací v DB

        //Prohledávám nahoru i dolu, kde je přímo můj záznam, abych našel i správnou adresu
        for (unsigned int i = 0; i < database.size(); ++i)
        {
            //cout << index << " | " << name << " | " << addr << " | " << i << endl;
            if((index + i) <= database.size()-1)
            {
                int cmpr_name = strcasecmp(name.c_str(), database.at(index + i)._name.c_str());
                int cmpr_addr = strcasecmp(addr.c_str(), database.at(index + i)._addr.c_str());
                if (cmpr_name == 0 && cmpr_addr == 0)
                {
                    index = index + i;
                    return index;
                }
            }
            else
            {UpTooMuch = true;}

            if((signed)(index - i) >= 0) //Aritmetika na unsigned + signed je scam, musím to takhle retypovat
            {
                int cmpr_name = strcasecmp(name.c_str(), database.at(index - i)._name.c_str());
                int cmpr_addr = strcasecmp(addr.c_str(), database.at(index - i)._addr.c_str());
                if (cmpr_name == 0 && cmpr_addr == 0)
                {
                    index = index - i;
                    return index;
                }
            }
            else{ DownTooMuch = true;}

            if(DownTooMuch && UpTooMuch)
                {break;}
        }
        return -1;
    }

};

#ifndef __PROGTEST__
int               main           ( void )
{
    //string name, addr;
    //unsigned int sumIncome;

    /*CVATRegister b1;
    assert ( b1 . newCompany ( "Martin", "Ahakussova", "1" ) );
    assert ( b1 . newCompany ( "Martin", "Bhakussova", "12" ) );
    assert ( b1 . newCompany ( "Martin", "Shakussova", "13" ) );
    assert ( b1 . newCompany ( "Martin", "Chakussova", "14" ) );
    assert ( b1 . newCompany ( "Martin", "Zhakussova", "15" ) );
    assert ( b1 . newCompany ( "Martin", "AAakussova", "16" ) );
    cout << "oh" << endl;  */



    return EXIT_SUCCESS;

}
#endif /* __PROGTEST__ */