Plánování polární výpravy je velmi zodpovědná práce. Úkolem je realizovat funkci (ne celý program, pouze funkci), která to usnadní.

Předpokládejme, že naším cílem je naplánovat palivo na cestu nehostinnou krajinou. Na nákladní automobil jsme schopni naložit palivo o objemu `maxLoad`. Z tohoto objemu spotřebovává automobil na svůj provoz 1 litr na 1 kilometr. Na počátku výpravy máme palivo o objemu `fuelTotal`. Úkolem funkce je vypočítat vzdálenost, kterou jsme za těchto podmínek schopni dojet (pochopitelně chceme dojet nejen tam, ale i zpět). Při plánování cesty si lze část paliva uložit na mezilehlém místě (palivo je v sudech) a později takto uložené palivo použít.

Realizovaná funkce má rozhraní:

```
double twoWayDistance ( int fuelTotal, int maxLoad );
```

- `fuelTotal`

  je vstupní parametr, který představuje celkovou zásobu paliva na počátečním stanovišti,

- `maxLoad`

  je vstupní parametr, který představuje nosnost použitého nákladního automobilu (max. objem převáženého paliva),

- návratová hodnota funkce

  bude nastavena na max. vzdálenost, kterou jsme za takových podmínek schopni ujet tam a vrátit se zpět.

Odevzdávejte zdrojový soubor, který obsahuje implementaci požadované funkce `twoWayDistance`. Do zdrojového souboru přidejte i další Vaše podpůrné funkce, které jsou z `twoWayDistance` volané. Funkce bude volaná z testovacího prostředí, je proto důležité přesně dodržet zadané rozhraní funkce. Za základ pro implementaci použijte kód z ukázky níže. V kódu chybí vyplnit funkci `twoWayDistance` (a případné další podpůrné funkce). Ukázka obsahuje testovací funkci `main`, uvedené hodnoty jsou použité při základním testu. Všimněte si, že vkládání hlavičkových souborů a funkce `main` jsou zabalené v bloku podmíněného překladu (`#ifdef/#endif`). Prosím, ponechte bloky podmíněného překladu i v odevzdávaném zdrojovém souboru. Podmíněný překlad Vám zjednoduší práci. Při kompilaci na Vašem počítači můžete program normálně spouštět a testovat. Při kompilaci na Progtestu funkce `main` a vkládání hlavičkových souborů "zmizí", tedy nebude kolidovat s hlavičkovými soubory a funkcí `main` testovacího prostředí.

Váš program bude spouštěn v omezeném testovacím prostředí. Je omezen dobou běhu (limit je vidět v logu referenčního řešení) a dále je omezena i velikost dostupné paměti. Žádný z uvedených limitů by ale pro tuto algoritmicky jednoduchou úlohu neměl být problém.

Ukázka použití funkce a sada testovacích dat je obsažena v přiloženém archivu.

------

**Nápověda:**

- Zkopírujte si přiložený zdrojový kód a použijte jej jako základ Vašeho řešení.
- Do funkce `main` si můžete doplnit i další Vaše testy, případně ji můžete libovolně změnit. Důležité je zachovat podmíněný překlad.
- V ukázce je použito makro `assert`. Pokud je parametrem nenulová hodnota, makro nedělá nic. Pokud je parametrem nepravda (nula), makro ukončí program a vypíše řádku, kde k selhání došlo. Pokud je tedy Vaše implementace projde ukázkovými testy, program doběhne a nic nezobrazí.
- Nezapomeňte, že zásoba musí stačit na cestu tam i zpět. Neudělejte stejnou chybu jako Karel Němec.