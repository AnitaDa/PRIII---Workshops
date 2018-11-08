#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

int SifraBrojac = 0;
#define endline cout<<endl;
#define endlines cout<<endl<<endl;
#define crt cout<<"\n-------------------------------------------\n";
#define PrintTest(name) cout<<"\n-------------> "<<name<<" <-------------\n";

char* AlocirajNizKaraktera(const char* original) {
	if (original == nullptr) return nullptr;
	char* temp = new char[strlen(original) + 1];
	strcpy_s(temp, strlen(original) + 1, original);
	return temp;
}

class Uposlenik {

	int _sifra; //Automatski uvecavati za 1 za svakog novog uspolenika.
	char _ime[30];
	char _prezime[30];
	char* _radnoMjesto;
	static const char* _kompanija;

public:
	//Implementirati potrebne konstruktor i destruktor funkcije
	Uposlenik(const char* ime, const char* prezime, const char* radnoMjesto) : _sifra(++SifraBrojac)
	{
		strncpy_s(_ime, ime, _TRUNCATE);
		strncpy_s(_prezime, prezime, _TRUNCATE);

		int vel = strlen(radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, radnoMjesto);
	}
	Uposlenik(const Uposlenik& original) {
		_sifra = original._sifra;
		strcpy_s(_ime, original._ime);
		strcpy_s(_prezime, original._prezime);
		_radnoMjesto = new char[strlen(original._radnoMjesto) + 1];
		strcpy_s(_radnoMjesto, strlen(original._radnoMjesto) + 1, original._radnoMjesto);
	}
	Uposlenik() {
		_radnoMjesto = nullptr;
	}
	~Uposlenik()
	{
		delete[] _radnoMjesto;
		_radnoMjesto = nullptr;
	}

	//Ostale funkcije dodati po potrebi

	void Info()
	{
		cout << "Sifra: " << _sifra << endl;
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Radno mjesto: " << _radnoMjesto << endl;
	}
	bool operator==(const Uposlenik& right) {
		return this->_sifra == right._sifra && strcmp(this->_ime, right._ime) == 0 && strcmp(this->_prezime, right._prezime) == 0;
	}
	Uposlenik& operator=(const Uposlenik& right) {
		if (this != &right) {
			if (_radnoMjesto != nullptr) { delete[]_radnoMjesto; _radnoMjesto = nullptr; }
			_sifra = right._sifra;
			strcpy_s(_ime, right._ime);
			strcpy_s(_prezime, right._prezime);
			_radnoMjesto = new char[strlen(right._radnoMjesto) + 1];
			strcpy_s(_radnoMjesto, strlen(right._radnoMjesto) + 1, right._radnoMjesto);
		}
		return *this;
	}
	int GetSifru() const { return _sifra; }
	friend ostream& operator<<(ostream& out, const Uposlenik& right) {
		return out << right._sifra << " :: " << right._ime << " " << right._prezime << endl;
	}
};


