#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct NodeOfTree
{
    NodeOfTree * left = nullptr;
    NodeOfTree * right = nullptr;
    char ascii;
    bool endOfBranch;
};

//Function, that converts scanned characters to their 8-bit binary equivalent
//For smaller numbers it also fill it to the 8-bit
deque<bool> asciiToBinary(int n)
{
    deque<bool> binaryNumber;
    int i = 0;
    while (n > 0) //Výpočet od nejnižších bitů
    {
        binaryNumber.push_front(n % 2);
        n = n / 2;
        i = i + 1;
    }
    while(binaryNumber.size() < 8) //Doplnění 0 zepředu na 8 bit (od nejvyšších bitů)
    {
        binaryNumber.push_front(false);
    }
    return binaryNumber;
}

//Function for nice formatted output of binary code
void outputAndFormat(const deque<bool> &bits)
{
    int oddelovac = 0;
    for (bool i : bits)
    {
        cout << i;
        oddelovac ++;
        if(oddelovac % 48 == 0)
            cout << endl;
        else if(oddelovac % 8 == 0)
            cout << " ";
    }
    cout << endl;
    cout << "Buffer size: " << bits.size() << endl;
}

//Converting 8-bit binary from the front of bits deque to it's ascii value
int binaryToAscii(deque<bool> &bits)
{
    //Chyba, pokud přijde málo bitů
    if(bits.size() < 8)
        return -1;

    int tmp[8] = {0};
    //Načti bity tak jak jsou
    for (int i = 8; i > 0; --i)
    {
        //cout << bits.front();
        tmp[i] = bits.front();
        bits.pop_front();
    }

    int asciiVal = 0;
    int divisor = 128;
    for (int i = 8; i > 0; --i)
    {
        asciiVal += tmp[i] * divisor;
        divisor = divisor/2;
    }
    //cout << " | " << "ASCII: " << asciiVal << endl;
    return asciiVal;
    //outputAndFormat(bits);
}

//Function creating the tree with all the encoding parameters
NodeOfTree * createTree(deque<bool> &bits, bool &error_flag)
{
    NodeOfTree *NewNode = new NodeOfTree; //Tvorba nového lístku/větve

    //Pokud nepřišly žádné bity, vrať prázdný node
    if(bits.empty())
    {
        error_flag = true;
        return NewNode;
    }

    //Default inicializace
    NewNode -> endOfBranch = false;
    NewNode -> right = nullptr;
    NewNode -> left = nullptr;
    NewNode -> ascii = -1; //To se pak bude hodit u určování, co je branch a co je lísteček

    //Při jedničce to znamená, že jsem našel lístek stromu -> přiřadím mu ASCII value následujících 8 bitů
    if (bits.front() == 1)
    {
        bits.pop_front(); //Zahodím první bit, co jsem ted nascanoval
        int charVal = binaryToAscii(bits);

        //Dělám pouze pro ASCII 0-127, pokud je rozsah mimo, tak error
        if (charVal < 0 || charVal >= 128)
        { error_flag = true; return NewNode; }

        NewNode -> ascii = charVal;
        NewNode -> endOfBranch = true;
        return NewNode;
    }
    //Při 0 to znamená, že vytvářím "mezičlánek - stavím větev, nikoliv lístek"
    else if (bits.front() == 0)
    {
        bits.pop_front(); //Zahodím první bit, co jsem ted zpracoval
        //Vždycky jdu směrem doleva, kde opět vytvářím rekurzivně s dalším bitem větev/lístek
        //Nutnost rekurze, protože nevím, kdy bude první lísteček
        NewNode -> left = createTree(bits, error_flag);
        if (error_flag)
        {return NewNode;} //Return spíš jen aby byl klid, stejně to pak prokaučuju
    }

    NewNode -> right = createTree(bits, error_flag);
    return NewNode;
}

//Function for removing the dynamically alocated tree
//Finding the lowest elements of the tree and removing them in the dirrection from bottom to the top
void freeTree(NodeOfTree * singleNodeOfTree)
{
    //Pokud jsem u lístečku (nemá ani pravý ani levý element, tak ho prostě smažu
    if(singleNodeOfTree -> left == nullptr && singleNodeOfTree -> right == nullptr)
    {
        delete singleNodeOfTree;
        return;
    }
    //Pokud ještě je něco na pravé straně, tak tam pokračuj
    else if(singleNodeOfTree -> right != nullptr)
    {freeTree(singleNodeOfTree->right);}
    //Pokud je ještě něco na levé straně, tak tam pokračuj
    else if(singleNodeOfTree -> left != nullptr)
    {freeTree(singleNodeOfTree->left);}

    //Smaž tu default node
    delete singleNodeOfTree;
}

//Check, how many bites you are going to decompress
//If returns -1, it means, that no more data about chunks were found (less than 12 bites)
int checkChunkLength(deque<bool> &bits)
{
    //Pokud už zbývá méně než 12 bitů, tak je to špatně
    if(bits.size() < 12)
        return -1;

    int numberOfChars = 0;
    int multiplier = 4096;
    //Pokud následuje celej chunk, tak prostě bude 4096 znaků
    if(bits.front() == 1)
    {
        bits.pop_front();
        numberOfChars = 4096;
    }
    //Pokud je 0 jako první, tak nenásleduje celej chunk, musím si spočítat počet znaků
    else if(bits.front() == 0)
    {
        for (int i = 0; i <= 12; ++i)
        {
            numberOfChars += bits.front()*multiplier;
            multiplier = multiplier / 2;
            bits.pop_front();
        }
    }
    return numberOfChars;
}

