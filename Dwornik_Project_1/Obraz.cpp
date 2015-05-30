#include "stdafx.h"
#include "Obraz.h"


Obraz::Obraz()
{
	blackP.Alpha = 0;
	blackP.Blue = 0;
	blackP.Green = 0;
	blackP.Red = 0;
	licznikOperacji = 0;
}


Obraz::~Obraz()
{
}

int Obraz::wczytajObraz(char file[])
{
	if (!mainImg.ReadFromFile(file)){
		cout << "Blad otwarcia pliku!!!\n";
		cout << "\nWcisnij dowolny klawisz aby zakonczyc.\n";
		return 1;
	}
	return 0;
}
int Obraz::zapiszObraz(){
	cout << "Zapisywanie obrazu!\n===>Operacja: "<< ostatniaOperacja <<"\n===>Nazwa pliku: transform_" << licznikOperacji <<".bmp\n";
	//string name = "transform_"+;
	//string file = name + ".bmp";
	//cout << file;
	lastTrans.WriteToFile("zapis.bmp");
	return 1;
}

BMP Obraz::monochrom()
{
	BMP monochrome = mainImg;

	RGBApixel tmpPixel;
	RGBApixel monoPixel;
	cout << "Przetwarzanie obrazu z koloru na skale szarosci\n";
	for (int kz = 0; kz < monochrome.TellHeight(); kz++){
		for (int kx = 0; kx < monochrome.TellWidth(); kx++){
			tmpPixel = monochrome.GetPixel(kx, kz);
			monoPixel = tmpPixel;

			monoPixel.Blue = (0.2125*tmpPixel.Red) + (0.7154*tmpPixel.Green) + (0.0721*tmpPixel.Blue);
			monoPixel.Green = (0.2125*tmpPixel.Red) + (0.7154*tmpPixel.Green) + (0.0721*tmpPixel.Blue);
			monoPixel.Red = (0.2125*tmpPixel.Red) + (0.7154*tmpPixel.Green) + (0.0721*tmpPixel.Blue);

			monochrome.SetPixel(kx, kz, monoPixel);
		}
	}

	lastTrans = monochrome;
	ostatniaOperacja = "Monochrom";
	return monochrome;
}

