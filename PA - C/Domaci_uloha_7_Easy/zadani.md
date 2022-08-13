Malé děti si rády hrají s kostkami. Z kostek lze postavit věže, které lze zbourat, což je další zábavná aktivita. Navíc si při hře s kostkami děti rozvíjejí schopnost porovnávání velikosti, to je důležité z hlediska psychosomatického vývoje. Některé děti ale dokáží při hře s kostkami klást dospělým nepříjemné otázky. Úkolem je relizovat program, který na některé takové otázky dokáže odpovědět.

Předpokládáme, že máme k dispozici `N` kostek různé velikosti. Dále známe velikosti jednotlivých kostek. Z kostek chceme postavit věže. Věž má výšku alepoň jedné kostky. Při stavbě lze kostky stavět nad sebe, ale nelze stavět větší kostku na menší. Úkolem je vypočítat, kolik tvarově různých věží lze postavit a vypsat jejich podoby.

Vstupem programu je počet kostek `N` následovaný velikostmi jednotlivých kostek.

Výstupem programu je výpis jednotlivých věží, tedy pořadí velikostí kostek, vždy od základny (vlevo) po vrchol (vpravo). Jedna věž je vypsána na jedné řádce. Dále program zobrazí počet nalezených věží.

Program musí ošetřovat vstupní data. Pokud jsou vstupní data nesmyslná, program to musí zjistit, vypsat chybové hlášení a ukončit se. Za chybu je považováno:

- nečíslelný, záporný nebo nulový počet kostek `N`,
- velikost kostky není celé číslo, je záporná nebo nulová.

Program je testován v omezeném prostředí. Je omezen dobou běhu a velikostí dostupné paměti. Úloha není náročná na paměť, pro velké vstupy je ale náročná na dobu běhu. Testované jsou relativně malé objemy dat, které lze zvládnout programem, který rozumně implementuje naivní algoritmus.

**Ukázka práce programu:**

------

```
Pocet kostek:
5
Velikosti:
1 2 3 4 5
5, 4, 3, 2, 1
5, 4, 3, 2
5, 4, 3, 1
5, 4, 3
5, 4, 2, 1
5, 4, 2
5, 4, 1
5, 4
5, 3, 2, 1
5, 3, 2
5, 3, 1
5, 3
5, 2, 1
5, 2
5, 1
5
4, 3, 2, 1
4, 3, 2
4, 3, 1
4, 3
4, 2, 1
4, 2
4, 1
4
3, 2, 1
3, 2
3, 1
3
2, 1
2
1
Celkem: 31
```

------

```
Pocet kostek:
7
Velikosti:
3 8 23
5 2 1 8
23, 8, 5, 3, 2, 1
23, 8, 5, 3, 2
23, 8, 5, 3, 1
23, 8, 5, 3
23, 8, 5, 2, 1
23, 8, 5, 2
23, 8, 5, 1
23, 8, 5
23, 8, 3, 2, 1
23, 8, 3, 2
23, 8, 3, 1
23, 8, 3
23, 8, 2, 1
23, 8, 2
23, 8, 1
23, 8
23, 8, 8, 5, 3, 2, 1
23, 8, 8, 5, 3, 2
23, 8, 8, 5, 3, 1
23, 8, 8, 5, 3
23, 8, 8, 5, 2, 1
23, 8, 8, 5, 2
23, 8, 8, 5, 1
23, 8, 8, 5
23, 8, 8, 3, 2, 1
23, 8, 8, 3, 2
23, 8, 8, 3, 1
23, 8, 8, 3
23, 8, 8, 2, 1
23, 8, 8, 2
23, 8, 8, 1
23, 8, 8
23, 5, 3, 2, 1
23, 5, 3, 2
23, 5, 3, 1
23, 5, 3
23, 5, 2, 1
23, 5, 2
23, 5, 1
23, 5
23, 3, 2, 1
23, 3, 2
23, 3, 1
23, 3
23, 2, 1
23, 2
23, 1
23
8, 5, 3, 2, 1
8, 5, 3, 2
8, 5, 3, 1
8, 5, 3
8, 5, 2, 1
8, 5, 2
8, 5, 1
8, 5
8, 3, 2, 1
8, 3, 2
8, 3, 1
8, 3
8, 2, 1
8, 2
8, 1
8
8, 8, 5, 3, 2, 1
8, 8, 5, 3, 2
8, 8, 5, 3, 1
8, 8, 5, 3
8, 8, 5, 2, 1
8, 8, 5, 2
8, 8, 5, 1
8, 8, 5
8, 8, 3, 2, 1
8, 8, 3, 2
8, 8, 3, 1
8, 8, 3
8, 8, 2, 1
8, 8, 2
8, 8, 1
8, 8
5, 3, 2, 1
5, 3, 2
5, 3, 1
5, 3
5, 2, 1
5, 2
5, 1
5
3, 2, 1
3, 2
3, 1
3
2, 1
2
1
Celkem: 95
```

------

