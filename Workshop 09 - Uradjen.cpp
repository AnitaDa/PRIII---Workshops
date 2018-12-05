#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
using namespace std;

// Ima mali bug kod MCMA (Multiple Choice - Multiple Answer)
// Ne racuna polovicno bodove, vec samo ako su svi tacni
// Na pocetku zadatka je zadano da se formira string sa tacnim odgovorima, pa sam njega iskoristio
// Tacno ce priznati pitanje samo ako su svi odgovori tacni i to uneseni redoslijedno, jer koristi funkciju strcmp()

char* AlocirajNizKaraktera(const char* t)
{
	if (t == nullptr) return nullptr;
	char* temp = new char[strlen(t) + 1];
	strcpy_s(temp, strlen(t) + 1, t);
	return temp;
}

template <class T1, class T2>
class List
{
	T1* _elementi1;
	T2* _elementi2;
	const int _max;
	int _trenutno;
public:
	List(int max = 10) : _max(max)
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}

	List(const List& list) : _max(list._max)
	{
		_elementi1 = new T1[list._trenutno];
		_elementi2 = new T2[list._trenutno];

		for (int i = 0; i < list._trenutno; i++)
		{
			_elementi1[i] = list._elementi1[i];
			_elementi2[i] = list._elementi2[i];
		}

		_trenutno = list._trenutno;
	}

	~List()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	bool Add(T1& el1, T2& el2)
	{
		if (_trenutno == _max)
			return false;

		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == el1 && _elementi2[i] == el2)
				return false;
		}

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
		return true;
	}

	bool Remove(int index)
	{
		if (index < 0 || index > _trenutno)
			return false;

		T1* temp1 = new T1[_trenutno - 1];
		T2* temp2 = new T2[_trenutno - 1];

		int j = 0;
		for (int i = 0; i < _trenutno; i++)
		{
			if (i != index)
			{
				temp1[j] = _elementi1[i];
				temp2[j] = _elementi2[i];
				j++;
			}
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno--;

		return true;
	}

	T1& GetElement1(int index)
	{
		if (index >= 0 && index < _trenutno)
			return _elementi1[index];
		//U suprotnom baciti izuzetak
		throw exception("\tERROR :: Indeks nije validan.\n");
	}

	T2& GetElement2(int index)
	{
		if (index >= 0 && index < _trenutno)
			return _elementi2[index];
		//U suprotnom baciti izuzetak
		throw exception("\tERROR :: Indeks nije validan.\n");
	}

	int GetTrenutno() const { return _trenutno; }
	int GetMax() const { return _max; }

	void ModifikujElement1(int indeks, T1& novo, T2& novo2)
	{
		_elementi1[indeks] = novo;
		_elementi2[indeks] = novo2;
	}

	friend ostream& operator<<<>(ostream&, const List&);
};

template <class T1, class T2>
ostream& operator<<(ostream& cout, const List<T1, T2>& list)
{
	cout << endl;
	for (int i = 0; i < list._trenutno; i++)
		cout << i + 1 << ": " << list._elementi1[i] << endl;
	return cout;
}

class Pitanje
{
protected:
	int _id;
	char* _oblast;
	char* _tekst;
	double _bodovi;
public:
	Pitanje(int id, const char* tekst, const char* oblast, double bodovi) :
		_id(id), _bodovi(bodovi)
	{
		int vel = strlen(tekst) + 1;
		_tekst = new char[vel];
		strcpy_s(_tekst, vel, tekst);

		vel = strlen(oblast) + 1;
		_oblast = new char[vel];
		strcpy_s(_oblast, vel, oblast);
	}

	Pitanje()
	{
		_oblast = nullptr;
		_tekst = nullptr;
		_id = 0;
		_bodovi = 0;
	}

	Pitanje(const Pitanje& p)
	{
		_id = p._id;
		_oblast = AlocirajNizKaraktera(p._oblast);
		_tekst = AlocirajNizKaraktera(p._tekst);
		_bodovi = p._bodovi;
	}