BMP Obraz::rotation(double angle){
	BMP newImage = mainImg;
	int ny, nx, iFloorX, iFloorY, iCeilingX, iCeilingY;
	float fDis, fAng, fTx, fTy, deltaX, deltaY;
	RGBApixel newPixel, topL, topR, botL, botR, top, bot;


	for (int ky = 0; ky < mainImg.TellHeight(); ky++){
		for (int kx = 0; kx < mainImg.TellWidth(); kx++){
			newImage.SetPixel(kx, ky, blackP);

			// konwersja na uk³ad kartezjañski
			nx = kx - mainImg.TellWidth() / 2;
			ny = mainImg.TellHeight() / 2 - ky;

			// konwersja na uk³adk polarny
			fDis = sqrt(ny*ny + nx*nx);
			fAng = 0.0;


			if (nx == 0){
				if (ny == 0){
					newImage.SetPixel(kx, ky, mainImg.GetPixel(kx, ky));
					continue;
				}
				else{
					//fAng = 0.5*PI;
					fAng = atan2((double)ny, (double)nx);
				}
			}
			else{
				fAng = atan2((double)ny, (double)nx);
			}


			fAng -= angle;

			fTx = fDis*cos(fAng);
			fTy = fDis*sin(fAng);

			fTx = fTx + (double)(mainImg.TellWidth() / 2);
			fTy = fTy + (double)(mainImg.TellHeight() / 2);

			// kordynanty punktów po których bêdziemy interpolowaæ kolor
			iFloorX = (int)(floor(fTx));
			iFloorY = (int)(floor(fTy));
			iCeilingX = (int)(ceil(fTx));
			iCeilingY = (int)(ceil(fTy));


			//sprawdzenie granic
			if (iFloorX < 0 || iCeilingX < 0 || iFloorX >= mainImg.TellWidth() || iCeilingX >= mainImg.TellWidth() || iFloorY < 0 || iCeilingY < 0 || iFloorY >= mainImg.TellHeight() || iCeilingY >= mainImg.TellHeight()){
				continue;
			}

			// ró¿nica
			deltaX = fTx - (double)iFloorX;
			deltaY = fTy - (double)iFloorY;

			topL = mainImg.GetPixel(iFloorX, iFloorY);
			topR = mainImg.GetPixel(iCeilingX, iFloorY);
			botL = mainImg.GetPixel(iFloorX, iCeilingY);
			botR = mainImg.GetPixel(iCeilingX, iCeilingY);

			// interpolacja liniowa pomiêdzy górnymi pixelami
			top.Blue = (1 - deltaX)*topL.Blue + deltaX*topR.Blue;
			top.Alpha = (1 - deltaX)*topL.Alpha + deltaX*topR.Alpha;
			top.Green = (1 - deltaX)*topL.Green + deltaX*topR.Green;
			top.Red = (1 - deltaX)*topL.Red + deltaX*topR.Red;

			// interpolacja liniowa miêdzy dolnymi pixelami
			bot.Blue = (1 - deltaX)*botL.Blue + deltaX*botR.Blue;
			bot.Alpha = (1 - deltaX)*botL.Alpha + deltaX*botR.Alpha;
			bot.Green = (1 - deltaX)*botL.Green + deltaX*botR.Green;
			bot.Red = (1 - deltaX)*botL.Red + deltaX*botR.Red;


			// interpolacja liniiowa miêdzy górnym a dolnym wynikiem
			newPixel.Alpha = (1 - deltaY)*top.Alpha + deltaY*bot.Alpha;
			newPixel.Blue = (1 - deltaY)*top.Blue + deltaY*bot.Blue;
			newPixel.Green = (1 - deltaY)*top.Green + deltaY*bot.Green;
			newPixel.Red = (1 - deltaY)*top.Red + deltaY*bot.Red;

			//ustawienie koloru nowego pixela
			newImage.SetPixel(kx, ky, newPixel);

		}
	}

	lastTrans = newImage;
	ostatniaOperacja = "Rotacja bilinowa";
	return newImage;
}

BMP Obraz::laplasjan(){
	BMP newImage = mainImg;
	int w[3][3] = { { 1, 1, 1 }, { 1, -9, 1 }, { 1, 1, 1 } };
	int ar, gr, br, rr;
	RGBApixel newPixel;
	int wj, wi;

	for (int ky = 1; ky < mainImg.TellHeight() - 1; ky++){
		for (int kx = 1; kx < mainImg.TellWidth(); kx++){
			ar = 0; gr = 0; br = 0; rr = 0;
			for (int i = ky - 1, wi = 0; i < ky + 1; i++, wi++){
				for (int j = kx - 1, wj = 0; j < kx + 1; j++, wj++){
					//ar += w[wj][wi] * image.GetPixel(j, i).Alpha;
					br += w[wj][wi] * mainImg.GetPixel(j, i).Blue;
					gr += w[wj][wi] * mainImg.GetPixel(j, i).Green;
					rr += w[wj][wi] * mainImg.GetPixel(j, i).Red;
				}
			}
			//newPixel.Alpha = image.GetPixel(kx, ky).Alpha - ar;
			//newPixel.Alpha = (newPixel.Alpha + 8 * 255) / 16;
			newPixel.Blue = mainImg.GetPixel(kx, ky).Blue - br;
			newPixel.Blue = (newPixel.Blue + 8 * 255) / 16;
			newPixel.Green = mainImg.GetPixel(kx, ky).Green - gr;
			newPixel.Green = (newPixel.Green + 8 * 255) / 16;
			newPixel.Red = mainImg.GetPixel(kx, ky).Red - rr;
			newPixel.Red = (newPixel.Red + 8 * 255) / 16;
			newImage.SetPixel(kx, ky, newPixel);
		}
	}

	lastTrans = newImage;
	ostatniaOperacja = "Filstracja Laplasjan";
	return newImage;
}