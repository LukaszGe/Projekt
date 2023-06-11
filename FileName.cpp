#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <locale.h>
#include <algorithm>
using namespace std;


class Produkt
{
public:
	string nazwa;
	int kod_produktu;
	int ilosc;
	float cena;
public:
	friend vector<Produkt> edytuj_produkt(vector<Produkt>p);
	friend vector<Produkt> dodaj_produkt(vector<Produkt> produkty);
	friend vector<Produkt> wczytaj_z_pliku(Produkt &p);
	friend void wczytaj_do_pliku(vector<Produkt>p);
	friend vector<Produkt> usun_produkt(vector<Produkt> produkty);

	Produkt(string , int , int , float);
};

class Dostawa
{
public:
	Produkt produkt;
	int ilosc_przyjeta;

	Dostawa(Produkt prod, int ilosc);
};

Dostawa::Dostawa(Produkt prod, int ilosc) : produkt(prod), ilosc_przyjeta(ilosc) {}


class Zamowienie
{
public:
	Produkt produkt;
	int ilosc_zamowiona;
	Zamowienie(Produkt prod, int ilosc);
};

Zamowienie::Zamowienie(Produkt prod, int ilosc) : produkt(prod), ilosc_zamowiona(ilosc) {}

void wyswietl_produkty(vector<Produkt> produkty);

Produkt::Produkt(string naz = "domyslny", int kod = 0, int il = 0, float cen = 0) : nazwa(naz), kod_produktu(kod), ilosc(il), cena(cen) {};

struct less_than_key
{
	inline bool operator() (const Produkt& obj1, const Produkt& obj2)
	{
		return (obj1.kod_produktu < obj2.kod_produktu);
	}
};

vector<Produkt> wczytaj_z_pliku(Produkt &p) // funkcja, ktora wczytuje produkty z pliku do wektora objektow Produkt, jest odpalana tylko przy odpaleniu programu
{
	ifstream plik;
	plik.open("baza.txt");

	if (!plik)
	{
		cout << "Error, nie uda³o siê wczytaæ produktów" << endl;
	}

	vector<Produkt> produkty;
	
	while (plik >> p.nazwa >> p.kod_produktu >> p.ilosc >> p.cena)
	{
		produkty.push_back(p);
	}
	plik.close();
	return produkty;
}

// funkcja, ktora na koncu programu pozwala wczytac wszystkie produkty (poniewaz w trakcie trwania programu moglo dojsc do zmian produktow) do pliku tekstowego 
void wczytaj_do_pliku(vector<Produkt> p) 
{
	ofstream plik;
	plik.open("baza2.txt");

	if (!plik)
	{
		cout << "Error, nie uda³o siê wczytaæ zmian (nie zostan¹ zapisane)" << endl;
	}
	for (int i = 0; i < p.size(); i++)
	{
		plik << p[i].nazwa << " " << p[i].kod_produktu << " " << p[i].ilosc << " " << p[i].cena << endl;
	}
	plik.close();
	remove("baza.txt");
	rename("baza2.txt", "baza.txt");
}