	Pitanje& operator=(const Pitanje& right)
	{
		if (this != &right)
		{
			_id = right._id;
			_oblast = AlocirajNizKaraktera(right._oblast);
			_tekst = AlocirajNizKaraktera(right._tekst);
			_bodovi = right._bodovi;
		}
		return *this;
	}

	virtual void DodajOdgovor(const char*, bool) = 0;
	virtual void UkloniOdgovor(int) = 0;
	virtual bool Start() = 0;

	double GetBodove() const { return _bodovi; }

	virtual ~Pitanje() = 0
	{
		delete[] _tekst;
		_tekst = nullptr;

		delete[] _oblast;
		_oblast = nullptr;
	}

	virtual void Info()
	{
		cout << _tekst << endl;
	}
};

//Pitanje na kojem studenti upisuju odgovore.
//Pohranjuje se tacan odgovor od strane edukatora.
class EsejskoPitanje : public Pitanje
{
protected:
	char* _tacanOdgovor;
	char* _odgovorStudenta;
public:
	//Konstruktor i destruktor
	EsejskoPitanje(int ID, const char* tekst, const char* oblast, double bodovi, const char* tacan,
	               const char* odgovor) : Pitanje(ID, tekst, oblast, bodovi)
	{
		_tacanOdgovor = AlocirajNizKaraktera(tacan);
		_odgovorStudenta = AlocirajNizKaraktera(odgovor);
	}

	EsejskoPitanje()
	{
		_tacanOdgovor = nullptr;
		_odgovorStudenta = nullptr;
	}

	EsejskoPitanje(const EsejskoPitanje& p) : Pitanje(p._id, p._tekst, p._oblast, p._bodovi)
	{
		_tacanOdgovor = AlocirajNizKaraktera(p._tacanOdgovor);
		_odgovorStudenta = AlocirajNizKaraktera(p._odgovorStudenta);
	}

	~EsejskoPitanje()
	{
		delete[] _tacanOdgovor;
		_tacanOdgovor = nullptr;
		delete[] _odgovorStudenta;
		_odgovorStudenta = nullptr;
	}

	//Funkciju za izmjenu svih atributa klase
	void IzmjeniAtribute(int ID, const char* tekst, const char* oblast, double bodovi, const char* tacan,
	                     const char* odgovor)
	{
		_id = ID;
		if (_tekst != nullptr) delete[] _tekst;
		_tekst = AlocirajNizKaraktera(tekst);
		if (_oblast != nullptr) delete[] _oblast;
		_oblast = AlocirajNizKaraktera(oblast);
		if (_tacanOdgovor != nullptr) delete[] _tacanOdgovor;
		_tacanOdgovor = AlocirajNizKaraktera(tacan);
		if (_odgovorStudenta != nullptr) delete[] _odgovorStudenta;
		_odgovorStudenta = AlocirajNizKaraktera(odgovor);
		_bodovi = bodovi;
	}

	void DodajOdgovor(const char* v, bool i) override
	{
	};

	void UkloniOdgovor(int v) override
	{
	};

	bool Start() override
	{
		cout << "Unesite vas odgovor:\n";
		char temp[200];
		cin.getline(temp, 200);
		if (_odgovorStudenta != nullptr) delete[] _odgovorStudenta;
		_odgovorStudenta = AlocirajNizKaraktera(temp);
		if (TacanOdgovor())
		{
			cout << "\n\tINFO :: Odgovorili ste tacno!\n";
			return true;
		}
		cout << "\n\tINFO :: Odgovorili ste netacno!\n";
		return false;
	}

	void Info() override
	{
		Pitanje::Info();
	}

	bool TacanOdgovor()
	{
		return strstr(_odgovorStudenta, _tacanOdgovor) != nullptr || strstr(_tacanOdgovor, _odgovorStudenta) != nullptr;
	}
};

