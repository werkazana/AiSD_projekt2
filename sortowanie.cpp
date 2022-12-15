#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <chrono>

using namespace std::chrono;

// tablica pomocnicza - zmienna globalna
std::vector<int> tab_pom;

// zmienne globalne - do pomiaru czasu
high_resolution_clock::time_point start;
high_resolution_clock::time_point stop;
std::chrono::duration<double> czas;

// deklaracje funkcji
void sortowanie_przez_wstawianie(std::vector<int>& tab);
void sortowanie_przez_scalanie(std::vector<int>& tab, int lewy, int prawy);
void sortowanie_przez_scalanie(std::vector<int>& tab);
std::vector<int> generuj_losowy_ciag(int n, int max);
void wypisz_ciag(std::vector<int> tab);
std::vector<int> odczytaj_ciag_z_pliku(std::string nazwa_pliku);
void zapisz_ciag_do_pliku(std::string nazwa_pliku, std::vector<int> tab);
void testy();


/////////////////////////////////////////////////////////////////////
// funkcja sortuje tablice tab
// z uzyciem algorytmu sortowanie przez wstawianie
void sortowanie_przez_wstawianie(std::vector<int>& tab)
{
    int liczba, j;
    // rozpoczynamy od drugiego elementu
    for (int i = 1; i < tab.size(); i++)
    {
        liczba = tab[i]; // zapamietanie wartosci elementu
        j = i - 1; // indeks poprzedniego elementu

        // cofamy sie w lewo i przesuwamy kolejne elementy większe od 'liczba' w prawo
        while (j >= 0 && tab[j] > liczba)
        {
            tab[j + 1] = tab[j]; // kopiowanie wartosci elementu poprzedniego do aktualnego
            --j;
        }
        tab[j + 1] = liczba; // jesli doszlismy do poczatku tablicy
        // albo poprzedzajacy element jest mniejszy niz w zmiennej 'liczba'
        // to wpisujemy do elementu zapamietana wartosc ze zmiennej 'liczba'
    }
}

/////////////////////////////////////////////////////////////////////
// funkcja sortuje fragment tablicy tab od indeksu 'lewy' do 'prawy'
// z uzyciem algorytmu sortowanie przez scalanie
void sortowanie_przez_scalanie(std::vector<int>& tab, int lewy, int prawy)
{
    // wyznaczamy srodek tablicy pomiedzy indeksami lewy i prawy
    int srodek = (lewy + prawy + 1) / 2;

    // jesli jest wiecej niz 1 element do posortowania -> sortuj lewa polowe
    if (srodek - lewy > 1)
        sortowanie_przez_scalanie(tab, lewy, srodek - 1);

    // jesli jest wiecej niz 1 element do posortowania -> sortuj prawa polowe
    if (prawy - srodek > 0)
        sortowanie_przez_scalanie(tab, srodek, prawy);

    // scalanie elementow z 2 polowek do tablicy pomocniczej 
    // wybierajac najpierw element mniejszy i przesuwajac odpowiednio indeksy j, k
    // dla mniejszego lub ostatniego elementu
    int j = lewy, k = srodek;
    for (int i = lewy; i <= prawy; i++)
    {
        if ((j == srodek) || ((k <= prawy) && (tab[j] > tab[k])))
            tab_pom[i] = tab[k++];
        else
            tab_pom[i] = tab[j++];
    }

    // przepisanie elementow z tablicy pomocniczej do wlasciwej
    for (int i = lewy; i <= prawy; i++)
    {
        tab[i] = tab_pom[i];
    }
}

/////////////////////////////////////////////////////////////////////
// funkcja sortuje tablice tab, z uzyciem algorytmu sortowanie przez scalanie
void sortowanie_przez_scalanie(std::vector<int>& tab)
{
    // ustawianie wielkosci tablicy pomocniczej do rozmiaru tablicy sortowanej
    tab_pom.resize(tab.size());
    // sortowanie tablicy od indeksu 0 do ostatniego
    sortowanie_przez_scalanie(tab, 0, tab.size() - 1);
}

