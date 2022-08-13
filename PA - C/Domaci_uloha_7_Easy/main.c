#include <stdio.h>
#include <stdlib.h>
int PrintAllPossiblePartiallyWorking(const int * velikosti, int delka_pole) //Rekurze, která vypíše všechny ty kombinace z toho sorted pole
{
    //printf("Delka pole = %d\n",delka_pole);
    int counter = 0;
    if (delka_pole == 0)
    {
        return counter;
    }
    else
    {
        int flag = 0;
        int cnt = 1;
        PrintAllPossiblePartiallyWorking(velikosti, delka_pole - 1);
        for (int i = 0; i < delka_pole; ++i)
        {
            printf("%d ", velikosti[i]);
            /*
            while (flag == 0 && cnt < delka_pole)
            {

                if(velikosti[i+cnt] < velikosti[i])
                {
                    printf("%d ", velikosti[i+cnt]);
                    cnt++;
                    printf("%d ", velikosti[i]);
                }
                else
                {
                    flag = 1;
                }
            }*/
            counter ++;
        }
        printf("\n");
        //PrintAllPossible(velikosti + 1, delka_pole - 1);
    }
    return counter;
}

int CompareForQsort (const void * a, const void * b) //Tato funkce je trochu zbytečná, jde to i bez ní přímo pomocí parametru do qsort
{
    return ( *(int*)b - *(int*)a ); //Takhle je to od největšího po nejmenší. Pokud by to bylo a-b, tak je to od nejmenšího po největší
}
void EndProgram(int * vsechny_velikosti)
{
    printf("Nespravny vstup.\n");
    free(vsechny_velikosti);
}
int main() {
    printf("Pocet kostek:\n");
    int *vsechny_velikosti = 0;
    int data_max = 0;
    int data_ted = 0;
    int pocet_kostek = 0;
    int naskenovanecislo = 0;
    int koliknaskenovano_velikosti = 0;

    //Začátek skenování
    if(scanf("%d", &pocet_kostek) == 1 && pocet_kostek > 0) //Pokud se povede načíst počet kostek (Jsou intem větším než 0)
    {
        printf("Velikosti:\n");
        while(!feof(stdin)) //Dokud je co skenovat (Dokud je vstup od uživatele)
        {
            if(data_max == data_ted) //Pokud jsem došel na maximum aktuální kapacity pole, tak realokuj
            {
                data_max = data_max / 2 + 4;
                vsechny_velikosti= (int*)realloc(vsechny_velikosti,sizeof (*vsechny_velikosti) * data_max);
            }

            if(scanf("%d", &naskenovanecislo) == 1) //Postupně zkouším skenovat číslo za číslem, dokud jsou konverze uspěšný
            {
                if(naskenovanecislo > 0) //Naskenované číslo musí být větší než 0
                {
                    vsechny_velikosti[data_ted] = naskenovanecislo; //Přiřadím naskenované číslo do pole
                    koliknaskenovano_velikosti++;
                    data_ted++; //Zvýším index na kterém se aktuálně zapisuje
                }
                else //Číslo bylo menší než 0
                {
                    EndProgram(vsechny_velikosti);
                    return 1;
                }
            }
            else //Pokud konverze následujícího čísla nebyla úspěšná, tak konec
            {
                if(koliknaskenovano_velikosti != pocet_kostek) //ALE JEN! Pokud nebylo naskenovano stejně velikosti jako kostek
                {
                    EndProgram(vsechny_velikosti);
                    return 1;
                }
            }
        }
    }
    else
    {
        EndProgram(vsechny_velikosti);
        return 1;
    }
    //Konec skenování

    /*
    printf("Before sorting:\n");
    for (int i = 0; i < koliknaskenovano_velikosti; ++i)
    {
        printf("%d\n",vsechny_velikosti[i]);
    }*/
    //Setřídění prvků
    qsort(vsechny_velikosti, koliknaskenovano_velikosti, sizeof (int), CompareForQsort); //(pointer na první prvek, kolik celkem prvku, velikost prvku, funkce porovnávající dva prvky)

    printf("Array After sorting:\n");
    for (int i = 0; i < koliknaskenovano_velikosti; ++i)
    {
        printf("%d ",vsechny_velikosti[i]);
    }
    printf("\n");
    printf("---------------------------\n");

    //Výpis všech kombinací, asi nějaká rekurze
    int pocet_prvku_vpoli = data_ted;
    int counter_moznosti_seskladani = 0;
    for (int i = 0; i < pocet_prvku_vpoli; ++i)
    {
        counter_moznosti_seskladani+= PrintAllPossiblePartiallyWorking(vsechny_velikosti + i, data_ted - i); //Data_ted = kolik bylo naskenovaných hodnot
    }
    printf("Celkem: %d\n", counter_moznosti_seskladani);
    return 0;
}