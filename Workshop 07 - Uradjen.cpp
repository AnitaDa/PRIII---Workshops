#include<iostream>
using namespace std;

class Ispit
{
	char* _predmet;
	char _datumOdrzavanja[12]; //
	char _vrijemeOdrzavanja[6]; // pocetak ispita
	int _trajanje; //trajanje ispita u minutama
	int _maxBrojBodova;
	double _rezultat; //procentualni rezultat ispita
public:
	Ispit(const char* predmet, const char* datumOdrzavanja, const char* vrijemeOdrzavanja, int trajanje)
	{
		int vel = strlen(predmet) + 1;
		_predmet = new char[vel];
		strcpy_s(_predmet, vel, predmet);

		strncpy_s(_datumOdrzavanja, datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = trajanje;
		_maxBrojBodova = 0;
		_rezultat = 0.0;
	}
	Ispit(const Ispit & original) {
		int x = strlen(original._predmet) + 1;
		_predmet = new char[x];
		strcpy_s(_predmet, x, original._predmet);
		strcpy_s(_datumOdrzavanja, original._datumOdrzavanja);
		strcpy_s(_vrijemeOdrzavanja, original._vrijemeOdrzavanja);
		_trajanje = original._trajanje;
		_maxBrojBodova = original._maxBrojBodova;
		_rezultat = original._rezultat;
	}
	Ispit & operator=(const Ispit & right) {
		if (this != &right) {
			int x = strlen(right._predmet) + 1;
			this->_predmet = new char[x];
			strcpy_s(this->_predmet, x, right._predmet);
			strcpy_s(this->_datumOdrzavanja, right._datumOdrzavanja);
			strcpy_s(this->_vrijemeOdrzavanja, right._vrijemeOdrzavanja);
			this->_trajanje = right._trajanje;
			this->_maxBrojBodova = right._maxBrojBodova;
			this->_rezultat = right._rezultat;
		}
		return *this;
	}
	~Ispit()
	{
		delete[] _predmet;
		_predmet = nullptr;
	}

};

template <class T1, class T2, int max>
class Kolekcija
{
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	//Implementirati potrebne funkcije za rad sa kolekcijom
	Kolekcija() : _trenutno(0) {}
	Kolekcija(const Kolekcija & original) {
		_trenutno = original._trenutno;
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}
	Kolekcija & operator=(const Kolekcija & right) {
		if (this != &right) {
			this->_trenutno = right._trenutno;
			for (size_t i = 0; i < _trenutno; i++)
			{
				this->_elementi1[i] = right._elementi1[i];
				this->_elementi2[i] = right._elementi2[i];
			}
		}
		return *this;
	}
	friend ostream & operator<<(ostream & cout, Kolekcija & k) {
		for (size_t i = 0; i < k._trenutno; i++)
			cout << k._elementi1[i] << endl;
		for (size_t i = 0; i < k._trenutno; i++)
			cout << k._elementi2[i] << endl;
		return cout;
	}
	friend ostream & operator<<(ostream & cout, Kolekcija * k) {
		for (size_t i = 0; i < k->_trenutno; i++)
			cout << k->_elementi1[i] << endl;
		for (size_t i = 0; i < k->_trenutno; i++)
			cout << k->_elementi2[i] << endl;
		return cout;
	}
	void AddElement(T1 el1, T2 el2) {
		if (_trenutno >= max) {
			cout << "Dostigli ste max broj elemenata!\n";
			return;
		}
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}
	int getTrenutno() const { return _trenutno; }
	T1 & getElement1(int indeks) {
		return _elementi1[indeks];
	}
	T2 & getElement2(int indeks) {
		return _elementi2[indeks];
	}
};

class PismeniIspit
{
	Kolekcija<char*, int, 50>* _pitanja; //kolekcija pitanja sa brojem bodova
	Kolekcija<char*, int, 50> _odgovori; //kolekcija odgovora sa brojem osvojenih bodova
	int _brTacnihOdgovora;
	int _brOsvojenihBodova;
public:
	//Konstruktor i destruktor funkcije
	PismeniIspit() : _pitanja(new Kolekcija<char*, int, 50>), _brOsvojenihBodova(0), _brTacnihOdgovora(0) {}
	//Konstruktor kopije
	PismeniIspit(const PismeniIspit & original) :_pitanja(new Kolekcija<char*, int, 50>(*original._pitanja)) {
		_odgovori = original._odgovori;
		_brOsvojenihBodova = original._brOsvojenihBodova;
		_brTacnihOdgovora = original._brTacnihOdgovora;
	}
	~PismeniIspit() {
		delete _pitanja;
		_pitanja = nullptr;
	}
	PismeniIspit & operator=(const PismeniIspit & right) {
		if (this != &right) {
			this->_pitanja = new Kolekcija<char*, int, 50>(*right._pitanja);
			this->_odgovori = right._odgovori;
			this->_brTacnihOdgovora = right._brTacnihOdgovora;
			this->_brOsvojenihBodova = right._brOsvojenihBodova;
		}
		return *this;
	}

	//Operator() za dodavanje pitanja u kolekciju uz pripadajuci broj bodova.
	//Ispit ne smije sadržavati ista pitanja. Broj bodova na jednom pitanju ne smije biti negativan.
	void operator()(const char* pitanje, int brBodova) {
		if (brBodova <= 0) {
			cout << "Pitanje ne moze imati negativan broj bodova!\n";
			return;
		}
		for (size_t i = 0; i < _pitanja->getTrenutno(); i++)
		{
			if (strcmp(_pitanja->getElement1(i), (char*)pitanje) == 0) {
				cout << "Pitanje vec postoji!\n";
				return;
			}
		}
		_pitanja->AddElement((char*)pitanje, brBodova);
		cout << "Pitanje uspjesno dodano!\n";
	}

