#include <iostream>
#include <string>
using namespace std;

int first_id = 0;
#define endline cout<<endl;
#define endlines cout<<endl<<endl;

template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	~Kolekcija()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	void DodajElemente(const T1& el1, const T2& el2)
	{
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}

	int GetTrenutno() const { return _trenutno; }
	T1 * GetElemente1() const { return _elementi1; }
	T2 * GetElemente2() const { return _elementi2; }

	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T1, class T2>
ostream& operator<<(ostream& cout, const Kolekcija<T1, T2>& k)
{
	for (int i = 0; i < k._trenutno; i++)
	{
		cout << k._elementi1[i];
	}

	return cout;
}

//U programu implementirati nasljedivanje izmedu odgovarajucih klasa
//Klasu Osoba deklarisati kao apstraktnu.
class Osoba
{
	char* _ime;
	char* _prezime;
	char* _adresa;
	char _telefon[15];
public:
	Osoba(const char* ime, const char* prezime, const char* adresa, const char* telefon)
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		vel = strlen(adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, adresa);

		strcpy_s(_telefon, 15, telefon);
	}

	virtual ~Osoba()
	{
		delete[] _ime;
		_ime = nullptr;
		delete[] _prezime;
		_prezime = nullptr;
		delete[] _adresa;
		_adresa = nullptr;
	}
	virtual bool IsPotrosnjaVeca(int mjesec, double potrosnja) = 0;
	virtual char* GetIme() const final { return _ime; }
	virtual char* GetPrezime() const final { return _prezime; }

	virtual void Info()
	{
		cout << "\n--------------- Licni Podaci --------------\n\n";
		cout << "\tIme i prezime: " << _ime << " " << _prezime << endl;
		cout << "\tAdresa: " << _adresa << endl;
		cout << "\tTelefon: " << _telefon << endl;
	}
};

class Proizvod
{
	char* _naziv;
	char* _opis;
	double _cijena;
public:
	Proizvod(const char* naziv, const char* opis, double cijena)
	{
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);

		vel = strlen(opis) + 1;
		_opis = new char[vel];
		strcpy_s(_opis, vel, opis);

		_cijena = cijena;
	}
	Proizvod() {}
	Proizvod(const Proizvod & original) {
		_naziv = new char[strlen(original._naziv) + 1];
		strcpy_s(_naziv, strlen(original._naziv) + 1, original._naziv);
		_opis = new char[strlen(original._opis) + 1];
		strcpy_s(_opis, strlen(original._opis) + 1, original._opis);
		_cijena = original._cijena;
	}
	Proizvod & operator=(const Proizvod & right) {
		if (this != &right) {
			this->_naziv = new char[strlen(right._naziv) + 1];
			strcpy_s(this->_naziv, strlen(right._naziv) + 1, right._naziv);
			this->_opis = new char[strlen(right._opis) + 1];
			strcpy_s(this->_opis, strlen(right._opis) + 1, right._opis);
			this->_cijena = right._cijena;
		}
		return *this;
	}
	~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;

		delete[] _opis;
		_opis = nullptr;
	}
	double GetCijenu() const { return _cijena; }
	char * GetNaziv() const { return _naziv; }

	friend ostream& operator<<(ostream&, const Proizvod&);
	friend ostream& operator<<(ostream&, const Proizvod*);

	void SetCijenu(double novaCijena) { _cijena = novaCijena; }
};

