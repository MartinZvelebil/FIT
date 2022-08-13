#include <stdio.h>
#include <stdlib.h>

void LzeDosahnoutSouctu(int pozadovanysoucet, int* vsechnycisla, int delkapole)
{
    int soucetDocasny = 0;
    int dosahnuto = 0;
    for (int i = 0; i < delkapole; ++i)
    {
        for (int j = i+1; j < delkapole; ++j)
        {
            soucetDocasny = vsechnycisla[i] + vsechnycisla[j];
           // printf("%d = %d + %d\n",soucetDocasny,vsechnycisla[i],vsechnycisla[j]);
            if(soucetDocasny == pozadovanysoucet)
            {
                dosahnuto = 1;
                printf("%s %d %s\n","> Soucet",pozadovanysoucet,"lze dosahnout.");
                return;
            }
        }
    }
    if(dosahnuto == 0)
    {
        printf("%s %d %s\n","> Soucet",pozadovanysoucet,"nelze dosahnout.");
    }
}

int main()
{
    printf("Vstupni posloupnost:\n");
    int *vsechnycisla = NULL;
    int dataMaximalni = 0;
    int dataKdeJsemTed = 0;
    int naskenovanecislo;
    char carkaNeboOtaznik;
    int cisloKSouctu;
    int bylOtaznik = 0;

    while (!feof(stdin))
    {
        if (dataMaximalni == dataKdeJsemTed)
        {
            dataMaximalni += dataMaximalni / 2 + 10;
            vsechnycisla = (int *) realloc(vsechnycisla, sizeof(*vsechnycisla) * dataMaximalni);
        }

        if(scanf("%d", &naskenovanecislo) == 1)
        {
            vsechnycisla[dataKdeJsemTed] = naskenovanecislo;
            dataKdeJsemTed++;
            //printf("Cislo: %d\n",naskenovanecislo);
        }
        else
        {
            printf("Nespravny vstup.\n");
            free(vsechnycisla);
            return 1;
        }

        if(scanf(" %c", &carkaNeboOtaznik) == 1 && carkaNeboOtaznik == ',')
        {
            continue;
        }
        else if (carkaNeboOtaznik == '?' && dataKdeJsemTed >= 2)
        {
            bylOtaznik=1;
            printf("%s","Testovane hodnoty:\n");
            while(scanf("%d",&cisloKSouctu) == 1)
            {
                LzeDosahnoutSouctu(cisloKSouctu,vsechnycisla,dataKdeJsemTed);
                //printf("%s","Metoda na delani veci se souctem:\n");
            }
        }
        else if(carkaNeboOtaznik!='?' || carkaNeboOtaznik!=',')
        {
            printf("Nespravny vstup.\n");
            free(vsechnycisla);
            return 1;
        }
    }
    if (bylOtaznik == 0)
    {
        printf("Nespravny vstup.\n");
        return 1;
    }
    free(vsechnycisla);
    return 0;
}