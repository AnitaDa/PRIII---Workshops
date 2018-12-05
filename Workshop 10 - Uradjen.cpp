#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

auto crt = "------------------------------------------------------------------------------\n";
auto crtj = "==============================================================================\n";

char* AlocirajNizKaraktera(const char* t) {
	if (t == nullptr) return nullptr;
	char* n = new char[strlen(t) + 1];
	strcpy_s(n, strlen(t) + 1, t);
	return n;
}

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
protected:
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
		//U slucaju nedozvoljenih vrijednosti za cijenu, popust i kolicinu baciti izuzetak
		if (popust < 0) throw NedozvoljenaOperacija("\tERROR :: Popust?", __LINE__);
		if (cijena < 0) throw NedozvoljenaOperacija("\tERROR :: Cijena?", __LINE__);
		if (kolicina < 0) throw NedozvoljenaOperacija("\tERROR :: Kolicina?", __LINE__);

		strncpy_s(_sifra, sifra, _TRUNCATE);

		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);

		_cijena = cijena;
		_kolicina = kolicina;
		_popust = popust;
	}
	Proizvod(const Proizvod& p) {
		strncpy_s(_sifra, p._sifra, _TRUNCATE);
		_naziv = AlocirajNizKaraktera(p._naziv);
		_cijena = p._cijena;
		_popust = p._popust;
		_kolicina = p._kolicina;
	}
	Proizvod& operator=(const Proizvod& right) {
		if (this != &right) {
			strncpy_s(_sifra, right._sifra, _TRUNCATE);
			if (_naziv != nullptr) delete[] _naziv;
			_naziv = AlocirajNizKaraktera(right._naziv);
			_cijena = right._cijena;
			_popust = right._popust;
			_kolicina = right._kolicina;
		}
		return *this;
	}

	virtual ~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}

	// Getteri
	char* GetNaziv() const { return _naziv; }
	const char* GetSifru() const { return _sifra; }
	int GetKolicinu() const { return _kolicina; }
	double GetCijenu() const { return _cijena; }
	int GetPopust() const { return _popust; }

	//Implementirati potrebne funkcije za rad sa klasom
	bool operator==(const Proizvod& right) {
		return strcmp(_sifra, right._sifra) == 0;
	}
	friend ostream& operator<<(ostream& cout, const Proizvod& p) {
		return cout << "Proizvod - Sifra: " << p._sifra << "\tNaziv: " << p._naziv << "\tCijena: " << p._cijena << "\tKolicina: " << p._kolicina << endl;
	}
};

//Implementirati nasljedivanje
class Odjeca : public Proizvod
{
	char* _velicine;
	char* _boje;
	char _kategorija[50];
public:
	//Potrebne konstruktor i destruktor funkcije
	Odjeca(const char* sifra, const char* naziv, double cijena, int kolicina,
		const char* velicine, const char* boje, const char* kategorija, double popust = 0) : Proizvod(sifra, naziv, cijena, kolicina, popust) {
		_velicine = AlocirajNizKaraktera(velicine);
		_boje = AlocirajNizKaraktera(boje);
		strncpy_s(_kategorija, kategorija, _TRUNCATE);
	}
	virtual ~Odjeca() {
		delete[] _velicine;
		_velicine = nullptr;
		delete[] _boje;
		_boje = nullptr;
	}
	Odjeca(const Odjeca& o) : Proizvod(o._sifra, o._naziv, o._cijena, o._kolicina, o._popust) {
		_velicine = AlocirajNizKaraktera(o._velicine);
		_boje = AlocirajNizKaraktera(o._boje);
		strncpy_s(_kategorija, o._kategorija, _TRUNCATE);
	}
	Odjeca& operator=(const Odjeca& right)
	{
		if (this != &right) {
			strncpy_s(_sifra, right._sifra, _TRUNCATE);
			if (_naziv != nullptr) delete[] _naziv;
			_naziv = AlocirajNizKaraktera(right._naziv);
			_cijena = right._cijena;
			_popust = right._popust;
			_kolicina = right._kolicina;
			if (_velicine != nullptr) delete[] _velicine;
			_velicine = AlocirajNizKaraktera(right._velicine);
			if (_boje != nullptr) delete[] _boje;
			_boje = AlocirajNizKaraktera(right._boje);
			strncpy_s(_kategorija, right._kategorija, _TRUNCATE);
		}
		return *this;
	}
	const char* GetSifru() const { return Proizvod::_sifra; }

