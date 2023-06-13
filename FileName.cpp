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
protected:
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
	friend struct porownanie;
	friend void wyswietl_produkty(vector<Produkt> produkty);
	friend vector<Produkt> dodaj_zamowienie(vector<Produkt> produkty);
	friend vector<Produkt> dodaj_dostawe(vector<Produkt> produkty);
	Produkt(string , int , int , float);
};

class Dostawa
{
protected:
	int ilosc_przyjeta;
	Produkt produkt;
public:
	Dostawa(Produkt prod, int ilosc);
};

Dostawa::Dostawa(Produkt prod, int ilosc) : produkt(prod), ilosc_przyjeta(ilosc) {}


class Zamowienie
{
protected:
	Produkt produkt;
	int ilosc_zamowiona;
public:
	Zamowienie(Produkt prod, int ilosc);
};

Zamowienie::Zamowienie(Produkt prod, int ilosc) : produkt(prod), ilosc_zamowiona(ilosc) {}

void wyswietl_produkty(vector<Produkt> produkty);

Produkt::Produkt(string naz = "domyslny", int kod = 0, int il = 0, float cen = 0) : nazwa(naz), kod_produktu(kod), ilosc(il), cena(cen) {};

struct porownanie
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
		cout << "Error, nie uda�o si� wczyta� produkt�w" << endl;
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
		cout << "Error, nie uda�o si� wczyta� zmian (nie zostan� zapisane)" << endl;
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
	string nazwa{};
	int kod, ilosc;
	float cena;

	cout<< endl << "Wprowad� nazw� nowego produktu: ";
	cin.clear();
	cin.ignore();
	getline(cin, nazwa);
	cout << "Kod produktu: ";
	cin >> kod;
	
	//sprawdzenie czy wprowadzony kod jest liczba
	while (cin.fail())
	{
		cout << "Kod musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}
	//sprawdzenie czy istnieje juz taki produkt o wpisanym kodzie, jesli tak, nalezy wpisac inny kod itd.
	for (int i = 0; i < produkty.size(); i++)
	{
		if (kod == produkty[i].kod_produktu)
		{
			cout << "W twoim sklepie ju� istnieje produkt o tym samym kodzie, wprowad� kod jeszcze raz: ";
			cin >> kod;
			i = -1;
		}
	}

	cout << "Stan magazynowy produktu: ";
	cin >> ilosc;
	while (cin.fail())
	{
		cout << "Ilosc musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> ilosc;
	}

	cout << "Cena produktu: ";
	cin >> cena;
	while (cin.fail())
	{
		cout << "Cena musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> cena;
	}
	cout << endl;
	Produkt p(nazwa, kod, ilosc, cena);
	produkty.push_back(p); // zapisuje produkt utworzony przez uzytkownika do wektora objektow Produkt
	cout << "POMY�LNIE DODANO PRODUKT!" << endl << endl;

	char wybor{};
	cout << "Czy chcesz doda� kolejny produkt? 1 - tak, dowolny klawisz zatwierdzony enterem - powr�t do menu" << endl;
	cin.ignore();
	cin >> wybor;
	if (wybor == '1') produkty = dodaj_produkt(produkty);
	
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
		cout << "Kod musi by� liczb�, podaj jeszcze raz: ";
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
	cout << "Czy chcesz usun�� kolejny produkt? 1 - tak, enter - powr�t do menu" << endl;
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
	cout << endl << "Podaj kod produktu, kt�ry chcesz edytowa�(nast�pnie nale�y wpisa� nazw�, ilo�� oraz cen� edytowanego produktu): ";
	cin >> kod;
	while (cin.fail())
	{
		cout << "Kod musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}
	cout << endl;
	for (int i = 0; i < p.size(); i++)
	{
		if (p[i].kod_produktu == kod)
		{
			cin.clear();
			cin.ignore();
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
			cout << endl << "POMYSLNIE EDYTOWANO PRODUKT!" << endl << endl;
		}
	}
	if (licznik == 0) cout << "Nie znaleziono takiego produktu" << endl;
	
	string wybor{};
	cout << "Czy chcesz edytowa� kolejny produkt? 1 - tak, enter - powr�t do menu" << endl;
	cin.ignore();
	getline(cin, wybor);
	if (wybor[0] == 49) p = edytuj_produkt(p);

	return p;
}

void wyswietl_produkty(vector<Produkt> produkty)
{
	sort(produkty.begin(), produkty.end(), porownanie());
	system("cls");
	cout << setw(15) << "Nazwa produktu" << setw(20) << "Kod produktu" << setw(20) << "Stan magazynowy" << setw(20) << "Cena" << endl << endl;
	for (int i = 0; i < produkty.size(); i++)
	{
		cout << setw(15) << produkty[i].nazwa << setw(20) << produkty[i].kod_produktu << setw(20) << produkty[i].ilosc << setw(20) << produkty[i].cena << endl;
	}
	system("pause");
}

void wyswietl_menu()
{
	system("cls");
	cout << "----------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "------Witaj w systemie zarz�dzania twoim sklepem internetowym!!!------" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "----------------------------------------------------------------------" << endl << endl;
	cout << "1. Dodaj nowy produkt" << endl;
	cout << "2. Usu� produkt" << endl;
	cout << "3. Edytuj produkt" << endl;
	cout << "4. Dodaj produkt z dostawy" << endl;
	cout << "5. Dodaj zam�wienie klienta" << endl;
	cout << "6. Wy�wietl wszystkie produkty" << endl << endl;
	cout << "Wpisanie innej opcji ni� jest dost�pna powoduje opuszczenie programu i zapisanie zmian" << endl << endl;
	cout << "Wybierz opcj� (1-6): ";
}

vector<Produkt> dodaj_dostawe(vector<Produkt> produkty)
{
	vector<Dostawa> dostawa;
	int kod, ilosc_przyjeta;
	//char wybor;
	cout << "Podaj kod produktu, kt�ry chcesz przyj�c na magazyn: ";
	cin >> kod;
	while (cin.fail())
	{
		cout << "Kod musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}

	bool produkt_znaleziony = false;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkt_znaleziony = true;
			cout << "Podaj ilo�� przyj�tego produktu: ";
			cin >> ilosc_przyjeta;
			while (cin.fail())
			{
				cout << "Ilo�� musi by� liczb�, podaj jeszcze raz: ";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> ilosc_przyjeta;
			}

			Dostawa zamowienie(produkty[i], ilosc_przyjeta);
			produkty[i].ilosc += ilosc_przyjeta;
			break;
		}
	}

	if (!produkt_znaleziony)
		cout << "Nie znaleziono takiego produktu" << endl;

	string wybor{};
	cout << "Czy chcesz przyj�� kolejny produkt? 1 - tak, enter - powr�t do menu" << endl;
	cin.ignore();
	getline(cin, wybor);
	if (wybor[0] == 49) produkty = edytuj_produkt(produkty);

	return produkty;
}

