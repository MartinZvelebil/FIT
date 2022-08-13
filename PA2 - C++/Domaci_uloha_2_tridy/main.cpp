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
        string nameAddr = name + " | | " + addr;
        if(CompanyUnique(*NewCompany, addr_whereToInsert, taxID_whereToInsert))
        {
            //Insert do name+addr database
            if(addr_whereToInsert == -1) //Pokud mám pushnout na konec, tak se vrátilo -1
            {
                database.push_back(*NewCompany);
                nameWithaddr.push_back(nameAddr);
            }
            else
            {
                database.insert(database.begin() + addr_whereToInsert,*NewCompany); //Insertnu na pozici, kde mi je řečeno metodou
                nameWithaddr.insert(nameWithaddr.begin() + addr_whereToInsert, nameAddr);
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
        string s = name + " | | "+ addr;
        int index = findCompanyNameAddr(s); //První najdu index v databázi sorted podle name & addr
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
            nameWithaddr.erase(nameWithaddr.begin()+index);

            return true;
        }
    }
    bool          cancelCompany  ( const string    & taxID )
    {
        int index = findCompanyTax(taxID);
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
                    nameWithaddr.erase(nameWithaddr.begin()+i);
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
        int index = findCompanyTax(taxID);
        //cout << "Index: " << index << endl;
        if(index == -1)
        {
            return false;
        }
        else
        {
            string name = taxID_sorted_database.at(index)._name;
            string addr = taxID_sorted_database.at(index)._addr;
            string s = name + " | | " + addr;
            int addr_index = findCompanyNameAddr(s);
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
        string s = name + " | | " + addr;
        int index = findCompanyNameAddr(s);
        if(index == -1)
        {
            return false;
        }
        else
        {
            string taxID = database.at(index)._taxID;
            int taxID_index = findCompanyTax(taxID);
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
        string s = name + " | | " + addr;
        int index = findCompanyNameAddr(s);
        if(index == -1)
        {
            return false;
        }
        else
        {
            sumIncome = database.at(index)._money;
            return true;
        }
    }
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const
    {
        int index = findCompanyTax(taxID);
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
        string s = name + " | | " + addr;
        int index = findCompanyNameAddr(s);
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
    vector<string> nameWithaddr;

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


    //Finding company index via binary search with desired parameters - přetěžování
    //Returns index of company
    //If company wasn't found, returns -1
    int findCompanyNameAddr(const string & nameAndAddr) const
    {
        return findNameNameAddr(nameAndAddr);
    }
    int findCompanyTax(const string & taxID) const
    {
        return findNameTax(taxID);
    }

    //Finding first possible index of the same name as provided
    //Returns index of that
    int findNameNameAddr(const string & nameAndAddr) const
    {
        int left = 0 ;
        int right = nameWithaddr.size() - 1;
        while (left <= right)
        {
            int middle = left + (right - left) / 2;
            int cmpr = strcasecmp(nameAndAddr.c_str(),nameWithaddr.at(middle).c_str());
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
    int findNameTax(const string & taxID) const
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
};

#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome = 0;

    CVATRegister b;
    assert(!b.audit("MBvFSkSvOcXIJnCXTEMR", "VVCpYEdsfJMRMHYgTQbJ", sumIncome));
    assert(b.newCompany("wFSGvCxFSQxCDWaQJHcF", "YNwCJXUsGzzNRtAnZnTF", "YWHPPHeRwAWrBgzXyQwY"));
    assert(!b.cancelCompany("gkiUbVdGXwHEluOzZZUF", "bIfxdArbJIZRTHMuDrDA"));
    assert(b.firstCompany(name, addr) && name == "wFSGvCxFSQxCDWaQJHcF" && addr == "YNwCJXUsGzzNRtAnZnTF");
    assert(!b.cancelCompany("GKiUBVdgXWHeLuOZZzUf", "bIfXdarBJIZRTHMUDRdA"));
    assert(b.newCompany("lsYBLRsFgBelRKxBwJEW", "LEPCRoDNaKLOkKuYtHPW", "tOAfYVeTCVszSFAVJVxY"));
    assert(!b.invoice("syrdlvchzoywlammerem", 25814));
    assert(b.invoice("YWHPPHeRwAWrBgzXyQwY", 40131));
    assert(!b.audit("mBVfsKSvOCXiJNCXTeMr", "VVCPYeDSfJMrMHYgtQbJ", sumIncome));
    assert(b.firstCompany(name, addr) && name == "lsYBLRsFgBelRKxBwJEW" && addr == "LEPCRoDNaKLOkKuYtHPW");
    assert(b.invoice("tOAfYVeTCVszSFAVJVxY", 77241));
    assert(b.firstCompany(name, addr) && name == "lsYBLRsFgBelRKxBwJEW" && addr == "LEPCRoDNaKLOkKuYtHPW");
    assert(!b.invoice("VIWQsilVZQxOAFpGzQnU", "MBSRVOzQbivyTRPLBCIa",89145));
    assert(b.newCompany("FZCYFYNlxyuffrWRSwHl", "TgMyZlRrJyEtIOBlxtvV", "PYNXoPLlIGGwkkCzTHBW"));
    assert(!b.audit("MBVFsKSvoCxIJncXTEMR", "vVCPyEdsfJMrMHygTQBJ", sumIncome));
    assert(!b.cancelCompany("CsdOCeUvUXfPVWSISxpT", "losXqgJVXPJBHMPJqLHN"));
    assert(b.audit("tOAfYVeTCVszSFAVJVxY", sumIncome) && sumIncome == 77241);
    assert(!b.invoice("hnnkodjsecmztutuyhpw", 4392));
    assert(b.medianInvoice() == 77241);
    assert(b.newCompany("nHEXdpWTSVLqBnFtkHzQ", "bKpDZvYKEXAAwSCCoOgk", "LLJjFnnuBvGHOcPWFbLp"));
    assert(b.invoice("nhEXdPwtSVLQBNftKHZQ", "BKPDZVyKEXAAwScCooGk",50195));
    assert(b.medianInvoice() == 50195);
    assert(b.newCompany("KcIIAUYoBuBmXboFqeBd", "DUfDxTkNZLRvZWFpQsxH", "HJgHFoyxGyEYHOpjgIcJ"));
    assert(!b.audit("CsDocEuVUxFPvWSIsXPt", "lOsXqGJvxPJBHMPJQLHn", sumIncome));
    assert(b.medianInvoice() == 50195);
    assert(b.medianInvoice() == 50195);
    assert(b.medianInvoice() == 50195);
    assert(b.newCompany("jtpFLpBjWkBQqHVDBpwT", "eIAuRvAvMYOEKdjozAGc", "ARXkUGbVIJXzxLCJRcHQ"));
    assert(b.audit("ARXkUGbVIJXzxLCJRcHQ", sumIncome) && sumIncome == 0);
    assert(!b.audit("shqumgcnwrjisdcnrbeu", sumIncome));
    assert(!b.audit("VIwQSILvzQXoafpgZqNU", "MbSRVoZQbiVYtrpLBCIA", sumIncome));
    assert(b.firstCompany(name, addr) && name == "FZCYFYNlxyuffrWRSwHl" && addr == "TgMyZlRrJyEtIOBlxtvV");
    assert(!b.cancelCompany("vIwqsiLVZqxOAFPGZqnu", "MBSrVOZQbIVytRPLBcIa"));
    assert(b.firstCompany(name, addr) && name == "FZCYFYNlxyuffrWRSwHl" && addr == "TgMyZlRrJyEtIOBlxtvV");
    assert(b.invoice("jTpfLpbjWKbQqHvdbPWT", "EiaURVaVMYoeKdjoZAgC",39615));
    assert(b.newCompany("pPXOOlmgKaIkqeWEzWCP", "XNnQTKUsIZJHSLzfCQMh", "YImQvMXxEKHngNIIDBBS"));
    assert(b.medianInvoice() == 50195);
    assert(b.firstCompany(name, addr) && name == "FZCYFYNlxyuffrWRSwHl" && addr == "TgMyZlRrJyEtIOBlxtvV");
    assert(b.medianInvoice() == 50195);
    assert(b.newCompany("WPdbrgvsGBHPMEXWAJGg", "QIUCorBODHxNQfLZPcUI", "jIKCnXbDcyNdKLskfoMm"));
    assert(b.audit("NHExDPWtSVLqBNftKHzQ", "BKpdzVYkEXAAWSCCOOGk", sumIncome) && sumIncome == 50195);
    assert(b.invoice("PpXOoLMGKAiKQEWEZWCp", "xnNQTKUSiZJHslZFCQmh",53314));
    assert(b.newCompany("zYHLqBqIUJiIIUUTbZch", "cQunKPgnoLXKGdiwJIYX", "pPIXuqUDAuARzNMrAHBH"));
    assert(!b.invoice("nkgzguahtumwdttitnje", 9617));
    assert(b.newCompany("CBQEKULttDUJrYHMvaYI", "OMXhKGVhGTPmZGOkKLSa", "rtHSBVCLiWmfwhOuBUof"));
    assert(b.newCompany("aDJJTgknuRISRMvvPkVT", "XVKQKfMZrjUJAOUckxxV", "ViBVyXAIuxFRznrVOPLw"));
    assert(!b.invoice("hnnkodjsecmztutuyhpw", 47667));
    assert(b.medianInvoice() == 50195);
    assert(!b.invoice("mBvFSKsvOcxIjnCxteMR", "VVcpyedsFJmrMhyGTQbj",78760));
    assert(b.cancelCompany("tOAfYVeTCVszSFAVJVxY"));
    assert(!b.cancelCompany("tOAfYVeTCVszSFAVJVxY"));
    assert(b.audit("KCIIaUYOBUBmxBOfqeBd", "dUFdxTKnZLRvZWFPqSXH", sumIncome) && sumIncome == 0);
    assert(b.medianInvoice() == 50195);
    assert(!b.invoice("hnnkodjsecmztutuyhpw", 54922));
    assert(b.medianInvoice() == 50195);
    assert(!b.cancelCompany("iocfmwnetfahvuelapiz"));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 50195);
    assert(b.newCompany("DcTOsBXEZQbggxtvBTmV", "DyTJxMhyFTWzLAVJStgO", "hfLSIIDAbLquGDnkFdyW"));
    assert(b.medianInvoice() == 50195);
    assert(!b.audit("LSybLRsfGbELRKXbwJEW", "LEPCRODNakLOkKUYthPW", sumIncome));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.invoice("YImQvMXxEKHngNIIDBBS", 2825));
    assert(b.cancelCompany("cBQEkuLTtDUJRyHMvaYi", "OmXHKGVhgtpMZGOkKlsA"));
    assert(b.cancelCompany("JTPflPbJWKBqqHvDBPWt", "eIAURVaVmYoEKDjOzAgc"));
    assert(!b.invoice("hnnkodjsecmztutuyhpw", 98150));
    assert(b.invoice("PpXoOLMGKAIKQEweZwCP", "XnNqtkuSIZJHSLzFCqMh",4858));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 40131);
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.newCompany("wDoBqaSZvCFvtBHUVXSg", "YLcTPjNkIHsnNWljEOSj", "AecoaPZGDsPbFbLDQboa"));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 40131);
    assert(b.medianInvoice() == 40131);
    assert(b.medianInvoice() == 40131);
    assert(b.cancelCompany("PYNXoPLlIGGwkkCzTHBW"));
    assert(b.invoice("AecoaPZGDsPbFbLDQboa", 2993));
    assert(b.medianInvoice() == 40131);
    assert(!b.audit("GKIUbVDGxwhElUOZZzUf", "BiFxdaRbjiZRThMUDrDa", sumIncome));
    assert(!b.invoice("tOAfYVeTCVszSFAVJVxY", 96477));
    assert(b.medianInvoice() == 40131);
    assert(b.invoice("WdoBQasZVCfVTBhUvxSG", "yLCTpJNKIHsNNWlJeOsJ",32652));
    assert(!b.cancelCompany("CSDOCeUVUXFPvWsISXpt", "losxQGjVxpJBhMpJqLHN"));
    assert(b.audit("LLJjFnnuBvGHOcPWFbLp", sumIncome) && sumIncome == 50195);
    assert(b.medianInvoice() == 39615);
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 39615);
    assert(!b.cancelCompany("syrdlvchzoywlammerem"));
    assert(b.invoice("jIKCnXbDcyNdKLskfoMm", 49971));
    assert(!b.cancelCompany("viWQSIlVZQXOAFpGZQnU", "MBSRVoZqBIVYTRPlbCIA"));
    assert(!b.invoice("JTpfLPBJWkBQqHVDBPWT", "EIaURVAVMYOEkDjoZAGC",21224));
    assert(b.newCompany("cPlQbQUEScFcpWWRifEU", "PVmGSKzCRDWzIgYRQnbN", "YHpgNnZFAcUHVKXNtxyr"));
    assert(b.medianInvoice() == 40131);
    assert(b.audit("hfLSIIDAbLquGDnkFdyW", sumIncome) && sumIncome == 0);
    assert(b.cancelCompany("nheXdpwtSvLQbNFTKHzQ", "bkPDzVYkEXAaWSCCOOGK"));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 40131);
    assert(b.newCompany("UMNUICaISZziZkBRtUpB", "kSNjEqAXNSZLIIASMKVD", "xTyFEaYQKTVjJmGJlMVV"));
    assert(b.medianInvoice() == 40131);
    assert(b.audit("YWHPPHeRwAWrBgzXyQwY", sumIncome) && sumIncome == 40131);
    assert(b.newCompany("XKqlXnGIdwNAPMvPIfKY", "HCWYqTqZyczLpUHHDXQv", "CsGTMnASmqNFYCFmsLuu"));
    assert(b.medianInvoice() == 40131);
    assert(!b.audit("nkgzguahtumwdttitnje", sumIncome));
    assert(!b.audit("GKIUBVDGxwHElUOZZZUF", "bIfXdARBJiZrTHMUDRdA", sumIncome));
    assert(!b.cancelCompany("ARXkUGbVIJXzxLCJRcHQ"));
    assert(b.invoice("ViBVyXAIuxFRznrVOPLw", 16785));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.invoice("pPIXuqUDAuARzNMrAHBH", 54884));
    assert(!b.cancelCompany("shqumgcnwrjisdcnrbeu"));
    assert(b.audit("UmNuICAiszZIzKBRtUPB", "ksnJeqAXNszLIiASMKVd", sumIncome) && sumIncome == 0);
    assert(!b.invoice("PYNXoPLlIGGwkkCzTHBW", 42056));
    assert(b.medianInvoice() == 40131);
    assert(b.audit("CsGTMnASmqNFYCFmsLuu", sumIncome) && sumIncome == 0);
    assert(b.audit("xTyFEaYQKTVjJmGJlMVV", sumIncome) && sumIncome == 0);
    assert(b.audit("YHpgNnZFAcUHVKXNtxyr", sumIncome) && sumIncome == 0);
    assert(b.cancelCompany("hfLSIIDAbLquGDnkFdyW"));
    assert(b.newCompany("zVAcsSJSRxQLlDPRArGK", "DaoQfDHFXqrXOKLVTIvL", "bfIPSvMnGiZQVEwZdrRJ"));
    assert(b.cancelCompany("cPlqBQuEsCFCPWWrIFeU", "pVmGsKZCRDwZiGYRqnBN"));
    assert(b.cancelCompany("WdObQAsZVcFVtbhUvxSG", "YLCTPJNKIHSNnWLJEOSj"));
    assert(!b.cancelCompany("CPLQBQUescfCpWWRIFeu", "PVmgSkZCrdwZIGYRQnBN"));
    assert(b.cancelCompany("UmNUIcaISZZIZkBRTupB", "KSNJEqAXnSZlIIasMkVd"));
    assert(!b.audit("CsdOcEuVUXFPVwsISxPt", "LOSXQGjVXPJBHMpjqLHN", sumIncome));
    assert(b.medianInvoice() == 40131);
    assert(!b.cancelCompany("rtHSBVCLiWmfwhOuBUof"));
    assert(b.audit("aDJJtGkNuRISRMvVPKVt", "XvKQkFMZrJUJAOuCkxXV", sumIncome) && sumIncome == 16785);
    assert(b.audit("YImQvMXxEKHngNIIDBBS", sumIncome) && sumIncome == 60997);
    assert(b.audit("ADJJTGknURiSRMvVPKVT", "xVKqKfMZrjUJaoUcKXxv", sumIncome) && sumIncome == 16785);
    assert(b.audit("bfIPSvMnGiZQVEwZdrRJ", sumIncome) && sumIncome == 0);
    assert(b.cancelCompany("jIKCnXbDcyNdKLskfoMm"));
    assert(b.medianInvoice() == 40131);
    assert(b.audit("zVACSsJSRXQLLDPrARGK", "DAoqfDHFxQrxoKLVtIVL", sumIncome) && sumIncome == 0);
    assert(!b.audit("GKiUbVdGxWhEluOZZzUF", "BIFxDARBJIZRThmUDrda", sumIncome));
    assert(b.audit("YWHPPHeRwAWrBgzXyQwY", sumIncome) && sumIncome == 40131);
    assert(!b.audit("ARXkUGbVIJXzxLCJRcHQ", sumIncome));
    assert(!b.audit("JTpFLPBJwkBQqhVDBPwT", "EiAuRVaVMYoEKDjOZAgC", sumIncome));
    assert(b.cancelCompany("PPxOOlmGKAIkQeweZWCp", "xNNqtKUSIzJHsLZFCqMh"));
    assert(b.audit("zyHLqBQiuJIIiuUTBzch", "CquNkpGNoLXKGDIwjIYX", sumIncome) && sumIncome == 54884);
    assert(!b.cancelCompany("JTPFLPBJWKbqQhvDbPWT", "eIauRVAvmyOEKdJoZagC"));
    assert(!b.cancelCompany("jTPFLPBjWKBQqHVDbPwT", "EIAURvAVmYoekDJOzAGC"));
    assert(!b.audit("PYNXoPLlIGGwkkCzTHBW", sumIncome));
    assert(!b.audit("FZCyFYnlXYUfFRWRswhL", "TgMYZLrRJYETIoBLxTVv", sumIncome));
    assert(b.audit("bfIPSvMnGiZQVEwZdrRJ", sumIncome) && sumIncome == 0);
    assert(b.newCompany("HkABwTFYlOmneMKYGVTV", "YUicIuboPWLYZXLWqReh", "VDdXaQTShZCbsIgbTVPK"));
    assert(b.cancelCompany("zYHlqBqiuJIiIUUTBZCH", "CQuNKpGNOlxkGdIWJIyX"));
    assert(!b.invoice("WDObQASZVCfVTBHUVxsG", "yLCtPJNKIHSNNwLJEOSj",86785));
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.audit("XKQLXNgIDWnapmVPiFKY", "HcWYQtQzYcZLPUhHDxQV", sumIncome) && sumIncome == 0);
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.firstCompany(name, addr) && name == "aDJJTgknuRISRMvvPkVT" && addr == "XVKQKfMZrjUJAOUckxxV");
    assert(b.medianInvoice() == 40131);
    assert(b.medianInvoice() == 40131);
    assert(b.cancelCompany("YWHPPHeRwAWrBgzXyQwY"));
    assert(b.cancelCompany("ADjJTGKNURISRmVVpKvt", "XVKQKfMZrJuJAoUCkXxV"));
    assert(b.newCompany("KhUcDEBxCrsTTrEqUquo", "ISADkeVGXPunQWTxMKQm", "DGzVDtSRfIfEeEbXSAvx"));
    assert(b.invoice("VDdXaQTShZCbsIgbTVPK", 3286));
    assert(!b.audit("YHpgNnZFAcUHVKXNtxyr", sumIncome));
    assert(!b.audit("CplQBquesCFCPWwRIfEU", "pvmGSkZCRDWZigyRQnbN", sumIncome));
    assert(b.invoice("kHUcDEBxCrsTTReQUQUo", "IsaDKEVgxPuNQWTxMKqm",23013));
    assert(b.medianInvoice() == 39615);
    assert(b.audit("bfIPSvMnGiZQVEwZdrRJ", sumIncome) && sumIncome == 0);
    assert(b.medianInvoice() == 39615);
    assert(b.firstCompany(name, addr) && name == "HkABwTFYlOmneMKYGVTV" && addr == "YUicIuboPWLYZXLWqReh");
    assert(!b.cancelCompany("viWQSILVzqxOAfPgZQNu", "MBsRvOZQBiVYtRPLBciA"));
    assert(b.audit("HJgHFoyxGyEYHOpjgIcJ", sumIncome) && sumIncome == 0);
    assert(b.medianInvoice() == 39615);
    assert(!b.invoice("hfLSIIDAbLquGDnkFdyW", 83694));
    assert(b.medianInvoice() == 39615);
    assert(b.medianInvoice() == 39615);
    assert(b.medianInvoice() == 39615);
    assert(b.firstCompany(name, addr) && name == "HkABwTFYlOmneMKYGVTV" && addr == "YUicIuboPWLYZXLWqReh");
    assert(b.newCompany("LBloDAisBUDVLxLKLZCR", "sWhdWTQHSsykgbDREGER", "gOGPRLIVRONSPDNygIPG"));
    assert(!b.cancelCompany("FzCyFYNLXyufFrwRSWhL", "TGMYzlrRJYETiOBLXtVv"));
    assert(b.medianInvoice() == 39615);
    assert(b.newCompany("xWVIYGeESpinJZEuDilG", "NnTwMASriFxEDlEGMSaZ", "JNvMyZVmSVLMFUeyUUCT"));
    assert(b.cancelCompany("LBLODAISBUDVLXLkLzCR", "SWHDwTqHssYkGBDREGEr"));
    assert(!b.invoice("DCTOSBXEZQbGGxTVBtMV", "DytJxMhYfTwZlAVjSTGo",5667));
    assert(b.invoice("khUCDEbXCRSTtREQUqUO", "ISADkEVGxPUNqWtxMkQm",28405));
    assert(b.newCompany("pOEKKMKMUWkAGkkOtWYu", "SYQAKAQeWOAZZHIzlDWT", "yAAXJjzUMXOswHpaJMyH"));
    assert(!b.invoice("gOGPRLIVRONSPDNygIPG", 77672));
    assert(b.invoice("yAAXJjzUMXOswHpaJMyH", 44406));
    assert(b.cancelCompany("kHuCDebXcRSTtrEQUQUO", "ISAdkEvGXPunQWTXMKqM"));
    assert(!b.cancelCompany("wPDbrgvsGBhPmeXWaJGG", "QIUCoRbODHxNQFlZPCUi"));
    assert(b.medianInvoice() == 39615);
    assert(!b.audit("WdOBqAsZVCfVtBHUVXsG", "yLCtPJNkIhsNNwLJEOsJ", sumIncome));
    assert(b.audit("kCIiAuYOBubMxbOfqEBD", "DUfDXTKNZlrVZWFpqSXH", sumIncome) && sumIncome == 0);
    assert(b.medianInvoice() == 39615);
    assert(!b.audit("LbLODAISBuDvLXlkLzCR", "sWHDWTQHSSYKGBDREGEr", sumIncome));
    assert(b.audit("yAAXJjzUMXOswHpaJMyH", sumIncome) && sumIncome == 44406);
    assert(b.firstCompany(name, addr) && name == "HkABwTFYlOmneMKYGVTV" && addr == "YUicIuboPWLYZXLWqReh");
    assert(b.cancelCompany("XkQLXngidWNApMvPiFKy", "HcwYQtqzYCZLpuHhDXQV"));
    assert(b.medianInvoice() == 39615);
    assert(b.audit("POeKkmKMUWKagKkoTwyu", "SyQAKAQEWoAzZHiZLDwt", sumIncome) && sumIncome == 44406);
    assert(b.medianInvoice() == 39615);
    assert(b.medianInvoice() == 39615);
    assert(b.audit("XWviYGEeSPINJZeuDILG", "NNtWMaSriFxEdlEGMsaZ", sumIncome) && sumIncome == 0);
    assert(b.cancelCompany("ZVACSSJSrxQLLdPRArGK", "dAoQFDHFXqRxOKLVtIVl"));
    assert(b.newCompany("JVrVUpNlSvwSiOVNRQWV", "bSpmGLZADVVQCgIkASSi", "lEWPKEzKZRUneAqCvEEW"));
    assert(b.audit("PoeKKmKMUWkaGkkOTWYU", "SYqaKAQEwoAZZHIZldwt", sumIncome) && sumIncome == 44406);
    assert(b.newCompany("JaWtUbUtLROFHrtYeqnQ", "XFvHyrHDHVVYKIqjYMXG", "EfLrnbAMPZUCAUObhYEd"));
    assert(!b.invoice("JTpFlPbJwKBQqHvDBpWT", "EIaURVAVMYoekDJOZagC",50));
    assert(!b.invoice("PYNXoPLlIGGwkkCzTHBW", 92122));
    assert(b.invoice("JvRVUPnLSVwSIOVNRQWV", "BSPMGLZAdVVQCgIkasSI",28616));
    assert(b.medianInvoice() == 32652);

    return EXIT_SUCCESS;

}
#endif /* __PROGTEST__ */