class Narudzba {
	char _datum[12]; //Format: dd.mm.yyyy.
	Kolekcija<Proizvod*, int> _proizvodi; //Podaci o narucenim proizvodima i kolicinama
public:
	//Potrebne osnovne funkcije za rad sa klasom
	Narudzba(const char * datum) {
		strcpy_s(_datum, 12, datum);
	}
	Narudzba() {}
	void DodajProizvod(const Proizvod & proizvod, int kolicina) {
		_proizvodi.DodajElemente(new Proizvod(proizvod), kolicina);
	}
	Narudzba(const Narudzba & original) {
		strcpy_s(_datum, 12, original._datum);
		for (size_t i = 0; i < original.GetTrenutnoProizvoda(); i++)
			_proizvodi.DodajElemente(original._proizvodi.GetElemente1()[i], original._proizvodi.GetElemente2()[i]);
	}
	Narudzba & operator=(const Narudzba & right) {
		if (this != &right) {
			strcpy_s(this->_datum, 12, right._datum);
			for (size_t i = 0; i < right.GetTrenutnoProizvoda(); i++)
				this->_proizvodi.DodajElemente(right._proizvodi.GetElemente1()[i], right._proizvodi.GetElemente2()[i]);
		}
		return *this;
	}
	//Funkciju GetIznosNarudzbe koja vraca ukupan iznos narudžbe na osnovu
	//podataka o cijenama proizvoda i narucenim kolicinama. Funkcija prima neobavezni parametar popust
	//koji inicijalno ima vrijednost 0, što znaci da ne postoji popust na postojece cijene proizvoda.
	double GetIznosNarudzbe(float popust = 0) {
		double suma = 0;
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			suma += _proizvodi.GetElemente1()[i]->GetCijenu() * _proizvodi.GetElemente2()[i];
		}
		return suma - popust;
	}
	//Funkciju za ispis svih vrijednosti atributa klase.
	void Ispis() {
		cout << "Datum narudzbe: " << _datum << endl;
		cout << _proizvodi;
	}
	int GetTrenutnoProizvoda() const { return _proizvodi.GetTrenutno(); }
	int GetMjesec() {
		string temp = "";
		int tmpBrojac = 0;
		for (size_t i = 0; i < strlen(_datum); i++)
		{
			if (_datum[i] == '.') break;
			else {
				temp += _datum[i];
			}
		}
		return stoi(temp);
	}
	friend ostream & operator<<(ostream & cout, const Narudzba & n) {
		cout << "\tDatum narudzbe: " << n._datum << endl;
		cout << "\nProizvodi:\n" << n._proizvodi << endl;
		return cout;
	}
};

ostream& operator<<(ostream& cout, const Proizvod& p)
{
	cout << "Naziv: " << p._naziv << "\tCijena: " << p._cijena << endl;
	return cout;
}
ostream& operator<<(ostream& cout, const Proizvod* p)
{
	cout << "Naziv: " << p->_naziv << "\tCijena: " << p->_cijena << endl;
	return cout;
}

class Klijent : public Osoba
{
	int _id;
	char* _email;
	Kolekcija<Narudzba, double> _narudzbe; //Podaci o narudžbama sa ukupnim utrošenim iznosom u KM
public:
	//Potrebne konstruktor i destruktor funkcije.
	Klijent(const char * email,
		const char * ime,
		const char * prezime,
		const char * telefon,
		const char * adresa)
		:_id(++first_id),
		_email(new char[strlen(email) + 1]),
		Osoba(ime, prezime, adresa, telefon)
	{
		strcpy_s(_email, strlen(email) + 1, email);
	}

	Kolekcija<Narudzba, double>& GetNarudzbe() { return _narudzbe; }

