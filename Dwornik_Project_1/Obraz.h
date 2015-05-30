#pragma once
#include "easyBMP.h"
#include <cstdlib>
#include <string>

#define PI  3.14159265359

using namespace std;

class Obraz
{
private:
	BMP mainImg;
	BMP lastTrans;
	RGBApixel blackP;
	string nazwaPliku;
	int licznikOperacji;
	string ostatniaOperacja;

public:
	Obraz();
	~Obraz();
	int wczytajObraz(char file[]);
	int zapiszObraz();
	BMP monochrom();
	BMP rotation(double angle);
	BMP laplasjan();
};

