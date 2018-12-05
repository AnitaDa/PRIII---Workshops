#include<iostream>

using namespace std;

auto crt = "----------------------------------------------------\n";

class Vrijeme {
	int _sati;
	int _minute;
	int _sekunde;
public:
	Vrijeme() {
		_sati = _minute = _sekunde = 0;
	}
	Vrijeme(int h, int m, int s) {
		_sati = h; _minute = m; _sekunde = s;
	}
	Vrijeme(int h, int m) {
		_sati = h; _minute = m; _sekunde = 0;
	}
	void Info(bool format) {
		if (_sati > 24) {
			int ostatak = _sati - 24; _sati -= 24;
		}
		if (format) {
			cout << _sati << ":" << _minute << ":" << _sekunde << endl;
		}
		else cout << _sati << ":" << _minute << endl;
	}
	Vrijeme Add(Vrijeme novo) {
		Vrijeme temp(novo._sati + this->_sati, novo._minute + this->_minute, novo._sekunde + this->_sekunde);
		return temp;
	}
	Vrijeme Add(int minute) {
		Vrijeme temp(this->_sati, this->_minute + minute, this->_sekunde);
		return temp;
	}
	int CompareTo(Vrijeme drugo) {
		if (this->_sati > drugo._sati) return 1;
		if (this->_sati < drugo._sati) return -1;
		if (this->_sati == drugo._sati) {
			if (this->_minute > drugo._minute) return 1;
			if (this->_minute < drugo._minute) return -1;
			if (this->_minute == drugo._minute) {
				if (this->_sekunde > drugo._sekunde) return 1;
				if (this->_sekunde < drugo._sekunde) return -1;
				if (this->_sekunde == drugo._sekunde) return 0;
			}
		}
	}
};


/*
Potrebno je kreirati aplikaciju za dijeljenje znanja putem razmjene clanaka medu korisnicima.

Izdvojene su sljedece funkcionalnosti:
- Administrator sistema je zadužen za upravljanje korisnickim nalozima (Ime, Prezime, Email, DatumRegistracije, KorisnickoIme, Lozinka).
- Svaki clanak (Naslov, Sadrzaj, Datum, Satnica, BrojPregleda) može sadržavati više tagova, ali pripada samo jednoj oblasti.
- Clanke mogu dodavati svi registrovani korisnici.
- Korisnici aplikacije takoder mogu ostaviti komentar (Tekst, Datum, Popularnost) na odredeni clanak.
*/

/*
Na osnovu datih klasa implementirati opisane funkcionalnosti sistema. Po potrebi dopuniti klase novim atributima i/ili funkcijama.
*/

class Datum
{
	int _dan;
	int _mjesec;
	int _godina;
public:
	//Potrebne konstruktor i destruktor funkcije
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = dan; _mjesec = mjesec; _godina = godina;
	}
	~Datum() {
		_dan = _mjesec = 1; _godina = 2000;
	}
	void Info() {
		cout << _dan << "." << _mjesec << "." << _godina << "." << endl;
	}
};

enum TipKorisnika { Administrator, Guest };
const char* TipKorisnikaChar[2] = { "Admin", "Guest" };