//Pitanje koje može imati više tacnih odgovora. Student bira ponudjene opcije. 
class MCMAPitanje : public Pitanje
{
protected:
	List<char*, bool> _odgovori; //ponudjeni odgovori sa oznakama tacnih odgovora
	char* _oznaceniOdgovoriStudenta;
	//cuvaju se redni brojevi (odvojeni zarezom) ponudjenih odgovora koje je odabrao student, npr. 1,2,6

	int GetBrojTacnih()
	{
		int brojTacnih = 0;
		for (int i = 0; i < _odgovori.GetTrenutno(); i++)
			if (_odgovori.GetElement2(i) == true) brojTacnih++;
		return brojTacnih;
	}

public:

	char* FormirajStringTacnih()
	{
		char temp[50];
		int count = 0;
		for (int i = 0; i < _odgovori.GetTrenutno(); i++)
		{
			if (_odgovori.GetElement2(i) == true)
			{
				temp[count++] = (i + 1) + '0';
				temp[count++] = ',';
				temp[count++] = ' ';
			}
		}
		temp[count - 2] = '\0';
		char* t = AlocirajNizKaraktera(temp);
		return t;
	}

	//Napomena: Podatke o odgovorima cuvati u zasebnoj memoriji i istu dealocirati unutar same klase
	//Konstruktor i destruktor
	//Broj bodova inicijalno postaviti na 0 i uvecavati svakim oznacenim tacnim odgovorom.
	MCMAPitanje(int id, const char* tekst, const char* oblast, double bodovi = 0) : Pitanje(id, tekst, oblast, bodovi)
	{
		_oznaceniOdgovoriStudenta = nullptr;
	}

	MCMAPitanje()
	{
		_oznaceniOdgovoriStudenta = nullptr;
	}

	~MCMAPitanje()
	{
		delete[] _oznaceniOdgovoriStudenta;
		for (int i = 0; i < _odgovori.GetTrenutno(); i++)
		{
			delete[] _odgovori.GetElement1(i);
			_odgovori.GetElement1(i) = nullptr;
		}
	}

	//Funkciju za izmjenu podataka o pitanju
	void IzmjeniPodatke(int id, const char* tekst, const char* oblast, int bodovi = 0)
	{
		_id = id;
		if (_tekst != nullptr) delete[] _tekst;
		_tekst = AlocirajNizKaraktera(tekst);
		if (_oblast != nullptr) delete[] _oblast;
		_oblast = AlocirajNizKaraktera(oblast);
		_bodovi = bodovi;
	}

	//Funkciju za dodavanje odgovora u listu koja treba da osigura da su najmanje 2 (od max) odgovora oznacena kao tacna.
	//Ukoliko se pokusa dodati i posljednji ili pretposljednji odgovor bez da je uslov ispunjen, baciti izuzetak. Dok se 
	//ne dodaju svi predvidjeni odgovori pitanje ne treba biti upotrebljivo ukoliko prethodni uslov nije ispunjen (ne prikazivati
	//pitanja na ispisu).
	void DodajOdgovor(const char* odgovor, bool tacan) override
	{
		int brojTacnih = GetBrojTacnih();
		int brojOdgovora = _odgovori.GetTrenutno();
		int max = _odgovori.GetMax();

		if (brojTacnih == 0 && brojOdgovora == max - 2 && !tacan)
		{
			throw exception("\tERROR :: Morate dodati najmanje 2 tacna odgovora!\n");
		}
		if (brojTacnih == 1 && brojOdgovora == max - 1 && !tacan)
		{
			throw exception("\tERROR :: Morate dodati najmanje 2 tacna odgovora!\n");
		}

		char* odg = AlocirajNizKaraktera(odgovor);
		_odgovori.Add(odg, tacan);
	}

	int GetTrenutnoOdgovora() const { return _odgovori.GetTrenutno(); }

	//Funkciju za uklanjanje odgovora na osnovu rednog broja u listi. Ukoliko se uklanja odgovor koji je oznacen kao tacan i 
	//pri tome u listi ostaje samo još jedan odgovor oznacen kao tacan, pitanje postaje neupotrebljivo dok se ne doda jos jedan
	//tacan odgovor.
	void UkloniOdgovor(int rb) override
	{
		rb--;
		delete[] _odgovori.GetElement1(rb);
		_odgovori.GetElement1(rb) = nullptr;
		_odgovori.Remove(rb);
		if (!Validno()) { cout << "\tINFO :: Odgovor uklonjen. Pitanje ima manje od dva tacna odgovora!\n"; }
	}

