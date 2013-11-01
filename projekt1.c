#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct
{
    double amplituda;
    double przesuniecie;
    double czestotliwosc_probkowania;
    double czestotliwosc_sygnalu;
} parametry;

typedef struct
{
    double *tab;
    int rozmiar;
} tablica;

typedef FILE * plik;

double sinus(parametry *p, int numer);
void wyswietlanie(tablica *s);
void generuj(parametry *p, tablica *s);
void zaszum(tablica *s, parametry *p);
void zapiszsygnal(tablica *s, parametry *p);

int main(void)
{
    tablica s;
    parametry p;
    srand(time(NULL));

    printf("Witam\n'1' - utworz nowy sygnal\n'2' - dodaj szum\n'3' - pokaz wygenerowany sygnal\n'4' - pokaz zaszumiony sygnal\n'5' - pokaz odszumiony sygnal\n");
    int komenda;
    if(scanf("%d", &komenda)!=0)
    {
        switch(komenda)
        {
        case 1:
        {
            generuj(&p, &s);
            break;
        }
        case 2:
        {
            zaszum(&s, &p);
            wyswietlanie(&s);
            break;
        }
        default:
            break;
        }
    }

    return 0;
}

void zapiszsygnal(tablica *s, parametry *p)
{
    plik sygnal; //tu cos jest zrypane
    sygnal=fopen("sygnal_czysty.dat", "wb");
    if(sygnal=NULL)
        printf("error");
    else
    {
        fprintf(sygnal, "%lf", s->tab[s->rozmiar-1]);
        fclose(sygnal);
    }
}

double sinus(parametry *p, int numer)
{
    return p->amplituda * sin(p->czestotliwosc_sygnalu*2*M_PI/p->czestotliwosc_probkowania*numer+
                              M_PI*p->przesuniecie/180);
}
void wyswietlanie(tablica *s)
{
    int i;
    for(i=0; i<s->rozmiar; i++)
    {
        printf("%lf\t", s->tab[i]);
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
    scanf("%d",&czas);

    s->rozmiar=czas*p->czestotliwosc_probkowania;
    s->tab=(double*)malloc(sizeof(double)*s->rozmiar);

    for(i=0; i<s->rozmiar; i++)
    {
        s->tab[i]=sinus(p,i);
    }
    zapiszsygnal(s, p);
    free(s->tab);
}
void zaszum(tablica *s, parametry *p)
{
    int i;
    for(i=0; i<s->rozmiar; i++)
    {
        double szum=rand()%1000;
        szum=p->amplituda*(szum/500-1)/20;
        s->tab[i]=s->tab[i]+szum;
    }
}