	//Implementirati potrebne funkcije za rad sa klasom
	friend ostream& operator<<(ostream& cout, const Odjeca& p) {
		return cout << "Odjeca:\t"<< p._sifra << " - Naziv: " << p._naziv << " - Cijena: " << p._cijena << " - Kolicina: " << p._kolicina << endl;
	}
};

class Katalog
{
	char* _naslov;
	char* _vrijediDo; //Datum u formatu dd.mm.yyyy
	Proizvod* _proizvodi[100];
	int _trenutnoProizvoda;

	bool ValidanDatum(const char* testDatum) {
		int count = 0;
		for (size_t i = 0; i < strlen(testDatum); i++)
		{
			if (testDatum[i] == '.') count++;
		}
		if (count != 3) throw NedozvoljenaOperacija("\tERROR :: Datum nije validan", __LINE__);
	}
	bool VecPostoji(const char* sifra) {
		for (int i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(_proizvodi[i]->GetSifru(), sifra) == 0) 
				return true;
		}
		return false;
	}
	int GetProizvod(const char* sifra) {
		for (int i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(_proizvodi[i]->GetSifru(), sifra) == 0)
				return i;
		}
		return -1;
	}

public:
	//Potrebne konstruktor i destruktor funkcije
	Katalog(const char* naslov, const char* vrijediDo)
	{
		//U slucaju da datum ne ispunjava zadati format baciti izuzetak
		try {
			ValidanDatum(vrijediDo);
		}
		catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

		_naslov = AlocirajNizKaraktera(naslov);

		_vrijediDo = AlocirajNizKaraktera(vrijediDo);

		for (size_t i = 0; i < 100; i++)
		{
			_proizvodi[i] = nullptr;
		}

		_trenutnoProizvoda = 0;
	}

	virtual ~Katalog()
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
	//Onemoguciti dodavanje identicnih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
	bool DodajProizvod(Proizvod& p) {
		cout << "\n\tDodavanje proizvoda " << p.GetNaziv() << "..." << endl;
		if (VecPostoji(p.GetSifru())) {
			cout << "Proizvod vec postoji u katalogu!\n";
			return false;
		}

		Odjeca* pointer = dynamic_cast<Odjeca*>(&p);
		if (pointer != nullptr) {
			cout << "-> Dodani proizvod je Odjeca!\n";
			_proizvodi[_trenutnoProizvoda++] = new Odjeca(*pointer);
		}
		else {
			cout << "-> Dodani proizvod je klasicni proizvod!\n";
			_proizvodi[_trenutnoProizvoda++] = new Proizvod(p);
			return true;
		}
		return true;
	}

	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre.
	//Ukoliko se ne pronade traženi proizvod baciti izuzetak.
	bool UkloniProizvod(const char* sifra) {
		int indeks = GetProizvod(sifra);
		if (indeks == -1) {
			cout << "Proizvod ne postoji u bazi!\n";
			return false;
		}
		delete _proizvodi[indeks];
		for (int i = indeks; i < _trenutnoProizvoda - 1; i++)
			_proizvodi[i] = _proizvodi[i + 1];
		_proizvodi[_trenutnoProizvoda] = nullptr;
		_trenutnoProizvoda--;
		cout << "Proizvod uspjesno uklonjen!\n";
		return true;
	}

	//Preklopiti operator za ispis kataloške ponude
	friend ostream& operator<<(ostream& cout, const Katalog& k) {
		cout << "\n\t\t\t:: KATALOSKA PONUDA ::" << endl << endl;
		for (int i = 0; i < k._trenutnoProizvoda; i++) {
			Odjeca* pointer = dynamic_cast<Odjeca*>(k._proizvodi[i]);
			if (pointer != nullptr) cout << i+1 << ": " << *pointer;
			else cout << i + 1 << ": " << *k._proizvodi[i];
		}
		return cout;
	}
};