	bool Validno()
	{
		return GetBrojTacnih() >= 2;
	}

	//Funkciju za promjenu odgovora na odredjenom rednom broju
	void PromijeniOdgovorRB(int rb, const char* noviOdg, bool tacan)
	{
		if (rb < 0 || rb >= _odgovori.GetTrenutno())
			throw exception("\tERROR :: Indeks pozicija nije validna\n");
		rb--;
		delete[] _odgovori.GetElement1(rb);
		_odgovori.GetElement1(rb) = nullptr;
		char* odgovor = AlocirajNizKaraktera(noviOdg);
		_odgovori.ModifikujElement1(rb, odgovor, tacan);
		cout << "\tINFO :: Odgovor uspjesno promijenjen!\n";
	}

	bool Start() override
	{
		cout << "\n\t Odaberite odgovore: [0 za prekid]";
		cout << _odgovori;
		char StringOdgovora[50];
		int i = 0;
		int counter = -1;

		int input = -1;
		while (input != 0)
		{
			cout << "Odgovor: ";
			cin >> input;

			StringOdgovora[i++] = (input) + '0';
			StringOdgovora[i++] = ',';
			StringOdgovora[i++] = ' ';
			counter++;
		}
		if (counter == 0) return false;
		StringOdgovora[i - 5] = '\0';
		char* t = AlocirajNizKaraktera(StringOdgovora);
		if (strcmp(t, FormirajStringTacnih()) == 0)
		{
			cout << "\n\t::INFO :: Odgovor je tacan!\n";
			return true;
		}
		cout << "\n\t::INFO :: Odgovor je netacan!\n";
		return false;
	}

	void Info() override
	{
		Pitanje::Info();
	}
};

//Pitanje koje može imati jedan tacan odgovor. Student bira jednu od ponudjenih opcija.
class MCSAPitanje : public MCMAPitanje
{
	int IndeksTacnog()
	{
		for (int i = 0; i < _odgovori.GetTrenutno(); i++)
		{
			if (_odgovori.GetElement2(i) == true) return i;
		}
		return -1;
	}

public:
	//Konstruktor
	MCSAPitanje(int id, const char* tekst, const char* oblast, double bodovi) : MCMAPitanje(id, tekst, oblast, bodovi)
	{
	}

	//Broj bodova se postavlja na vrijednost proslijedjenog parametra.

	//Preklopljene verzije funkcija za dodavanje i uklanjanje odgovora koje dozvoljavaju da samo 
	// jedan odgovor bude oznacen kao tacan.
	//Svi prethodno navedeni uslovi trebaju biti ispunjeni, ali u ovom slucaju sa iskljucivo jednim tacnim odgovorom.
	void DodajOdgovor(const char* odgovor, bool tacan) override
	{
		int brojTacnih = GetBrojTacnih();
		int brojOdgovora = _odgovori.GetTrenutno();
		int max = _odgovori.GetMax();

		if (brojTacnih > 1)
		{
			throw exception("\tERROR :: Pitanje moze imati samo jedan tacan odgovor!\n");
		}

		if (brojTacnih == 0 && brojOdgovora == max - 1 && !tacan)
		{
			throw exception("\tERROR :: Morate dodati najmanje 2 tacna odgovora!\n");
		}

		char* odg = AlocirajNizKaraktera(odgovor);
		_odgovori.Add(odg, tacan);
	}

	void UkloniOdgovor(int rb) override
	{
		rb--;
		delete[] _odgovori.GetElement1(rb);
		_odgovori.GetElement1(rb) = nullptr;
		_odgovori.Remove(rb);
		if (!Validno()) { cout << "\tINFO :: Odgovor uklonjen. Pitanje trenutno nema tacnog odgovora!\n"; }
	}