/////////////////////////////////////////////////////////////////////
// funkcja generuje tablice n elementow typu int z przedzialu [0, max]
std::vector<int> generuj_losowy_ciag(int n, int max)
{
    std::vector<int> tab;
    for (int i = 0; i < n; i++)
    {
        int liczba = rand() % (max + 1);
        tab.push_back(liczba);
    }

    return tab;
}

/////////////////////////////////////////////////////////////////////
// funkcja wypisuje wszystkie elementy tablicy
void wypisz_ciag(std::vector<int> tab)
{
    for (size_t i = 0; i < tab.size(); i++)
    {
        std::cout << tab[i];
        if (i < tab.size() - 1)
            std::cout << ", ";
    }

    std::cout << std::endl;
}

/////////////////////////////////////////////////////////////////////
// funkcja odczytuje tablice elementow int z pliku
std::vector<int> odczytaj_ciag_z_pliku(std::string nazwa_pliku)
{
    std::vector<int> arr;

    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::in); // otworz plik o nazwie nazwa do odczytu (in)

    int liczba;
    while (plik >> liczba)
    {
        arr.push_back(liczba);
    }

    plik.close();

    return arr;
}

/////////////////////////////////////////////////////////////////////
// funkcja zapisuje tablice elementow int do pliku
void zapisz_ciag_do_pliku(std::string nazwa_pliku, std::vector<int> tab)
{
    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::out); // otworz plik o nazwie nazwa do zapisu (out)
    for (size_t i = 0; i < tab.size(); i++)
    {
        plik << tab[i] << " "; // wrzuc element tablicy do pliku
    }

    plik.close(); // zamknij plik
}

/////////////////////////////////////////////////////////////////////
// funkcja zapisuje tablice elementow float do pliku
void zapisz_ciag_floatow_do_pliku(std::string nazwa_pliku, std::vector<double> tab)
{
    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::out);
    for (size_t i = 0; i < tab.size(); i++)
    {
        plik << tab[i] << std::endl;
    }

    plik.close();
}

/////////////////////////////////////////////////////////////////////
// funkcja przeprowadza pomiary czasow obliczen dla obu algorytmow i roznych wielkosci tablic
void testy()
{
    int arrLength = 100; // dlugosc tablicy
    std::vector<int> tab_w, tab_s; // tablice z wygenerowanymi liczbami dla obu algorytmów
    int liczba_testow = 10;
    std::vector<double> czasWstawianie(liczba_testow); // czasy obliczen alg. wstawianie
    std::vector<double> czasScalanie(liczba_testow); // czasy obliczen alg. scalanie
    std::string plik_we = "plik_we";
    std::string plik_wy = "plik_wy";
    for (int i = 0; i < liczba_testow; i++)
    {
        // zmienne pomocnicze - nazwy plikow wejsciowych i wyjsciowych

        std::string nazwa_we = plik_we + std::to_string(i) + ".txt";
        std::string nazwa_wy = plik_wy + std::to_string(i) + ".txt";

        tab_w = tab_s = generuj_losowy_ciag(arrLength, 30000);
        zapisz_ciag_do_pliku(nazwa_we, tab_w);

        // testy algorytmu wstawianie
        start = high_resolution_clock::now();
        sortowanie_przez_wstawianie(tab_w);
        stop = high_resolution_clock::now();
        czas = stop - start;
        czasWstawianie[i] = czas.count();

        // testy algorytmu scalanie
        start = high_resolution_clock::now();
        sortowanie_przez_scalanie(tab_s);
        stop = high_resolution_clock::now();
        czas = stop - start;
        czasScalanie[i] = czas.count();

        // zapisz_ciag_do_pliku(nazwa_wy, tab_s);

        // zwiekszenie rozmiarow tablicy
        arrLength = arrLength * 2;
    }

    zapisz_ciag_floatow_do_pliku("czasy_Wstawianie.txt", czasWstawianie);
    zapisz_ciag_floatow_do_pliku("czasy_Scalanie.txt", czasScalanie);
}

/////////////////////////////////////////////////////////////////////
// funkcja main()
int main()
{
    srand(time(NULL));

    // std::vector<int> tab = { 5, 4, 3, 2, 1 };
    // sortowanie_przez_wstawianie(tab);
    // sortowanie_przez_scalanie(tab);

    testy();

    return 0;
}
