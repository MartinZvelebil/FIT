Úkolem je vytvořit program, který bude rozebírat sekvenci čísel.

Předpokládáme sekvenci nezáporných celých čísel (0, 1, 2, ...). Sekvence začíná číslem 0, jednotlivá čísla jsou zapsaná ve vzestupném pořadí bez vložených mezer a zbytečných nul. Sekvence bude začínat takto:

```
01234567891011121314151617181920...
```

Pro člověka je takový zápis obtížně čitelný, proto jej uvedeme ještě v obarvené podobě:

```
01234567891011121314151617181920...
```

Úkolem programu je přečíst ze vstupu pozici (pořadové číslo od počátku) a určit, jaká číslice kterého čísla je na zadané pozici sekvence. Například:

- na pozici 0 je číslice 0 čísla 0,
- na pozici 15 je pak číslice 2 čísla 12,
- na pozici 2021 je číslice 1 čísla 710, ...



Aby byl program univerzálnější, dokáže zadaný problém řešit pro různé číselné soustavy. Například pro zápis čísel ve dvojkové soustavě by sekvence vypadala takto:

```
01101110010111011110001001...
```

Pro dvojkovou soustavu např. bude na pozici 14 číslice 0 čísla 110.





Program bude mít na svém vstupu zadané jednotlivé problémy k vyřešení. Každý problém bude zadaný jako dvě celá desítková čísla. První z čísel udává pozici v sekvenci, druhé číslo pak základ soustavy, pro kterou problém řešíme. Pro každý takto zadaný problém program vypočte výsledek a zobrazí jej ve formátu podle ukázky (číslo s vyznačením vybrané číslice). Zadávání problémů skončí v okamžiku, kdy program dosáhne konce vstupu (je signalizován aktivní EOF).



Pokud je vstup neplatný, program to musí detekovat a zobrazit chybové hlášení. Chybové hlášení zobrazujte na standardní výstup (ne na chybový výstup). Za chybu považujte:

- pozice v sekvenci je neplatná (neplatný zápis čísla, záporná hodnota),
- základ číselné soustavy je neplatný (nečíselné zadání, mimo uzavřený interval < 2 ; 36 >).

**Ukázka práce programu:**

------

```
Pozice a soustava:
0 10
0
^
3 10
3
^
9 10
9
^
10 10
10
^
11 10
10
 ^
12 10
11
^
13 10
11
 ^
189 10
99
 ^
190 10
100
^
191 10
100
 ^
192 10
100
  ^
193 10
101
^
```

------

```
Pozice a soustava:
42 10
26
^
666 10
258
  ^
1889 10
666
 ^
12345 10
3363
   ^
1000000 10
185185
^
1234321 10
224238
   ^
```

------

```
Pozice a soustava:
abc 10
Nespravny vstup.
```

------

```
Pozice a soustava:
12 37
Nespravny vstup.
```

------

**Poznámky:**

- Ukázkové běhy zachycují očekávané výpisy Vašeho programu (tučné písmo) a vstupy zadané uživatelem (základní písmo). Zvýraznění tučným písmem je použité pouze zde na stránce zadání, aby byl výpis lépe čitelný. Váš program má za úkol pouze zobrazit text bez zvýrazňování (bez HTML markupu).
- Znak odřádkování (\n) je i za poslední řádkou výstupu (i za případným chybovým hlášením).
- Odevzdaný program je testován více testy se zvyšující se náročností. Základní řešení nemusí být efektivní a musí správně pracovat pouze s desítkovou soustavou (nemusí umět ostatní číselné soustavy, může pro ně hlásit chybu). Takové řešení projde povinnými testy. a některými nepovinnými testy. Bude hodnoceno 75% bodů.
- Vylepšené řešení musí správně pracovat s různými základy číselné soustavy. Pro zvládnutí tohoto testu nemusí program implementovat žádný efektivní algoritmus (postačuje naivní algoritmus). Při úspěšném zvládnutí tohoto testu (a všech předchozích testů) bude program hodnocen nominálními 100 % bodů.
- Poslední (bonusový) test kontroluje efektivitu použitého algoritmu. Na vstupu jsou zadávaná velká čísla - pozice v sekvenci. Program ve vymezeném čase nemá šanci všechna potřebná mezilehlá čísla vygenerovat. Je potřeba vymyslet vhodný algoritmus, který dokáže mezilehlá čísla efektivně přeskakovat. Zvládnutím tohoto testu lze získat i více než nominálních 100 % bodů.
- Základní řešení vystačí s proměnnými datového typu `int`. V bonusovém testu efektivity se zadávají velké hodnoty pozice, které se do typu `int` nevejdou. Pokud chcete řešit i bonusový test, použijte pro uložení pozice typ `long` nebo `long long`.
- Pro zobrazení čísel v soustavách se základem vyšším než 10 použijte malá písmena (např. `ff` pro zobrazení čísla 255 v šestnáctkové soustavě).
- Neukládejte si sekvenci čísel `01234567891011...` do stringu ani do pole. Je to zbytečně pracné, náročné na paměť, není to vhodné, ani rychlé a hlavně to není potřeba. Čísla v sekvenci si vygenerujte (např. for-cyklem), ale opravdu není potřeba je nikam ukládat. Dokonce ani jednotlivá čísla není potřeba převádět na řetězce.