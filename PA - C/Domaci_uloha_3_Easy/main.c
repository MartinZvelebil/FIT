#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <math.h>
#endif /* __PROGTEST__ */
int dbl_eq (double a, double b) //Musím úplně stejnou funkci jako je dole udělat znovu, protože mi ji progtest smaže :))
{
    return fabs(a - b) < 1e-6 * fabs(b);
}

double twoWayDistance ( int fuelTotal, int maxLoad )
{
    double vysledek = 0;
    if (fuelTotal <= maxLoad) //pokud vím rovnou, že paliva je méně nebo rovno, kolik může naložit, tak hned je výsledkem fuel/2
    {
        vysledek = fuelTotal / 2.0;
        return vysledek;
    }
    if (maxLoad <= 0)
    {
        return 0;
    }
    int n = ((fuelTotal / (double)maxLoad)+((fuelTotal % maxLoad) != 0)); //pokud mi vznikne desetinný číslo, tak zaokrouhlím nahoru (ceil equivalent)
    int wholeNumber = fuelTotal / maxLoad;  //tady pokud chci pouze bez zaokrouhlení
    double decimalValue = fuelTotal / (double)maxLoad;
    double decimalPartOnly = decimalValue - wholeNumber;

    for (int i = 1; i <= n; ++i) // celý cyklus chci dělat pro to n, které je zaokrouhlené nahoru (aby byl zahrnutý i "zbytek" paliva, který nevychází na celý průchod) - pro tu poslední interaci se chovám jinak
    {
        if (i == n && dbl_eq(decimalValue, wholeNumber) == 0) //Při poslední interaci
        {
            vysledek += (decimalPartOnly * maxLoad) / ((double) i); //pokud je poslední interace, tak mi zbyde část za desetinou čárkou reálně - když jsem počítal počet cest. Takže tolikrát vynásobím "vršek" mého vzorečku a vydělím tím "nadhodnoceným doublem"
        }
        else
        {
            vysledek += maxLoad / ((double) i); //toto vychází z toho basic vzorečku
        }
    }
    return vysledek / 2;
}

#ifndef __PROGTEST__
int smallDiff ( double a, double b )
{
  return fabs ( a - b ) < 1e-6 * fabs ( b );
}
int main ( int argc, char * argv [] )
{
   // printf("%lf\n",twoWayDistance (132717, 907));
    //printf("%lf\n",twoWayDistance (2000, 1000));
    //printf("%lf\n",twoWayDistance (132717, 1000));
    //printf("%lf\n",twoWayDistance ( 4365, 1000 ));
    /*
    printf("%lf\n",twoWayDistance ( 800, 1000 ));
    printf("%lf\n",twoWayDistance ( 2000, 1000 ));
    printf("%lf\n",twoWayDistance ( 20, 1 ));
    printf("%lf\n",twoWayDistance ( 20000, 1000 ));
    printf("%lf\n",twoWayDistance ( 200000, 1000 ));
    printf("%lf\n",twoWayDistance ( 2000000, 1000 ));
    printf("%lf\n",twoWayDistance ( 20000, 100 ));
    printf("%lf\n",twoWayDistance ( 20000, 10000 ));
    printf("%lf\n",twoWayDistance ( 4365, 1000 ));
    //ověřování do funkce níže
    printf("%d\n",n);
    printf("%d\n",wholeNumber);
    printf("%lf\n",decimalValue);
    printf("%lf\n",decimalPartOnly);
 */
  assert ( smallDiff ( twoWayDistance ( 800, 1000 ), 400.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 2000, 1000 ), 750.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 20, 1 ), 1.798869 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 1000 ), 1798.869829 ) );
  assert ( smallDiff ( twoWayDistance ( 200000, 1000 ), 2939.015474 ) );
  assert ( smallDiff ( twoWayDistance ( 2000000, 1000 ), 4089.184052 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 100 ), 293.901547 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 10000 ), 7500.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 4365, 1000 ), 1078.166667 ) );
  assert ( smallDiff ( twoWayDistance ( 132717, 907 ), 2524.387618 ) );

  return 0;
}
#endif /* __PROGTEST__ */
