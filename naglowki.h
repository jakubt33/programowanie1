#ifndef NAGLOWKI_H_INCLUDED
#define NAGLOWKI_H_INCLUDED

#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXROZMIAR 1000
#define MAXNAZWA 23

typedef struct
{
    double *tabczysty;
    double *tabszum;
    double *tabfiltr;
    char flaga[3]; //0 - sygnal czysty, 1 zaszumiony, 2 odszumiony
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
void fwritetablica(tablica *, plik, int*);
void freadtablica(tablica *, plik, int *);
void pytaniewyswietlanie(tablica*);
void freetab(tablica *);
void error();
void srand_init();
void powitanie();

void powitanie()
{
    printf("PRAGRAM NAPISANY NA ZAJĘCIA\n"
           "PROGRAMOWANIE W C, MGR INŻ. MARIUSZ OSTROWSKI\n"
           "PRZEZ: JAKUB TRZYNA 205687 W-10\n\n");
}

void srand_init()
{
    srand(time(NULL));
}
void error()
{
    printf("złe dane\n");
    while((getchar()) != '\n');
}
void freetab(tablica *s)
{
    free(s->tabczysty);
    free(s->tabszum);
    free(s->tabfiltr);
}
void zapisdopliku(tablica *s)
{
    if(s->flaga[0]==1)
    {
        int rozmiar=s->rozmiar;
        plik np;
        char nazwa[MAXNAZWA];
        printf("podaj nazwę pliku z rozszerzeniem '.dat' (max 20znakow)\n");
        if(scanf("%24s", nazwa))
        {
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
            }
            fclose(np);
            np=fopen(nazwa, "ab");
            if(np== NULL)
            {
                perror("\nbłąd otwarcia pliku\n");
                exit(-10);
            }
            else
            {
                fwritetablica(s, np, &rozmiar);
            }
            fclose(np);
        }
        else printf("błąd w nazwie\n");
    }
    else printf("brak sygnalu do zapisania");
}
void odczytzpliku(tablica *s)
{
    plik np;
    char nazwa[MAXNAZWA];
    printf("podaj nazwę pliku z rozszerzeniem '.dat'\n");
    if(scanf("%24s", nazwa))
    {
        np=fopen(nazwa, "rb");
        if(np==NULL)
        {
            perror("błąd otwarcia pliku");
            exit(-10);
        }
        else
        {
            if(fread(&s->rozmiar, sizeof(int),1,np))
            {
                printf("...odczytano rozmiar\n");
                mallocuj(s, &s->rozmiar);
                freadtablica(s, np, &s->rozmiar);
            }
            else printf("...błąd odczytu rozmiaru\n");
        }
        fclose(np);
    }
    else printf("błąd w nazwie\n");
}
void freadtablica(tablica *s, plik np, int *rozmiar)
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
    counter+=fread(s->flaga, 3*sizeof(char),1, np);
    if (counter==8)
    {
        printf("...poprawnie wczytano tablicę i parametry do bufora\n");
        printf("\n\nw buforze znajduje sie teraz sygnał o parametrach:\n"
               "amplituda = %.2lf\n"
               "częstotliwość sygnału = %.2lf\n"
               "częstotliwość probkowania = %.2lf\n"
               "przesunięcie = %.2lf\n", s->amplituda, s->czestotliwosc_sygnalu, s->czestotliwosc_probkowania, s->przesuniecie);
        printf("dostępne sygnały:\n");
        if(s->flaga[0]==1)
            printf("-czysty\n");
        if(s->flaga[1]==1)
            printf("-zaszumiony\n");
        if(s->flaga[2]==1)
            printf("-odszumiony\n");
    }
    else
        printf("...odczytano nieprawidlową ilość danych\n");
}
void fwritetablica(tablica *s, plik np, int *rozmiar)
{
    int counter=0;
    counter+=fwrite(&s->amplituda, sizeof(double),1,np);
    counter+=fwrite(&s->przesuniecie, sizeof(double),1,np);
    counter+=fwrite(&s->czestotliwosc_probkowania, sizeof(double),1,np);
    counter+=fwrite(&s->czestotliwosc_sygnalu, sizeof(double),1,np);

    counter+=fwrite(s->tabczysty, sizeof(double)*(*rozmiar), 1, np);
    counter+=fwrite(s->tabszum, sizeof(double)*(*rozmiar), 1, np);
    counter+=fwrite(s->tabfiltr, sizeof(double)*(*rozmiar), 1, np);
    counter+=fwrite(s->flaga, sizeof(double)*3, 1, np);

    fflush(stdout);

    if (counter==8)
        printf("...poprawnie zapisano tablicę i parametry\n");
    else
        printf("odczytano nieprawidlowa ilosc danych\n");
}
void odszum(tablica *s)
{
    if(s->flaga[0]==1)
    {
        int i=0 , k=0 , f=0, z=0, temp=0;
        printf("z ilu elementow ma być liczona średnia? (zazwyczaj 5)\n");
        if(scanf("%d", &temp)!=1)
        {
            error();
        }
        else if (temp<s->rozmiar)
        {
            z=temp;

            printf("Wybierz na którym miejscu od 1 do %d wstawiać średnią. Zalecana wartość: %d\n"
                   "UWAGA! wartości skrajne mogą przyjmować rozbierzne wartosci\n", z, (z+1)/2);
            if(scanf("%d", &temp)!=1)
            {
                error();
            }
            else if(temp>0 && temp<=z)
            {
                f=temp;
                s->tabfiltr[0]=s->tabszum[0];
                for(i=0; i<f-1; i++) //robienie pierwszych skrajnych wynikkow,f-1 bo tablica zaczyna sie od 0
                {
                    for(k=0; k<=i; k++)
                        s->tabfiltr[i]+=s->tabszum[k];
                    s->tabfiltr[i]=s->tabfiltr[i]/(k+1);//ewentualnie /(k+1) lub /z
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
                    s->tabfiltr[i]=(s->tabfiltr[i])/(s->rozmiar-i+1);//ewentualnie /(s->rozmiar-i+1); lub /z
                }
                s->flaga[2]=1;
                printf("\nzastosowano filtr\n");
            }
            else printf ("złe dane\n");
        }
        else printf ("średnia liczona ze zbyt dużej liczby wynikow\n");
    }

    else
        printf("brak sygnału do odszumienia\n");
}
void tablica_init(tablica *s)
{
    s->rozmiar=0;
    s->amplituda=0;
    s->czestotliwosc_probkowania=0;
    s->czestotliwosc_sygnalu=0;
    s->przesuniecie=0;
    s->flaga[0,0,0]; //brak sygnalow
}
double sinus(tablica *s, double numer)
{
    return s->amplituda * sin(s->czestotliwosc_sygnalu*2*M_PI/s->czestotliwosc_probkowania*numer+
                              M_PI*s->przesuniecie/180);
}
void pytaniewyswietlanie(tablica *s)
{
    if(s->flaga[0]==1)
    {
        printf("\nDane którego sygnału wyswietlić?\n"
               "'1' - sygnał czysty\n"
               "'2' - sygnał zaszumiony\n"
               "'3' - sygnał odszumiony\n");
        int jakisygnal=0;
        if(scanf("%d", &jakisygnal))
        {
            if(jakisygnal==1)
                wyswietlanie(s->tabczysty, &s->rozmiar);
            else if(jakisygnal==2)
            {
                if(s->flaga[1]==1)
                    wyswietlanie(s->tabszum, &s->rozmiar);
                else printf("brak dostępnego sygnału w buforze\n");
            }
            else if(jakisygnal==3)
            {
                if(s->flaga[2]==1)
                    wyswietlanie(s->tabfiltr, &s->rozmiar);
                else printf("brak dostępnego sygnału w buforze\n");
            }
            else printf("złe dane\n");
        }
        else printf ("złe dane\n");
    }
    else printf("brak sygnału do wyświetlenia\n");
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
    int i=0 , czas=0;
    double temp;
    tablica_init(s);
    printf("..wyczyszczono bufor\n");

    printf("Podaj amplitudę sygnału [V]: ");
    if(scanf("%lf", &temp)!=1)
    {
        error();
    }
    else
    {
        s->amplituda=temp;
        printf("Podaj częstotliwość sygnału [Hz]: ");
        if(scanf("%lf", &temp)!=1)
        {
            temp=0;
            error();
        }
        else
        {
            s->czestotliwosc_sygnalu=temp;
            printf("Podaj częstotliwość próbkowania [Hz]: ");
            if(scanf("%lf",&temp)!=1)
            {
                temp=0;
                error();
            }
            else
            {
                s->czestotliwosc_probkowania=temp;
                printf("Podaj przesunięcie fazowe [stopnie]: ");
                if(scanf("%lf",&temp)!=1)
                {
                    temp=0;
                    error();
                }
                else
                {
                    s->przesuniecie=temp;
                    printf("Podaj czas trwania sygnału [s]: ");
                    if(scanf("%lf", &temp)!=1)
                    {
                        temp=0;
                        error();
                    }
                    else
                    {
                        czas=temp;
                        if (s->amplituda<0||s->czestotliwosc_sygnalu<0||s->czestotliwosc_probkowania<s->czestotliwosc_sygnalu||s->przesuniecie<0||czas<0)
                            printf("złe dane, parametry muszą być dodatnie oraz częstotliwość próbkowania musi byc większa niż częstotliwość sygnału\n");
                        else
                        {
                            s->rozmiar=czas*s->czestotliwosc_probkowania;
                            mallocuj(s, &s->rozmiar);

                            for(i=0; i<s->rozmiar; i++)
                            {
                                s->tabczysty[i]=s->tabszum[i]=sinus(s,i); //domyslny szum =0
                            }
                            s->flaga[0]=1; //falga od czystego sygnalu
                        }
                    }
                }
            }
        }
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
    printf("\nKtóry sygnał wygenerować?[GOOGLE CHARTS]\n"
           "'1' - sygnał czysty\n"
           "'2' - sygnał zaszumiony\n"
           "'3' - sygnał odszumiony\n");
    int jakisygnal=0, temp;
    if(scanf("%d", &temp)!=1)
    {
        error();
    }
    else
    {
        jakisygnal=temp;

        switch (jakisygnal)
        {
        case 1:
        {
            if(s->flaga[0]==1)
                generujgoogle(s->tabczysty, &s->rozmiar, &s->amplituda);
            else printf("brak dostępnego sygnału w buforze");
            break;
        }
        case 2:
        {
            if(s->flaga[1]==1)
                generujgoogle(s->tabszum, &s->rozmiar, &s->amplituda);
            else printf("brak dostępnego sygnału w buforze");
            break;
        }
        case 3:
        {
            if(s->flaga[2]==1)
                generujgoogle(s->tabfiltr, &s->rozmiar, &s->amplituda);
            else printf("brak dostępnego sygnału w buforze");
            break;
        }
        default:
            printf("niepoprawne polecenie\n");
            break;
        }
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
        fprintf(np, "<html> <head>  <script type=\"text/javascript\" "
                " src=\"https://www.google.com/jsapi\">"
                " </script> <script type=\"text/javascript\"> "
                " google.load(\"visualization\", \"1\","
                " {packages:[\"corechart\"]});"
                " google.setOnLoadCallback(drawChart);"
                " function drawChart() { var data = google.visualization.arrayToDataTable([\n['x', 'f(x)'],");
        fflush(stdout);
        fclose(np);

        np=fopen("sygnal.html", "at");
        if(np==NULL)
        {
            perror("błąd otwarcia pliku");
            exit(-10);
        }
        else
        {
            for(i=0; i<*rozmiar; i++)
            {
                fprintf(np, "\n[%d, %lf],", i, tablica[i]);
            }
            fflush(stdout);
            fprintf(np, " ]); var options = "
                    "{ title: 'Wykres sinusoidalny',"
                    "hAxis: {title: 'x', minValue: 0, maxValue: %d},"
                    "vAxis: {title: 'f(x)', minValue: -%lf, maxValue: %lf},"
                    "legend: 'none', pointSize: 2 };"
                    "var chart = new google.visualization.ScatterChart(document.getElementById('chart_div'));"
                    "chart.draw(data, options); } "
                    "</script> </head> <body> <div id=\"chart_div\" style=\"width: 900px; height: 500px;\"></div> </body></html>", *rozmiar, *amplituda, *amplituda);
            fflush(stdout);
            fclose(np);
        }
        printf("wygenerwano sygnał do pliku sygnal.html\n");
    }
}
void zaszum(tablica *s)
{
    int i;
    if(s->flaga[0]==1)
    {
        printf("jaki procent amplitudy ma stanowic szum?\n");
        int procent, temp;
        if(scanf("%d", &temp)!=1)
        {
            error();
        }
        else
        {
            procent=temp;
            for(i=0; i<s->rozmiar; i++)
            {
                double szum=rand()%1000;
                szum=(s->amplituda*(szum/500-1)/100)*procent;
                s->tabszum[i]=s->tabczysty[i]+szum;
            }
            printf("\nzaszumienie zakonczone powodzeniem\n");
            s->flaga[1]=1;
        }
    }
    else
        printf("nie ma czystego sygnalu do zaszumienia");
}

#endif // NAGLOWKI_H_INCLUDED
