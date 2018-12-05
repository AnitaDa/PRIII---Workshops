
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
****************************************************************************/
#include <iostream>
using namespace std;

const char *crt = "\n--------------------------------------------------\n";

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;		// po elementu2 se sortira
	int _trenutnoElemenata;
public:
	Kolekcija():_trenutnoElemenata(0), _elementi1(nullptr), _elementi2(nullptr) {}
	Kolekcija(const Kolekcija & original) {
		_trenutnoElemenata = original._trenutnoElemenata;
		_elementi1 = new T1[_trenutnoElemenata];
		_elementi2 = new T2[_trenutnoElemenata];
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}
	void AddElement(T1 element1, T2 element2) {
		T1 * temp1 = new T1[_trenutnoElemenata + 1];
		T2 * temp2 = new T2[_trenutnoElemenata + 1];
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutnoElemenata] = element1;
		temp2[_trenutnoElemenata] = element2;
		delete[] _elementi1;	_elementi1 = temp1;
		delete[] _elementi2;	_elementi2 = temp2;
		_trenutnoElemenata++;
	}
	friend ostream & operator<<(ostream & cout, const Kolekcija & k) {
		cout << "\t\tIspis kolekcije:\n";
		for (size_t i = 0; i < k._trenutnoElemenata; i++)
		{
			cout << "Element 1 [" << i + 1 << "]: " << k._elementi1[i];
			cout << "\tElement 2 [" << i + 1 << "]: " << k._elementi2[i] << endl;
		}
		return cout;
	}
	void RemoveElement(int index) {
		for (size_t i = index; i < _trenutnoElemenata - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutnoElemenata--;
	}
	Kolekcija & operator=(const Kolekcija & right) {
		if (this != &right) {
			this->_trenutnoElemenata = right._trenutnoElemenata;
			this->_elementi1 = new T1[this->_trenutnoElemenata];
			this->_elementi2 = new T2[this->_trenutnoElemenata];
			for (size_t i = 0; i < this->_trenutnoElemenata; i++)
			{
				this->_elementi1[i] = right._elementi1[i];
				this->_elementi2[i] = right._elementi2[i];
			}
		}
		return *this;
	}
	int getTrenutno() const { return _trenutnoElemenata; }
	bool VecPostoji(T1 element1, T2 element2) {
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if ((_elementi1[i] == element1) && (_elementi2[i] == element2)) return true;
		}
		return false;
	}
	bool Postoji(T1 element) {
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == element) return true;
		}
		return false;
	}
	void Sort_T2() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
			{
				if (_elementi2[i] > _elementi2[i + 1]) {
					T1 temp = _elementi1[i];
					_elementi1[i] = _elementi1[i + 1];
					_elementi1[i + 1] = temp;
					T2 temp1 = _elementi2[i];
					_elementi2[i] = _elementi2[i + 1];
					_elementi2[i + 1] = temp1;
					promjena = true;
				}
			}
		}
	}
	T2 operator[](int indeks) {
		return _elementi2[indeks];
	}
	void _changeT2(T2 vrijednost, int indeks) {
		_elementi2[indeks] = vrijednost;
	}
};
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum & original) {
		_dan = new int(*original._dan);
		_mjesec = new int(*original._mjesec);
		_godina = new int(*original._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << ".";
		return COUT;
	}
	Datum & operator=(const Datum & right) {
		if (this != &right) {
			this->_dan = new int(*right._dan);
			this->_mjesec = new int(*right._mjesec);
			this->_godina = new int(*right._godina);
		}
		return *this;
	}
	bool operator==(const Datum & right) {
		if (*this->_dan == *right._dan && *this->_mjesec == *right._mjesec && *this->_godina == *right._godina)
			return true;
		else return false;
	}
};
class Student {
	char * _imePrezime;
	Datum * _datumRodjenja;
public:
	Student(const char * imePrezime = "---", Datum d = Datum()) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum(d);
	}
	~Student() { delete[] _imePrezime; delete _datumRodjenja; }
	Student(const Student & original) {
		_imePrezime = new char[strlen(original._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(original._imePrezime) + 1, original._imePrezime);
		_datumRodjenja = new Datum(*original._datumRodjenja);
	}
	Student & operator=(const Student & right) {
		if (this != &right) {
			this->_imePrezime = new char[strlen(right._imePrezime) + 1];
			strcpy_s(this->_imePrezime, strlen(right._imePrezime) + 1, right._imePrezime);
			this->_datumRodjenja = new Datum(*right._datumRodjenja);
		}
		return *this;
	}
	friend ostream & operator<<(ostream & cout, const Student & s) {
		cout << s._imePrezime << ", " << *s._datumRodjenja << endl;
		return cout;
	}
	bool operator==(const Student & right) {
		if (strcmp(this->_imePrezime, right._imePrezime) == 0) {
			if (*this->_datumRodjenja == *right._datumRodjenja) {
				return true;
			}
		}
		return false;
	}
};
class Ispit {
	Datum _datumOdrzavanja;
	char * _opisIspita;
	Kolekcija<Student, Datum> _prijave;
	Kolekcija<Student, int> _rezultati;
public:
	Ispit(const char * opis, Datum datum) : _datumOdrzavanja(datum) {
		_opisIspita = new char[strlen(opis) + 1];
		strcpy_s(_opisIspita, strlen(opis) + 1, opis);
	}
	Ispit(const Ispit & original) {
		_datumOdrzavanja = original._datumOdrzavanja;
		_opisIspita = new char[strlen(original._opisIspita) + 1];
		strcpy_s(_opisIspita, strlen(original._opisIspita) + 1, original._opisIspita);
		_prijave = original._prijave;
		_rezultati = original._rezultati;
	}
	bool DodajPrijavu(const Student & s, const Datum & d) {
		if(!_prijave.VecPostoji(s, d))
			_prijave.AddElement(s, d);
		else {
			cout << "Prijava vec postoji!\n"; return false;
		}

	}
	void DodajRezultat(const Student & s, int ocjena) {
		if (_prijave.Postoji(s)) {
			_rezultati.AddElement(s, ocjena);
			cout << "Rezultat spremljen!\n";
		}
		else cout << "Predmet mora biti prijavljen!\n";
	}
	void Sortiraj() {
		_rezultati.Sort_T2();
		cout << "\nRezultati sortirani!\n";
	}
	void ZakljuciRezultate() {
		for (size_t i = 0; i < _rezultati.getTrenutno(); i++)
		{
			if (_rezultati[i] <= 5) {
				_rezultati._changeT2(5, i);
			}
		}
		cout << "Rezultati zakljuceni!\n";
	}
	friend ostream & operator<<(ostream & cout, const Ispit & ispit) {
		cout << crt << "Opis ispita: " << ispit._opisIspita << endl;
		cout << "\tPRIJAVE:\n" << endl;
		cout << ispit._prijave;
		cout << crt << "\tREZULTATI:\n" << endl;
		cout << ispit._rezultati;
		return cout;
	}
};

