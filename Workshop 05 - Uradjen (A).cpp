#include<iostream>
using namespace std;

auto crt = "\n--------------------------------------------------------\n";

template<class T1, class T2, int max>
class FITKolekcija
{
	int* _trenutno;
	T1 _elementi1[max];
	T2* _elementi2[max];
public:
	//Potrebne konstruktor i destruktor funkcije
	//Kreirati konstruktor kopije za kolekciju
	FITKolekcija() : _trenutno(new int(0)) {}
	FITKolekcija(const FITKolekcija & original) {
		_trenutno = new int(*original._trenutno);
		for (size_t i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = new T2(*original._elementi2[i]);
		}
	}
	~FITKolekcija() {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			delete _elementi2[i];
		}
		delete _trenutno; _trenutno = nullptr;
	}

	//Preklopiti operator() na nacin da omoguci dodavanje novih elemenata u kolekciju
	//Pri tome sprijeciti dodavanja duplih elemenata (odnosi se i na T1 i na T2)
	void operator()(T1 novit1, T2 novit2) {
		_elementi1[*_trenutno] = novit1;
		_elementi2[*_trenutno] = new T2(novit2);
		(*_trenutno)++;
		cout << "Clanovi uspjesno dodani!\n\tTrenutno clanova: " << *_trenutno << endl;
	}

	//Preklopiti operator[] na nacin da vraca element tipa T1 na osnovu njegove lokacije u nizu
	T1 & operator[](int lokacija) {
		while (lokacija < 1) lokacija++;
		return _elementi1[--lokacija];
	}

	//Preklopiti operator-= na nacin da uklanja elemente iz kolekcije na osnovu rednog broja proslijedenog kao parametar
	void operator-=(int lokacija) {
		while (lokacija < 1) lokacija++;
		lokacija--;

		for (int i = lokacija; i < *_trenutno - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			delete _elementi2[i];
			_elementi2[i] = new T2(*_elementi2[i + 1]);
		}
		(*_trenutno)--;
	}

	//Preklopiti operator za ispis. Implementaciju raditi izvan tijela klase.
	friend ostream & operator<< <>(ostream &, FITKolekcija<T1, T2, max>);
	bool provjeriPostojanje(T1 data) {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (_elementi1[i] == data) return false;
		}
		return true;
	}
	bool provjeriPostojanje(const char * data) {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (strcmp(_elementi1[i], data) == 0) return false;
		}
		return true;
	}
	int GetTrenutno() const { return *_trenutno; }
};

template<class T1, class T2, int max>
ostream & operator<< (ostream & out, FITKolekcija<T1, T2, max> kolekcija) {
	out << crt << "Trenutno clanova: " << *kolekcija._trenutno << endl;
	for (size_t i = 0; i < *kolekcija._trenutno; i++)
	{
		out << "Element [T1]" << i + 1 << ": " << kolekcija._elementi1[i] << endl;
		out << "Element [T2]" << i + 1 << ": " << *kolekcija._elementi2[i] << endl << endl;
	}
	out << crt;
	return out;
}
int brOdgovora = 0;
class Anketa
{
	char* _pitanje;
	int _trajanje; //izraženo u danima
	//Kolekcija treba da pohrani moguce odgovore na pitanja, sa pripadajucim rednim brojem
	FITKolekcija<char*, int, 8> _odgovori;
public:
	//Kreirati potrebne konstruktor i destruktor funkcije
	Anketa(int trajanje = 0, const char* pitanje = "Empty") : _trajanje(_trajanje), _pitanje(new char[strlen(pitanje) + 1]) {
		strcpy_s(_pitanje, strlen(pitanje) + 1, pitanje);
	}
	~Anketa() {
		delete[] _pitanje; _pitanje = nullptr;
		_trajanje = 0;
	}

	//Kreirati funkciju DodajOdgovor koja treba da doda novi odgovor za anketu.
	//Sprijeciti dodavanje istog odgovora više puta.
	void DodajOdgovor(const char* noviOdgovor) {
		if (!_odgovori.provjeriPostojanje(noviOdgovor)) {
			cout << "Odgovor " << noviOdgovor << " vec postoji!\n";
			return;
		}

		_odgovori((char*)noviOdgovor, brOdgovora++);
		cout << "Odgovor uspjesno dodan!\n";
	}

	//Kreirati funkciju UkloniOdgovor koja uklanja odgovor na osnovu njegovog rednog broja.
	void UkloniOdgovor(int pozicija) {
		if (pozicija < 1 || pozicija>8) {
			cout << "Error!\n";
			return;
		}
		_odgovori -= pozicija;
		cout << "Odgovor uspjesno uklonjen!\n";
	}

	//Preklopiti operator++ na nacin da povecava trajanje ankete za jedan dan
	//Pri tome voditi racuna da se implementiraju dvije verzije ovog operatora (objekat++; ++objekat)
	void operator++(int v)
	{
		_trajanje += 1;
	}
	void operator++()
	{
		_trajanje += 1;
	}

	//Preklopiti operator za ispis
	friend ostream & operator<<(ostream & out, const Anketa & anketa) {
		out << crt;
		out << "Pitanje: " << anketa._pitanje << endl;
		out << anketa._odgovori;
		out << crt;
		return out;
	}
};

int main()
{
	FITKolekcija<float, int, 5> kolekcija;
	kolekcija(7.81f, 15);
	kolekcija(9.11f, 101);
	kolekcija(322.178f, 62);
	kolekcija(81.5f, 1);
	kolekcija(0.122f, 33);

	cout << "Clan u nizu tipa T1 na lokaciji 2 je: " << kolekcija[2];

	cout << kolekcija;

	kolekcija -= 2;

	cout << kolekcija;

	Anketa anketa1(2, "Gdje se nalazi FIT?");
	anketa1.DodajOdgovor("Mostar");
	anketa1.DodajOdgovor("Sarajevo");
	anketa1.DodajOdgovor("Tuzla");
	anketa1.DodajOdgovor("Tuzla");

	cout << anketa1;

	return 0;
}
