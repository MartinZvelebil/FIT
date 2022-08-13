#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TEmployee
{
  struct TEmployee         * m_Next;
  struct TEmployee         * m_Bak;
  char                     * m_Name;
} TEMPLOYEE;

#endif /* __PROGTEST__ */
void               freeList     ( TEMPLOYEE       * src )
{
    TEMPLOYEE* temp; //Jen deklarace structu dočasně, abychom měli mezi čím přehazovat
    while (src != NULL) //Dokud list není prázdnej
    {
        temp = src; //Uloží aktuálního zaměstnance do tempu - všechny jeho prvky
        src = src -> m_Next; //Změní aktuální ukazatel na zaměstnance na toho dalšího
        free(temp -> m_Name); //Dealokuju jednotlivé m_Names
        free(temp); //Uvolní místo v paměti ukazujícího na zaměstnance
    }
}
TEMPLOYEE        * newEmployee  ( const char      * name,
                                  TEMPLOYEE       * next )
{
    //Alokace paměti nového employee o velikosti structu (Tvorba odkazu na něj)
    TEMPLOYEE *newEmp = (TEMPLOYEE*) malloc(sizeof (TEMPLOYEE));
    if(next == NULL)
    {
        newEmp -> m_Next = NULL; //Když je prázdnej seznam, tak není žádnej next pro nově vytvořeného zaměstnance
    }
    else
    {
        newEmp -> m_Next = next; //Přiřadím pointer na dalšího pracovníka (Možná nejsem sure, nevím jestli přidává na začátek nebo na konec)
    }
    newEmp -> m_Bak = NULL; //Dle zadání nemá nový zaměstnanec nástupce
   // newEmp -> m_Name = (char*)malloc(sizeof (name) * strlen(name));
   // strncpy(newEmp->m_Name,name,sizeof (name)* strlen(name)); //Přiřadím jméno pracovníka, musí být přes strdup, jinak problémy u strcpy
    newEmp -> m_Name = (char*) strdup(name);
    return newEmp; //Vrátím pointer na začátek (Pozice kde začíná nově vytvořený pracovník)
}
TEMPLOYEE        * cloneList    ( TEMPLOYEE       * src )
{
  TEMPLOYEE* source = src; //Pointer na source, který budeme kopírovat
  TEMPLOYEE* newEmployeeList = NULL; //Hloupě pojmenovaná proměnná, prostě ukazatel na první prvek (Head nebo spíš newList prostě)
  TEMPLOYEE* tail = NULL; //Na poslední (tail)

  while(source != NULL) //Dokud je co kopírovat
  {
      if(newEmployeeList == NULL) //Speciální case, pokud mám prázdnej list zatím ještě
      {
          newEmployeeList = (TEMPLOYEE*) malloc(sizeof (TEMPLOYEE));
          newEmployeeList -> m_Name = strdup(source->m_Name);
          newEmployeeList-> m_Bak = NULL; //Zatím jen NULL, než to níže změním
          newEmployeeList -> m_Next = NULL;
          tail = newEmployeeList;
      }
      else
      {
          tail -> m_Next = (TEMPLOYEE*) malloc(sizeof (TEMPLOYEE)); //Na místě, kam ukazuje aktuálně newlist, tak udělám alokaci na další kopírování zaměstnance
          tail = tail -> m_Next;
          tail -> m_Name = strdup(source->m_Name);
          tail -> m_Bak = NULL; //Zatím jen NULL, než to níže změním
          tail -> m_Next = NULL;
      }
      source = source -> m_Next;
  }
    //source počet prvku = newlistemployee počet prvků
    TEMPLOYEE* newEmployeeList_bkcp = newEmployeeList; //Pointer na první prvek spojáku zkopírovaného

    source = src; //Obnovení úvodního pointeru na první prvek
    TEMPLOYEE* helpOldListSrc = src;
    TEMPLOYEE* helpNewList = newEmployeeList_bkcp; //Obnovení pointeru na první prvek
    while (source != NULL) //Kopírování Bak's, dokud je kde hledat bak
    {
        if (source -> m_Bak != NULL) //Kamo našel jsem, že tenhle týpek má nástupce
        {
            helpOldListSrc = src; //Nastavím na výchozí prvek source linked listu který chcí kopírovat
            helpNewList = newEmployeeList_bkcp; //Výchozí prvek seznamu, který je kopií
            while (source -> m_Bak != helpOldListSrc) //Dokud nenajdu bak, počítám kolikátej je ten nástupce (Dělám jenom kvůli counteru)
            {
                helpOldListSrc = helpOldListSrc -> m_Next; //Jdeme dál, pokud jsme nenašli nástupce
                helpNewList = helpNewList -> m_Next;
            } //Tak bych měl mít v helpNewList pointer na Bak v novém poli
            //Na koho to vlastně byl Bak - komu ten bak patří
            newEmployeeList -> m_Bak = helpNewList;
        }
        source = source -> m_Next;
        newEmployeeList = newEmployeeList -> m_Next;
    }
  return newEmployeeList_bkcp;
}