	bool Validno()
	{
		return GetBrojTacnih() == 1;
	}

	bool Start() override
	{
		cout << "\nOdaberite odgovor:\n";
		cout << _odgovori;
		int indeksOdg;
		cin >> indeksOdg;
		if (indeksOdg - 1 == IndeksTacnog())
		{
			cout << "\n\tINFO :: Odgovor je tacan!\n";
			return true;
		}
		cout << "\n\tINFO :: Odgovor je netacan!\n";
		return false;
	}

	void Info() override
	{
		Pitanje::Info();
	}
};

//Pojam je tip pitanja kod kojeg se definiše pojam zadat tekstom pitanja. Definicija/Opis pojma se tretira kao
//esejski dio pitanja, dok se karakteristike tog pojma postavljaju kroz MCMA tip pitanja.
class Pojam : public EsejskoPitanje, public MCMAPitanje
{
	bool _slozen; //pojam moze biti prost ili slozen
public:
	//Konstruktor
	//Jednostavno pitanje tipa Pojam nosi minimalno 3 boda, a ukoliko je rijec o slozenom pojmu minimalno 5 bodova.
	//Ukoliko navedeni uslov nije ispunjen baciti izuzetak.
	Pojam(bool slozen, int id, const char* tekst, const char* oblast,
	      const char* tacan, const char* odgovor)
	{
		_slozen = slozen;
		Pitanje::_id = id;
		Pitanje::_tekst = AlocirajNizKaraktera(tekst);
		Pitanje::_oblast = AlocirajNizKaraktera(oblast);
		if (_slozen) Pitanje::_bodovi = 5;
		else Pitanje::_bodovi = 3;
		_tacanOdgovor = AlocirajNizKaraktera(tacan);
		_odgovorStudenta = AlocirajNizKaraktera(odgovor);
	}

	//Dodati ostale potrebne funkcije za rad sa klasom
	void Info() override
	{
		Pitanje::Info();
	}
};

class Test
{
	char _brojIndeksa[10]; //broj indeksa studenta za kojeg je formiran test
	Pitanje* _pitanja[30]; // test studenta sadrzi najvise 30 pitanja razlicitog tipa
	double _maksimalnoBodova;
	double _osvojenoBodova;
	double _rezultat; //izrazen u procentima
	int ocjena;

	int GetTrenutno()
	{
		int trenutno = 0;
		for (size_t i = 0; i < 30; i++)
		{
			if (_pitanja[i] == nullptr) return trenutno;
			trenutno++;
		}
		return trenutno;
	}

public:
	//Omoguciti formiranje testa sa razlicitim tipom pitanja, te dodati funkcije za unos, provjeru tacnih odgovora i ocjenu studenta.
	//Po potrebi dopuniti i prethodne klase novim atributima/funkcijama.
	Test(const char* brojIndeksa = "170000", double max = 0)
	{
		strcpy_s(_brojIndeksa, brojIndeksa);
		_maksimalnoBodova = max;
		_osvojenoBodova = 0;
		_rezultat = 0;
		ocjena = 0;
		for (size_t i = 0; i < 30; i++)
			_pitanja[i] = nullptr;
	}

	void DodajPitanje(Pitanje* p)
	{
		const int trenutno = GetTrenutno();
		_pitanja[trenutno] = p;
		_maksimalnoBodova += p->GetBodove();
	}

