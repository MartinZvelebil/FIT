Úkolem je vytvořit program, který bude zpracovávat logy a počítat unikátní návštěvy.

Předpokládáme že zaznamenáváme jednotlivé přístupy na WWW server. Zaznamenáváme uživatele, kteří přistupují - každý je označen jednoznačným identifikátorem. Identifikátor je celé číslo z uzavřeného intervalu 0 až 99999. Na vstupu jsou jednotlivé přístupy zadány v podobě příkazu `+ id`, např.:

```
+ 123
+ 456
+ 123
+ 123
+ 789
```

Zde na pozici 0, 2 a 3 jsou přístupy uživatele 123, na pozici 1 je přístup uživatele 456, na pozici 4 pak přístup uživatele 789.



Ze zaznamenaných přístupů je potřeba vypočítat informaci o počtu unikátních uživatelů. Pro výpočet bude zadáno rozmezí, které má být analyzováno. Zadání má podobu `? from to`, kde `from` a `to` jsou pozice od/do kterých máme unikátní uživatele počítat. Například pro ukázku by dotaz v podobě `? 0 4` dal odpověď `3 / 5` - 3 unikátní uživatelé z celkových 5 záznamů v dotazovaném rozmezí.

Vstupem programu jsou přístupy a dotazy, ty mohou být libovolně promíchané. Program zpracovává vstup, ukládá si informace o přístupech a průběžně odbavuje dotazy. Zpracování vstupu končí buď detekcí chyby (nesprávný vstup) nebo dosažením konce vstupu (EOF). Celkem je na vstupu nejvýše 1000000 přístupů, počet požadavků o výpočet unikátních přístupů není shora omezen.

Výstupem programu je reakce na každý vstup:

- pro každý přístup program zobrazí, kolikátý přístup uživatele to je,
- pro dotaz na počet unikátních přístupů program zobrazí informaci o počtu unikátních uživatelů v daném rozsahu / celkovém počtu přístupů ve vymezeném rozsahu.



Pokud je vstup neplatný, program to musí detekovat a zobrazit chybové hlášení. Chybové hlášení zobrazujte na standardní výstup (ne na chybový výstup). Za chybu při přidání přístupu považujte:

- chybí `id` uživatele,
- `id` není celé číslo,
- `id` není v intervalu < 0 ; 99999 >, nebo
- přístupů je více než 1000000.

Za chybu v dotazu považujte:

- chybí informace `from` a `to`,
- `from` / `to` nejsou celá čísla,
- `from` je menší než 0,
- `to` je větší nebo roven počtu dosud zaregistrovaných přístupů, nebo
- `from` je větší než `to`.

Chybou je rovněž situace, kdy na vstupu není ani přidání přístupu ani dotaz.

**Ukázka práce programu:**

------

```
Pozadavky:
+ 10
> prvni navsteva
+ 20
> prvni navsteva
+ 99999
> prvni navsteva
+ 5
> prvni navsteva
+ 20
> navsteva #2
+ 25
> prvni navsteva
+ 10
> navsteva #2
? 0 6
> 5 / 7
? 0 5
> 5 / 6
? 1 6
> 5 / 6
? 1 5
> 4 / 5
? 2 6
> 5 / 5
? 2 5
> 4 / 4
? 2 4
> 3 / 3
? 2 3
> 2 / 2
? 2 2
> 1 / 1
+ 5
> navsteva #2
+ 10
> navsteva #3
+ 5
> navsteva #3
+ 20
> navsteva #3
? 0 10
> 5 / 11
? 7 10
> 3 / 4
? 1 11
Nespravny vstup.
```

------

```
Pozadavky:
+ 10
> prvni navsteva
+ 20
> prvni navsteva
+ 30
> prvni navsteva
? 2 1
Nespravny vstup.
```

------

```
Pozadavky:
+ 12
> prvni navsteva
* 33
Nespravny vstup.
```

------

**Poznámky:**

- Ukázkové běhy zachycují očekávané výpisy Vašeho programu (tučné písmo) a vstupy zadané uživatelem (základní písmo). Zvýraznění tučným písmem je použité pouze zde na stránce zadání, aby byl výpis lépe čitelný. Váš program má za úkol pouze zobrazit text bez zvýrazňování (bez HTML markupu).
- Znak odřádkování (\n) je i za poslední řádkou výstupu (i za případným chybovým hlášením).
- Využijte toho, že počet různých identifikátorů návštěvníků je omezený (0 až 99999) a počet přístupů je také shora omezený (1000000). Pokud potřebujete ukládat např. všechny vstupní přístupy do paměti, postačí staticky alokované pole.
- Odevzdaný program je testován více testy se zvyšující se náročností. Nejprve je kontrolovaná správnost řešení, dále se kontroluje správnost práce s polem (test paměťovým debuggerem) a nakonec efektivita řešení.
- Základní řešení nemusí být efektivní, postačuje naivní (kvadratický) algoritmus vyhledávání unikátních hodnot. Takové řešení projde povinnými testy a některými nepovinnými testy. Bude hodnoceno maximálně 100 % bodů.
- První bonusový test kontroluje efektivitu odevzdaného programu. Jsou zadávané dlouhé vstupy a dotazy nad dlouhými intervaly. Naivní algoritmus není dostatečně efektivní a nezvládne výpočet v nastaveném časovém limitu.
- Druhý bonusový test dále kontroluje efektivitu použitého algoritmu. Jsou zadávané dlouhé vstupy, dotazy zpracovávají velké intervaly přístupů a dotazů je zadáno mnoho. Pro zvládnutí časového limitu je potřeba zaznamenané přístupy předzpracovat, aby program během analýzy nemusel opakovaně počítat to, co stačí počítat pouze jednou.
- Program, který projde všemi povinnými a nepovinnými testy na 100 %, může být použit pro code review. (Pro code review není potřeba projít bonusovými testy).