#ifndef __PROGTEST__
int                main         ( int               argc, 
                                  char            * argv [] )
{
    TEMPLOYEE * a, *b;
    char tmp[100];

    assert ( sizeof ( TEMPLOYEE ) == 3 * sizeof ( void * ) );
    a = NULL;
    a = newEmployee ( "Peter", a );
    a = newEmployee ( "John", a );
    a = newEmployee ( "Joe", a );
    a = newEmployee ( "Maria", a );
    a -> m_Bak = a -> m_Next;
    a -> m_Next -> m_Next -> m_Bak = a -> m_Next -> m_Next -> m_Next;
    a -> m_Next -> m_Next -> m_Next -> m_Bak = a -> m_Next;
    //printf("Pamet list A_Next: %p\n",a -> m_Next);
    //printf("Pamet list A_Bak: %p\n",a -> m_Bak);
    assert ( a
             && ! strcmp ( a -> m_Name, "Maria" )
             && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    b = cloneList ( a );
/*
    a = newEmployee ( "Moe", a );
    a = newEmployee ( "Victoria", a );
    a = newEmployee ( "Peter", a );
    b -> m_Next -> m_Next -> m_Next -> m_Bak = b -> m_Next -> m_Next;
    */
       strncpy ( tmp, "Moe", sizeof ( tmp ) );
       a = newEmployee ( tmp, a );
       strncpy ( tmp, "Victoria", sizeof ( tmp ) );
       a = newEmployee ( tmp, a );
       strncpy ( tmp, "Peter", sizeof ( tmp ) );
       a = newEmployee ( tmp, a );
       b -> m_Next -> m_Next -> m_Next -> m_Bak = b -> m_Next -> m_Next;
/*
      char *tvojemam = a -> m_Name;
      printf(tvojemam);
      char *tvojema = a -> m_Next -> m_Name;
      printf(tvojema);
      char *tvojemama = a -> m_Next -> m_Next -> m_Name;
      printf(tvojemama);
      char *tvojemama2 = a -> m_Next -> m_Next -> m_Next -> m_Name;
      printf(tvojemama2);
      char *tvojemama3 = a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name;
      printf(tvojemama3);
    */
    assert ( a
             && ! strcmp ( a -> m_Name, "Peter" ) //Peter
             && a -> m_Bak == NULL );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Victoria" ) //Peter
             && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" ) //Peter
             && a -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" ) //Maria
             && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" ) //Joe
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" ) //John
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) //Peter
             && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );

    char *tvojemam = b -> m_Name;
    printf(tvojemam);
    char *tvojema = b -> m_Next -> m_Name;
    printf(tvojema);
    char *tvojemama = b -> m_Next -> m_Next -> m_Name;
    printf(tvojemama);
    char *tvojemama2 = b -> m_Next -> m_Next -> m_Next -> m_Name;
    printf(tvojemama2);
    // printf("Pamet list B_Next: %p\n", b -> m_Next);
    //printf("Pamet list B_Bak: %p\n", b -> m_Bak); //This nigger ukazuje furt na a -> m_bak
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" ) //Maria
             && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" ) //Joe
             && b -> m_Next -> m_Bak == NULL );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" ) //John
             && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) //Peter
             && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( a );
    b -> m_Next -> m_Bak = b -> m_Next;
    a = cloneList ( b );
    //printf("%s\n",a->m_Next->m_Next->m_Name);
    //char *name = a -> m_Name;
    //printf(name);
    assert ( a
             && ! strcmp ( a -> m_Name, "Maria" )
             && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
             && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
             && a -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
             && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
             && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    assert ( b
             && ! strcmp ( b -> m_Name, "Maria" )
             && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
             && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
             && b -> m_Next -> m_Bak == b -> m_Next );
    assert ( b -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
             && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
             && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
             && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( b );
    freeList ( a );
    return 0;
}
#endif /* __PROGTEST__ */