	//Funkciju za evidenciju nove narudžbe.
	void DodajNarudzbu(const Narudzba & narudzba, double iznos) {
		_narudzbe.DodajElemente(narudzba, iznos);
	}
	//Funkciju koja vraca logicku vrijednost true ukoliko je klijent za odabrani mjesec u godini imao
	//potrošnju vecu od zadate. Prototip funkcije: bool IsPotrosnjaVeca(int mjesec, double potrosnja);
	//Za pronalazak potrosnje po mjesecima koristiti podatak o datumu narudžbe. 
	bool IsPotrosnjaVeca(int mjesec, double potrosnja) override {
		int indeks = -1;
		for (size_t i = 0; i < _narudzbe.GetTrenutno(); i++)
		{
			if (_narudzbe.GetElemente1()[i].GetMjesec() == mjesec) {
				indeks = i;
				break;
			}
		}
		if (indeks == -1) {
			return false;
		}
		return _narudzbe.GetElemente1()[indeks].GetIznosNarudzbe() > potrosnja;
	}
	//Funkciju za izmjenu e-mail adrese.
	void IzmjenaEmailAdrese(const char* email) {
		delete[] _email;
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);
	}
	Narudzba & GetPosljednjuNarudzbu() const {
		return _narudzbe.GetElemente1()[_narudzbe.GetTrenutno() - 1];
	}
	//Funkciju za ispis svih vrijednosti atributa klase.
	void Info() override {
		Osoba::Info();
		cout << "\n------------ Podaci o narudzbi ------------\n\n";
		cout << "\t-> ID Kupca: " << _id << endl;
		cout << "\t-> E-mail: " << _email << endl << endl;
		cout << _narudzbe << endl;
	}
	~Klijent() {
		delete[] _email; _email = nullptr;
	}
	int GetBrojNarudzbi() const { return _narudzbe.GetTrenutno(); }
	void GenerisiEmail(const char * nastavak = "edu.fit.ba") {
		char tempmail[40];
		for (size_t i = 0; i < strlen(_email); i++)
		{
			if (_email[i] == '@') {
				tempmail[i] = '@';
				int brojac = i + 1;
				int brojacNastavka = 0;
				while (brojacNastavka < strlen(nastavak)) {
					tempmail[brojac++] = nastavak[brojacNastavka++];
				}
				tempmail[brojacNastavka + i + 1] = '\0';
				break;
			}
			else {
				tempmail[i] = _email[i];
			}
		}
		if (_email != nullptr) {
			delete[] _email;
			_email = new char[strlen(tempmail) + 1];
			strcpy_s(_email, strlen(tempmail) + 1, tempmail);
		}
		else {
			_email = new char[strlen(tempmail) + 1];
			strcpy_s(_email, strlen(tempmail) + 1, tempmail);
		}
	}
	char * GetEmail() const { return _email; }
};

class VIPKlijent : public Klijent
{
	char* _parkingOznaka;
	double _popust;
public:
	//Potrebne konstruktor i destruktor funkcije
	VIPKlijent(const char * email,
		const char * ime,
		const char * prezime,
		const char * telefon,
		const char * adresa,
		const char * parkingOznaka,
		double popust) : _popust(popust), _parkingOznaka(new char[strlen(parkingOznaka) + 1]),
		Klijent(email, ime, prezime, telefon, adresa)
	{
		strcpy_s(_parkingOznaka, strlen(parkingOznaka) + 1, parkingOznaka);
	}
	//Funkciju za evidenciju nove narudžbe sa zaracunatim popustom na sve proizvode.
	void DodajNarudzbuVIP(const Narudzba & n, double iznos) {
		Klijent::DodajNarudzbu(n, iznos * _popust / 100);
	}
	double GetPotrosnju()const {
		int x = Klijent::GetPosljednjuNarudzbu().GetIznosNarudzbe() * _popust / 100;
		return Klijent::GetPosljednjuNarudzbu().GetIznosNarudzbe() - x;
	}
	//Funkciju koja posljednjoj narudžbi klijenta dodaje gratis proizvod proslijeden kao parametar.
	void DodajProizvodGratis(Proizvod gratis) {
		gratis.SetCijenu(0);
		if (Klijent::GetBrojNarudzbi() == 0) {
			Klijent::GetNarudzbe().GetElemente1()->DodajProizvod(gratis, 1);
		}
		else
			Klijent::GetPosljednjuNarudzbu().DodajProizvod(gratis, 1);
	}
	//Funkciju za ispis svih vrijednosti atributa klase
	virtual void Info() {
		cout << "\n--------------- VIP KLIJENT ---------------\n\n";
		cout << "-> Parking oznaka: " << _parkingOznaka << endl;
		cout << "-> Popust: " << _popust << "%" << endl;
		Klijent::Info();
	}
	int GetPopust() const { return _popust; }

	void InfoVIP() {
		for (size_t i = 0; i < Klijent::GetBrojNarudzbi(); i++)
		{
			cout << Klijent::GetNarudzbe();
		}
	}
};

