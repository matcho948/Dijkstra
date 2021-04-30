#include <iostream>
#include "mem-leak-detect.h"
#include "funkcje.h"
#include <fstream> 
#include <queue>

int main(int argc, char* argv[])
{
	std::ifstream pwejsciowy;
	std::ofstream pwyjsciowy;
	Trasa* tr = nullptr;
	Parametry p = {};
	if (!odczytajParametry(argc, argv, p))
		return 0;
	pwejsciowy.open(p.plikwejsciowy);
	Drzewo* Korzen = new Drzewo{p.siedziba, nullptr, nullptr, nullptr,0,nullptr };
	if (!WczytajDane(pwejsciowy, Korzen))
		return 0;
	AlgorytmDijkstry(Korzen);
	pwyjsciowy.open(p.plikwyjsciowy);
	ZapisywanieTras(Korzen,tr,pwyjsciowy);
	UsunListy(Korzen);
	Usun(Korzen);
	pwejsciowy.close();
	pwyjsciowy.close();
	return 0; 
}