	void PokreniTest()
	{
		system("CLS");
		cout << "\n\t Pokrece se test za studenta " << _brojIndeksa << "...\n";
		Sleep(1000);
		const int trenutno = GetTrenutno();
		for (int i = 0; i < trenutno; i++)
		{
			system("CLS");
			cout << "Pitanje " << i + 1 << " : ";
			_pitanja[i]->Info();
			if (_pitanja[i]->Start())
			{
				_osvojenoBodova += _pitanja[i]->GetBodove();
			}
			else _osvojenoBodova -= 2.78;
			Sleep(1000);
		}
		cout << "\n\tINFO :: Test je zavrsen! Osvojeni broj bodova: " << _osvojenoBodova << "/" << _maksimalnoBodova <<
			endl;
		_rezultat = _osvojenoBodova * 100 / _maksimalnoBodova;
		cout << "\tOsvojeni procenat: " << _rezultat << "%\n";
		if (_rezultat < 55) ocjena = 5;
		if (_rezultat >= 55 && _rezultat < 65) ocjena = 6;
		if (_rezultat >= 65 && _rezultat < 75) ocjena = 7;
		if (_rezultat >= 75 && _rezultat < 85) ocjena = 8;
		if (_rezultat >= 85 && _rezultat < 95) ocjena = 9;
		if (_rezultat >= 95) ocjena = 10;
		cout << "\t\t\t -> OCJENA : " << ocjena << " <-\n\n";
	}
};

int main()
{
	//Testirati sve implementirane funkcionalnosti programa. 
	//Kreirati najmanje jedan test sa 10 pitanja.

	EsejskoPitanje esejsko1(1001, "Sta je to rekurzija?", "Programiranje", 28.5, "Funkcija koja poziva samu sebe",
	                        "Zenica");
	EsejskoPitanje esejsko2(esejsko1);

	MCMAPitanje mcma1(1002, "Koja su osnovna obiljezja OOP?", "Programiranje", 21.5);
	MCSAPitanje mcsa3(1003, "Sta predstavlja objekat?", "Programiranje", 18.2);
	MCSAPitanje mcsa1(1003, "Da li pokazivac moze biti tipa void?", "Programiranje", 10.8);
	MCSAPitanje mcsa4(1003, "Da li je sljedeca sintaksa ispravna: \nint a;\nint b(--a);", "Programiranje", 21);
	try
	{
		mcma1.DodajOdgovor("POLIMORFIZAM", true);
		mcma1.DodajOdgovor("Rekurzija", false);
		mcma1.DodajOdgovor("Nasljedjivanje", true);
		mcma1.DodajOdgovor("Enkapsulacija", true);
		mcma1.DodajOdgovor("Klasa", false);
		mcma1.DodajOdgovor("Objekat i instanca", false);
		mcma1.DodajOdgovor("#include <iostream>", false);
		mcma1.DodajOdgovor("test", true);

		mcsa3.DodajOdgovor("3D prikaz neke klase", false);
		mcsa3.DodajOdgovor("Instancu neke klase", true);
		mcsa3.DodajOdgovor("Objekti ne postoje u programskom jeziku C++", false);
	}
	catch (exception& e) { cout << e.what() << endl << "Trenutno odgovora dodano -> " << mcma1.GetTrenutnoOdgovora(); }

	try
	{
		mcsa1.DodajOdgovor("NE", false);
		mcsa1.DodajOdgovor("DA", true);
		mcsa1.DodajOdgovor("Samo ako ga dereferenciramo", false);
		mcsa1.DodajOdgovor("Test", false);

		mcsa4.DodajOdgovor("DA", true);
		mcsa4.DodajOdgovor("NE", false);
	}
	catch (exception& e) { cout << e.what() << endl; }

	try
	{
		mcma1.UkloniOdgovor(8);
	}
	catch (exception& e) { cout << e.what() << endl; }

	mcma1.PromijeniOdgovorRB(1, "Polimorfizam", true);
	try
	{
		mcsa1.UkloniOdgovor(4); // Uklanjamo odgovor radi provjere
		mcsa1.UkloniOdgovor(99); // Ovdje baca izuzetak
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		Sleep(1500);
	}
	mcsa1.DodajOdgovor("Samo C++ ne podrzava void pokazivace", false); // Dodajemo 

	Test _test1("IB170094");
	_test1.DodajPitanje(&esejsko1);
	_test1.DodajPitanje(&mcma1);
	_test1.DodajPitanje(&mcsa1);
	_test1.DodajPitanje(&mcsa3);
	_test1.DodajPitanje(&mcsa4);

	_test1.PokreniTest();

	return 0;
}