class Aktivnost {
	char* _naziv;
	bool _zavrsena;
	int* _trajanje; //Izrazeno u danima
	Uposlenik* _uposlenici[5];
public:
	//Implementirati potrebne konstruktor i destruktor funkcije
	Aktivnost(const char* naziv, int trajanje) :
		_zavrsena(false)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);
		for (int i = 0; i < 5; i++)
			_uposlenici[i] = nullptr;
		_trajanje = new int(trajanje);
	}
	Aktivnost() {
		_naziv = nullptr;
		_trajanje = nullptr;
		for (int i = 0; i < 5; i++)
			_uposlenici[i] = nullptr;
	}
	~Aktivnost() {
		delete[]_naziv; _naziv = nullptr;
		delete _trajanje; _trajanje = nullptr;
		for (int i = 0; i < 5; i++)
		{
			delete _uposlenici[i];
			_uposlenici[i] = nullptr;
		}
	}
	bool VecPostoji(const Uposlenik& uposlenik) {
		for (int i = 0; i < 5; i++)
			if (_uposlenici[i] != nullptr)
				if (*_uposlenici[i] == uposlenik) return true;
		return false;
	}
	//Funkciju koja omogucava angazovanje uposlenika na projektnoj aktivnosti. Funkcija treba da vrati vrijednost tipa
	//bool za uspjesno, odnosno neuspjesno dodavanje uposlenika. Nije moguce dodati istog uposlenika vise puta. Nije moguce
	//angazovati uposlenike na vec zavrsenoj aktivnosti.
	bool AngazujUposlenika(const Uposlenik& uposlenik) {
		if (_zavrsena) {
			cout << "Aktivnost je zavrsena!\n";
			return false;
		}
		if (VecPostoji(uposlenik)) {
			cout << "Uposlenik vec postoji!\n";
			return false;
		}
		for (int i = 0; i < 5; i++)
		{
			if (_uposlenici[i] == nullptr) {
				_uposlenici[i] = new Uposlenik(uposlenik);
				return true;
			}
		}
		cout << "Dostigli ste max. broj uposlenika!\n";
		return false;
	}

	//Funkciju koja uklanja uposlenika na aktivnosti sa sifrom proslijedjenom kao parametar.
	void UkloniUposlenika(int sifra) {
		int indeks = -1;
		for (int i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr) {
				if (_uposlenici[i]->GetSifru() == sifra) {
					indeks = i;
					break;
				}
			}
		}
		if (indeks == -1) {
			cout << "Uposlenik nije pronadjen!\n";
			return;
		}
		for (int i = indeks; i < 4; i++)
		{
			delete _uposlenici[i];
			_uposlenici[i] = nullptr;
			if (_uposlenici[i + 1] != nullptr) {
				_uposlenici[i] = new Uposlenik(*_uposlenici[i + 1]);
				delete _uposlenici[i + 1];
				_uposlenici[i + 1] = nullptr;
			}
		}
	}

	//Funkciju koja uklanja sve uposlenike angazovane na aktivnosti.
	void UkloniSveUposlenike() {
		for (int i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr) {
				delete _uposlenici[i];
				_uposlenici[i] = nullptr;
			}
		}
		cout << "Svi uposlenici uklonjeni!\n";
	}
	bool operator==(const Aktivnost& right) {
		return strcmp(this->_naziv, right._naziv) == 0;
	}
	Aktivnost& operator=(const Aktivnost& right) {
		if (this != &right) {
			this->_naziv = AlocirajNizKaraktera(right._naziv);
			this->_trajanje = new int(*right._trajanje);
			this->_zavrsena = right._zavrsena;
			for (int i = 0; i < 5; i++)
			{
				if (right._uposlenici[i] != nullptr) {
					this->_uposlenici[i] = new Uposlenik(*right._uposlenici[i]);
				}
			}
		}
		return *this;
	}
	char* GetNaziv()const { return _naziv; }
	void SetZavrseno() {
		_zavrsena = true;
	}
	int GetTrenutno() const {
		int brojac = 0;
		for (int i = 0; i < 5; i++)
			if (_uposlenici[i] != nullptr) brojac++;
		return brojac;
	}
	Uposlenik* GetUposlenika(int indeks) const { return _uposlenici[indeks]; }
	friend ostream& operator<<(ostream& cout, const Aktivnost& akt) {
		for (int i = 0; i < 5; i++)
		{
			if (akt._uposlenici[i] != nullptr)
				cout << *akt._uposlenici[i];
		}
		return cout;
	}
};

class Projekat {
	char* _naziv;
	char* _oblast;
	int _trajanje; //Izrazeno u mjesecima
	Aktivnost* _aktivnosti;
	int _brojAktivnosti;
public:

	//Implementirati potrebne konstruktor i destruktor funkcije.
	Projekat(const char* naziv, const char* oblast, int trajanje_mjeseci) {
		_naziv = AlocirajNizKaraktera(naziv);
		_oblast = AlocirajNizKaraktera(oblast);
		_trajanje = trajanje_mjeseci;
		_aktivnosti = nullptr;
		_brojAktivnosti = 0;
	}
	Projekat() {
		_naziv = nullptr;
		_oblast = nullptr;
		_trajanje = 0;
		_aktivnosti = nullptr;
		_brojAktivnosti = 0;
	}
	~Projekat() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _oblast; _oblast = nullptr;
		delete[] _aktivnosti; _aktivnosti = nullptr;
	}
	Projekat& operator=(const Projekat& right)
	{
		if (this != &right) {
			this->_naziv = AlocirajNizKaraktera(right._naziv);
			this->_oblast = AlocirajNizKaraktera(right._oblast);
			this->_trajanje = right._trajanje;
			this->_brojAktivnosti = right._brojAktivnosti;
			this->_aktivnosti = new Aktivnost[this->_brojAktivnosti];
			for (int i = 0; i < this->_brojAktivnosti; i++)
				_aktivnosti[i] = right._aktivnosti[i];
		}
		return *this;
	}

	char* GetNazivProjekta() const { return _naziv; }

	//Funkciju za dodavanje projektnih aktivnosti. Sprijeciti dodavanje vise aktivnosti sa istim nazivom.
	//Za potrebe poredjenja aktivnosti kreirati funkciju unutar klase Aktivnosti.
	void DodajAktivnost(const Aktivnost& nova) {
		Aktivnost* temp = new Aktivnost[_brojAktivnosti + 1];
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			temp[i] = _aktivnosti[i];
		}
		temp[_brojAktivnosti] = nova;
		delete[] _aktivnosti;
		_aktivnosti = temp;
		_brojAktivnosti++;
	}

	//Funkciju koja oznacava aktivnost kao zavrsenu. Funkcija kao parametar prima naziv aktivnosti.
	void OznaciZavrsenom(const char* nazivAktivnosti) {
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].GetNaziv(), nazivAktivnosti) == 0) {
				_aktivnosti[i].SetZavrseno();
				return;
			}
		}
		cout << "\tERROR :: Aktivnost nije pronadjena!\n";
	}

	//Funkciju koja treba da postojecoj aktivnosti dodijeli novog uposlenika.
	void DodajUposlenika(const char* nazivAktivnosti, const Uposlenik& u) {
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].GetNaziv(), nazivAktivnosti) == 0) {
				_aktivnosti[i].AngazujUposlenika(u);
				return;
			}
		}
		cout << "\tERROR :: Aktivnost nije pronadjena!\n";
	}

	//Funkciju koja treba da vrati sve uposlenike angazovane na odredjenoj aktivnosti. Ulazni parametar je naziv aktivnosti.
	Uposlenik* GetUposlenikeSaAktivnosti(const char* nazivAktivnosti, int& broj) {
		int indeks = -1;
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].GetNaziv(), nazivAktivnosti) == 0) {
				indeks = i;
				break;
			}
		}
		if (indeks == -1) {
			cout << "\tERROR :: Aktivnost nije pronadjena!\n";
			return nullptr;
		}
		broj = _aktivnosti[indeks].GetTrenutno();
		Uposlenik* up = new Uposlenik[_aktivnosti[indeks].GetTrenutno()];
		for (int i = 0; i < _aktivnosti[indeks].GetTrenutno(); i++)
			up[i] = *_aktivnosti[indeks].GetUposlenika(i);
		return up;
	}

	//Funkciju koja uklanja odredjenog uposlenika na aktivnosti. Ulazni parametri funkcije su naziv aktivnosti i sifra uposlenika.
	void UkloniUposlenikaSaAktivnosti(const char* nazivAktivnosti, int sifraUposlenika) {
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].GetNaziv(), nazivAktivnosti) == 0) {
				_aktivnosti[i].UkloniUposlenika(sifraUposlenika);
				return;
			}
		}
		cout << "\tERROR :: Aktivnost nije pronadjena!\n";
	}

	//Funkciju koja uklanja sve uposlenike angazovane na aktivnosti sa nazivom proslijedjenim kao parametar.
	void UkloniSveUposlenikeSaAktivnosti(const char* nazivAktivnosti) {
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].GetNaziv(), nazivAktivnosti) == 0) {
				_aktivnosti[i].UkloniSveUposlenike();
				return;
			}
		}
	}

	//Funkciju koja pronalazi uposlenika sa najvecim brojem angazmana na aktivnostima.
	Uposlenik GetNajboljeg(int& brojAngazmana) {
		struct SviRadnici {
			Uposlenik _uposlenik;
			int brojAngazmana = 0;
			SviRadnici(const Uposlenik& u = Uposlenik(), int brAngazmana = 0) {
				_uposlenik = u;
				brojAngazmana = brAngazmana;
			}
			void PovecajAngazman() { brojAngazmana++; }
			bool operator==(const SviRadnici& right) {
				return this->_uposlenik == right._uposlenik;
			}
		};

		// STRPAMO SVE UPOSLENIKE U JEDAN NIZ
		vector<SviRadnici> _uposlenici_all;
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (_aktivnosti[i].GetUposlenika(j) != nullptr) {
					SviRadnici temp(*_aktivnosti[i].GetUposlenika(j), 0);
					_uposlenici_all.push_back(temp);
				}
			}
		}

		// PROVJERAVAMO PONAVLJAJU LI SE UPOSLENICI, AKO SE PONAVLJAJU:
		//		- povecavamo im brojAngazmana
		//		- brisemo element iz niza kojeg smo pronasli (da se ne dupliciraju)
		for (size_t i = 0; i < _uposlenici_all.size(); i++)
		{
			for (size_t j = i + 1; j < _uposlenici_all.size(); j++)
			{
				if (_uposlenici_all[i] == _uposlenici_all[j]) {
					_uposlenici_all[i].PovecajAngazman();
					_uposlenici_all.erase(_uposlenici_all.begin() + j);
					j--;
				}
			}
		}

		// NA KRAJU PRONALAZIMO UPOSLENIKA SA NAJVECIM BROJEM ANGAZMANA:
		//		- sacuvamo mu trenutnu index poziciju
		//		- nakon for petlje vracamo element sa te indeks pozicije
		int maxAngazman = _uposlenici_all[0].brojAngazmana;
		int indeks = 0;
		for (size_t i = 1; i < _uposlenici_all.size(); i++)
		{
			if (_uposlenici_all[i].brojAngazmana > maxAngazman) {
				maxAngazman = _uposlenici_all[i].brojAngazmana;
				indeks = i;
			}
		}
		brojAngazmana = _uposlenici_all[indeks].brojAngazmana;
		return _uposlenici_all[indeks]._uposlenik;
	}

	//Funkciju koja pronalazi 3 uposlenika koja su angazovana na najvecem broju nezavrsenih aktivnosti.
	Uposlenik* GetTop3Uposlenika() {
		struct SviRadnici {
			Uposlenik _uposlenik;
			int brojAngazmana = 0;
			SviRadnici(const Uposlenik& u = Uposlenik(), int brAngazmana = 0) {
				_uposlenik = u;
				brojAngazmana = brAngazmana;
			}
			void PovecajAngazman() { brojAngazmana++; }
			bool operator==(const SviRadnici& right) {
				return this->_uposlenik == right._uposlenik;
			}
		};

		// STRPAMO SVE UPOSLENIKE U JEDAN NIZ
		vector<SviRadnici> _uposlenici_all;
		for (int i = 0; i < _brojAktivnosti; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (_aktivnosti[i].GetUposlenika(j) != nullptr) {
					SviRadnici temp(*_aktivnosti[i].GetUposlenika(j), 0);
					_uposlenici_all.push_back(temp);
				}
			}
		}

		// PROVJERAVAMO PONAVLJAJU LI SE UPOSLENICI, AKO SE PONAVLJAJU:
		//		- povecavamo im brojAngazmana
		//		- brisemo element iz niza kojeg smo pronasli (da se ne dupliciraju)
		for (size_t i = 0; i < _uposlenici_all.size(); i++)
		{
			for (size_t j = i + 1; j < _uposlenici_all.size(); j++)
			{
				if (_uposlenici_all[i] == _uposlenici_all[j]) {
					_uposlenici_all[i].PovecajAngazman();
					_uposlenici_all.erase(_uposlenici_all.begin() + j);
					j--;
				}
			}
		}
		if (_uposlenici_all.size() < 3) {
			cout << "Trenutno nema 3 uposlenika na aktivnostima!\n";
			return nullptr;
		}

		// NA KRAJU PRONALAZIMO UPOSLENIKA SA NAJVECIM BROJEM ANGAZMANA:
		//		- sacuvamo mu trenutnu index poziciju
		//		- nakon for petlje vracamo element sa te indeks pozicije

		int indeksi[3];				// Ovdje smjestamo tri indeksa uposlenika sa najvecim angazmanima
		int brojacIndeksa = 0;		// zbog while petlje, prvo se ubacuje najbolji, pa srednji, pa iza srednjeg
		Uposlenik* top3_uposlenici = new Uposlenik[3];	// niz uposlenika

		while (brojacIndeksa != 3) {
			int maxAngazman = _uposlenici_all[0].brojAngazmana;	// pretpostavimo da prvi element ima najvise angazmana
			for (size_t i = 1; i < _uposlenici_all.size(); i++)
			{
				if (_uposlenici_all[i].brojAngazmana > maxAngazman) {	// ako se nadje neki da je veci, rotiramo vrijednosti
					indeksi[brojacIndeksa] = i;
					maxAngazman = _uposlenici_all[i].brojAngazmana;
				}
			}
			top3_uposlenici[brojacIndeksa] = _uposlenici_all[indeksi[brojacIndeksa]]._uposlenik;
			// nakon sto smo nasli prvog najboljeg, spasavamo ga u niz top3_uposlenici
			_uposlenici_all.erase(_uposlenici_all.begin() + indeksi[brojacIndeksa]);
			// nakon spasavanja, brisemo taj element iz originalnog niza, da se ne bi opet on nasao kao najbolji
			brojacIndeksa++;
		}
		return top3_uposlenici;	// vracamo citav niz
	}

	//Funkciju koja ispisuje sve zavrsene aktivnosti na projektu.
	friend ostream& operator<<(ostream& cout, const Projekat& p) {
		for (int i = 0; i < p._brojAktivnosti; i++)
		{
			PrintTest(p._aktivnosti[i].GetNaziv());
			cout << p._aktivnosti[i];
		}
		return cout;
	}

};