void main() {

	//REGIONI olaksavaju organizaciju programskog code-a, te ih mozete ukloniti ukoliko vam otezavaju preglednost

#pragma region TestiranjeKolekcije
	Kolekcija<int, double> kolekcija1;
	for (size_t i = 0; i < 50; i++)
		kolekcija1.AddElement(i, i + 0.3);
	cout << kolekcija1 << endl;
	cout << crt;
	Kolekcija<int, double> kolekcija2;
	kolekcija2 = kolekcija1;
	kolekcija1.RemoveElement(2);//UMANJITI VELICINU NIZA
	cout << kolekcija2 << crt;
	if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, double> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeStudenta
	Student denis("Denis Music", Datum(26, 11, 1990));
	Student jasmin("Jasmin Azemovic", Datum(22, 11, 1990));
	Student goran;
	goran = jasmin;
	Student adel("Adel Handzic", Datum(25, 8, 1990));
	Student marija("Marija Herceg", Datum(15, 6, 1990));
#pragma endregion

#pragma region TestiranjeIspita
	Ispit prIII("PRIII::26.11.2015", danas);
	//DodajPrijavu - dodaje novog studenta u listu prijavljenih za ispit. onemoguciti dodavanje dva ista studenta
	if (prIII.DodajPrijavu(denis, danas))
		cout << denis << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(jasmin, danas))
		cout << jasmin << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(goran, danas))					// Gorana ne dodaje jer ima ista obiljezja kao Jasmin..
		cout << goran << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(adel, danas))
		cout << adel << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(marija, danas))
		cout << marija << " DODAT na spisak" << endl;

	cout << crt << endl;
	/*DodajRezultat - dodaje ocjenu koju je student ostvario na ispitu. za dodavanje ocjene student je prethodno trebao biti prijavljen na ispit
	*/
	prIII.DodajRezultat(goran, 7);
	prIII.DodajRezultat(jasmin, 6);
	prIII.DodajRezultat(adel, 8);
	prIII.DodajRezultat(marija, 9);

	/*Sortiraj - sortira rezultate ispita prema ocjeni*/
	prIII.Sortiraj();
	///*ZakljuciRezultate - dodaje ocjenu 5 svim studentima koji su prijavili ispit a nije im evidentirana ocjena*/
	prIII.ZakljuciRezultate();
	//rezultati ispita se smiju ispisivati jedino ukoliko su prethodno zakljuceni
	cout << prIII << crt;

	Ispit prIII_2(prIII);
	cout << prIII_2 << crt;
#pragma endregion
}