class Korisnik
{
	char _ime[30];
	char _korisnickoIme[20];
	char* _lozinka;
	TipKorisnika _tipKorisnika; //Administrator, Guest
public:
	Korisnik(const char* ime = "Nema", const char* KorisnickoIme = "Nema", const char* lozinka = "0000") {
		strcpy_s(_ime, ime);
		strcpy_s(_korisnickoIme, KorisnickoIme);
		_lozinka = new char[strlen(lozinka) + 1];
		strcpy_s(_lozinka, strlen(lozinka) + 1, lozinka);
		_tipKorisnika = Guest;
	}
	Korisnik(const Korisnik & original) {
		strcpy_s(_ime, original._ime);
		strcpy_s(_korisnickoIme, original._korisnickoIme);
		_lozinka = new char[strlen(original._lozinka) + 1];
		strcpy_s(_lozinka, strlen(original._lozinka) + 1, original._lozinka);
		_tipKorisnika = original._tipKorisnika;
	}
	void PromjenaLozinke(const char* nova) {
		int brojPokusaja = 3;
		char temp[100];
		while (brojPokusaja) {
			brojPokusaja--;
			system("CLS");
			cout << "Unesite vasu staru lozinku: [Pokusaj " << brojPokusaja + 1 << "]" << endl;
			cin.getline(temp, 100);
			if (strcmp(temp, _lozinka) == 0) {
				delete[]_lozinka;
				_lozinka = new char[strlen(nova) + 1];
				strcpy_s(_lozinka, strlen(nova) + 1, nova);
				cout << "Lozinka uspjesno promijenjena!\n";
				return;
			}
		}
		cout << "Niste uspjeli pogoditi lozinku!\n";
	}
	void Info() {
		cout << crt << "Puno ime i prezime:\t " << _ime << endl;
		cout << "Korisnicno ime:\t\t '" << _korisnickoIme << "'" << endl;
		cout << "Tip korisnika:\t\t " << TipKorisnikaChar[_tipKorisnika] << endl << crt;
	}
	char * getIme() { return _ime; }
	char * getLozinku() { return _lozinka; }
	~Korisnik() {
		strcpy_s(_ime, 5, "Nema");
		strcpy_s(_korisnickoIme, 5, "Nema");
		delete[] _lozinka; _lozinka = nullptr;
	}
};

class Komentar
{
	char* _tekst;
	Datum _datum;
	Vrijeme _satnica;
	int _popularnost;
	Korisnik* _autor;
public:
	//Potrebne konstruktor i destruktor funkcije
	Komentar() {
		_tekst = new char[5];
		strcpy_s(_tekst, 5, "Nema");
		_popularnost = 0;
	}
	Komentar(Vrijeme vrijeme, Datum datum, Korisnik korisnik, int popularnost = 0) :_satnica(vrijeme), _datum(datum), _autor(new Korisnik(korisnik)) {
		char temp[200];
		cout << "Unesite tekst komentara: ";
		cin.getline(temp, 200);
		_tekst = new char[strlen(temp) + 1];
		strcpy_s(_tekst, strlen(temp) + 1, temp);
		_popularnost = popularnost;
	}
	Komentar(const Komentar & original) {
		_tekst = new char[strlen(original._tekst) + 1];
		strcpy_s(_tekst, strlen(original._tekst) + 1, original._tekst);
		_datum = original._datum;
		_satnica = original._satnica;
		_popularnost = original._popularnost;
		_autor = new Korisnik(*original._autor);
	}
	void IzmjeniPopularnost(int vrijednost) {
		_popularnost = vrijednost;
	}
	void Info() {
		cout << crt << "Komentar: " << _tekst << endl;
		cout << "Datum: "; _datum.Info();
		cout << "Satnica: "; _satnica.Info(false);
		cout << "Popularnost: " << _popularnost << endl;
	}
	Komentar & operator=(const Komentar & drugi) {
		delete[]this->_tekst; this->_tekst = new char[strlen(drugi._tekst) + 1];
		strcpy_s(this->_tekst, strlen(drugi._tekst) + 1, drugi._tekst);
		this->_datum = drugi._datum;
		this->_satnica = drugi._satnica;
		this->_popularnost = drugi._popularnost;
		this->_autor = nullptr;
		this->_autor = new Korisnik(*drugi._autor);
		return *this;
	}
	~Komentar() {
		delete[] _tekst; _tekst = nullptr;
		_popularnost = 0;
		delete _autor;
	}
};

class Clanak
{
	char _naslov[150];
	char* _sadrzaj;
	Datum _datum;
	Vrijeme _satnica;
	int _brojPregleda;
	char* _tagovi[10];
	char _oblast[100];
	Korisnik* _autor;
	Komentar* _komentari;
	int _brojKomentara;
public:

	Clanak(Datum datum, Vrijeme vrijeme, Korisnik korisnik, const char* naslov = "\t\tNema naslova", const char* oblast = "Nepoznato") : _datum(datum), _satnica(vrijeme), _autor(new Korisnik(korisnik)) {
		strcpy_s(_naslov, naslov);
		_sadrzaj = new char[11];
		strcpy_s(_sadrzaj, 11, "- PRAZNO -");
		_brojPregleda = 0;
		for (size_t i = 0; i < 10; i++)
			_tagovi[i] = nullptr;
		strcpy_s(_oblast, oblast);
		_komentari = nullptr;
		_brojKomentara = 0;
	}
	~Clanak() {
		delete[]_sadrzaj; _sadrzaj = nullptr;
		_brojPregleda = 0;
		for (size_t i = 0; i < 10; i++)
		{
			delete[]_tagovi[i]; _tagovi[i] = nullptr;
		}
		_autor = nullptr;
		_komentari = nullptr;
		_brojKomentara = 0;
	}

	bool PromijeniSadrzaj(const char* sadrzaj, Vrijeme novo, Korisnik autorizacija) {
		if (strcmp(this->_autor->getIme(), autorizacija.getIme()) != 0) { cout << "Greska pri autorizaciji!\n"; return false; }
		if (strcmp(this->_autor->getLozinku(), autorizacija.getLozinku()) != 0) { cout << "Greska pri autorizaciji!\n"; return false; }
		delete[]_sadrzaj; _sadrzaj = new char[strlen(sadrzaj) + 1];
		strcpy_s(_sadrzaj, strlen(sadrzaj) + 1, sadrzaj);
		_satnica = novo;
		return true;
	}
	void DodajKomentar(Komentar comment) {
		Komentar * temp = new Komentar[_brojKomentara + 1];
		for (size_t i = 0; i < _brojKomentara; i++)
		{
			temp[i] = _komentari[i];
		}
		temp[_brojKomentara] = comment;
		_komentari = temp;
		_brojKomentara++;
		cout << "Komentar uspjesno dodan!\n";
	}
	void Info() {
		cout << crt << _naslov << endl;
		cout << "Sadrzaj: " << _sadrzaj << endl;
		cout << "Datum: "; _datum.Info();
		cout << "Satnica: "; _satnica.Info(false);
		cout << "Broj pregleda: " << _brojPregleda << endl;
		cout << "Oblast: " << _oblast << endl;
		cout << "Podaci o autoru:\n"; _autor->Info();
		cout << "Komentari:\n";
		for (size_t i = 0; i < _brojKomentara; i++)
			_komentari[i].Info();
		cout << endl << crt;
	}
};

int main()
{

	Vrijeme sad(17, 23);
	Vrijeme test(17, 22);
	cout << sad.CompareTo(test) << endl;

	Datum danas(7, 8, 2018);

	Korisnik Ajdin("Ajdin Hukara", "_neurouz_", "aa");		// treci parametar je lozinka
	Korisnik Nidal("Nidal Nidalovic", "NIDO", "bb");
	Korisnik Munir("Munir Nurkic", "mUnJa_##", "cc");

	Ajdin.PromjenaLozinke("password");

	Komentar comment(sad, danas, Ajdin);
	Komentar com2(sad, danas, Nidal);
	Komentar com3(sad, danas, Munir);

	Clanak _clanak(danas, test, Ajdin, "\tObjektno Programiranje u C++", "Programiranje/Racunarstvo/IT");
	if (_clanak.PromijeniSadrzaj("Objektno programiranje u programskom jeziku C++ je idealna stvar za kreiranje objekata pomocu klasa. Naime, klase imaju dosta funkcionalnosti te je sa njima mnogo lakse programirati, jer je programer blizi realnom svijetu.", test, Ajdin)) {
		cout << "Sadrzaj uspjesno promijenjen!\n";
	}
	comment.IzmjeniPopularnost(7);
	com3.IzmjeniPopularnost(329);

	_clanak.DodajKomentar(comment);
	_clanak.DodajKomentar(com2);
	_clanak.DodajKomentar(com3);

	_clanak.Info();

	return 0;
}
