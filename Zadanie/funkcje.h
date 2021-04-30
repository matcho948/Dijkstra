#ifndef _FUNKCJE_H_
#define _FUNKCJE_H_
#include <iostream>
#include <fstream>
/** wartosc przechowujaca odleglosci miedzy miastami */
typedef int T;
/** wartosc przechowujaca nazwe miast oraz parametrow*/
typedef std::string M;
struct Odleglosc;
/** najwieksza mozliwa wartosc int */
const T MAXINT = 2147483647;
struct ListaPoprzednikow;
/** wezel drzewa poszukiwan binarnych */
struct Drzewo
{
	M Miasto; ///< nazwa miasta przechowywanego w wezle
	Drzewo* pLewy; ///< wskaznik na lewy wezel drzewa
	Drzewo* pPrawy; ///< wskaznik na prawy wezel drzewa
	Odleglosc* pOdleglosci; ///< wskaznik na liste zawierajaca trasy wychodzace z danego miasta
	double current_dist; ///< obecna odleglosc od siedziby 
	Drzewo* poprzednik_w_najkrotszej_trasie; ///< poprzednie odwiedzone miasto w danej trasie 
};
/** element listy jednokierunkowej */
struct Odleglosc
{
	T kilometry; ///< odleglosc miedzy miastami
	Drzewo* Polaczenie;///< wskaznik odnoszacy sie do miasta z ktorym jest polaczenie
	Odleglosc* pNastepny;///< wskaznik na nastepny element listy
};
/** struktura przechowujaca wyznaczone trasy */
struct Trasa
{
	M Miasto; ///< nazwa miasta w trasie
	Trasa* Nastepny;///< wskaznik na nastepny element listy (miasto w trasie)
};
/** struktura przechowujaca odczytane parametry */
struct Parametry
{
	M plikwejsciowy,  ///< nazwa pliku wejsciowego
		plikwyjsciowy, ///< nazwa pliku wyjsciowego
		siedziba; ///< nazwa siedziby
};
/** struktura porownujaca elementy w kolejce priorytetowej */
struct PorownajDrzewa
{
	bool operator()(Drzewo *& d1,Drzewo *& d2)
	{
			if (d1->current_dist > d2->current_dist)
				return true;
			else
				return false;

	}
};
/** Funkcja odczytuje parametry potrzebne do wykonania programu 
@param argc Liczba parametrow
@param argv Tablica parametrow
@param p Struktura przechowujaca parametry na potrzeby programu
@return zwraca false gdy ilosc parametrow sie nie zgadza true gdy uda sie prawidlowo odczytac parametry
*/
bool odczytajParametry(int argc, char** argv, Parametry& p);
/** Funkcja dodaje miasto do drzewa
@param pKorzen adres korzenia
@param miasto nazwa miasta w drzewie
@return zwraca adres dodanego wezla
*/
Drzewo* DodajdoDrzewa(Drzewo*& pKorzen,const M & miasto);
/** Funkcja dodaje do listy trase laczaca jedno miasto z drugim oraz odleglosc miedzy nimi
@param w1 adres pierwszego miasta
@param w2 adres drugiego miasta
@param km odleglosc miedzy miastami
*/
void DodawanieDoListy(Drzewo*& w1, Drzewo* w2, T km);
/** Funkcja usuwa polaczenia miedzy miastami
@param pKorzen adres korzenia
*/
void UsunListy(Drzewo* pKorzen);
/** Funkcja usuwa miasta z drzewa
@param pKorzen adres korzenia
*/
void Usun(Drzewo*& pKorzen);
/** Funkcja znajduje najkrotsze trasy z danej siedziby do innych miast za pomoca Algorytmu Dijkstry
@param Korzen adres korzenia
*/
void AlgorytmDijkstry(Drzewo*& Korzen);
/** Funkcja wczytuje dane z pliku wejsciowego
@param is strumien z ktorego pobieramy dane
@param Korzen wskaznik na korzen drzewa poszukiwan binarnych do ktorego bedziemy dodawac dane
@return zwraca true gdy uda sie prawidlowo wczytac wszystkie dane, false w przypadku gdy dane w pliku sa nieprawidlowe
*/
bool WczytajDane(std::ifstream& is, Drzewo*& Korzen);
/** Funkcja dzieki ktorej pominiemy siedzibe przy przechodzeniu po wezlach drzewa przy wyznaczaniu tras od danego wezla 
@param Korzen wskaznik na korzen drzewa 
@param T1 struktura pomocnicza w ktorej bedziemy przechowywac wyznaczone trasy 
@param os strumien sluzacy do zapisu tras do pliku
*/
void ZapisywanieTras(Drzewo* Korzen,Trasa*T1,std::ostream & os);
/** Funkcja dzieki ktorej odwiedzimy kazdy wezel drzewa (oprocz siedziby) przy zapisywaniu tras 
@param Korzen wskaznik na korzen drzewa
@param T1 struktura pomocnicza w ktorej bedziemy przechowywac wyznaczone trasy
@param os strumien sluzacy do zapisu tras do pliku
*/
void ZapisywanieFaktyczne(Drzewo* Korzen,Trasa* T1, std::ostream& os);
/** Funkcja zapisujaca przebieg tras do pomocniczej struktury T1
@param Korzen wskaznik na korzen drzewa
@param T1 struktura pomocnicza w ktorej bedziemy przechowywac wyznaczone trasy
@param os strumien sluzacy do zapisu tras do pliku
*/
void Zapisz(Drzewo* Korzen, Trasa* T1, std::ostream& os);
/** Funkcja zapisujaca do pliku wyznaczony przebieg tras i ich dlugosc
@param Korzen wskaznik na korzen drzewa
@param T1 struktura pomocnicza w ktorej bedziemy przechowywac wyznaczone trasy
@param os strumien sluzacy do zapisu tras do pliku
*/
void ZapiszdoPliku(std::ostream& os, Trasa* T1, Drzewo* Korzen);
/** Funkcja usuwajaca strukture pomocnicza sluzaca do przechowywania tras
@param T1 wskaznik na glowe listy jednokierunkowej przechowujacej trasy
*/
void UsunStruktureTrasy(Trasa*& T1);
#endif
