#include<iostream>
using namespace std;

const char* SpisakJela[5] = { "Pileci fileti", "Cevapi (5kom)", "Cevapi(10kom)", "Sudzukice    ", "Cheeseburger    " };
const char* SpisakPica[7] = { "Senzacija", "Coca-Cola", "Burn      ", "Red Bull", "Kafa      ", "Caj       ", "Kisela voda" };
float CijeneJela[5] = { 5, 3.5, 7, 4.5, 3.5 };
float CijenePica[7] = { 2,3,4.5, 5.5, 1.5, 2, 1.5 };

auto crt = "\n--------------------------------------------\n";

/*     Vježbe 1 - Principi OOP. klase, objekti.
Zadatak: Na osnovu opisa sistema prepoznati osnovne klase i atribute, kao i njihove relacije, te implementirati osnovne
metode koje omogucavaju kreiranje i
ispis narudzbi klijentu. Obavezno implementirati Get i Set metode.

Tema: e-Restoran

Namjena aplikacije e-Restoran jeste da omoguci kreiranje narudzbi klijentima restorana putem standardnih PC i mobilnih uredjaja.
U procesu kreiranja narudzbe neophodno je da klijent obavi registraciju gdje upisuje svoje osnovne podatke: ime, prezime,
kontakt broj i adresu.
Nakon uspjesne registracije klijenti mogu da kreiraju narudzbu na osnovu online ponude odabranog restorana.
Ponuda restorana se sastoji od jela i pica.
Uz svako jelo i pice se prikazuje kratak opis i cijena. Narudzba koju klijent kreira sadrzi najmanje jedno jelo ili pice,
ukupnu cijenu narudzbe i status isporuke.

*/

char* GetChar(char * temp) {
	char * novi = new char[strlen(temp) + 1];
	strcpy_s(novi, strlen(temp) + 1, temp);
	return novi;
}

class Klijent
{
private:
	char * imePrezime;
	char * adresa;
	char brojTelefona[10];
public:
	Klijent() {
		imePrezime = new char[13];
		strcpy_s(imePrezime, 13, "-> Nepoznato");
		adresa = new char[13];
		strcpy_s(adresa, 13, "-> Nepoznato");
		strcpy_s(brojTelefona, "000000000");
	}
	~Klijent() {
		delete[]imePrezime; imePrezime = nullptr;
		delete[]adresa; adresa = nullptr;
	}
	void SetPodatke() {
		char tempImePrezime[100];
		cout << "Unesite ime i prezime: ";
		cin.getline(tempImePrezime, 100);
		imePrezime = GetChar(tempImePrezime);
		char tempAdresa[100];
		cout << "Unesite vasu adresu: ";
		cin.getline(tempAdresa, 100);
		adresa = GetChar(tempAdresa);
		cout << "Unesite broj telefona: ";
		cin.getline(brojTelefona, 10);
	}
	void GetPodatke() {
		cout << crt << "PODACI KLIJENTA:" << crt;
		cout << "\tIme i prezime:\t" << imePrezime << endl;
		cout << "\tAdresa:\t\t" << adresa << endl;
		cout << "\tKontakt tel:\t" << brojTelefona << endl;
	}
};

class Jelo
{
private:
	char * naziv;
	float cijena;
public:
	Jelo() {
		naziv = new char[10];
		strcpy_s(naziv, 10, "Nepoznato");
		cijena = 0.0;
	}
	~Jelo() {
		delete[] naziv; naziv = nullptr;
		cijena = 0.0;
	}
	Jelo(const Jelo & original) {
		naziv = new char[strlen(original.naziv) + 1];
		strcpy_s(naziv, strlen(original.naziv) + 1, original.naziv);
		cijena = original.cijena;
	}
	void DodajJelo(char * n, float c) {
		delete[] naziv;
		naziv = GetChar(n);
		cijena = c;
	}
	void Info() {
		cout << naziv << "\t-\t" << cijena << " KM" << endl;
	}
	Jelo operator= (const Jelo & drugo) {
		delete[]this->naziv;
		this->naziv = new char[strlen(drugo.naziv) + 1];
		strcpy_s(this->naziv, strlen(drugo.naziv) + 1, drugo.naziv);
		this->cijena = drugo.cijena;
		return *this;
	}
	float GetCijenu() {
		return cijena;
	}
};


class Pice
{
private:
	char * naziv;
	float cijena;
public:
	Pice() {
		naziv = new char[10];
		strcpy_s(naziv, 10, "Nepoznato");
		cijena = 0.0;
	}
	~Pice() {
		delete[] naziv; naziv = nullptr;
		cijena = 0.0;
	}
	Pice(const Pice & original) {
		naziv = new char[strlen(original.naziv) + 1];
		strcpy_s(naziv, strlen(original.naziv) + 1, original.naziv);
		cijena = original.cijena;
	}
	void DodajPice(char * n, float c) {
		delete[] naziv;
		naziv = GetChar(n);
		cijena = c;
	}
	void Info() {
		cout << naziv << "\t-\t" << cijena << " KM" << endl;
	}
	Pice operator= (const Pice & drugo) {
		delete[]this->naziv;
		this->naziv = new char[strlen(drugo.naziv) + 1];
		strcpy_s(this->naziv, strlen(drugo.naziv) + 1, drugo.naziv);
		this->cijena = drugo.cijena;
		return *this;
	}
	float GetCijenu() { return cijena; }
};