//ZAMOWIENIE
vector<Produkt> dodaj_zamowienie(vector<Produkt> produkty)
{
	int kod, ilosc_zamowiona;
	char wybor;
	vector<Zamowienie> zamowienie;

	cout << "Podaj kod produktu, kt�ry zostal zam�wiony: " << endl;
	cin >> kod;

	while (cin.fail())
	{
		cout << "Kod musi by� liczb�, podaj jeszcze raz: ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> kod;
	}

	bool produkt_znaleziony = false;
	for (int i = 0; i < produkty.size(); i++)
	{
		if (produkty[i].kod_produktu == kod)
		{
			produkt_znaleziony = true;
			cout << "Podaj ilo�� zam�wionego produktu: " << endl;
			cin >> ilosc_zamowiona;
			while (cin.fail())
			{
				cout << "Ilo�� musi by� liczb�, podaj jeszcze raz: ";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> ilosc_zamowiona;
			}

			Zamowienie zamowienie(produkty[i], ilosc_zamowiona);
			produkty[i].ilosc -= ilosc_zamowiona;
			if (produkty[i].ilosc < 0)
			{
				cout << "OSTRZE�ENIE! Brak wystarczaj�cej ilo�ci produktu na magazynie, czy chcesz potwierdzi� operacj�?" << endl << "1 - tak, 2 - nie" << endl;
				while (true)
				{
					cin >> wybor;

					if (wybor == '1' || wybor == '2')
					{
						break;
					}
					else
					{
						cout << "nieprawidlowy wybor, wybierz opcj� 1 lub 2: ";
					}
				}

				switch (wybor)

				{
				case 1:
					break;
				case 2:
					produkty[i].ilosc += ilosc_zamowiona;

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

	while (true)
	{
		cin >> wybor;
		if (wybor == '1' || wybor == '2')
		{
			break;
		}
		else
		{
			cout << "nieprawidlowy wybor, wybierz opcj� 1 lub 2: ";
		}
	}

	switch (wybor)
	{
	case 1:
		produkty = dodaj_zamowienie(produkty);
		break;
	case 2:
		break;
	default:
		cout << "wybierz opcj� 1 lub2 ";
	}
	return produkty;
}

bool logowanie()
{
	string nazwa, haslo, uzytkownik, pass;
	cout << "Wprowad� nazw� u�ytkownika: \n";
	cin >> nazwa;
	cout << "Podaj has�o: \n";
	cin >> haslo;

	ifstream czytaj(nazwa + "login.txt");
	getline(czytaj, uzytkownik);
	getline(czytaj, pass);

	if (uzytkownik == nazwa && pass == haslo)
	{
		return true;

	}
	else
	{
		return false;
	}
}

int login()
{
	char wybor;

	cout << endl << endl;
	cout << setw(69) << "Wybierz opcj�:" << endl;
	cout << endl << setw(65) << "1:Zaloguj si�";
	cout << endl << setw(69) << "2:Zarejestruj si�" << endl;

	cin >> wybor;
	system("cls");
	if (wybor == '2')
	{
		string nazwa, haslo;
		cout << "Wybierz nazw� u�ytkownika: ";
		cin >> nazwa;
		cout << "Wybierz has�o: ";
		cin >> haslo;
		cout << "pomy�lnie zarejestrowano";
		ofstream plik;
		plik.open(nazwa + "login.txt");
		plik << nazwa << endl << haslo;
		plik.close();
		system("cls");
		login();
	}
	else if (wybor == '1')
	{
		bool status = logowanie();
		if (!status)
		{
			cout << "Wprowadzono nieprawid�owe dane, spr�buj ponownie" << endl;
			system("PAUSE");
			cout << endl;
			system("cls");
			login();
		}
	}
	else
	{
		login();
		return 0;
		system("cls");
	}
}

int main()
{
	setlocale(LC_CTYPE, "Polish");
	Produkt p1;
	vector<Produkt> produkty;
	produkty = wczytaj_z_pliku(p1);
	
	login();

	char wybor;
	do
	{
		wyswietl_menu();
		cin >> wybor;
		switch (wybor)
		{
		case '1': produkty = dodaj_produkt(produkty); break;
		case '2': produkty = usun_produkt(produkty); break;
		case '3': produkty = edytuj_produkt(produkty); break;
		case '4': produkty = dodaj_dostawe(produkty); break;
		case '5': produkty = dodaj_zamowienie(produkty); break;
		case '6': wyswietl_produkty(produkty); break;
		}
		if (wybor <= '0' || wybor >= '7')
		{
			cout << endl <<"OPUSZCZANIE PROGRAMU, ZMIANY ZOSTA�Y ZAPISANE!" << endl;
			break;
		}
	} while (1);

	wczytaj_do_pliku(produkty);
}