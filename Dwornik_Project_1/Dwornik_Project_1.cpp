// Dwornik_Project_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "Obraz.h"
#include <cmath>



using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2){
		cout << "Zla Liczba argumentow! Podaj nazwe obrazu do przetworzenia!\n";
		return 1;
	}
	Obraz image;
	if (image.wczytajObraz(argv[1])){
		getchar();
		return 2;
	}
	

	(image.monochrom()).WriteToFile("trans1.bmp");
	//image.zapiszObraz();

	image.rotation((double)(PI / 2));
	//image.zapiszObraz();

	//koniec programu
	cout << "\nWcisnij dowolny klawisz aby zakonczyc.\n";
	getchar();
	return 0;
}