```
Pocet kostek:
6
Velikosti:
1 2 3 2 3 4
4, 3, 2, 1
4, 3, 2
4, 3, 2, 2, 1
4, 3, 2, 2
4, 3, 1
4, 3
4, 3, 3, 2, 1
4, 3, 3, 2
4, 3, 3, 2, 2, 1
4, 3, 3, 2, 2
4, 3, 3, 1
4, 3, 3
4, 2, 1
4, 2
4, 2, 2, 1
4, 2, 2
4, 1
4
3, 2, 1
3, 2
3, 2, 2, 1
3, 2, 2
3, 1
3
3, 3, 2, 1
3, 3, 2
3, 3, 2, 2, 1
3, 3, 2, 2
3, 3, 1
3, 3
2, 1
2
2, 2, 1
2, 2
1
Celkem: 35
```

------

```
Pocet kostek:
8
Velikosti:
1 2 3 2 3 2 5 2
5, 3, 2, 1
5, 3, 2
5, 3, 2, 2, 1
5, 3, 2, 2
5, 3, 2, 2, 2, 1
5, 3, 2, 2, 2
5, 3, 2, 2, 2, 2, 1
5, 3, 2, 2, 2, 2
5, 3, 1
5, 3
5, 3, 3, 2, 1
5, 3, 3, 2
5, 3, 3, 2, 2, 1
5, 3, 3, 2, 2
5, 3, 3, 2, 2, 2, 1
5, 3, 3, 2, 2, 2
5, 3, 3, 2, 2, 2, 2, 1
5, 3, 3, 2, 2, 2, 2
5, 3, 3, 1
5, 3, 3
5, 2, 1
5, 2
5, 2, 2, 1
5, 2, 2
5, 2, 2, 2, 1
5, 2, 2, 2
5, 2, 2, 2, 2, 1
5, 2, 2, 2, 2
5, 1
5
3, 2, 1
3, 2
3, 2, 2, 1
3, 2, 2
3, 2, 2, 2, 1
3, 2, 2, 2
3, 2, 2, 2, 2, 1
3, 2, 2, 2, 2
3, 1
3
3, 3, 2, 1
3, 3, 2
3, 3, 2, 2, 1
3, 3, 2, 2
3, 3, 2, 2, 2, 1
3, 3, 2, 2, 2
3, 3, 2, 2, 2, 2, 1
3, 3, 2, 2, 2, 2
3, 3, 1
3, 3
2, 1
2
2, 2, 1
2, 2
2, 2, 2, 1
2, 2, 2
2, 2, 2, 2, 1
2, 2, 2, 2
1
Celkem: 59
```

------

```
Pocet kostek:
3
Velikosti:
1 2 0
Nespravny vstup.
```

------

```
Pocet kostek:
abcd
Nespravny vstup.
```

------

**Poznámky:**

- Ukázkové běhy zachycují očekávané výpisy Vašeho programu (tučné písmo) a vstupy zadané uživatelem (základní písmo). Zvýraznění tučným písmem je použité pouze zde na stránce zadání, aby byl výpis lépe čitelný. Váš program má za úkol pouze zobrazit text bez zvýrazňování (bez HTML markupu).

- Znak odřádkování (\n) je i za poslední řádkou výstupu (i za případným chybovým hlášením).

- Pořadí věží ve výpisu není dané. Váš program může zobrazovat nalezené věže v libovolném pořadí, testovací prostředí si výsledky před porovnáním spáruje. Tedy například pro vstupní kostky velikosti

   

  1 2 3 2 3 4

   

  může program vypsat věže jako:

  ```
  4, 3, 2, 1
  4, 3, 2
  4, 3, 2, 2, 1
  4, 3, 2, 2
  4, 3, 1
  4, 3
  4, 3, 3, 2, 1
  4, 3, 3, 2
  4, 3, 3, 2, 2, 1
  4, 3, 3, 2, 2
  4, 3, 3, 1
  4, 3, 3
  4, 2, 1
  4, 2
  4, 2, 2, 1
  4, 2, 2
  4, 1
  4
  3, 2, 1
  3, 2
  3, 2, 2, 1
  3, 2, 2
  3, 1
  3
  3, 3, 2, 1
  3, 3, 2
  3, 3, 2, 2, 1
  3, 3, 2, 2
  3, 3, 1
  3, 3
  2, 1
  2
  2, 2, 1
  2, 2
  1
  ```

  nebo

  ```
  3, 2, 1
  3, 2
  3, 2, 2, 1
  3, 2, 2
  3, 1
  3
  3, 3, 2, 1
  3, 3, 2
  3, 3, 2, 2, 1
  3, 3, 2, 2
  3, 3, 1
  3, 3
  2, 1
  2
  2, 2, 1
  2, 2
  1
  4, 3, 2, 1
  4, 3, 2
  4, 3, 2, 2, 1
  4, 3, 2, 2
  4, 3, 1
  4, 3
  4, 3, 3, 2, 1
  4, 3, 3, 2
  4, 3, 3, 2, 2, 1
  4, 3, 3, 2, 2
  4, 3, 3, 1
  4, 3, 3
  4, 2, 1
  4, 2
  4, 2, 2, 1
  4, 2, 2
  4, 1
  4
  ```

  nebo v libovolném jiném ze zbývajících 10333147966386144929666651337523299999998 pořadí.