/*Kreirati globalnu funkciju koja pronalazi najuspjesnijeg usposlenika na projektima proslijedjenim kao parametar.
Uposlenik se smatra najuspjesnijim ako je angazovan na najvecem broju zavrsenih aktivnosti. Ako vise uposlenika
ima isti broj zavrsenih aktivnosti, uspjesnijim se smatra onaj cije su aktivnosti trajale duze.
*/
Uposlenik PronadjiNajuspjesnijegUposlenika(Projekat* projekti, int max) {

	int indeksProjektaNajboljegUposlenika = 0;
	int maxAngazmana = 0;

	for (int i = 1; i < max; i++)
	{
		int brAngazmana;
		projekti[i].GetNajboljeg(brAngazmana);
		if (brAngazmana > maxAngazmana) {
			maxAngazmana = brAngazmana;
			indeksProjektaNajboljegUposlenika = i;
		}
	}
	int temp;
	return projekti[indeksProjektaNajboljegUposlenika].GetNajboljeg(temp);
}

const char* Uposlenik::_kompanija = "Procedo d.o.o";

int main()
{
	//Testirati sve dostupne funkcionalnosti
#pragma region Test -> Uposlenici
	crt;
	Uposlenik ajdin("Ajdin", "Hukara", "CNC Operater");
	Uposlenik emir("Emir", "Zukic", "Inzenjer");
	Uposlenik mujo("Mujo", "Zukic", "Brusac");
	Uposlenik dino("Dino", "Spahic", "Lotar");
	Uposlenik ensar("Ensar", "Pasovic", "Ispravlja zage");
	Uposlenik almir("Almir", "Becic", "CNC Operater");
	Uposlenik alemka("Alemka", "Bajramovic", "Inzenjer");

	almir.Info();
	if (almir == ajdin) { cout << "\tIsti su!\n"; }
	else cout << "\tNisu isti!\n";
	almir = ajdin;
	almir.Info();
	if (almir == ajdin) { cout << "\tIsti su!\n"; }
	else cout << "\tNisu isti!\n";

#pragma endregion
#pragma region Test -> Aktivnost

	Aktivnost okrugloBrusenje("Okruglo brusenje", 5);
	Aktivnost rezanjeCelika("Rezanje celika", 7);
	Aktivnost lotanjeZuba("Lotanje zuba", 15);
	crt;
	if (okrugloBrusenje.AngazujUposlenika(almir))
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(ajdin))		// NE TREBA DODATI
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(mujo))
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(emir))
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(ensar))
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(dino))
		cout << "\tUposlenik uspjesno angazovan!\n";
	if (okrugloBrusenje.AngazujUposlenika(alemka))		// VEC IMA 5 UPOSLENIKA
		cout << "\tUposlenik uspjesno angazovan!\n";

	okrugloBrusenje.UkloniUposlenika(2);
	okrugloBrusenje.UkloniUposlenika(1);
	okrugloBrusenje.UkloniUposlenika(9);				// NE POSTOJI
	crt;

