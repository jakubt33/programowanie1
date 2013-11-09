#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXROZMIAR 1000

typedef struct
{
    double amplituda;
    double przesuniecie;
    double czestotliwosc_probkowania;
    double czestotliwosc_sygnalu;
} parametry;

typedef struct
{
    double *tabczysty;
    double *tabszum;
    double *tabfiltr;
    int rozmiar;
} tablica;

typedef FILE * plik;

double sinus(parametry *p, double numer);
void wyswietlanie(double *s, int *rozmiar);
void generuj(parametry *p, tablica *s);
void zaszum(tablica *s, parametry *p);
void zapiszsygnal(tablica *s);
void wczytaj(tablica *s);

int main(void)
{
    tablica s;
    parametry p;
    int wyjscie=0;
    srand(time(NULL));
    while(wyjscie==0)
    {
        printf("\nWitam\n'1' - utworz nowy sygnal\n'2' - dodaj szum\n'3' - odszum\n'4' - wczytaj sygnal z pliku\n'5' - pokarz czystysygnal\n'6' - pokarz zaszumiony sygnal\n'7' - pokarz odszumiony sygnal\n'9' - zakoncz\n");
        int komenda=0;
        if(scanf("%d", &komenda)!=0)
        {
            switch(komenda)
            {
            case 1:
            {
                generuj(&p, &s);
                zapiszsygnal(&s);
                break;
            }
            case 2:
            {
                if(p.amplituda>0.1)
                {
                    printf("\nwszedlem w zaszumainie\n");
                    zaszum(&s, &p);
                    zapiszsygnal(&s);
                }
                break;
            }
            case 3:
            {
                //odszum
            }
            case 4:
            {
                wczytaj(&s);
                break;
            }
            case 5:
            {
                wyswietlanie(s.tabczysty, &s.rozmiar);
                break;
            }
            case 6:
            {
                wyswietlanie(s.tabszum, &s.rozmiar);
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
    if(p.amplituda>0.1)
    {
        free(s.tabczysty);
        free(s.tabszum);
        free(s.tabfiltr);
    }
    return 0;
}

double sinus(parametry *p, double numer)
{
    return p->amplituda * sin(p->czestotliwosc_sygnalu*2*M_PI/p->czestotliwosc_probkowania*numer+
                              M_PI*p->przesuniecie/180);
}
void wyswietlanie(double *s, int *rozmiar)
{
    int i;
    for(i=0; i<*rozmiar; i++)
    {
        printf("%.2lf\t", s[i]);
    }
    printf("\n");
}
void generuj(parametry *p, tablica *s)
{
    int i=0;
    int czas=0;
    printf("Podaj amplitude sygnalu [V]: ");
    scanf("%lf",&p->amplituda);
    printf("Podaj czestotliwosc sygnalu [Hz]: ");
    scanf("%lf",&p->czestotliwosc_sygnalu);
    printf("Podaj czestotliwosc probkowania [Hz]: ");
    scanf("%lf",&p->czestotliwosc_probkowania);
    printf("Podaj przesuniecie fazowe [stopnie]: ");
    scanf("%lf",&p->przesuniecie);

    printf("Podaj czas trwania sygnalu [s]: ");
    scanf("%d", &czas);

    s->rozmiar=czas*p->czestotliwosc_probkowania;

    s->tabczysty=(double*)malloc(sizeof(double)*(s->rozmiar));
    s->tabszum=(double*)malloc(sizeof(double)*(s->rozmiar));
    s->tabfiltr=(double*)malloc(sizeof(double)*(s->rozmiar));

    for(i=0; i<s->rozmiar; i++)
    {
        s->tabczysty[i]=sinus(p,i);
    }
}
void zapiszsygnal(tablica *s)
{
    int i;
    plik np;
    np=fopen("sygnal.dat", "wb");
    if(np==NULL)
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else
    {
        fwrite(s, 3*(s->rozmiar)*sizeof(double)+sizeof(int), 1 , np);
        fclose(np);
    }
}
void wczytaj(tablica *s)
{
    plik op;
    op=fopen("sygnal.dat", "rb");
    if(op==NULL)
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else
    {
        s->tabczysty=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        s->tabszum=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        s->tabfiltr=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        if(fread(s, 3*MAXROZMIAR*sizeof(double)+sizeof(int), 1 , op)!=0);
            printf("wczytanocos");
        fclose(op);
    }
}
void zaszum(tablica *s, parametry *p)
{
    int i;
    if(p->amplituda!=0)
    {
        for(i=0; i<s->rozmiar; i++)
        {
            double szum=rand()%1000;
            szum=p->amplituda*(szum/500-1)/20;
            s->tabszum[i]=s->tabczysty[i]+szum;
        }
    }
    else
        printf("nie ma czystego sygnalu do zaszumienia");
}