//Funkcija treba da pronade sve one klijente koji su za odabrani mjesec ostvarili potrošnju vecu
//od zadate. Ispisati njihove podatke i vratiti ukupan broj pronadenih klijenata.
int PronadjiKlijentePoPotrosnji(Osoba* niz[], int max, double zadanaPotrosnja, int mjesec) {
	int brojKlijenata = 0;
	for (size_t i = 0; i < max; i++)
		if (niz[i]->IsPotrosnjaVeca(mjesec, zadanaPotrosnja)) brojKlijenata++;
	return brojKlijenata;
}

//Pronaci sve VIP klijente, te im dodati gratis proizvod proslijeden kao parametar.
void UkljuciGratisProizvodVIPKlijentima(Osoba* niz[], int max, const Proizvod& p) {

	for (size_t i = 0; i < max; i++)
	{
		VIPKlijent * o = dynamic_cast<VIPKlijent*>(niz[i]);
		if (o != nullptr) {
			o->DodajProizvodGratis(p);
			cout << "VIP Klijentu " << niz[i]->GetIme() << " dodan gratis proizvod " << p.GetNaziv() << endl;
		}
	}
}

//Svim klijentima postaviti mail adresu u formatu: ime.prezime@firma.ba.
void GenerisiMailAdreseKlijentima(Osoba* niz[], int max) {
	for (size_t i = 0; i < max; i++)
	{
		Klijent * k = dynamic_cast<Klijent*>(niz[i]);
		if (k != nullptr) {
			k->GenerisiEmail();
			cout << "E-mail uspjesno generisan -> " << k->GetEmail() << endl;
		}
	}
}

//Omoguciti pretragu klijenata po imenu ili prezimenu (ispisati informacije
//o svim klijentima koji sadrže uneseni tekst u dijelu imena ili prezimena.
void PronadjiKlijentePoImenuPrezimenu(Osoba* niz[], int max, const char * imePrezime)
{
	bool pronadjen = false;
	for (size_t i = 0; i < max; i++)
	{
		if ((strstr(niz[i]->GetIme(), imePrezime) != nullptr) ||
			(strstr(niz[i]->GetPrezime(), imePrezime) != nullptr))
		{
			cout << "Pronadjena osoba: " << niz[i]->GetIme() << " " << niz[i]->GetPrezime() << endl;
			pronadjen = true;
		}
	}
	if (!pronadjen) { cout << "Ne postoji takva osoba!\n"; }
}