// funkcja, ktora pozwala dodawac produkty do wektora objektow Produkt
vector<Produkt> dodaj_produkt(vector<Produkt> produkty) 
{
	system("cls");
	string nazwa;
	int kod, ilosc;
	float cena;

	cout<< endl << "WprowadŸ nazwê nowego produktu: ";
	cin.ignore();
	cin.clear();
	getline(cin, nazwa);
	cout << "Kod produktu: ";
	cin >> kod;
	
	//sprawdzenie czy wprowadzony kod jest liczba
	while (cin.fail())
	{
		cout << "Kod musi byæ liczb¹, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}
	//sprawdzenie czy istnieje juz taki produkt o wpisanym kodzie, jesli tak, nalezy wpisac inny kod itd.
	for (int i = 0; i < produkty.size(); i++)
	{
		if (kod == produkty[i].kod_produktu)
		{
			cout << "W twoim sklepie ju¿ istnieje produkt o tym samym kodzie, wprowadŸ kod jeszcze raz: ";
			cin >> kod;
			i = -1;
		}
	}

	cout << "Stan magazynowy produktu: ";
	cin >> ilosc;
	while (cin.fail())
	{
		cout << "Ilosc musi byæ liczb¹, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> ilosc;
	}

	cout << "Cena produktu: ";
	cin >> cena;
	cout << endl;
	Produkt p(nazwa, kod, ilosc, cena);
	produkty.push_back(p); // zapisuje produkt utworzony przez uzytkownika do wektora objektow Produkt
	cout << "POMYSLNIE DODANO PRODUKT!" << endl << endl;

	string wybor{};
	cout << "Czy chcesz dodaæ kolejny produkt? 1 - tak, enter - powrót do menu" << endl;
	cin.ignore();
	getline(cin, wybor);
	if (wybor[0] == 49) produkty = dodaj_produkt(produkty);

	return produkty;
}


//funkcja, ktora pozwala usunac dany produkt podajac jego kod
vector<Produkt> usun_produkt(vector<Produkt> produkty)
{
	system("cls");
	wyswietl_produkty(produkty);
	int kod, licznik{};
	cout << endl << "Podaj kod produktu, ktory chcesz usunac: ";
	cin >> kod;
	while (cin.fail())
	{
		cout << "Kod musi byæ liczb¹, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}
	cout << endl;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkty.erase(produkty.begin() + i);
			licznik++;
			cout << "POMYSLNIE USUNIETO PRODUKT!" << endl;
		}
	}
	if (licznik == 0) cout << "Nie znaleziono takiego produktu" << endl;

	string wybor{};
	cout << "Czy chcesz usun¹æ kolejny produkt? 1 - tak, enter - powrót do menu" << endl;
	cin.ignore();
	getline(cin, wybor);
	if (wybor[0] == 49) produkty = usun_produkt(produkty);

	return produkty;
}

//funkcja, ktora pozwala edytowac produkt
vector<Produkt> edytuj_produkt(vector<Produkt>p)
{
	system("cls");
	string nazwa;
	int kod, ilosc, licznik{};
	float cena;
	wyswietl_produkty(p);
	cout << endl << "Podaj kod produktu, który chcesz edytowaæ(nastêpnie nale¿y wpisaæ nazwê, iloœæ oraz cenê edytowanego produktu): ";
	cin >> kod;
	while (cin.fail())
	{
		cout << "Kod musi byæ liczb¹, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}
	cout << endl;
	for (int i = 0; i < p.size(); i++)
	{
		if (p[i].kod_produktu == kod)
		{
			cout << "Wprowadz nowa nazwe: ";
			cin >> nazwa;
			cout << "Wprowadz nowa ilosc: ";
			cin >> ilosc;
			cout << "Wprowadz nowa cene: ";
			cin >> cena;
			p[i].nazwa = nazwa;
			p[i].ilosc = ilosc;
			p[i].cena = cena;
			licznik++;
			cout << "POMYSLNIE EDYTOWANO PRODUKT!" << endl;
		}
	}
	if (licznik == 0) cout << "Nie znaleziono takiego produktu" << endl;
	
	string wybor{};
	cout << "Czy chcesz edytowaæ kolejny produkt? 1 - tak, enter - powrót do menu" << endl;
	cin.ignore();
	getline(cin, wybor);
	if (wybor[0] = 49) p = edytuj_produkt(p);

	return p;
}

void wyswietl_produkty(vector<Produkt> produkty)
{
	sort(produkty.begin(), produkty.end(), less_than_key());
	system("cls");
	cout << setw(15) << "Nazwa produktu" << setw(20) << "Kod produktu" << setw(20) << "Stan magazynowy" << setw(20) << "Cena" << endl;
	for (int i = 0; i < produkty.size(); i++)
	{
		cout << setw(15) << produkty[i].nazwa << setw(20) << produkty[i].kod_produktu << setw(20) << produkty[i].ilosc << setw(20) << produkty[i].cena << endl;
	}
	system("pause");
}

void wyswietl_menu()
{
	system("cls");
	cout << "Witaj w systemie zarz¹dzania twoim sklepem internetowym!!!" << endl << endl;
	cout << "Wybierz dostepn¹ opcjê: " << endl;
	cout << "1. Dodaj nowy produkt" << endl;
	cout << "2. Usuñ produkt" << endl;
	cout << "3. Edytuj produkt" << endl;
	cout << "4. Wyœwietl wszystkie produkty" << endl;
	cout << "Aby opuœciæ program i zapisaæ zmiany, nale¿y wpisaæ inn¹ liczbê ni¿ jest dostêpna" << endl << endl;
}

vector<Produkt> dodaj_dostawe(vector<Produkt> produkty)
{
	vector<Dostawa> dostawa;
	int kod, ilosc_przyjeta, wybor;

	cout << "Podaj kod produktu, który chcesz przyj¹c na magazyn: ";
	cin >> kod;

	bool produkt_znaleziony = false;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkt_znaleziony = true;
			cout << "Podaj iloœæ przyjêtego produktu: ";
			cin >> ilosc_przyjeta;

			Dostawa zamowienie(produkty[i], ilosc_przyjeta);
			produkty[i].ilosc += ilosc_przyjeta;
			break;
		}
	}

	if (!produkt_znaleziony)
		cout << "Nie znaleziono takiego produktu" << endl;

	cout << endl << "Czy chcesz przyj¹æ nastêpny produkt z dostawy?" << endl << "1 - tak, 2 - nie" << endl;
	cin >> wybor;
	cout << endl;
	switch (wybor)
	{
	case 1:
		produkty = dodaj_dostawe(produkty);
		break;
	case 2:
		break;
	default:
		cout << "Nie ma takiej opcji, powrót do menu" << endl;
	}

	return produkty;


}


