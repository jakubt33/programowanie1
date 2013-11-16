#include <stdio.h>

#include "naglowki.h"

int main(void)
{
    tablica s;
    tablica_init(&s);
    srand_init();

    int wyjscie=0;
    while(wyjscie==0)
    {
        printf("\n\nWitam\n'1' - utworz nowy sygnal\n"
               "'2' - dodaj szum\n"
               "'3' - odszum\n"
               "'4' - wczytaj sygnal z pliku\n"
               "'5' - wyswietl wartosci sygnalu\n"
               "'6' - generowanie wykresu w Google CHARTS\n"
               "'7' - zapisz do pliku\n"
               "'9' - zakoncz\n\n");
        int komenda=0, temp;
        if(scanf("%d", &temp)!=1)
        {
            error();
        }
        else
        {
            komenda=temp;
            switch(komenda)
            {
            case 1:
            {
                generuj(&s);
                break;
            }
            case 2:
            {
                zaszum(&s);
                break;
            }
            case 3:
            {
                odszum(&s);
                break;
            }
            case 4:
            {
                odczytzpliku(&s);
                break;
            }
            case 5:
            {
                pytaniewyswietlanie(&s);
                break;
            }
            case 6:
            {
                pytaniegoogle(&s);
                break;
            }
            case 7:
            {
                zapisdopliku(&s);
                break;
            }
            case 9:
            {
                wyjscie=1;
                break;
            }
            }
        }
    }
    if(s.amplituda!=0) //zabezpieczenie przed wywołaniem funkcji free bez wczesniejszego malloc
    {
        freetab(&s);
    }
    return 0;
}