class Narudzba
{
	int _brojNarudzbe;
	char* _datumIsporuke; //Datum u formatu dd.mm.yyyy
	Proizvod* _naruceniProizvodi[100];
	int _brojProizvoda;
	bool otkazana;

	bool ValidanDatum(const char* testDatum) {
		int count = 0;
		for (size_t i = 0; i < strlen(testDatum); i++)
		{
			if (testDatum[i] == '.') count++;
		}
		if (count != 3) throw NedozvoljenaOperacija("\tERROR :: Datum nije validan", __LINE__);
	}
	bool Postoji(const char* sifra) {
		for (int i = 0; i < _brojProizvoda; i++)
			if (strcmp(_naruceniProizvodi[i]->GetSifru(), sifra) == 0) return true;
		return false;
	}

public:
	//Potrebne konstruktor i destruktor funkcije
	//U slucaju da datum ne ispunjava zadati format baciti izuzetak
	Narudzba(int brojNarudzbe = 0, const char* datum = " ") {
		_brojNarudzbe = brojNarudzbe;
		_datumIsporuke = AlocirajNizKaraktera(datum);
		for (int i = 0; i < 100; i++)
			_naruceniProizvodi[i] = nullptr;
		_brojProizvoda = 0;
		otkazana = false;
	}
	Narudzba(const Narudzba& original) {
		_brojNarudzbe	= original._brojNarudzbe;
		_datumIsporuke	= AlocirajNizKaraktera(original._datumIsporuke);
		_brojProizvoda	= original._brojProizvoda;

		for (int i = 0; i < 100; i++){

			if (i < _brojProizvoda) {

				Odjeca* p = dynamic_cast<Odjeca*>(original._naruceniProizvodi[i]);
				if(p != nullptr)	_naruceniProizvodi[i] = new Odjeca(*p);
				else				_naruceniProizvodi[i] = new Proizvod(*original._naruceniProizvodi[i]);

			}
			else _naruceniProizvodi[i] = nullptr;
		}
		otkazana = original.otkazana;
	}
	Narudzba& operator=(const Narudzba& right) {
		if (this != &right) {
			_brojNarudzbe = right._brojNarudzbe;
			if (_datumIsporuke != nullptr) delete[] _datumIsporuke;
			_datumIsporuke = AlocirajNizKaraktera(right._datumIsporuke);
			_brojProizvoda = right._brojProizvoda;
			otkazana = right.otkazana;
			if (_brojProizvoda != 0) {
				for (int i = 0; i < _brojProizvoda; i++)
				{
					delete _naruceniProizvodi[i];
					_naruceniProizvodi[i] = nullptr;
				}
			}
			for (int i = 0; i < 100; i++)
			{
				if (i < _brojProizvoda) {
					Odjeca* p = dynamic_cast<Odjeca*>(right._naruceniProizvodi[i]);
					if (p != nullptr)	_naruceniProizvodi[i] = new Odjeca(*p);
					else				_naruceniProizvodi[i] = new Proizvod(*right._naruceniProizvodi[i]);
				}
				else _naruceniProizvodi[i] = nullptr;
			}
		}
		return *this;
	}

	//Getteri
	int GetBrojNarudzbe() const { return _brojNarudzbe; }

	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednosti tipa bool.
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.
	bool DodajProizvod(Proizvod& proizvod) {

		if (proizvod.GetKolicinu() == 0) throw NedozvoljenaOperacija("Proizvod vise nije dostupan!\n", __LINE__);

		Odjeca* p = dynamic_cast<Odjeca*>(&proizvod);
		if (p != nullptr)	_naruceniProizvodi[_brojProizvoda] = new Odjeca(*p);
		else				_naruceniProizvodi[_brojProizvoda] = new Proizvod(proizvod);

		_brojProizvoda++;
		cout << "Proizvod " << proizvod.GetNaziv() << " uspjesno dodan!\n";
		return true;
	}

