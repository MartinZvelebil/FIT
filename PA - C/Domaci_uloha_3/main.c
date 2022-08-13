#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */
int isTimespanValid(int year1, int month1, int day1, int hour1, int minute1,
                    int year2, int month2, int day2, int hour2, int minute2) /*Function for checking if the first date is before the second date.*/
{
    if(year1 < year2)
    {
        return 0;
    }
    else if (year1 > year2)
    {
        return 1;
    }
    else if (year1 == year2) //Tady je to tak dlouhý, že jsem zvolil závorkování, co normálně nepoužívám
    {
        if (month1 > month2) {
            return 1;
        } else if (month1 < month2) {
            return 0;
        } else if (day1 > day2) {
            return 1;
        } else if (day1 < day2) {
            return 0;
        } else if (hour1 > hour2) {
            return 1;
        } else if (hour1 < hour2) {
            return 0;
        } else if (minute1 > minute2) {
            return 1;
        } else if (minute1 < minute2) {
            return 0;
        } else {
            return 0;
        }
    }
    else{
        return 0;
    }
}
int isDateValid (int year, int month, int day, int hour, int minute) /*Function for checking if input date is valid. YES = return 1 | NO = return 0*/
{
    if(year >= 1600 &&
      month >= 1 && month <= 12 &&
      hour >= 0 && hour <= 23 &&
      minute >= 0 && minute <= 59) {
        if ((day == 29 && month == 2) && ((year % 400 == 0 && year % 4000 != 0) || (year % 4 == 0 && year % 100 !=0))) { //Požadavek na přestupný rok - 29 dní má únor
            return 1;
        } else if ((day >= 1 && day <= 28) && (month == 2)){ //Pokud únor není přestupný
            return 1;
        } else if ((day >= 1 && day <= 30) && (month == 4 || month == 6 || month == 9 || month == 11)){ //Podmínka pro všechny měsíce s 30 dny
            return 1;
        } else if ((day >= 1 && day <= 31) && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
            return 1;
        } else {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
int valueOfDigit ()
{
    int zero = 126; //1111110
    int one = 48; //0110000
    int two = 109; //1101101
    int three = 121; //1111001
    int four = 51; //0110011
    int five = 91; //1011011
    int six = 95; //1011111
    int seven = 112; //1110000
    int eight = 127; //1111111
    int nine = 123; //1111011
}
int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption ) /*Function, ...*/
{
    if(isDateValid(y1, m1, d1, h1, i1) == 0 || isDateValid(y2, m2, d2, h2, i2) == 0) //Pokud validace datumu vrátila nulu, tak rovnou program ukončím
    {
        return 0;
    }
    else //Pokud projde testem validace datumu, ověřím, že jde o validní timespan
    {
        if (isTimespanValid(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2) == 0) //Pokud neprojde validací timespanu, opět ukončím program
        {
            return 0;
        }
    }
    while(y1 != y2 && m1 != m2 && d1 != d2 && h1 != h2 && i1 != i2)
    {

    }
    //zde stoprocentně vím, že celý můj časový input je validní :)
    /*Zde se implementuje část, která bude řešit, kolik energie bylo spotřebováno*/
    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    valueOfDigit();
    /*
  long long int consumption;

  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  1, 18, 45, &consumption ) == 1
           && consumption == 67116LL );
  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  2, 11, 20, &consumption ) == 1
           && consumption == 269497LL );
  assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                               2024, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81398919LL );
  assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                               1900, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 146443LL );
  assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                               2021, 10,  1,  0, 25, &consumption ) == 1
           && consumption == 2035LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  1, &consumption ) == 1
           && consumption == 204LL );
  assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                               2021, 11, 10, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                               2100,  2, 29, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                               2400,  2, 29, 12,  0, &consumption ) == 1
           && consumption == 0LL );
           */
     /* //IS DATE VALID CHECK, IF NEEDED
    printf("%d",isDateValid (1901, 2,  29, 13, 15));
    printf("%d",isDateValid (1902, 2,  29, 13, 15));
    printf("%d",isDateValid (1903, 2,  29, 13, 15));
    printf("%d",isDateValid (1905, 2,  29, 13, 15));
   printf ("\n");
    printf("%d",isDateValid (2004, 2,  29, 13, 15));
    printf("%d",isDateValid (1904, 2,  29, 13, 15));
    printf("%d",isDateValid (1908, 2,  29, 13, 15));
    printf("%d",isDateValid (1996, 2,  29, 13, 15));
   printf ("\n");
    printf("%d",isDateValid (1700, 2,  29, 13, 15));
    printf("%d",isDateValid (1800, 2,  29, 13, 15));
    printf("%d",isDateValid (1900, 2,  29, 13, 15));
    printf("%d",isDateValid (2100, 2,  29, 13, 15));
   printf ("\n");
    printf("%d",isDateValid (1600, 2,  29, 13, 15));
    printf("%d",isDateValid (2000, 2,  29, 13, 15));
    printf("%d",isDateValid (2400, 2,  29, 13, 15));
    printf("%d",isDateValid (3600, 2,  29, 13, 15));
   printf ("\n");
    printf("%d",isDateValid (4000, 2,  29, 13, 15));
    printf("%d",isDateValid (8000, 2,  29, 13, 15));
    printf("%d",isDateValid (12000, 2,  29, 13, 15));
    printf("%d",isDateValid (16000, 2,  29, 13, 15));
      */

  return 0;
}
#endif /* __PROGTEST__ */