int main()
{
	//Testirati sve dostupne funkcionalnosti uz obaveznu provjeru globalnih funkcija.
	//VIP klijentima sve narudžbe kreirati uz zaracunati popust pozivom odgovarajuce funkcije.

#pragma region Testiranje Kolekcije
	Kolekcija<int, double> k1;
	k1.DodajElemente(74, 22);
	k1.DodajElemente(75, 22.7);
	k1.DodajElemente(76, 233.31);
	k1.DodajElemente(747, 2.414);
	cout << k1 << endl;

	Proizvod majoneza("Majoneza", "Stetno", 2.70);
	Proizvod salama("Salama", "Stetno", 3.85);
	Proizvod kuhalo("Kuhalo", "Za kafu", 69.99);
	Proizvod telefon("Samsung", "Galaxy S9 Plus", 914.99);
	Proizvod casa("Casa", "Plasticna", 1.85);
	Proizvod patike("Nike", "Air Max Turbo Fast 2.0 TDI", 150.00);
	Proizvod brzeCvike("Brzzze", "Samo brzina", 13.90);
	Proizvod zvucnici("KRK", "Rokit 10 (par)", 1099.99);
	Proizvod flasa("Flasa", "Staklena", 0.75);

	Kolekcija<int, Proizvod> k2;
	k2.DodajElemente(1, majoneza);
	k2.DodajElemente(2, salama);
	k2.DodajElemente(3, kuhalo);
	cout << k2 << endl;
#pragma endregion


#pragma region Testiranje Narudzbe
	Narudzba narudzba("1.10.2018");
	cout << "-> Trenutno proizvoda: " << narudzba.GetTrenutnoProizvoda() << endl;
	cout << "-> Iznos narudzbe: " << narudzba.GetIznosNarudzbe() << endl;
	narudzba.DodajProizvod(majoneza, 5);
	narudzba.DodajProizvod(salama, 7);
	narudzba.DodajProizvod(kuhalo, 2);
	cout << "-> Trenutno proizvoda: " << narudzba.GetTrenutnoProizvoda() << endl;
	cout << "-> Iznos narudzbe: " << narudzba.GetIznosNarudzbe() << endl;
	cout << "Mjesec: "; narudzba.GetMjesec();
	cout << endl << endl;
#pragma endregion


#pragma region Testiranje Klijenta
	Klijent klijent1("ajdin.hukara@gmail.com", "Ajdin", "Hukara", "061/550-135", "Adresa 5");
	klijent1.DodajNarudzbu(narudzba, 10);
	klijent1.Info();
	if (klijent1.IsPotrosnjaVeca(10, 181)) cout << "Potrosnja je veca!\n";
	else cout << "Potrosnja je manja!\n";
	klijent1.IzmjenaEmailAdrese("neurouzmedia@gmail.com");
	klijent1.Info();
#pragma endregion

#pragma region Testiranje VIP Klijenta
	VIPKlijent klijentVIP("nerma.curic@gmail.com", "Nerma", "Curic", "062/611-193", "Adresa 7", "C8", 17);
	klijentVIP.DodajNarudzbuVIP(narudzba, 70);
	Proizvod _gratis("Bicikl", "Sportski", 199.90);
	klijentVIP.DodajProizvodGratis(_gratis);
	klijentVIP.Info();
	cout << "Posljednja narudzba (" << klijentVIP.GetPopust() << "% popust) ------> " << klijentVIP.GetPotrosnju() << " KM\n\n";
#pragma endregion

#pragma region Testiranje Funkcija
	Narudzba n1("14.10.2018.");
	Narudzba n2("12.10.2018.");

	n1.DodajProizvod(zvucnici, 7);		n2.DodajProizvod(patike, 3);
	n1.DodajProizvod(salama, 15);		n2.DodajProizvod(flasa, 500);
	n1.DodajProizvod(brzeCvike, 80);	n2.DodajProizvod(telefon, 20);

	Klijent prvi("denis.m@gmail.com", "Denis", "Music", "061/107-994", "Adresa br. 5");
	prvi.DodajNarudzbu(narudzba, 1);
	Klijent drugi("indira@hotmail.com", "Indira", "Hamulic", "033/711-211", "Adresa br. 4");
	drugi.DodajNarudzbu(n1, 3);
	VIPKlijent treci("jasmin.azemovic@edu.fit.ba", "Jasmin", "Azemovic", "061/201-747", "Adresa br. 3", "D11", 15);
	treci.DodajNarudzbu(n2, 2);
	Klijent cetvrti("adel_cybersecurity@w.cn", "Adel", "Handzic", "063/515-001", "Adresa br. 2");
	VIPKlijent peti("amila_h@gmail.com", "Amila", "Handzic", "033/991-171", "Adresa br. 1", "B01", 4.5);
	peti.DodajNarudzbu(n1, 87);

	Osoba * _nizOsoba[5];

	_nizOsoba[0] = &prvi;
	_nizOsoba[1] = &drugi;
	_nizOsoba[2] = &treci;
	_nizOsoba[3] = &cetvrti;
	_nizOsoba[4] = &peti;

	int brojKlijenata = PronadjiKlijentePoPotrosnji(_nizOsoba, 5, 20, 10);
	cout << "-- Broj klijenata koji su ostvarili potrosnju vecu od 20KM -> " << brojKlijenata << endl;
	UkljuciGratisProizvodVIPKlijentima(_nizOsoba, 5, _gratis);
	_nizOsoba[4]->Info();
	_nizOsoba[3]->Info();
	_nizOsoba[2]->Info();
	_nizOsoba[1]->Info();
	_nizOsoba[0]->Info();
	GenerisiMailAdreseKlijentima(_nizOsoba, 5);
	endline;
	PronadjiKlijentePoImenuPrezimenu(_nizOsoba, 5, "Ajda");
	PronadjiKlijentePoImenuPrezimenu(_nizOsoba, 5, "Denis");

#pragma endregion


	return 0;
}