//ZAMOWIENIE
vector<Produkt> dodaj_zamowienie(vector<Produkt> produkty)
{
	int kod, ilosc_zamowiona, wybor;
	vector<Zamowienie> zamowienie;

	cout << "Podaj kod produktu, który zostal zamówiony: " << endl;
	cin >> kod;

	bool produkt_znaleziony = false;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkt_znaleziony = true;
			cout << "Podaj iloœæ zamówionego produktu: " << endl;
			cin >> ilosc_zamowiona;

			Zamowienie zamowienie(produkty[i], ilosc_zamowiona);
			produkty[i].ilosc -= ilosc_zamowiona;
			if (produkty[i].ilosc < 0)
			{
				cout << "OSTRZE¯ENIE! Brak wystarczaj¹cej iloœci produktu na magazynie, czy chcesz potwierdziæ operacjê?" << endl << "1 - tak, 2 - nie" << endl;
				cin >> wybor;
				switch (wybor)
				{
				case 1:
					break;
				case 2:
					produkty[i].ilosc += ilosc_zamowiona;
					/*cout << "Czy chcesz dodaæ nastêpny produkt do zamówienia?" << endl << "1 - tak, 2 - nie" << endl;
					cin >> wybor;

					switch (wybor)
					{
					case 1:
						produkty = dodaj_zamowienie(produkty);
						break;
					case 2:
						break;
					default:
						cout << "Nie ma takiej opcji, powrot do menu";
					}*/
					break;
				default:
					cout << "Nie ma takiej opcji, powrot do menu";
				}
			}
			break;
		}
	}

	if (!produkt_znaleziony)
		cout << "Nie znaleziono takiego produktu" << endl;

	cout << "Czy chcesz dodac nastepny produkt do zamowienia?" << endl << "1 - tak, 2 - nie" << endl;
	cin >> wybor;

	switch (wybor)
	{
	case 1:
		produkty = dodaj_zamowienie(produkty);
		break;
	case 2:
		break;
	default:
		cout << "Nie ma takiej opcji, powrot do menu";
	}

	return produkty;


}

int main()
{
	setlocale(LC_CTYPE, "Polish");
	Produkt p1;
	vector<Produkt> produkty;
	produkty = wczytaj_z_pliku(p1);
	
	int wybor;
	do
	{
		wyswietl_menu();
		cout << "Wybierz opcjê (1-4): ";
		cin >> wybor;
		switch (wybor)
		{
		case 1: produkty = dodaj_produkt(produkty); break;
		case 2: produkty = usun_produkt(produkty); break;
		case 3: produkty = edytuj_produkt(produkty); break;
		case 4: wyswietl_produkty(produkty); break;
		}
	} while (wybor > 0 && wybor < 5);
	/*wyswietl_produkty(produkty);
	produkty = dodaj_zamowienie(produkty);
	wyswietl_produkty(produkty);*/

	wczytaj_do_pliku(produkty);
}