Úkolem je vytvořit program, který bude porovnávat dvojice geometrických obrazců. Porovná jejich obsah a obvod.

Na vstupu programu je dvojice geometrických obrazců v rovině. Pro každý obrazec je zadaná jeho velikost. Obrazcem může být:

- trojúhelník - na vstup je identifikace obrazce (písmeno T) následovaná trojicí desetinných čísel (délky stran),
- obdélník - na vstup je identifikace obrazce (písmeno R) následovaná dvojicí desetinných čísel (délky stran) nebo
- čtverec - na vstup je identifikace obrazce (písmeno S) následovaná jedním desetinným číslem (délka strany).

Program určí, zda mají zadané obrazce stejný obsah/obvod a výsledek zobrazí dle formátu podle ukázky (viz níže).

Pokud je vstup neplatný, program to musí detekovat a zobrazit chybové hlášení. Chybové hlášení zobrazujte na standardní výstup (ne na chybový výstup). Za chybu považujte:

- nebyl zadaný žádný povolený obrazec (trojúhelník/obdélník/čtverec),
- nebyl zadaný potřebný počet desetinných čísel - rozměrů obrazce,
- zadaný rozměr byl neplatný, neměl podobu platného čísla, byl záporný nebo nulový,
- zadané velikosti stran netvoří trojúhelník nebo
- obdélník má obě strany stejně dlouhé (měl být zadaný jako čtverec).

**Ukázka práce programu:**

------

```
Obrazec #1
S 4.5
Obrazec #2
R 2 10.125
Obvod: ctverec #1 < obdelnik #2
Obsah: ctverec #1 = obdelnik #2
```

------

```
Obrazec #1
R 4 5
Obrazec #2
T 4.5 6 7.5
Obvod: obdelnik #1 = trojuhelnik #2
Obsah: obdelnik #1 > trojuhelnik #2
```

------

```
Obrazec #1

   T
 1
 1
 1
Obrazec #2
 S
 2
Obvod: trojuhelnik #1 < ctverec #2
Obsah: trojuhelnik #1 < ctverec #2
```

------

```
Obrazec #1
T 7.981 8.899 2.996
Obrazec #2
R 2.959 6.979
Obvod: trojuhelnik #1 = obdelnik #2
Obsah: trojuhelnik #1 < obdelnik #2
```

------

```
Obrazec #1
R 6.474 0.559
Obrazec #2
T 0.507 14.276 14.285
Obvod: obdelnik #1 < trojuhelnik #2
Obsah: obdelnik #1 = trojuhelnik #2
```

------

```
Obrazec #1
T 20 30 50
Nespravny vstup.
```

------

```
Obrazec #1
S -4
Nespravny vstup.
```

------

```
Obrazec #1
R 20 30
Obrazec #2
T 0.444 0.521 0.965
Nespravny vstup.
```

------

```
Obrazec #1
R 12 12
Nespravny vstup.
```

------

```
Obrazec #1
R 1 abcd
Nespravny vstup.
```

------

**Poznámky:**

- Ukázkové běhy zachycují očekávané výpisy Vašeho programu (tučné písmo) a vstupy zadané uživatelem (základní písmo). Zvýraznění tučným písmem je použité pouze zde na stránce zadání, aby byl výpis lépe čitelný. Váš program má za úkol pouze zobrazit text bez zvýrazňování (bez HTML markupu).

- Znak odřádkování (\n) je i za poslední řádkou výstupu (i za případným chybovým hlášením).

- Pro reprezentaci hodnot použijte desetinná čísla typu `double`. Nepoužívejte typ `float`, jeho přesnost nemusí být dostatečná.

- Úlohu lze vyřešit bez použití funkcí. Pokud ale správně použijete funkce, bude program přehlednější a bude se snáze ladit.

- Číselné vstupní hodnoty jsou zadávané tak, aby se vešly do rozsahu datového typu `double`. Referenční řešení si vystačí s číselnými typy `double` a `int`.

- Pro načítání vstupu se hodí funkce `scanf`.

- Při programování si dejte pozor na přesnou podobu výpisů. Výstup Vašeho programu kontroluje stroj, který požaduje přesnou shodu výstupů Vašeho programu s výstupy referenčními. Za chybu je považováno, pokud se výpis liší. I chybějící nebo přebývající mezera/odřádkování je považováno za chybu. Abyste tyto problémy rychle vyloučili, použijte přiložený archiv se sadou vstupních a očekávaných výstupních dat. Podívejte se na videotutoriál (materiály -> cvičebnice -> video tutoriály), jak testovací data použít a jak testování zautomatizovat.

- Pro výpočet obsahu trojúhelníku lze použít Heronův vzorec.

- Váš program bude spouštěn v omezeném testovacím prostředí. Je omezen dobou běhu (limit je vidět v logu referenčního řešení) a dále je omezena i velikost dostupné paměti (ale tato úloha by ani s jedním omezením neměla mít problém). Testovací prostředí dále zakazuje používat některé "nebezpečné funkce" -- funkce pro spouštění programu, pro práci se sítí, ... Pokud jsou tyto funkce použité, program se nespustí. Možná ve svém programu používáte volání:

  ```
    int main ( int argc, char * argv [] )
    {
     
      ...
         
      system ( "pause" ); /* aby se nezavrelo okno programu */
      return 0;
    }
    
  ```

  Toto nebude v testovacím prostředí fungovat - je zakázáno spouštění jiného programu. (I pokud by se program spustil, byl by odmítnut. Nebyl by totiž nikdo, kdo by pauzu "odmáčkl", program by čekal věčně a překročil by tak maximální dobu běhu.) Pokud tedy chcete zachovat pauzu pro testování na Vašem počítači a zároveň chcete mít jistotu, že program poběží správně, použijte následující trik:

  ```
    int main ( int argc, char * argv [] )
    {
     
      ...
    
    #ifndef __PROGTEST__
      system ( "pause" ); /* toto progtest "nevidi" */
    #endif /* __PROGTEST__ */
      return 0;
    } 
  ```