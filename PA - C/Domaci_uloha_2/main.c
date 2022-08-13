#include <stdio.h>
#include <string.h>

int HowManyChars(char *string)
{
    int c = 0;
    while (string[c] != '\0') //dokud char není prázdný, tak c++ (Prostě pokud tam není ten ukončovací znak \0
    {
        c++;
    }
    return c;
}

int ConvertToAnyBase(int decadicNumber, int base, int flag) //čísla budu jen vypisovat
{
    char numberInBase[64]=""; //max longint decimal in binary + last bit for věc
    int remainder = 0;
    int length_converted = 0;
    while(decadicNumber!=0) //Dokud není dekadické číslo nulou -> není kompletně převedeno
    {
        remainder = decadicNumber % base;
        if(remainder>=10) //Pokud ten zbytek po dělení base je větší než 10, tak ho převedu na písmenko
        {
            char temp = 87 + remainder; //87 = konstanta, kterou vždy přičtu k číslu, pokud je to číslo 10+
            strncat(numberInBase, &temp, 1); //Přidávám písmenko do polenumberInBase
        }
        else if(remainder<10) //Pro zbytky pod 10 bude reprezentací číslo, který má jinou konstantu v ascii
        {
            char temp = 48+remainder;
            strncat(numberInBase, &temp, 1);
        }
        decadicNumber = decadicNumber / base;
    }

    //Tohle je naprosto příšerný a mělo by to jít do funkce -> obrací to pořadí prvků
    int length;
    int i;
    length = strnlen( numberInBase ,64);
    int j = length -1;
    char temp;
    for( i = 0; i <= (length-1) /2; i++) {
        temp = numberInBase[i];
        numberInBase[i] = numberInBase[j];
        numberInBase[j] = temp;
        j--;
    }
    //Tady končí příšernost

    if(flag == 1)
    {
        printf("%s\n", numberInBase);
    }
    length_converted = HowManyChars(numberInBase); //Metoda na to, kolik to má charů

    return length_converted; //Navrátí délku toho stringu, co jsem vygeneroval
}

int main()
{
    int position;
    int numberSystem;
    int conversion;
    printf("%s\n", "Pozice a soustava:"); //Čtu file, dokud má inputy
    while ((conversion = (scanf("%d %d", &position, &numberSystem))) != EOF) //To proč tam jsou troje uvozovky je nějaká syntax chyba věc, aby to vyhodnotilo cleý výraz
    {
        int enoughGenerated = 0; // Pouze int na escape z while
        int searchedNumber = 1; //Jaké číslo vkládám do pole
        int digitsGenerated = 0;
        int exactNumberPosition = 0; //Ukazuje na přesný digit
        int flag = 0;

        if (conversion != 2 || numberSystem < 2 || numberSystem > 36 || position < 0) //pokud nenačetl scanf obě dvě čísla nebo je číslo mimo range soustav, tak špatný vstup
        {
            printf("%s\n", "Nespravny vstup.");
            return 1; //mby to pak udělá chyby
        }

        //Tady budu projíždět forem pole, kde budu zjistovat, že nebylo dost vygenerováno a vždy si zavolám generování nového čísla přes mou metodu
        while (enoughGenerated == 0)
        {
            if (position != 0) //Pokud je pozice 0, tak totiž vzniká aritmetická chyba, kde je pozice pro číslo 0, což u mě znamená v počtu mezer -1
            {
                exactNumberPosition = position - digitsGenerated; //Zajímá mě, kde se reálně nachází hledaná cifra
            }
            else
            {
                printf("%d\n",0); //Neelegantní řešení toho, když přijde na vstupu na pozici 0
                exactNumberPosition = 1;
                break;
            }

            //Extrémně neefektivní řešení
            int Predict_one_ahead = digitsGenerated+ConvertToAnyBase(searchedNumber, numberSystem, flag);
            if(Predict_one_ahead >= position)
            {
                flag = 1;
            }
            //Extrémně neefektivní řešení konec

            digitsGenerated += ConvertToAnyBase(searchedNumber, numberSystem, flag); //Kolik cifer bylo již vygenerováno
            if (digitsGenerated >= position) //Pokud bylo vygenerováno více cifer než chce uživatel jako pozici, tak nemusím dál generovat
            {
                enoughGenerated = 1; //nastavím si flag enoughGenerated na 1 - ukončím while, že už je dost čísel vygenerovaných
            }
            else
            {
                searchedNumber++; //zvyšuji decimálně hledané číslo
            }
        }

        //printf("%d\n", searchedNumber); //Vypíše výsledné číslo
        printf("%*s%c\n", exactNumberPosition - 1, "", '^'); //Exact number position je totiž na místě toho, kde má být šipka nahoru a mě zajímá počet mezer
    }

    return 0;
}
