#include "funkcje.h"
#include <iostream>
#include <queue>
bool odczytajParametry(int argc, char** argv, Parametry& p)
{
	if (argc == 1)
	{
		std::cout << "Program realizuje zadanie obliczenia dlugosci najkrotszej trasy od danego miasta do innych miast" << std::endl;
		std::cout << " Aby program dzialal prawidlowo potrzeba odpowiednich przelacznikow -i nazwaplikuwejsciowego, -o nazwa pliku wyjsciowego,-s nazwa siedziby" << std::endl;
		return 0;
	}
	if (argc != 7 && argc != 8)
	{
		std::cout << "Podano zla liczbe parametrow" << std::endl;
		std::cout << " Aby program dzialal prawidlowo potrzeba odpowiednich przelacznikow -i nazwaplikuwejsciowego, -o nazwa pliku wyjsciowego,-s nazwa siedziby" << std::endl;
		return 0;
	}
	std::string tmp = argv[1];
	if (tmp == "-h")
	{
		std::cout << "Program realizuje zadanie obliczenia dlugosci najkrotszej trasy od danego miasta do innych miast" << std::endl;
		std::cout << " Aby program dzialal prawidlowo potrzeba odpowiednich przelacznikow -i nazwaplikuwejsciowego, -o nazwa pliku wyjsciowego,-s nazwa siedziby" << std::endl;
		return 0;
	}
	for (int i = 0;i < argc;i++)
	{
		std::string tmp = argv[i];
		if (tmp == "-i")
			p.plikwejsciowy = argv[++i];
		else if (tmp == "-o")
			p.plikwyjsciowy = argv[++i];
		else if (tmp == "-s")
			p.siedziba = argv[++i];
		else if (tmp == "-h")
		{
			std::cout << "Program realizuje zadanie obliczenia dlugosci najkrotszej trasy od danego miasta do innych miast" << std::endl;
			std::cout << " Aby program dzialal prawidlowo potrzeba odpowiednich przelacznikow -i nazwaplikuwejsciowego, -o nazwa pliku wyjsciowego,-s nazwa siedziby" << std::endl;
		}
		}
	return 1; 
}
Drzewo* DodajdoDrzewa(Drzewo*& pKorzen,const std::string & miasto)
{
	if (!pKorzen)
	{
		pKorzen = new Drzewo{ miasto,nullptr,nullptr,nullptr,MAXINT,nullptr };
		return pKorzen;
	}
	else if (pKorzen->Miasto == miasto)
		return pKorzen;
	else if (pKorzen->Miasto> miasto)
	{
	return DodajdoDrzewa(pKorzen->pLewy, miasto);
	}
	else
	{
	return DodajdoDrzewa(pKorzen->pPrawy, miasto);
	}
}
void DodawanieDoListy(Drzewo*& w1, Drzewo* w2, T km)
{
	w1->pOdleglosci = new Odleglosc{ km,w2,w1->pOdleglosci };
}

void UsunListy(Drzewo* pKorzen)
{
	if (pKorzen)
	{
		UsunListy(pKorzen->pLewy);
		UsunListy(pKorzen->pPrawy);
		while (pKorzen->pOdleglosci)
		{
			auto p = pKorzen->pOdleglosci->pNastepny;
			delete pKorzen->pOdleglosci;
			pKorzen->pOdleglosci = nullptr;
			pKorzen->pOdleglosci = p;
		}
	}
}
void Usun(Drzewo*& pKorzen)
{
	if (pKorzen)
	{
		Usun(pKorzen->pLewy);
		Usun(pKorzen->pPrawy);
		delete pKorzen;
		pKorzen = nullptr;
	}
}
void AlgorytmDijkstry(Drzewo*& Korzen)
{
	std::priority_queue<Drzewo*, std::vector<Drzewo*>, PorownajDrzewa> pq;
	pq.push(Korzen);
	while (!pq.empty())
	{
		auto  u = pq.top();
		pq.pop();
		auto cur_nei = u->pOdleglosci;
		while (cur_nei)
		{
			if (cur_nei->Polaczenie->current_dist > u->current_dist + cur_nei->kilometry)
			{
				cur_nei->Polaczenie->current_dist = u->current_dist + cur_nei->kilometry;
				cur_nei->Polaczenie->poprzednik_w_najkrotszej_trasie = u;
				pq.push(cur_nei->Polaczenie);
			}
			cur_nei = cur_nei->pNastepny;
		}
	}
}
bool WczytajDane(std::ifstream& is,Drzewo *& Korzen)
{
	M m1, m2;
	T km;
	if (!is.is_open())// sprawdzenie czy plik wejsciowy siê otworzy³
	{
		std::cerr << "Plik sie nie otworzyl" << std::endl;
		delete Korzen;
		Korzen = nullptr;
		return 0;
	}
	is >> m1 >> m2 >> km;
	do	
	{
		if (is.fail())
		{
			std::cout << "Zle dane w pliku" << std::endl;
			UsunListy(Korzen);
			Usun(Korzen);
			return 0;
		}
		auto w1 = DodajdoDrzewa(Korzen, m1);
		auto w2 = DodajdoDrzewa(Korzen, m2);
		DodawanieDoListy(w1, w2, km);
		DodawanieDoListy(w2, w1, km); // dodajemy dwa razy aby trasy by³y dwukierunkowe
	} while (is >> m1 >> m2 >> km);

	return 1;
}
void ZapisywanieTras(Drzewo* Korzen,Trasa* T1,std::ostream & os)
{
	if (Korzen)
	{
		ZapisywanieFaktyczne(Korzen->pLewy,T1,os);
		ZapisywanieFaktyczne(Korzen->pPrawy,T1,os);
	}
}
void ZapisywanieFaktyczne(Drzewo* Korzen,Trasa* T1, std::ostream& os)
{
	if (Korzen)
	{	
		ZapisywanieFaktyczne(Korzen->pLewy,T1,os);
		Zapisz(Korzen,T1,os);
		ZapisywanieFaktyczne(Korzen->pPrawy,T1,os);
	}
}
void Zapisz(Drzewo* Korzen,Trasa* T1, std::ostream& os)
{
	auto K = Korzen;
	while (K)
	{
	T1 = new Trasa{ K->Miasto,T1 };
	K = K->poprzednik_w_najkrotszej_trasie;
	}
	ZapiszdoPliku(os,T1,Korzen);
}
void ZapiszdoPliku(std::ostream &os,Trasa* T1,Drzewo* Korzen)
{
	auto T = T1;
	if (Korzen->current_dist == MAXINT)
	{
		UsunStruktureTrasy(T1);
		return;
	}
	while (T)
	{
		os << T->Miasto<< " " ;
		if (T->Nastepny)
			os << "-> ";
		T=T->Nastepny;
	}
	os << ": " << Korzen->current_dist;
	os << std::endl;
	UsunStruktureTrasy(T1);
}
void UsunStruktureTrasy(Trasa*& T1)
{
	while (T1)
	{
		auto tmp = T1->Nastepny;
		delete T1;
		T1 = nullptr;
		T1 = tmp;
	}
}