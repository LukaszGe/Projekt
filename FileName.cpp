#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Produkt
{
public:
	string nazwa;
	int kod_produktu;
	int ilosc;
	float cena;
public:
	friend vector<Produkt> dodaj_produkt(vector<Produkt> produkty);
	friend vector<Produkt> wczytaj_z_pliku(Produkt &p);
	friend void wczytaj_do_pliku(vector<Produkt>p);
	friend vector<Produkt> usun_produkt(vector<Produkt> produkty);

	Produkt(string , int , int , float);
};


Produkt::Produkt(string naz = "domyslny", int kod = 0, int il = 0, float cen = 0) : nazwa(naz), kod_produktu(kod), ilosc(il), cena(cen) {};


vector<Produkt> wczytaj_z_pliku(Produkt &p)
{
	ifstream plik;
	plik.open("baza.txt");

	if (!plik)
	{
		cout << "Error, nie udalo sie wczytac pliku" << endl;
	}

	vector<Produkt> produkty;
	
	while (plik >> p.nazwa >> p.kod_produktu >> p.ilosc >> p.cena)
	{
		produkty.push_back(p);
	}
	plik.close();
	return produkty;
}

void wczytaj_do_pliku(vector<Produkt> p)
{
	ofstream plik;
	plik.open("baza2.txt");

	if (!plik)
	{
		cout << "Error, nie udalo sie wczytac pliku" << endl;
	}
	for (int i = 0; i < p.size(); i++)
	{
		plik << p[i].nazwa << " " << p[i].kod_produktu << " " << p[i].ilosc << " " << p[i].cena << endl;
	}
	plik.close();
	remove("baza.txt");
	rename("baza2.txt", "baza.txt");
}

vector<Produkt> dodaj_produkt(vector<Produkt> produkty)
{
	string nazwa;
	int kod, ilosc, wybor;
	float cena;
	cout << "Wprowadz nazwe produktu: ";
	cin >> nazwa;
	cout << "Kod produktu: ";
	cin >> kod;
	for (int i = 0; i < produkty.size(); i++)
	{
		while (produkty[i].kod_produktu == kod)
		{
			cout << "W twoim sklepie juz istnieje produkt o tym samym kodzie, wprowadz kod jeszcze raz: ";
			cin >> kod;
		}
	}
	cout << "Stan magazynowy produktu: ";
	cin >> ilosc;
	cout << "Cena produktu: ";
	cin >> cena;
	cout << endl;
	Produkt p(nazwa, kod, ilosc, cena);
	produkty.push_back(p);

	cout << "Czy chcesz dodac nastepny produkt?" << endl << "1 - tak, 2 - nie" << endl;
	cin >> wybor;

	switch (wybor)
	{
	case 1: produkty = dodaj_produkt(produkty);
		break;
	case 2:
		break;
	default: cout << "Nie ma takiej opcji, powrot do menu";
	}

	return produkty;
}

vector<Produkt> usun_produkt(vector<Produkt> produkty)
{
	int kod, licznik{};
	cout << "Podaj kod produktu, ktory chcesz usunac: ";
	cin >> kod;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkty.erase(produkty.begin() + i);
			licznik++;
		}
	}
	if (licznik == 0)cout << "Nie znaleziono takieg produktu" << endl;
	return produkty;
}

int main()
{
	Produkt p1;
	vector<Produkt> produkty;
	produkty = wczytaj_z_pliku(p1);
	for (int i = 0; i < produkty.size(); i++)
	{
		cout << produkty[i].nazwa << " " << produkty[i].kod_produktu << " " << produkty[i].ilosc << " " << produkty[i].cena << endl;
	}
	produkty = dodaj_produkt(produkty);
	for (int i = 0; i < produkty.size(); i++)
	{
		cout << produkty[i].nazwa << " " << produkty[i].kod_produktu << " " << produkty[i].ilosc << " " << produkty[i].cena << endl;
	}
	produkty = usun_produkt(produkty);
	for (int i = 0; i < produkty.size(); i++)
	{
		cout << produkty[i].nazwa << " " << produkty[i].kod_produktu << " " << produkty[i].ilosc << " " << produkty[i].cena << endl;
	}
	wczytaj_do_pliku(produkty);
	system("pause");
	//ytfuytdutrdhgfdrye
}