	//Funkciju za uklanjanje odredenog proizvoda iz narudžbe. Voditi racuna da narudžba može 
	//imati više puta isti proizvod.
			/// Nisam bas shvatio, vjerovatno treba ukloniti sve proizvode koji su dodani vise puta...
	bool UkloniProizvod(const Proizvod& p) {
		if (!Postoji(p.GetSifru()))
		{
			cout << "\n\tNemoguce ukloniti! Proizvod nije pronadjen!\n";
			return false;
		}
		bool postoji = true;
		while (postoji) {
			postoji = false;
			for (int i = 0; i <= _brojProizvoda; i++)
			{
				if (strcmp(_naruceniProizvodi[i]->GetSifru(), p.GetSifru()) == 0) {
					delete _naruceniProizvodi[i];
					for (int j = i; j < _brojProizvoda; j++)
					{
						_naruceniProizvodi[i] = _naruceniProizvodi[i + 1];
					}
					_naruceniProizvodi[_brojProizvoda] = nullptr;
					postoji = true;
					cout << "\t\t\tPronadjen, obrisan!\n";
					_brojProizvoda--;
				}
			}
			
		}
		cout << "\t\t Trenutno proizvoda (nakon brisanja) :: " << _brojProizvoda << endl;
		return true;
	}

	//Dodati funkciju za proracun ukupne cijene narudžbe.
	//Voditi racuna o mogucnosti dodavanja više istih proizvoda (kolicina).
	//Uracunati popust u slucaju da je definisan.
	double GetUkupnuCijenu() const {
		double cijena = 0;
		vector<char*> _sifre;
		for (int i = 0; i < _brojProizvoda; i++)
		{
			int kolicina = 0;
			bool Postoji = false;
			for (int k = 0; k < _sifre.size(); k++)
			{
				if (strcmp(_naruceniProizvodi[i]->GetSifru(), _sifre[k]) == 0) Postoji = true;
			}
			if (!Postoji) {
				for (int j = i; j < _brojProizvoda; j++)
				{
					if (strcmp(_naruceniProizvodi[i]->GetSifru(), _naruceniProizvodi[j]->GetSifru()) == 0) kolicina+= _naruceniProizvodi[j]->GetKolicinu();
				}
				_sifre.push_back((char*)_naruceniProizvodi[i]->GetSifru());
			}
			double popust =
				_naruceniProizvodi[i]->GetPopust() * _naruceniProizvodi[i]->GetCijenu() * _naruceniProizvodi[i]->GetKolicinu() / 100;
			cijena += kolicina * _naruceniProizvodi[i]->GetCijenu() - popust;
		}
		return cijena;
	}

	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
	friend ostream& operator<<(ostream& cout, const Narudzba& n) {
		string otkazana = n.Otkazana() ? " [OTKAZANO] " : "";
		cout <<"\t\tBroj narudzbe: " << n._brojNarudzbe << "\tDatum: " << n._datumIsporuke << otkazana << endl << crt;
		for (int i = 0; i < n._brojProizvoda; i++)
			cout << *n._naruceniProizvodi[i];

		cout << crt << "Ukupno [sa popustom]: " << n.GetUkupnuCijenu() << "KM" << endl << crt;
		return cout;
	}

	//Dodatne funkcije
	void SetOtkazano() { otkazana = true; }
	bool Otkazana() const { return otkazana; }
	int GetTrenutno() const { return _brojProizvoda; }
	Proizvod& GetProizvod(int indeks) const { return *_naruceniProizvodi[indeks]; }

};

//U posljednjoj klasi samostalno prepoznati izuzetne situacije
class Prodavac
{
	char* _imePrezime;
	Narudzba* _listaNarudzbi;
	int _brojNarudzbi;