	//Operator+= za dodavanje odgovora na pitanje. Odgovori se dodaju u redoslijedu dodavanja pitanja.
	//Broj osvojenih bodova se inicijalno postavlja na 0. Moguce je dodati i prazan tekst kao odgovor.
	void operator+=(const char * odgovor) {
		_odgovori.AddElement((char*)odgovor, 0);
	}

	//Funkciju za ocjenu tacnog odgovora koja kao parametar prima redni broj pitanja u kolekciji i tacan odgovor. 
	//Funkcija treba da pronade odgovor na traženo pitanje, te da taj odgovor uporedi sa tacnim odgovorom i postavi
	//osvojene bodove. Student može osvojiti 0 ili maksimalan broj bodova na pitanju. U skladu sa rezultatom
	//provjere postaviti vrijednosti odgovarajucih atributa.
	void TacanOdgovor(int rb, const char * tacanOdg) {
		rb--;
		if (rb < 0 || rb > _odgovori.getTrenutno()) {
			cout << "Redni broj pitanja nije validan!\n";
			return;
		}
		if (strcmp(_odgovori.getElement1(rb), tacanOdg) == 0) {
			_brTacnihOdgovora++;
			_brOsvojenihBodova += _pitanja->getElement2(rb);
			cout << "Odgovor je tacan!\n";
			return;
		}
		cout << "Nazalost, odgovor nije tacan..\n";
	}
	friend ostream & operator<<(ostream & cout, const PismeniIspit & p) {
		cout << p._pitanja << endl;
		return cout;
	}
};

class PrakticniIspit
{
	Kolekcija<char*, char*, 3> _alati; //potrebni alati i njihove verzije, npr. alat "Visual Studio", verzija "2013"
	char* _zadatak; //tekst zadatka
	bool _zadovoljeno; //true ako je student zadovoljio na ispitu, u suprotnom false
public:
	//Na osnovu prethodne implementacije klase PismeniIspit osmisliti i implementirati
	//potrebne funkcije za rad sa klasom PrakticniIspit.
	PrakticniIspit(const char* zadatak = "Nepoznato") : _zadovoljeno(false) {
		int x = strlen(zadatak) + 1;
		_zadatak = new char[x];
		strcpy_s(_zadatak, x, zadatak);
	}
	PrakticniIspit(const PrakticniIspit & original) {
		_alati = original._alati;
		int x = strlen(original._zadatak) + 1;
		_zadatak = new char[x];
		strcpy_s(_zadatak, x, original._zadatak);
		_zadovoljeno = original._zadovoljeno;
	}
	PrakticniIspit & operator=(const PrakticniIspit & right) {
		if (this != &right) {
			this->_alati = right._alati;
			this->_zadatak = new char[strlen(right._zadatak) + 1];
			strcpy_s(this->_zadatak, strlen(right._zadatak) + 1, right._zadatak);
			this->_zadovoljeno = right._zadovoljeno;
		}
		return *this;
	}
	~PrakticniIspit() {
		delete[] _zadatak;
		_zadatak = nullptr;
	}
	void operator()(const char * alat, const char * verzija) {
		_alati.AddElement((char*)alat, (char*)verzija);
	}
	void SetZadovoljeno() {
		_zadovoljeno = true;
	}
	bool GetZadovoljeno() const { return _zadovoljeno; }
	friend ostream & operator<<(ostream & out, const PrakticniIspit & ispit) {
		out << "Zadatak: " << ispit._zadatak << endl;
		return out;
	}
};

int main()
{
	//Testirati sve dostupne funkcionalnosti putem datih kolekcija.
	//Drugi parametar kolekcije predstavlja broj indeksa studenta.
#pragma region Testiranje Kolekcije
	Kolekcija<int, const char*, 5> test;
	test.AddElement(5, "Ajdin Hukara");
	test.AddElement(6, "Ajdina Hukara");
	test.AddElement(7, "Ajdins Hukara");
	test.AddElement(8, "Ajdind Hukara");
	test.AddElement(9, "Ajdinf Hukara");
	test.AddElement(4, "Ajding Hukara");
	cout << test;
	Kolekcija<int, const char*, 5> test2(test);
	cout << test2;
#pragma endregion

#pragma region Testiranje pismenog ispita
	PismeniIspit ispit1;
	ispit1("Gdje se nalazi FIT?", 3);
	ispit1("Gdje se nalazi FIT?", 5);
	ispit1("Sta je pokazivac?", -9);
	ispit1("Sta ima kod mene?", 1);

	PismeniIspit ispit2(ispit1);
	ispit2 += "Sarajevo";
	ispit2 += "Nista, kod mene?";

	ispit2.TacanOdgovor(1, "Mostar");				// tacan odgovor je Mostar, a uneseno je Sarajevo
	ispit2.TacanOdgovor(2, "Nista, kod mene?");
#pragma endregion

#pragma region Testiranje prakticnog ispita
	PrakticniIspit prakticni("Seminarski rad iz predmeta Web Razvoj i Dizajn");
	prakticni("Visual Studio ", "2017");
	prakticni("Visual Studio Code ", "2018");
	prakticni("Javascript ", "v4.01");
	prakticni.SetZadovoljeno();

#pragma endregion


	Kolekcija<PismeniIspit, const char*, 100> _pismeniIspiti;
	_pismeniIspiti.AddElement(ispit1, "Ispit iz predmeta Programiranje II");
	Kolekcija<PrakticniIspit, const char*, 100> _prakticniIspiti;
	_prakticniIspiti.AddElement(prakticni, "Prakticni ispit iz predmeta WRD");
	
	return 0;
}
