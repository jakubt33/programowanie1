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
void pytaniegoogle(tablica *);
void zapisdopliku(tablica *);
void odczytzpliku(tablica *);
void mallocuj(tablica *, int* );
void fwritetablica(tablica *, plik, int*, char*);
void freadtablica(tablica *, plik, int *, char *);
void pytaniewyswietlanie(tablica*);

int main(void)
{
    tablica s;
    tablica_init(&s);
    int wyjscie=0;
    srand(time(NULL));

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
        int komenda=0;
        if(scanf("%d", &komenda)!=0)
        {
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
    int rozmiar=s->rozmiar;
    plik np;
    char nazwa[MAXNAZWA];
    printf("podaj nazwe pliku z rozszerzeniem .dat\n");
    scanf("%s", nazwa);

    np=fopen(nazwa, "wb");
    if(np== NULL)
    {
        perror("\nbłąd otwarcia pliku\n");
        exit(-10);
    }
    else
    {
        if(fwrite(&rozmiar, sizeof(int),1,np)==1)
            printf("\n...poprawnie zapisano rozmiar\n");
        fclose(np);
    }
    np=fopen(nazwa, "ab");
    if(np== NULL)
    {
        perror("\nbłąd otwarcia pliku\n");
        exit(-10);
    }
    else
    {
        fwritetablica(s, np, &rozmiar, nazwa);
        fclose(np);
    }
}
void odczytzpliku(tablica *s)
{
    plik np;
    char nazwa[MAXNAZWA];
    int rozmiar;
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
        int i=fread(&rozmiar, sizeof(int),1,np);
        s->rozmiar=rozmiar;
        mallocuj(s, &rozmiar);
        freadtablica(s, np, &rozmiar, nazwa);

    }
}
void freadtablica(tablica *s, plik np, int *rozmiar, char *nazwa)
{
    int counter=0;
    counter+=fread(&s->amplituda, sizeof(double),1,np);
    counter+=fread(&s->przesuniecie, sizeof(double),1,np);
    counter+=fread(&s->czestotliwosc_probkowania, sizeof(double),1,np);
    counter+=fread(&s->czestotliwosc_sygnalu, sizeof(double),1,np);

    printf("...odczytano parametry\n");

    counter+=fread(s->tabczysty, (*rozmiar)*sizeof(double),1, np);
    counter+=fread(s->tabszum, (*rozmiar)*sizeof(double),1, np);
    counter+=fread(s->tabfiltr, (*rozmiar)*sizeof(double),1, np);
    if (counter==7)
        printf("...poprawnie wczytano tablice i parametry do bufora\n");
    else
        printf("...odczytano nieprawidlowa ilosc danych, sprobuj ponownie\n");
    fclose(np);

    printf("\n\nw buforze znajduje sie teraz sygnal o parametrach:\n"
           "amplituda = %.2lf\n"
           "czestotliwosc sygnalu = %.2lf\n"
           "czestotliwosc probkowania = %.2lf\n"
           "przesuniecie =  %.2lf", s->amplituda, s->czestotliwosc_sygnalu, s->czestotliwosc_probkowania, s->przesuniecie);
}
void fwritetablica(tablica *s, plik np, int *rozmiar, char *nazwa)
{
    int counter=0;
    counter+=fwrite(&s->amplituda, sizeof(double),1,np);
    counter+=fwrite(&s->przesuniecie, sizeof(double),1,np);
    counter+=fwrite(&s->czestotliwosc_probkowania, sizeof(double),1,np);
    counter+=fwrite(&s->czestotliwosc_sygnalu, sizeof(double),1,np);

    counter+=fwrite(s->tabczysty, sizeof(double)*(*rozmiar), 1, np);
    counter+=fwrite(s->tabszum, sizeof(double)*(*rozmiar), 1, np);
    counter+=fwrite(s->tabfiltr, sizeof(double)*(*rozmiar), 1, np);
    if (counter==7)
        printf("...poprawnie zapisano tablice i parametry\n", *nazwa);
    else
        printf("odczytano nieprawidlowa ilosc danych, sprobuj ponownie\n");
}
void odszum(tablica *s)
{
    if(s->amplituda!=0)
    {
        int i=0 , k=0 , f=0, z=0;
        printf("z ilu elementow ma byc liczona srednia? (zazwyczaj 5)\n");
        scanf("%d", &z);
        printf("wybierz moc filtra w sklali od 1 do %d.zalecana wartosc: %d\n"
               "UWAGA! wartosci skrajne moga przyjmowac rozbierzne wartosci\n", z, (z+1)/2);
        scanf("%d", &f);
        s->tabfiltr[0]=s->tabszum[0];
        for(i=0; i<f-1; i++) //robienie pierwszych skrajnych wynikkow,f-1 bo tablica zaczyna sie od 0
        {
            for(k=0; k<=i; k++)
                s->tabfiltr[i]+=s->tabszum[k];
            s->tabfiltr[i]=s->tabfiltr[i]/z;//k+1
        }
        for (i=f-1; i<s->rozmiar-z+(f); i++) //robienie srodkowych srednich
        {
            for (k=i-(f-1); k<=i+5-(f); k++)
                s->tabfiltr[i]+=s->tabszum[k];
            s->tabfiltr[i]/=z;
        }

        for(i=s->rozmiar-z+(f); i<s->rozmiar; i++) //robienie ostatnich
        {
            for(k=i; k<s->rozmiar; k++)
                s->tabfiltr[i]+=s->tabszum[k];
            s->tabfiltr[i]=(s->tabfiltr[i])/z;//(s->rozmiar-i+1);
        }
        printf("\nzastosowano filtr\n");
    }
    else
        printf("brak sygnalu do odszumienia");
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
void pytaniewyswietlanie(tablica *s)
{
    if(s->amplituda!=0)
    {
        printf("\nDane ktorego sygnalu wyswietlic?\n"
           "'1' - sygnal czysty\n"
           "'2' - sygnal zaszumiony\n"
           "'3' - sygnal odszumiony\n");
    int jakisygnal=0;
    scanf("%d", &jakisygnal);
    if(jakisygnal==1)
        wyswietlanie(s->tabczysty, &s->rozmiar);
    else if(jakisygnal==2)
        wyswietlanie(s->tabszum, &s->rozmiar);
    else if(jakisygnal==3)
        wyswietlanie(s->tabfiltr, &s->rozmiar);
    else
        printf("niepoprawne polecenie, powrot do menu glownego\n");
    }
    else printf("brak sygnalu do wyswietlenia");
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
        mallocuj(s, &s->rozmiar);

        for(i=0; i<s->rozmiar; i++)
        {
            s->tabczysty[i]=s->tabszum[i]=sinus(s,i); //domyslny szum =0
        }
        //s->flaga=[1,1,0];

    }
}
void mallocuj(tablica *s, int *x)
{
    s->tabczysty=(double*)malloc(sizeof(double)*(*x));
    s->tabszum=(double*)malloc(sizeof(double)*(*x));
    s->tabfiltr=(double*)malloc(sizeof(double)*(*x));
}
void pytaniegoogle (tablica *s)
{
    printf("\nKtory sygnal wygenerowac?[GOOGLE CHARTS]\n"
           "'1' - sygnal czysty\n"
           "'2' - sygnal zaszumiony\n"
           "'3' - sygnal odszumiony\n");
    int jakisygnal=0;
    scanf("%d", &jakisygnal);
    if(jakisygnal==1)
        generujgoogle(s->tabczysty, &s->rozmiar, &s->amplituda);
    else if(jakisygnal==2)
        generujgoogle(s->tabszum, &s->rozmiar, &s->amplituda);
    else if(jakisygnal==3)
        generujgoogle(s->tabfiltr, &s->rozmiar, &s->amplituda);
    else
        printf("niepoprawne polecenie, powrot do menu glownego\n");
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
        fprintf(np, "<html> <head> <script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script> <script type=\"text/javascript\"> google.load(\"visualization\", \"1\", {packages:[\"corechart\"]}); google.setOnLoadCallback(drawChart); function drawChart() { var data = google.visualization.arrayToDataTable([\n['Age', 'Weight'],");
        fclose(np);
        fopen("sygnal.html", "at");
        for(i=0; i<*rozmiar; i++)
        {
            fprintf(np, "\n[%d, %lf],", i, tablica[i]);
        }
        fprintf(np, " ]); var options = "
                "{ title: 'Wykres sinusoidalny',"
                "hAxis: {title: 'x', minValue: 0, maxValue: %d},"
                "vAxis: {title: 'f(x)', minValue: -%lf, maxValue: %lf},"
                "legend: 'none', pointSize: 2 };"
                "var chart = new google.visualization.ScatterChart(document.getElementById('chart_div'));"
                "chart.draw(data, options); } "
                "</script> </head> <body> <div id=\"chart_div\" style=\"width: 900px; height: 500px;\"></div> </body></html>", *rozmiar, *amplituda, *amplituda);
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
        for(i=0; i<s->rozmiar; i++)
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