	bool VecPostoji(int brojNarudzbe) {
		for (int i = 0; i < _brojNarudzbi; i++)
			if (_listaNarudzbi[i].GetBrojNarudzbe() == brojNarudzbe) return true;
		return false;
	}
	int GetBrNarudzbe(int brojNarudzbe) {
		for (int i = 0; i < _brojNarudzbi; i++)
			if (_listaNarudzbi[i].GetBrojNarudzbe() == brojNarudzbe) return i;
		return -1;
	}

public:
	//Potrebne konstruktor i destruktor funkcije
	Prodavac(const char* ip) {
		_imePrezime = AlocirajNizKaraktera(ip);
		_listaNarudzbi = nullptr;
		_brojNarudzbi = 0;
	}
	~Prodavac() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete[] _listaNarudzbi; _listaNarudzbi = nullptr;
		_brojNarudzbi = 0;
	}

	//Omoguciti dodavanje narudžbi za prodavaca
	bool DodajNarudzbu(const Narudzba& narudzba) {

		if (VecPostoji(narudzba.GetBrojNarudzbe())) throw NedozvoljenaOperacija("Narudzba je vec dodana!\n", __LINE__);

		Narudzba* temp = new Narudzba[_brojNarudzbi + 1];

		for (int i = 0; i < _brojNarudzbi; i++)
			temp[i] = _listaNarudzbi[i];

		temp[_brojNarudzbi] = narudzba;
		_brojNarudzbi++;

		delete[] _listaNarudzbi;
		_listaNarudzbi = temp;

		cout << "\t Narudzba uspjesno dodana!\n";
		return true;
	}

	//Omoguciti otkazivanje narudžbe na osnovu njenog broja.
	void OtkaziNarudzbu(int brojNarudzbe) {
		if (!VecPostoji(brojNarudzbe)) throw NedozvoljenaOperacija("Narudzba ne postoji!\n", __LINE__);
		int index = GetBrNarudzbe(brojNarudzbe);
		_listaNarudzbi[index].SetOtkazano();
		cout << "Narudzba uspjesno otkazana!\n";
	}

	//Ispis svih podataka o prodavacu i kratke informacije o
	//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.
	void InfoNarudzba() {
		cout << endl << crtj << "\t\tIme i prezime prodavca: " << _imePrezime << endl << crtj;
		for (int i = 0; i < _brojNarudzbi; i++) {
			cout << _listaNarudzbi[i];
		}
	}

	//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
	//sa informacijom o tome da li se radi o odjeci ili osnovnom proizvodu.
	void NajprodavanijiProizvodi() {
		struct _PROIZVODI {
			char* ime;
			int kolicina;
			string vrsta;

			_PROIZVODI(char* i, string vrsta) {
				ime = AlocirajNizKaraktera(i);
				this->vrsta = vrsta;
				kolicina = 1;
			}
			void PovecajKolicinu() { kolicina++; }
			bool operator==(const _PROIZVODI& right) {
				return strcmp(this->ime, right.ime) == 0;
			}
			void Info() {
				cout << ime << " [" << vrsta << "] " << endl;
			}
		};

		for (int i = 0; i < _brojNarudzbi; i++)
		{
			if (!_listaNarudzbi[i].Otkazana())
			{
				vector<_PROIZVODI> _sviProizvodi;
				for (int j = 0; j < _listaNarudzbi[i].GetTrenutno(); j++)
				{
					Odjeca* pointer = dynamic_cast<Odjeca*>(&_listaNarudzbi[i].GetProizvod(j));
					string vrsta = pointer != nullptr ? "Odjeca" : "Osnovni proizvod";
					
					_PROIZVODI temp(_listaNarudzbi[i].GetProizvod(j).GetNaziv(), vrsta);
					_sviProizvodi.push_back(temp);
				}
				for (int k = 0; k < _sviProizvodi.size(); k++)
				{
					for (int m = k + 1; m < _sviProizvodi.size(); m++)
					{
						if (_sviProizvodi[k] == _sviProizvodi[m]) {
							_sviProizvodi[k].PovecajKolicinu();
							_sviProizvodi.erase(_sviProizvodi.begin() + m);
							m--;
						}
					}
				}
				int maxKolicina = _sviProizvodi[0].kolicina;
				int index = 0;
				for (int i = 1; i < _sviProizvodi.size(); i++)
				{
					if (_sviProizvodi[i].kolicina > maxKolicina) {
						maxKolicina = _sviProizvodi[i].kolicina;
						index = i;
					}
				}
				cout << "Najprodavaniji proizvod iz narudzbe " << _listaNarudzbi[i].GetBrojNarudzbe() << ": "; 
				_sviProizvodi[index].Info();
			}
		}
	}
};