//Function that is reading data and decompressing it
bool decompressData(deque<bool> & bits, const char *outFileName, NodeOfTree * node)
{
    //Kolik mám řešit písmenek
    int chunkLength = checkChunkLength(bits);

    //Když nešlo získat více info o chunku - neměla hlavička požadované vlastnosti
    if(chunkLength == -1)
        return false;

    //Když chunk udává, že je víc dat, než ve skutečnosti je
    if((int)(bits.size()) < chunkLength)
        return false;

    //cout << "Chunk length: " << chunkLength;

    //cout << endl << "DATA ONLY:" << endl;
    //outputAndFormat(bits);
    //cout << "----------------------------------" << endl;

    fstream outputFile(outFileName, ios::out);
    if(!outputFile.is_open() || !outputFile.good())
        return false;

    NodeOfTree * top = node;
    while(chunkLength != 0 && !bits.empty())
    {
        //Pokud ascii hodnota node je neplatná -> jedná se o branch, tak se posouvám někam na lísteček
        while(node -> ascii < 0 || node -> ascii > 127)
        {
            if(bits.front() == 0) //Na nule jdu doleva
            {
                bits.pop_front();
                node = node -> left;
            }
            else if(bits.front() == 1) //Na jedničce jdu doprava
            {
                bits.pop_front();
                node = node -> right;
            }
        }

        //Node -> ascii = písmenko
        outputFile << node -> ascii;
        //cout << node -> ascii;
        node = top; //Opět nastavím node na topovou, když jsem našel písmenko
        chunkLength--; //Našel jsem písmenko -> zmenšuju písmenkový buffer

        if(chunkLength == 0) //Pokud už nejsou znaky dle chunk ověření
        {
            while(!bits.empty()) //Tak ověř, že už není prázdný buffer bitů
            {
                bool IsOnlyZero = (bits.end() == std::find(bits.begin(), bits.end(), true)); //Z INTERNETU - ověřuje, že jsou v bitech pouze nuly
                if(IsOnlyZero) //Pokud to jsou už jen nuly, tak je prostě vyflusej ven
                {
                    while(!bits.empty())
                    {
                        bits.pop_front();
                    }
                }
                else //Pokud fakt není prázdný a ani to nejsou samé nuly, tak udělej check na další chunk
                {
                    chunkLength = checkChunkLength(bits); //Jen vlastně refreshnu chunkLength
                    //cout << "BIT SIZE: " << bits.size() << endl;
                    if(chunkLength == -1) //Pokud bylo v hlavičce chunk-checku méně než 12 bitů
                    {
                        //cout << endl << "Chunk length == -1" << endl;
                        outputFile.close();
                        return false;
                    }
                    else if(chunkLength > ((int)bits.size())) //Pokud chunk length tvrdí, že je více písmen, než je ve skutečnosti
                    {
                        //cout << endl << "Chunk length mi lže: " << chunkLength << " | "<< bits.size() << endl;
                        remove(outFileName);
                        return false;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    if (outputFile.bad() || chunkLength != 0)
    {
        outputFile.close();
        remove(outFileName);
        return false;
    }
    outputFile.close();

    return true;
}

//Function, that manages all the decompression
bool decompressFile ( const char * inFileName, const char * outFileName )
{
    bool error_flag = false;

    //Inicializace otevření souboru
    ifstream input;
    input.open( inFileName, ios::in | ios::binary );

    //Ověření vyjímek
    if (!input.is_open())
        return false;
    if (input.eof())
        return false;

    //Načtu vše jako chary
    vector<unsigned char> buffer(istreambuf_iterator<char>(input), {}); //Černá magie, které ještě příliš nerozumím

    //Convertnu chary na 1 a 0
    deque<bool> bits;
    for (unsigned char i : buffer) //Každý char convertuju na bity
    {
        deque<bool> tmp = asciiToBinary(i); //Dočasně uložím z metody ty 1 a 0 a pak je dám do mého bufferu bits
        for(bool j : tmp)
        {
            bits.push_back(j);
        }
    }

    //Pouze výpis + formátování
    //cout << endl << "Before tree:" << endl;
    //outputAndFormat(bits);

    NodeOfTree * tree = createTree(bits, error_flag);
    //Pokud nastala chyba při tvorbě stromu, tak nemá cenu pokračovat a ukončuju funkci
    if(error_flag)
    {
        //cout << "err flag free" << endl;
        input.close();
        freeTree(tree);
        return false;
    }

    //Výpis po tvorbě stromu
    //cout << endl << "After tree:" << endl;
    //outputAndFormat(bits);

    //Dekomprese souboru
    bool decompresion = decompressData(bits, outFileName, tree);
    if(!decompresion)
    {
        //cout << "decompresion free" << endl;
        input.close();
        freeTree(tree);
        return false;
    }

    input.close();
    //freeTree(tree);
    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
    // todo: Ta je jen pro mě pro ověření, když bych chtěl - preferuju terminal diff
    return true;
}

int main ( void )
{
    //assert( decompressFile("tests/test4.huf", "tempfile.txt"));
    //assert(decompressFile ( "tests/test0.huf", "tempfile.txt"));
    //decompressFile ( "tests/test1.huf", "tempfile.txt" );
    assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

    assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );

    return 0;
}
#endif /* __PROGTEST__ */
