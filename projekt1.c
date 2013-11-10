#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXROZMIAR 1000
#define MAXNAZWA 20

typedef struct
{
    double *tabczysty;
    double *tabszum;
    double *tabfiltr;
    int rozmiar;

    double amplituda;
    double przesuniecie;
    double czestotliwosc_probkowania;
    double czestotliwosc_sygnalu;
} tablica;

typedef FILE * plik;

double sinus(tablica *, double );
void wyswietlanie(double *, int *);
void generuj(tablica *);
void zaszum(tablica *);
void generujgoogle(double *, int*, double *);
void tablica_init(tablica *);
void odszum(tablica *);
void pytaniegoogle(tablica *, int *);
void zapisdopliku(tablica *);
void odczytzpliku(tablica *);

int main(void)
{
    tablica s;
    tablica_init(&s);
    int wyjscie=0;
    srand(time(NULL));
    tablica *bazasygnalow;

    while(wyjscie==0)
    {
        printf("\n\nWitam\n'1' - utworz nowy sygnal\n'2' - dodaj szum\n'3' - odszum\n'4' - wczytaj sygnal z pliku\n'5' - wyswietl wartosci czystego sygnalu\n'6' - wyswietl wartosci zaszumionego sygnalu\n'7' - wyswietl wartosci odszumionego sygnalu\n'9' - zakoncz\n\n");
        int komenda=0;
        if(scanf("%d", &komenda)!=0)
        {
            switch(komenda)
            {
            case 1:
            {
                generuj(&s);
                pytaniegoogle(&s, &komenda);
                zapisdopliku(&s);
                break;
            }
            case 2:
            {
                zaszum(&s);
                pytaniegoogle(&s, &komenda);
                break;
            }
            case 3:
            {
                odszum(&s);
                pytaniegoogle(&s, &komenda);
                break;
            }
            case 4:
            {
                odczytzpliku(&s);
                break;
            }
            case 5:
            {
                if(s.amplituda!=0)
                {
                    wyswietlanie(s.tabczysty, &s.rozmiar);
                }
                else printf("\nbrak danych");
                break;
            }
            case 6:
            {
                if(s.amplituda!=0)
                {
                    wyswietlanie(s.tabszum, &s.rozmiar);
                }
                else printf("\nbrak danych");
                break;
            }
            case 7:
            {
                if(s.amplituda!=0)
                {
                    wyswietlanie(s.tabfiltr, &s.rozmiar);
                }
                else printf("\nbrak danych");
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
    if(s.amplituda!=0)
    {
        free(s.tabczysty);
        free(s.tabszum);
        free(s.tabfiltr);
    }
    return 0;
}
void zapisdopliku(tablica *s)
{
    plik np;
    char nazwa[MAXNAZWA];
    printf("podaj nazwe pliku z rozszerzeniem .dat\n");
    scanf("%s", nazwa);

    np=fopen(nazwa, "wb");
    if(np== NULL)
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else
    {
        int i=fwrite(s, 3*(s->rozmiar)*sizeof(double)+sizeof(int)+4*sizeof(double),1,np);
        printf("utworzono/nadpisano plik: %s\nliczba zapisanych elementow wynosi %d\n", nazwa, i);
        fclose(np);
    }
}
void odczytzpliku(tablica *s)
{
    plik np;
    char nazwa[MAXNAZWA];
    printf("podaj nazwe pliku z rozszerzeniem '.dat'(max 20znakow)\n");
    scanf("%s", nazwa);

    np=fopen(nazwa, "rb");
    if(np==NULL)
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else
    {
        s->tabczysty=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        s->tabszum=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        s->tabfiltr=(double*)malloc(sizeof(double)*(MAXROZMIAR));
        int i=fread(s, 3*MAXROZMIAR*sizeof(double)+sizeof(int)+4*sizeof(double),1,np);
        printf("odczytano plik: %s\nliczba odczytanych elementow wynosi %d\n", nazwa, i);
        fclose(np);
        printf("rozmiar tablicy %lf", s->czestotliwosc_probkowania);
    }
}
void odszum(tablica *s)
{
    printf("wybierz moc filtra w sklali od 1 do 5\n");
    int i=2 , f=0;
    scanf("%d", &f);
    s->tabfiltr[0]=s->tabszum[0];
    if(f==1)
    {
        for (i=0; i<=s->rozmiar-4; i++)
            s->tabfiltr[i]=(s->tabszum[i]+s->tabszum[i+1]+s->tabszum[i+2]+s->tabszum[i+3]+s->tabszum[i+4])/5;
        s->tabfiltr[s->rozmiar-3]=(s->tabszum[s->rozmiar-3]+s->tabszum[s->rozmiar-2]+s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/4;
        s->tabfiltr[s->rozmiar-2]=(s->tabszum[s->rozmiar-2]+s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/3;
        s->tabfiltr[s->rozmiar-1]=(s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/2;
        s->tabfiltr[s->rozmiar]=s->tabszum[s->rozmiar];
    }
    if(f==2)
    {
        for (i=1; i<=s->rozmiar-3; i++)
            s->tabfiltr[i]=(s->tabszum[i-1]+s->tabszum[i]+s->tabszum[i+1]+s->tabszum[i+2]+s->tabszum[i+3])/5;
        s->tabfiltr[s->rozmiar-2]=(s->tabszum[s->rozmiar-3]+s->tabszum[s->rozmiar-2]+s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/4;
        s->tabfiltr[s->rozmiar-1]=(s->tabszum[s->rozmiar-2]+s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/3;
        s->tabfiltr[s->rozmiar]=s->tabszum[s->rozmiar];
    }
    if(f==3)
    {
        s->tabfiltr[1]=(s->tabszum[0]+s->tabszum[1]+s->tabszum[2])/3;
        for (i=2; i<=s->rozmiar-2; i++)
            s->tabfiltr[i]=(s->tabszum[i-2]+s->tabszum[i-1]+s->tabszum[i]+s->tabszum[i+1]+s->tabszum[i+2])/5;
        s->tabfiltr[s->rozmiar-1]=(s->tabszum[s->rozmiar-2]+s->tabszum[s->rozmiar-1]+s->tabszum[s->rozmiar])/3;
        s->tabfiltr[s->rozmiar]=s->tabszum[s->rozmiar];
    }
    if(f==4)
    {
        s->tabfiltr[1]=(s->tabszum[0]+s->tabszum[1]+s->tabszum[2])/3;
        s->tabfiltr[2]=(s->tabszum[0]+s->tabszum[1]+s->tabszum[2]+s->tabszum[3])/4;
        for (i=3; i<=s->rozmiar-1; i++)
            s->tabfiltr[i]=(s->tabszum[i-3]+s->tabszum[i-2]+s->tabszum[i-1]+s->tabszum[i]+s->tabszum[i+1])/5;
        s->tabfiltr[s->rozmiar]=s->tabszum[s->rozmiar];
    }
    if(f==5)
    {
        s->tabfiltr[1]=(s->tabszum[0]+s->tabszum[1])/2;
        s->tabfiltr[2]=(s->tabszum[0]+s->tabszum[1]+s->tabszum[2])/3;
        s->tabfiltr[3]=(s->tabszum[0]+s->tabszum[1]+s->tabszum[2]+s->tabszum[3])/4;
        for (i=4; i<=s->rozmiar; i++)
            s->tabfiltr[i]=(s->tabszum[i-4]+s->tabszum[i-3]+s->tabszum[i-2]+s->tabszum[i-1]+s->tabszum[i])/5;
    }
    printf("\nzastosowano filtr\n");
}
void tablica_init(tablica *s)
{
    s->rozmiar=0;
    s->amplituda=0;
    s->czestotliwosc_probkowania=0;
    s->czestotliwosc_sygnalu=0;
    s->przesuniecie=0;
}
double sinus(tablica *s, double numer)
{
    return s->amplituda * sin(s->czestotliwosc_sygnalu*2*M_PI/s->czestotliwosc_probkowania*numer+
                              M_PI*s->przesuniecie/180);
}
void wyswietlanie(double *s, int *rozmiar)
{
    int i;
    for(i=0; i<=*rozmiar; i++)
    {
        printf("%.2lf\t", s[i]);
    }
    printf("\n");
}
void generuj(tablica *s)
{
    int i=0;
    int czas=0;

    printf("Podaj amplitude sygnalu [V]: ");
    scanf("%lf",&s->amplituda);
    printf("Podaj czestotliwosc sygnalu [Hz]: ");
    scanf("%lf",&s->czestotliwosc_sygnalu);
    printf("Podaj czestotliwosc probkowania [Hz]: ");
    scanf("%lf",&s->czestotliwosc_probkowania);
    printf("Podaj przesuniecie fazowe [stopnie]: ");
    scanf("%lf",&s->przesuniecie);

    printf("Podaj czas trwania sygnalu [s]: ");
    scanf("%d", &czas);
    if (s->amplituda<0||s->czestotliwosc_sygnalu<0||s->czestotliwosc_probkowania<s->czestotliwosc_sygnalu||s->przesuniecie<0||czas<0)
        printf("zle dane, parametry musza byc dodatnie oraz czestotliwosc probkowania musi byc wieksza nic czestotliwosc sygnalu\n");
    else
    {
        s->rozmiar=czas*s->czestotliwosc_probkowania;

        s->tabczysty=(double*)malloc(sizeof(double)*(s->rozmiar));
        s->tabszum=(double*)malloc(sizeof(double)*(s->rozmiar));
        s->tabfiltr=(double*)malloc(sizeof(double)*(s->rozmiar));

        for(i=0; i<s->rozmiar; i++)
        {
            s->tabczysty[i]=s->tabszum[i]=sinus(s,i);
        }

    }
}
void pytaniegoogle (tablica *s, int *jakisygnal)
{
    printf("\nczy wygenerowac wykres sygnalu? [GOOGLE CHARTS]\n'1' - tak\ndowolna liczba - nie\n");
    int x;
    scanf("%d", &x);
    if(x==1)
        {
            if(*jakisygnal==1)
                generujgoogle(s->tabczysty, &s->rozmiar, &s->amplituda);
            else if(*jakisygnal==2)
                generujgoogle(s->tabszum, &s->rozmiar, &s->amplituda);
            else if(*jakisygnal==3)
                generujgoogle(s->tabfiltr, &s->rozmiar, &s->amplituda);
            else
                printf("wystopil nieoczekiwany blad");
        }
}
void generujgoogle(double *tablica, int *rozmiar, double *amplituda)
{
    int i;
    plik np;
    np=fopen("sygnal.html", "wt");
    if(np==NULL)
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else
    {
        fprintf(np, "<html>  <head>    <script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>    <script type=\"text/javascript\">      google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});      google.setOnLoadCallback(drawChart);      function drawChart() {        var data = google.visualization.arrayToDataTable([\n['Age', 'Weight'],");
        fclose(np);
        fopen("sygnal.html", "at");
        for(i=0; i<=*rozmiar; i++)
        {
            fprintf(np, "\n[%d, %lf],",  i, tablica[i]);
        }
        fprintf(np, "        ]);        var options = {          title: 'Wykres sinusoidalny',          hAxis: {title: 'x', minValue: 0, maxValue: %d},          vAxis: {title: 'f(x)', minValue: -%lf, maxValue: %lf},          legend: 'none',       pointSize: 2        };        var chart = new google.visualization.ScatterChart(document.getElementById('chart_div'));        chart.draw(data, options);      }    </script>  </head>  <body>    <div id=\"chart_div\" style=\"width: 900px; height: 500px;\"></div>  </body></html>", *rozmiar, *amplituda, *amplituda);
        fclose(np);
        printf("wygenerwano sygnal do pliku sygnal.html\n");
    }
}
void zaszum(tablica *s)
{
    int i;
    if(s->amplituda!=0)
    {
        printf("jaki procent amplitudy ma stanowic szum?\n");
        int procent;
        scanf("%d", &procent);
        for(i=0; i<=s->rozmiar; i++)
        {
            double szum=rand()%1000;
            szum=(s->amplituda*(szum/500-1)/100)*procent;
            s->tabszum[i]=s->tabczysty[i]+szum;
        }
        printf("\nzaszumienie zakonczone powodzeniem\n");
    }
    else
        printf("nie ma czystego sygnalu do zaszumienia");
}
