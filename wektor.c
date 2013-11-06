#include <stdio.h>


typedef struct
{
    double *tablica;
    int rozmiar;
    int pozycja;
} wektor;

void push(wektor*, int); //int zapisuje liczbe int na gorze stosu
void pop(wektor*);
int size(wektor*); //zwraca wartosc wielkosci tablicy
int at(wektor*, int); //zwraca wartosc int-owego+1 argumentu
void init(wektor*);

int main()
{
    wektor w;
    init(&w);

    int x;


    free(w.tablica);
}

void init(wektor* w)
{
    w->rozmiar=10;
    w->tablica=(double*)malloc(w->rozmiar*sizeof(double));
    w->pozycja=0;

    /*
    int licznik=0;
    for (licznik=0;licznik<10;licznik++)
    {
       push(w, licznik+1);
    }

    for(licznik=0; licznik<10; licznik++)
        printf("%.2lf\n", w->tablica[licznik]);
    */
}
void push(wektor* w, int x)
{
    if(w->pozycja==w->rozmiar)
    {
        w->tablica=realloc(w->tablica, w->rozmiar*2*sizeof(double));
        w->rozmiar=2*w->rozmiar;
    }
    w->tablica[w->pozycja]=x;
    w->pozycja++;
}
void pop(wektor *w)
{
    w->pozycja--;
    return w->tablica[w->pozycja-1];
}
int size(wektor *w)
{
    return w->rozmiar;
}
int at(wektor *w, int x) //jest ok
{
    w->pozycja=x;
    x=w->tablica[w->pozycja];
    return x; // na zwracac warotsc na tym miejcu
}