int main()
{
	
#pragma region Proizvodi

	Odjeca airmax("4417", "Nike AirMax", 179.99, 5, "43, 44, 45", "crna, bijela, plava", "Sportska obuca", 20);
	Proizvod jakna("9110", "Adidas jakna", 229.99, 7, 25);
	Odjeca majica("7714", "Tom Tailor", 14.99, 50, "43, 44, 45", "crna, bijela, plava", "Casual", 40);
	Proizvod sprej("4001", "Sprej za kosu", 4.90, 30, 75);
	Odjeca pantole("3121", "Texas Jeans", 74.90, 10, "36, 37, 38, 44", "plava", "Slim-fit", 10);
	Proizvod rukavice("9171", "Zimske rukavice", 11.95, 100, 80);

#pragma endregion

#pragma region Katalog

	Katalog katalog("Zimska akcija", "1.1.2019.");

	katalog.DodajProizvod(airmax);
	katalog.DodajProizvod(jakna);

	katalog.DodajProizvod(airmax);	// Ponovo se dodaje
	katalog.DodajProizvod(jakna);	// Ponovo se dodaje

	katalog.DodajProizvod(majica);
	katalog.DodajProizvod(sprej);
	katalog.DodajProizvod(pantole);
	katalog.DodajProizvod(rukavice);

	katalog.UkloniProizvod("4001");

	cout << katalog;

#pragma endregion

#pragma region Testiranje narudzbe

	Narudzba narudzba1(1, "3.12.2018.");

	narudzba1.DodajProizvod(majica);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);	// Moze se dodavati vise puta

	Proizvod test("7001", "Testni proizvod", 4.90, 0, 75);	// Kolicina je 0 pa bi trebalo baciti izuzetak

	try {
		narudzba1.DodajProizvod(test);
	}
	catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	narudzba1.UkloniProizvod(sprej);		// Brise sva tri spreja iz narudzbe

	narudzba1.DodajProizvod(rukavice);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(sprej);
	narudzba1.DodajProizvod(pantole);
	narudzba1.DodajProizvod(rukavice);
	narudzba1.DodajProizvod(jakna);
	narudzba1.DodajProizvod(rukavice);

	cout << "\nUkupna cijena narudzbe [Uracunat popust] :: " << narudzba1.GetUkupnuCijenu() << "KM" << endl;
	cout << narudzba1;

	// Testiranje operatora=
	Narudzba t(2, "22.7.2018.");
	t = narudzba1;
	cout << t;

	Narudzba narudzba2(3, "4.12.2018.");
	narudzba2.DodajProizvod(airmax);
	narudzba2.DodajProizvod(pantole);
	narudzba2.DodajProizvod(jakna);

	Narudzba narudzba3(4, "11.3.2018.");
	narudzba3.DodajProizvod(majica);
	narudzba3.DodajProizvod(majica);
	narudzba3.DodajProizvod(jakna);
	narudzba3.DodajProizvod(pantole);

#pragma endregion

#pragma region Prodavac

	Prodavac _prodavac("Ajdin Hukara");
	_prodavac.DodajNarudzbu(narudzba1);
	_prodavac.DodajNarudzbu(narudzba2);
	_prodavac.DodajNarudzbu(narudzba3);
	try {
		_prodavac.DodajNarudzbu(t);		// Dodavanje iste narudzbe
	}
	catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	try {
	_prodavac.OtkaziNarudzbu(47);		// Otkazivanje narudzbe koja ne postoji
	}
	catch (NedozvoljenaOperacija& n) { n.ErrorInfo(); }

	_prodavac.OtkaziNarudzbu(3);		

	_prodavac.InfoNarudzba();

	_prodavac.NajprodavanijiProizvodi();

#pragma endregion

	system("pause>0");

	return 0;
}