#pragma endregion
#pragma region Test -> Projekat

	Projekat projekat("Projekat 1", "Informatika", 2);
	Projekat projekat_A("Projekat 2", "Matematika", 4);
	Projekat projekat_B("Projekat 3", "Hemija", 6);

	projekat_A.DodajAktivnost(rezanjeCelika);
	projekat_A.DodajAktivnost(okrugloBrusenje);
	projekat_A.DodajUposlenika("Rezanje celika", ajdin);
	projekat_A.DodajUposlenika("Rezanje celika", emir);
	projekat_A.DodajUposlenika("Rezanje celika", mujo);
	projekat_A.DodajUposlenika("Rezanje celika", alemka);
	projekat_A.DodajUposlenika("Okruglo brusenje", alemka);

	projekat_B.DodajAktivnost(rezanjeCelika);
	projekat_B.DodajAktivnost(lotanjeZuba);
	projekat_B.DodajUposlenika("Rezanje celika", alemka);
	projekat_B.DodajUposlenika("Rezanje celika", mujo);
	projekat_B.DodajUposlenika("Lotanje zuba", mujo);

	projekat.DodajAktivnost(okrugloBrusenje);
	projekat.DodajAktivnost(rezanjeCelika);
	projekat.DodajAktivnost(lotanjeZuba);

	projekat.DodajUposlenika("Rezanje celika", emir);
	projekat.DodajUposlenika("Rezanje celika", ajdin);
	projekat.DodajUposlenika("Rezanje celika", dino);
	projekat.DodajUposlenika("Lotanje zuba", emir);
	projekat.DodajUposlenika("Lotanje zuba", dino);
	projekat.DodajUposlenika("Lotanje zuba", alemka);
	projekat.DodajUposlenika("Okruglo brusenje", emir);
	projekat.DodajUposlenika("Okruglo brusenje", alemka);
	projekat.UkloniUposlenikaSaAktivnosti("Okruglo brusenje", 3);

	int okrugloBrusenjeBroj = 0;
	Uposlenik* uposlenici_okrugloBrusenje = projekat.GetUposlenikeSaAktivnosti("Okruglo brusenje", okrugloBrusenjeBroj);
	for (int i = 0; i < okrugloBrusenjeBroj; i++)
	{
		cout << uposlenici_okrugloBrusenje[i];
	}
	cout << projekat;

