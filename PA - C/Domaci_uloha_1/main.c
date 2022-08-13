#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>

int main() {
    double O1rozmer1, O1rozmer2, O1rozmer3; //ještě inicializovat
    char O1identifier;
    char O1typobrazce[16];
    double O1obsah, O1obvod;

    // načtu první 1. obrazec, definuju věci pro něj, kam přiřadím hodnoty
    // if (eof věci) ( ! feof( st din ) ) - někdy je to potřeba pro end of file věci
    printf("Obrazec #1\n");
    int pocetkonverzi = scanf(" %c %lf %lf %lf",&O1identifier,&O1rozmer1,&O1rozmer2,&O1rozmer3); //scanf, který ovšem může načítat i méně, než to co má definované (třeba jen 1 písmeno a 1 double)
    if(pocetkonverzi == 2 && O1identifier == 'S' && O1rozmer1 > 0) //pokud čtverec, ověřuju jen že vstup není 0 nebo zápot
    {
        O1obsah = O1rozmer1 * O1rozmer1;
        O1obvod = O1rozmer1 * 4;
        strcpy(O1typobrazce, "ctverec #1");
    }
    else if(pocetkonverzi == 3 && O1identifier == 'R' && O1rozmer1 > 0 && O1rozmer2 > 0 && O1rozmer1 != O1rozmer2) //pokud obdelník, ještě ověřuju, že to není ve skutečnosti čtverec
    {
        O1obsah = O1rozmer1 * O1rozmer2;
        O1obvod = (O1rozmer1 * 2) + (O1rozmer2 * 2);
        strcpy(O1typobrazce, "obdelnik #1"); //do pole charů / stringu nelze nahrávat, takže se musí tam kopírovat pomocí strcpy
    }
    else if(pocetkonverzi == 4 && O1identifier == 'T' && O1rozmer1 > 0 && O1rozmer2 > 0 && O1rozmer3 > 0
           && (fabs((O1rozmer1 + O1rozmer2) - O1rozmer3) > 1024 * DBL_EPSILON * (O1rozmer1 + O1rozmer2)) //trojuhelnikova nerovnost
           && (fabs((O1rozmer2 + O1rozmer3) - O1rozmer1) > 1024 * DBL_EPSILON * (O1rozmer2 + O1rozmer3))
           && (fabs((O1rozmer1 + O1rozmer3) - O1rozmer2) > 1024 * DBL_EPSILON * (O1rozmer1 + O1rozmer3))) //počítá s nepřesností doublů při operacích
    {
        O1obvod = O1rozmer1+O1rozmer2+O1rozmer3;
        double s  = O1obvod / 2;
        O1obsah = sqrt(s*(s-O1rozmer1)*(s-O1rozmer2)*(s-O1rozmer3)); //heronův vzorec
        strcpy(O1typobrazce, "trojuhelnik #1");
        //v prezentaci je dbl epsilon apod věci
    }
    else
    {
        printf("%s","Nespravny vstup.\n");
        return 1;
    }

    //načtu 2. obrazec, definuju věci pro něj, kam přiřadím hodnoty
    double O2rozmer1, O2rozmer2, O2rozmer3;
    char O2identifier;
    double O2obsah, O2obvod;
    char O2typobrazce[16];

    printf("Obrazec #2\n");
    int pocetkonverzi2 = scanf(" %c %lf %lf %lf",&O2identifier,&O2rozmer1,&O2rozmer2,&O2rozmer3);
    if(pocetkonverzi2 == 2 && O2identifier == 'S' && O2rozmer1 > 0)
    {
        O2obsah = O2rozmer1 * O2rozmer1;
        O2obvod = O2rozmer1 * 4;
        strcpy(O2typobrazce, "ctverec #2");
    }
    else if(pocetkonverzi2 == 3 && O2identifier == 'R' && O2rozmer1 > 0 && O2rozmer2 > 0 && O2rozmer1 != O2rozmer2)
    {
        O2obsah = O2rozmer1 * O2rozmer2;
        O2obvod = (O2rozmer1 * 2) + (O2rozmer2 * 2);
        strcpy(O2typobrazce, "obdelnik #2");
    }
    else if(pocetkonverzi2 == 4 && O2identifier == 'T' && O2rozmer1 > 0 && O2rozmer2 > 0 && O2rozmer3 > 0
            && (fabs((O2rozmer1 + O2rozmer2) - O2rozmer3) > 1024 * DBL_EPSILON * (O2rozmer1 + O2rozmer2)) //trojuhelnikova nerovnost
            && (fabs((O2rozmer2 + O2rozmer3) - O2rozmer1) > 1024 * DBL_EPSILON * (O2rozmer2 + O2rozmer3))
            && (fabs((O2rozmer1 + O2rozmer3) - O2rozmer2) > 1024 * DBL_EPSILON * (O2rozmer1 + O2rozmer3))) //možná bude potřeba divná math věc dělat i tady (epsilon atd)
    {
        O2obvod = O2rozmer1+O2rozmer2+O2rozmer3;
        double s  = O2obvod / 2;
        O2obsah = sqrt(s*(s-O2rozmer1)*(s-O2rozmer2)*(s-O2rozmer3));
        strcpy(O2typobrazce, "trojuhelnik #2");
    }
    else
    {
        printf("%s","Nespravny vstup.\n");
        return 1;
    }

    //v tuhle chvíli mám 100% 1) Obvod a obsah 1. obrazce 2) Obvod a obsah 2. obrazce
    if(fabs(O1obvod-O2obvod) <= 1024 * DBL_EPSILON * (O1obvod + O2obvod)) //if, kde první je důležitý zjišťovat, jestli si nejsou obvody rovný, což se dělá s tím epsilon
    {
        printf("%s %s %c %s\n","Obvod:",O1typobrazce,'=',O2typobrazce);
    }
    else if (O1obvod > O2obvod) //pokud už víme že nejsou rovný, tak už lze lidsky porovnávat a ne divný věci
    {
        printf("%s %s %c %s\n","Obvod:",O1typobrazce,'>',O2typobrazce);
    }
    else if (O1obvod < O2obvod)
    {
        printf("%s %s %c %s\n","Obvod:",O1typobrazce,'<',O2typobrazce);
    }


    if(fabs(O1obsah-O2obsah) <= 1024 * DBL_EPSILON * (O1obsah + O2obsah)) //stejně jako viz výše jen obsah
    {
        printf("%s %s %c %s\n","Obsah:",O1typobrazce,'=',O2typobrazce);
    }
    else if (O1obsah > O2obsah)
    {
        printf("%s %s %c %s\n","Obsah:",O1typobrazce,'>',O2typobrazce);
    }
    else if (O1obsah < O2obsah)
    {
        printf("%s %s %c %s\n","Obsah:",O1typobrazce,'<',O2typobrazce);
    }
    return 0;
}
