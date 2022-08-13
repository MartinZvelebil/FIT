Úkolem je realizovat program, který pro zadanou posloupnost hodnot rozhodne, zda z ní lze vybrat dvojici s požadovaným součtem.

Na vstupu programu je posloupnost celých čísel `xi`. Čísla jsou oddělená čárkami. Za touto posloupností následuje znak otazník a dále sekvence celých čísel `y` - dotazů. Dotazů může být libovolné množství. Úkolem programu je rozhodnout, zda se číslo `y` dá sestavit jako součet dvou čísel `xi+xj, i ≠ j`.

Výstupem programu je rozhodnutí ano/ne pro každý dotaz `y`.

Program musí ošetřovat vstupní data. Pokud je vstup neplatný, program to musí zjistit, vypsat chybové hlášení a ukončit se. Za chybu je považováno:

- nečíselné zadání `xi`,
- čísla `xi` nejsou oddělená čárkami,
- méně než 2 čísla `xi`,
- chybějící znak ?
- nečíselné zadání dotazů `y`.

Program je spouštěn v omezeném prostředí. Je omezen dobou běhu a velikostí dostupné paměti. Pro zvládnutí povinných testů postačuje správná implementace naivního algoritmu. Je potřeba, aby program zbytečně neplýtval pamětí. Pro zvládnutí bonusu je potřeba algoritmus, který pracuje rychle i pro dlouhé posloupnosti `xi`.

Ukázka práce programu:

------

```
Vstupni posloupnost:
1, 5, 19, 6, 8, 24, 135,
  -90  ,   230  ,   15
?
Testovane hodnoty:
6
> Soucet 6 lze dosahnout.
39
> Soucet 39 lze dosahnout.
3000
> Soucet 3000 nelze dosahnout.
-400
> Soucet -400 nelze dosahnout.
```

------

```
Vstupni posloupnost:
15,20,30,15,-40?20 40 -10 30 -80
Testovane hodnoty:
> Soucet 20 nelze dosahnout.
> Soucet 40 nelze dosahnout.
> Soucet -10 lze dosahnout.
> Soucet 30 lze dosahnout.
> Soucet -80 nelze dosahnout.
```

------

```
Vstupni posloupnost:
1, 5, 19 20
Nespravny vstup.
```

------

```
Vstupni posloupnost:
1, 7, 3, 20
?
Testovane hodnoty:
hello
Nespravny vstup.
```

------

Nápověda:

- Ukázkové běhy zachycují očekávané výpisy Vašeho programu (tučné písmo) a vstupy zadané uživatelem (základní písmo). Zvýraznění tučným písmem je použité pouze zde na stránce zadání, aby byl výpis lépe čitelný. Váš program má za úkol pouze zobrazit text bez zvýrazňování (bez HTML markupu).
- Znak odřádkování (\n) je i za poslední řádkou výstupu (i za případným chybovým hlášením).
- Pro zpracování vstupu se hodí funkce `scanf` s formátem `"%c"` a `" %c"`.
- Další užitečnou funkcí může být funkce `ungetc()`.
- Pro vyřešeni používejte konstrukce jazyka C. Nepoužívejte datové struktury z C++ - jsou potlačené.