#pragma endregion
#pragma region Test -> Najteze funkcije

	int brAngazmanaNajbolji = 0;
	cout << "\n\tNajbolji uposlenik firme Procedo -> " << projekat.GetNajboljeg(brAngazmanaNajbolji);
	Uposlenik* top3 = projekat.GetTop3Uposlenika();
	PrintTest("Top 3 uposlenika:")
		for (int i = 0; i < 3; i++)
			cout << top3[i];

#pragma endregion
#pragma region Test -> Globalna funkcija

	Projekat * _projekti = new Projekat[3];
	_projekti[0] = projekat;
	_projekti[1] = projekat_A;
	_projekti[2] = projekat_B;

	cout << "\n\t ISPIS SVIH PROJEKATA: \n";
	for (int i = 0; i < 3; i++)
	{
		cout << "--------------> PROJEKAT " << _projekti[i].GetNazivProjekta() << endl;
		cout << _projekti[i] << endl << endl;
	}

	cout << "Najbolje angazovan radnik na svim projektima: " << PronadjiNajuspjesnijegUposlenika(_projekti, 3);

#pragma endregion
#pragma region Dealokacija

	delete[] _projekti;
	_projekti = nullptr;

	delete[] top3;
	top3 = nullptr;

	delete[] uposlenici_okrugloBrusenje;
	uposlenici_okrugloBrusenje = nullptr;

#pragma endregion

	return 0;
}