class Restoran
{
private:
	char * nazivRestorana;
	Jelo _jela[5];
	Pice _pica[7];
public:
	Restoran(const char* naziv) {
		nazivRestorana = GetChar((char*)naziv);
		for (int i = 0; i < 5; i++)
			_jela[i].DodajJelo((char*)SpisakJela[i], CijeneJela[i]);
		for (int i = 0; i < 7; i++)
			_pica[i].DodajPice((char*)SpisakPica[i], CijenePica[i]);
	}
	~Restoran() {
		delete[] nazivRestorana; nazivRestorana = nullptr;
	}
	int PrikaziMenu(int & Jelo_Pice) {
		int odabir;
		do {
			system("CLS");
			cout << "Odaberite narudzbu: \n\t1 -> Jelo\n\t2 -> Pice\n\t3 -> Pregled narudzbe\n\t4 -> Izlaz\n\nVas odabir: ";
			cin >> odabir;
		} while (odabir < 1 || odabir > 4);
		if (odabir == 1) {
			system("CLS");
			cout << "\tJELA:" << crt;
			for (size_t i = 0; i < 5; i++) {
				cout << i + 1 << ". ";
				_jela[i].Info();
			}
			do {
				cout << "\nUnesite sta zelite pojesti: ";
				cin >> Jelo_Pice;
			} while (Jelo_Pice < 1 || Jelo_Pice > 5);
		}
		else if(odabir == 2){
			system("CLS");
			cout << crt << "\tPICA:" << crt;
			for (size_t i = 0; i < 7; i++) {
				cout << i + 1 << ". ";
				_pica[i].Info();
			}
			do {
				cout << "\nUnesite sta zelite popiti: ";
				cin >> Jelo_Pice;
			} while (Jelo_Pice < 1 || Jelo_Pice > 7);
		}
		else Jelo_Pice = -1;
		--Jelo_Pice;
		return odabir;
	}
	char* GetNazivRestorana(){
		return nazivRestorana;
	}
	Jelo * GetJela() {
		return _jela;
	}
	Pice * GetPica() {
		return _pica;
	}
};

class Narudzba
{
private:
	Klijent _klijent;
	Jelo * _jela;
	Pice * _pica;
	int brojJela;
	int brojPica;
public:
	Narudzba() {
		_klijent.SetPodatke();
		_jela = nullptr;
		_pica = nullptr;
		brojJela = 0;
		brojPica = 0;
	}
	~Narudzba() {
		_jela = nullptr;
		_pica = nullptr;
		brojJela = 0;
		brojPica = 0;
	}
	void GetPodatkeOKlijentu() {
		_klijent.GetPodatke();
	}
	void DodajJelo(Jelo novo) {
		Jelo * temp = new Jelo[brojJela + 1];
		for (size_t i = 0; i < brojJela; i++)
		{
			temp[i] = _jela[i];
		}
		temp[brojJela] = novo;
		_jela = temp;
		brojJela++;
	}
	void DodajPice(Pice novo) {
		Pice * temp = new Pice[brojPica + 1];
		for (size_t i = 0; i < brojPica; i++)
		{
			temp[i] = _pica[i];
		}
		temp[brojPica] = novo;
		_pica = temp;
		brojPica++;
	}
	float GetTroskove() {
		float ukupno = 0;
		for (size_t i = 0; i < brojJela; i++)
			ukupno += _jela[i].GetCijenu();
		for (size_t i = 0; i < brojPica; i++)
			ukupno += _pica[i].GetCijenu();
		return ukupno;
	}
	void GetPodatkeONarudzbi() {
		cout << crt << "Jela: " << crt;
		for (size_t i = 0; i < brojJela; i++)
		{
			cout << i + 1 << ". ";  _jela[i].Info();
		}
		cout << "Pica: " << crt;
		for (size_t i = 0; i < brojPica; i++)
		{
			cout << i + 1 << ". ";  _pica[i].Info();
		}
		cout << "\nUkupno: " << GetTroskove() << "KM\n";
	}
};

#include <windows.h>

void Timer(int sec) {
	cout << endl;
	for (size_t i = sec; i > 0; i--)
	{
		cout << i;
		Sleep(1000);
		cout << "\b";
	}
}

int main()
{
	Narudzba _narudzba;
	Restoran BosanskaAvlija("Bosanska Avlija");
	int indeks = 1;
	int odabir = BosanskaAvlija.PrikaziMenu(indeks);
	while (odabir != 4) {
		if (odabir == 1) {
			_narudzba.DodajJelo(BosanskaAvlija.GetJela()[indeks]);
			cout << "Jelo uspjesno dodano!\nJelo koje ste odabrali je:\n";
			BosanskaAvlija.GetJela()[indeks].Info();
			Sleep(1500);
		}
		if (odabir == 2) {
			_narudzba.DodajPice(BosanskaAvlija.GetPica()[indeks]);
			cout << "Pice uspjesno dodano!\nPice koje ste odabrali je:\n";
			BosanskaAvlija.GetPica()[indeks].Info();
			Sleep(1500);
		}
		if (odabir == 3) {
			system("CLS");
			_narudzba.GetPodatkeOKlijentu();
			_narudzba.GetPodatkeONarudzbi();
			Timer(5);
		}
		odabir = BosanskaAvlija.PrikaziMenu(indeks);
		if (odabir == -1) break;
	}
	cout << "\n\t Hvala vam sto ste koristili usluge restorana " << BosanskaAvlija.GetNazivRestorana() << endl;
	return 0;
}
