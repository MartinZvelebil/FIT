#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */
char **separateWords2(char * str, int * counter)
{
    //Kopírování input stringu do nového stringu
    //char str[lengthOfString];
    //strncpy(str,string,lengthOfString); //Překopíruje lengthOfString znaků z input stringu do pole charů u mě v metodě
    //char words[10][10];

    // Polo - Dynamická alokace
    char **words;
    words = (char**)malloc(sizeof (char*) * 1000); //[row_size][column_size] (Maximálně row_size stringů s column_size charakterů)
    for (int i = 0; i < 1000; ++i)
    {
        words[i] = (char*)malloc(sizeof (char*) * 1000);
    }

    //Vyřazovací for speciálních znaků
    for (int i = 0; i < (int) strlen(str); i++)
    {
        if(str[i] < 97 || str[i] > 122) //Pokud to není písmenko, tak to nahradím mezerou
        {
            str[i]=' ';
        }
    }

    //Zápis slov do pole
    char *pointer = strtok(str, " "); //Strtok = string, který se bude splitovat, string se všemi rozdělovači, vrací první výskyt toho, co není delimiter (jako pointer)
    while(pointer != NULL) //Pokud je ještě vůbec co zkoumat na stringu - tak mi byl navrácen do pointeru místo, kde je ten nový token
    {
        strcpy(words[*counter],pointer); //Kopíruju na místo, kam ukazuje ten pointer ted - to co je další slovo ve slovech
        *counter += 1; //Netuším, proč to nefunguje uvnitř strcpy
        pointer = strtok(NULL, " "); //" " - Char, podle kterého se to celé rozděluje
    }

    //Vypisovací část
    /*
    for (int i = 0; i < *counter; ++i)
    {
       // printf("%s\n", words[i]);
    } */
    return words;
}
int sameWords ( const char * a, const char * b )
{
    //Vše na malá písmena
    char * string1 = strdup(a); //strdup nakopíruje string a alokuje paměť
    char * string2 = strdup(b);

    for(int i = 0; string1[i]; i++)
    {
        string1[i] = tolower(string1[i]);
    }
    for(int i = 0; string2[i]; i++)
    {
        string2[i] = tolower(string2[i]);
    }

    //Výpis a ověření s dvourozměrným polem charů
    char** poleslov1;
    char** poleslov2;
    int counter1 = 0;
    int counter2 = 0;

    poleslov1 = separateWords2(string1, &counter1);
    poleslov2 = separateWords2(string2, &counter2);
    /*
    printf("Prvni pole     : \n");
    for (int i = 0; i < counter1; ++i)
    {
        printf("%s\n", poleslov1[i]);
    }
    printf("\n");
    printf("Druhy pole     : \n");
    for (int i = 0; i < counter2; ++i)
    {
        printf("%s\n", poleslov2[i]);
    }
    printf("\n");
    */
    //Porovnani, zda-li jsou stejny - return value decision
    int foundmatchcounter = 0;
    for (int i = 0; i < counter1; ++i)
    {
        for (int j = 0; j < counter2; ++j)
        {
            if(strcmp(poleslov1[i],poleslov2[j]) == 0)
            {
                foundmatchcounter++;
                //printf("jsou shodne\n");
                break;
            }
        }
    }
    int returnValue = 0;
    if(foundmatchcounter == counter1 || foundmatchcounter == counter2) //Pokud bylo těch matched z prvního pole stejně jako jich tam bylo, tak je to úspěch
    {
        returnValue = 1;
    }

    //Dealokace paměti

    for (int i = 0; i < 1000; ++i)
    {
        free (poleslov1[i]);
        free (poleslov2[i]);
    }
    free (poleslov2);
    free (poleslov1);
    free (string1);
    free (string2);

    return returnValue;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  assert ( sameWords ( "Hello students.", "HELLO studEnts!" ) == 1 );
  assert ( sameWords ( "  He said 'hello!'", "'Hello.' he   said." ) == 1 );
  assert ( sameWords ( "He said he would do it.", "IT said: 'He would do it.'" ) == 1 );
  assert ( sameWords ( "one two three", "one two five" ) == 0 );
    printf("%d\n",sameWords ( "", "  " ));
    printf("%d\n",sameWords ( "  ", "" ));
    printf("%d\n",sameWords ( "", ""));
    printf("%d\n",sameWords ( " ", " " ));
    printf("%d\n",sameWords ( "slovo", ""));
    printf("%d\n",sameWords ( " slovo", " " ));
    /*
    printf("%d\n",sameWords ( "Hello students.", "HELLO studEnts!" ));
    printf("%d\n",sameWords( " He said 'hello!'", "'Hello.' he   said." ));
    printf("%d\n",sameWords( "He said he would do it.", "IT said: 'He would do it.'" ));
    printf("%d\n",sameWords( "one two three", "one two five" ));*/
  return 0;
}
#endif /* __PROGTEST__ */
