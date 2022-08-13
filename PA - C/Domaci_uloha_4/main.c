#include <stdio.h>
#define MAX_ID 99999
#define MAX_PRISTUPU 1000000

/* Funkce na spočítání kolik přístupů je unikátních v rozmezí stanoveném uživatelem
 *
 * @param[vstupní] vsechnyPristupy - Pole se všemi aktuálně zaznamenanými přístupy
 * @param[vstupní] indexZacatkuProhledavani - Od jakého přístupu uživatel chce začít analyzovat
 * @param[vstupní] indexKonceProhledavani - Do jakého přístupu uživatel chce analyzovat
 *
 * Funkce vypíše na standartní výstup počet unikátních přístupů z celkového počtu analyzovaných
 */
void F_KolikUnikatnich (int vsechnyPristupy[], int indexZacatkuProhledavani, int indexKonceProhledavani)
{
    int pocetJakychPristupu[MAX_ID+1] = {0};
    int kolikUnikatni = 0;

    for (int k = indexZacatkuProhledavani; k <= indexKonceProhledavani; k++)
    {
        if(pocetJakychPristupu[vsechnyPristupy[k]] == 0)
        {
            pocetJakychPristupu[vsechnyPristupy[k]]++;
            kolikUnikatni++;
        }
    }

    int kolikcelkemAnalyzovat = (indexKonceProhledavani - indexZacatkuProhledavani) + 1;
    printf("> %d / %d\n", kolikUnikatni, kolikcelkemAnalyzovat);
}

/* Funkce, která uživateli zobrazí na standartní výstup kolikátou návštěvu na WEBu absolvoval
 *
 * Očekávané vstupní parametry:
 *    @param[vstupní & výstupní] kolikataNavasteva - Pole zaznamenávající počet návštěv všech unikátních identifikátorů (Index = identifikator), které ve funkci inkrementuji
 *    @param[vstupní] unikatniIdentifikator - Aktuálně zadaný identifikátor k analyzování
 *
 * Výsledkem je pole s aktualizovanou hodnotou počtu přístupů pro daný unikátní identifikátor a výpis na standartní výstup
 */
void F_KolikataNavsteva (int kolikataNavsteva[], int unikatniIdentifikator)
{
    kolikataNavsteva[unikatniIdentifikator]++;
    if(kolikataNavsteva[unikatniIdentifikator] == 1)
    {
        printf("> prvni navsteva\n");
    }
    else
    {
        printf("%s%d%s", "> navsteva #", kolikataNavsteva[unikatniIdentifikator], "\n");
    }
}

int main() {
    char identifikator;
    int indexZacatkuProhledavani, indexKonceProhledavani;
    int unikatniIdentifikator;
    int pocetPridanychPristupu = 0;
    int vsechnyPristupy[MAX_PRISTUPU];
    int kolikataNavsteva[MAX_ID + 1] = {0};
    char jeMezera;

    printf("Pozadavky:\n");
    while(!feof(stdin))
    {
        int pocetKonverzi = scanf(" %c%c", &identifikator, &jeMezera);
        if (identifikator == '?') //Dle úvodního znaku (?/+) rozhoduji, kolik hodnot od uživatele bude validním vstupem pro daný znak
        {
            pocetKonverzi += scanf(" %d %d ", &indexZacatkuProhledavani, &indexKonceProhledavani);
        }
        else if (identifikator == '+')
        {
            pocetKonverzi += scanf(" %d ", &unikatniIdentifikator);
        }

            //Ověření kompletní validity dotazu / vstupu uživatele
        if (identifikator == '?'&&
            pocetKonverzi == 4 &&
            jeMezera == ' ' &&
            indexZacatkuProhledavani <= indexKonceProhledavani && //Začátek musí být dříve nebo stejně než konec
            indexZacatkuProhledavani >= 0 && //Začátek musí být nejdříve na nultém prvku
            indexKonceProhledavani < pocetPridanychPristupu) //Nemůže být konec za hranicí přidaných prvků
        {
            F_KolikUnikatnich(vsechnyPristupy, indexZacatkuProhledavani, indexKonceProhledavani);
        }
        else if (identifikator == '+' &&
                 pocetKonverzi == 3 &&
                 jeMezera == ' ' &&
                 unikatniIdentifikator >= 0 && //Unikátní identifikátor musí být větší než 0
                 unikatniIdentifikator <= MAX_ID &&  //Unikátní identifikátor musí být menší než MAX_ID value
                 pocetPridanychPristupu < MAX_PRISTUPU) //Nesmí být více než milion záznamů ve všech přístupech
        {
            vsechnyPristupy[pocetPridanychPristupu] = unikatniIdentifikator;
            F_KolikataNavsteva(kolikataNavsteva, unikatniIdentifikator);
            pocetPridanychPristupu++;
        }
        else
        {
            printf("Nespravny vstup.\n");
            return 1;
        }
    }
    return 0;
}