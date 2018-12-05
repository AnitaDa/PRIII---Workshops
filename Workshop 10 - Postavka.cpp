#include<iostream>
using namespace std;

class NedozvoljenaOperacija : public exception {
	int linijaKoda;
public:
	NedozvoljenaOperacija(const char * poruka, int lin) :exception(poruka) {
		linijaKoda = lin;
	}
	void ErrorInfo() { cout << "Greska: " << exception::what() << " Linija koda: " << linijaKoda << endl; }
};

//Kataloška prodaja
class Proizvod
{
	char _sifra[10];
	char* _naziv;
	double _cijena;
	double _popust; //Izrazen procentualno
	//Dostupno proizvoda
	int _kolicina;
public:
	//Konstruktor i destruktor funkcije

	Proizvod(const char* sifra, const char* naziv, double cijena, int kolicina, double popust = 0)
	{
		//U slučaju nedozvoljenih vrijednosti za cijenu, popust i količinu baciti izuzetak

		strncpy_s(_sifra, sifra, _TRUNCATE);

		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);

		_cijena = cijena;
		_kolicina = kolicina;
		_popust = popust;
	}

	~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}

	//Implementirati potrebne funkcije za rad sa klasom
};

//Implementirati nasljeđivanje
class Odjeca
{
	char*_velicine;
	char* _boje;
	char _kategorija[50];
public:
	//Potrebne konstruktor i destruktor funkcije

	//Implementirati potrebne funkcije za rad sa klasom
};

class Katalog
{
	char* _naslov;
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy
	Proizvod* _proizvodi[100];
	int _trenutnoProizvoda;
public:
	//Potrebne konstruktor i destruktor funkcije
	Katalog(const char* naslov, char* vrijediDo)
	{
		//U slučaju da datum ne ispunjava zadati format baciti izuzetak
		int size = strlen(naslov) + 1;
		_naslov = new char[size];
		strcpy_s(_naslov, size, naslov);

		size = strlen(vrijediDo) + 1;
		_vrijediDo = new char[size];
		strcpy_s(_vrijediDo, size, vrijediDo);

		for (size_t i = 0; i < 100; i++)
		{
			_proizvodi[i] = nullptr;
		}

		_trenutnoProizvoda = 0;
	}

	~Katalog()
	{
		delete[] _naslov;
		_naslov = nullptr;

		delete[] _vrijediDo;
		_vrijediDo = nullptr;

		for (size_t i = 0; i < _trenutnoProizvoda; i++)
		{
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase. Funkcija treba da ima povratnu vrijednost tipa bool.
	//Onemogućiti dodavanje identičnih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).

	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre.
	//Ukoliko se ne pronađe traženi proizvod baciti izuzetak.

	//Preklopiti operator za ispis kataloške ponude
};


class Narudzba
{
	int _brojNarudzbe;
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy
	Proizvod* _naruceniProizvodi[100];
	int _brojProizvoda;
	bool otkazana;
public:
	//Potrebne konstruktor i destruktor funkcije
	//U slučaju da datum ne ispunjava zadati format baciti izuzetak

	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednosti tipa bool.
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.

	//Funkciju za uklanjanje određenog proizvoda iz narudžbe. Voditi računa da narudžba može 
	//imati više puta isti proizvod.

	//Dodati funkciju za proračun ukupne cijene narudžbe.
	//Voditi računa o mogućnosti dodavanja više istih proizvoda (količina).
	//Uračunati popust u slučaju da je definisan.

	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
};

//U posljednjoj klasi samostalno prepoznati izuzetne situacije
class Prodavac
{
	char* _imePrezime;
	Narudzba* _listaNarudzbi;
	int _brojNarudzbi;
public:
	//Potrebne konstruktor i destruktor funkcije

	//Omogućiti dodavanje narudžbi za prodavača

	//Omogućiti otkazivanje narudžbe na osnovu njenog broja.

	//Ispis svih podataka o prodavaču i kratke informacije o
	//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.

	//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
	//sa informacijom o tome da li se radi o odjeći ili osnovnom proizvodu.

};


int main()
{
	//Dodati minimalno 5 proizvoda u katalog.
	//Ispisati ponudu kupcu
	//Omogućiti kreiranje narudžbe za jednog prodavača na osnovu izbora kupca.

	system("pause");

	return 0;
}
