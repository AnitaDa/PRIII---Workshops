#include<iostream>
using namespace std;

char* AlocirajNizKaraktera(const char* t) {
	if (t == nullptr) return nullptr;
	char* temp = new char[strlen(t) + 1];
	strcpy_s(temp, strlen(t) + 1, t);
	return temp;
}

template<class T, int max>
class Kolekcija
{
	T* _elementi;
	int _trenutnoElemenata;
public:
	Kolekcija() : _elementi(new T[max]), _trenutnoElemenata(0) {}
	~Kolekcija() {
		delete[] _elementi;
		_elementi = nullptr;
	}
	Kolekcija(const Kolekcija& right) {
		_trenutnoElemenata = right._trenutnoElemenata;
		_elementi = new T[max];
		for (size_t i = 0; i < right._trenutnoElemenata; i++)
			_elementi[i] = right._elementi[i];
	}
	Kolekcija& operator=(const Kolekcija& right) {
		if (this != &right) {
			if (this->_elementi != nullptr) delete[] this->_elementi;
			this->_trenutnoElemenata = right._trenutnoElemenata;
			this->_elementi = new T[right._trenutnoElemenata];
			for (size_t i = 0; i < right._trenutnoElemenata; i++)
				this->_elementi[i] = right._elementi[i];
		}
		return *this;
	}

	void DodajElement(T element) {
		if (_trenutnoElemenata >= max) {
			cout << "\tERROR :: Dostigli ste maksimalan broj elemenata!\n";
			return;
		}
		_elementi[_trenutnoElemenata++] = element;
	}
	void UkloniElement(int indeks) {
		for (size_t i = indeks; i < _trenutnoElemenata - 1; i++)
			_elementi[i] = _elementi[i + 1];
		_trenutnoElemenata--;
	}
	T& GetElement(int indeks) const {
		return _elementi[indeks];
	}
	int GetTrenutno() const { return _trenutnoElemenata; }

	//Implementirati potrebne funkcije za rad sa kolekcijom.
	friend ostream& operator<<(ostream& cout, const Kolekcija<T, max>& k) {
		cout << "\nTrenutno elemenata: " << k._trenutnoElemenata << endl;
		for (size_t i = 0; i < k._trenutnoElemenata; i++)
			cout << "\t" << k._elementi[i] << endl;
		return cout;
	}
};

class Prisustvo
{
	char* _brojIndeksa;
	int _brojSati;
public:
	//Osnovne funkcije za rad s klasom
	Prisustvo(const char* brojIndeksa, int brojSati) : _brojSati(brojSati)
	{
		_brojIndeksa = AlocirajNizKaraktera(brojIndeksa);
	}
	Prisustvo(const Prisustvo& right) {
		_brojIndeksa = AlocirajNizKaraktera(right._brojIndeksa);
		_brojSati = right._brojSati;
	}
	Prisustvo() : _brojIndeksa(nullptr), _brojSati(0) {}

	Prisustvo& operator=(const Prisustvo& right) {
		if (this != &right) {
			if (this->_brojIndeksa != nullptr) delete[] this->_brojIndeksa;
			this->_brojIndeksa = AlocirajNizKaraktera(right._brojIndeksa);
			this->_brojSati = right._brojSati;
		}
		return *this;
	}

	char* GetBrojIndeksa() const { return _brojIndeksa; }
	int GetBrojSati() const { return _brojSati; }

	~Prisustvo()
	{
		delete[] _brojIndeksa;
		_brojIndeksa = nullptr;
	}

	//Preklopljeni operator + i += za uvecavanje broja sati prisustva za vrijednost proslijedenog parametra
	Prisustvo& operator+(int vrijednost) {
		this->_brojSati += vrijednost;
		return *this;
	}
	Prisustvo& operator+=(int vrijednost) {
		this->_brojSati += vrijednost;
		return *this;
	}
	bool operator==(const Prisustvo& right) const {
		return strcmp(_brojIndeksa, right._brojIndeksa) == 0;
	}
	bool operator<(const Prisustvo& right) {
		int lijevi = atoi(this->_brojIndeksa);
		int desni = atoi(right._brojIndeksa);
		return lijevi < desni;
	}
	bool operator>(const Prisustvo& right) {
		return !(*this < right);
	}

	friend ostream& operator<<(ostream& cout, const Prisustvo& p) {
		return cout << p._brojIndeksa << " :: " << p._brojSati << endl;
	}
};

class Nastava
{
	char _datum[12];
	char _tipNastave; //P ili V
	int _odrzanoSati;
	Prisustvo* _prisutni;
	int _brojStudenata;

	void SortirajPrisutne() {
			// KOPIRANJE INDEKSA U NIZ (Radi poredjenja)
		int* indeksi = new int[_brojStudenata];
		for (size_t i = 0; i < _brojStudenata; i++)
			indeksi[i] = atoi(_prisutni[i].GetBrojIndeksa());
		
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (size_t i = 0; i < _brojStudenata - 1; i++)
			{
				if (_prisutni[i] < _prisutni[i + 1]) {
					Prisustvo temp(_prisutni[i].GetBrojIndeksa(), _prisutni[i].GetBrojSati());
					_prisutni[i] = _prisutni[i + 1];
					_prisutni[i + 1] = temp;
					promjena = true;
				}
			}
		}
	}
	bool VecPostoji(const Prisustvo& p) {
		for (size_t i = 0; i < _brojStudenata; i++)
			if (_prisutni[i] == p) return true;
		return false;
	}
	int Validan(const char* indeks) {
		for (size_t i = 0; i < _brojStudenata; i++)
			if (strcmp(_prisutni[i].GetBrojIndeksa(), indeks) == 0) return i;
		return -1;
	}

public:
	//Konstruktor sa osnovnim atributima klase
	Nastava(const char* datum, char tip, int sati) : _prisutni(nullptr), _brojStudenata(0) {
		strcpy_s(_datum, 12, datum);
		_tipNastave = tip;
		_odrzanoSati = sati;
	}
	Nastava() {
		_prisutni = nullptr;
		_brojStudenata = 0;
	}
	Nastava(const Nastava& right) {
		strcpy_s(_datum, right._datum);
		_tipNastave = right._tipNastave;
		_odrzanoSati = right._odrzanoSati;
		_brojStudenata = right._brojStudenata;
		if (_brojStudenata == 0) _prisutni = nullptr;
		else _prisutni = new Prisustvo[_brojStudenata];
		for (size_t i = 0; i < _brojStudenata; i++)
			_prisutni[i] = right._prisutni[i];
	}
	char* GetDatum() const { return (char*)_datum; }
	char GetTip() const { return _tipNastave; }
	int GetOdrzanoSati() const { return _odrzanoSati; }
	int GetBrojStudenata() const { return _brojStudenata; }

	Nastava& operator=(const Nastava& right) {
		if (this != &right) {
			strcpy_s(_datum, right._datum);
			_tipNastave = right._tipNastave;
			_odrzanoSati = right._odrzanoSati;
			_brojStudenata = right._brojStudenata;
			if (_prisutni != nullptr) delete[] _prisutni;
			if (_brojStudenata == 0) _prisutni = nullptr;
			else _prisutni = new Prisustvo[_brojStudenata];
			for (size_t i = 0; i < _brojStudenata; i++)
				_prisutni[i] = right._prisutni[i];
		}
		return *this;
	}

	//Destruktor
	~Nastava() {
		delete[] _prisutni;
		_prisutni = nullptr;
	}
	Prisustvo& GetPrisustvo(int indeks) {
		return _prisutni[indeks];
	}

	//Preklopljeni operator+= za dodavanje objekata tipa Prisustvo u niz _prisutni.
	//Nije moguce dodati više puta prisustvo za istog studenta (preklopiti operator== u klasi Prisustvo).
	//Elementi u svakom momentu treba da budu sortirani po broju indeksa studenta (preklopiti operator < u klasi Prisustvo).
	void operator+=(const Prisustvo& novo) {
		if (VecPostoji(novo)) {
			cout << "\tERROR :: Prisustvo je vec evidentirano!\n";
			return;
		}
		Prisustvo* temp = new Prisustvo[_brojStudenata + 1];
		for (size_t i = 0; i < _brojStudenata; i++)
			temp[i] = _prisutni[i];
		temp[_brojStudenata] = novo;
		delete[] _prisutni;
		_prisutni = temp;
		_brojStudenata++;

		SortirajPrisutne();
	}

	//Preklopljeni operator-= za uklanjanje prisustva za studenta sa proslijedjenim brojem indeksa. Nakon uklanjanja,
	//potrebno je sacuvati redoslijed dodavanja, odnosno podaci ostaju sortirani po broju indeksa.
	void operator-=(const char* brojIndeksa) {
		int pozicija = Validan(brojIndeksa);
		if (pozicija == -1) {
			cout << "\tERROR :: Broj indeksa nije validan!\n";
			return;
		}
		int count = 0;
		Prisustvo* temp = new Prisustvo[_brojStudenata - 1];
		for (size_t i = 0; i < _brojStudenata; i++)
		{
			if (strcmp(_prisutni[i].GetBrojIndeksa(), brojIndeksa) == 0)
				continue;
			else
				temp[count++] = _prisutni[i];		
		}
		delete[] _prisutni;
		_prisutni = temp;
		cout << "\tINFO :: Student uspjesno uklonjen!\n";
		_brojStudenata--;
	}

	//Preklopljeni operator [] koji na osnovu broja indeksa provjerava da li je 
	//odredeni student prisustvovao nastavi i vraca njegove podatke o prisustvu.
	Prisustvo& operator[](const char* brojIndeksa) {
		int pozicija = Validan(brojIndeksa);
		if (pozicija == -1) throw exception("Student nije pronadjen!\n");
		if (_prisutni[pozicija].GetBrojSati() > 0) cout << "\nStudent je prisustvovao nastavi :: ";
		else cout << "Student nije prisustvovao na nastavi :: ";
		return _prisutni[pozicija];
	}

	//Preklopljeni operator () koji pronalazi studenta sa proslijedenim brojem indeksa i uvecava mu broj sati prisustva
	//za vrijednost drugog parametra. Ako je studentu vec evidentiran odrzani broj sati nastave prekinuti funkciju.
	void operator()(const char* brojIndeksa, int brojSati) {
		int pozicija = Validan(brojIndeksa);
		if (pozicija == -1) {
			cout << "\tERROR :: Broj indeksa nije validan!\n";
			return;
		}
		if (_prisutni[pozicija].GetBrojSati() > 0) {
			cout << "\tINFO :: Studentu su vec evidentirani sati!\n";
			return;
		}
		else {
			_prisutni[pozicija] += brojSati;
			cout << "\tINFO :: Sati uspjesno evidentirani!\n";
		}
	}

	//Preklopljeni operator + koji zbraja dva objekta tipa Nastava na nacin da kreira novi objekat na osnovu lijevog operanda
	//i dodaje mu prisutne studente desnog operanda. Funkciju napustiti ukoliko objekti koji ucestvuju u operaciji sabiranja
	//nisu istog tipa nastave. Sprijeciti dodavanje prisustva za istog studenta više puta.
	//Operator implementirati kao globalnu funkciju.
	
	friend Nastava operator+(const Nastava&, const Nastava&);
	friend ostream& operator<<(ostream& cout, const Nastava& n) {
		cout << "\n\t Trenutno studenata: " << n._brojStudenata << "\n\n";
		for (size_t i = 0; i < n._brojStudenata; i++)
			cout << n._prisutni[i];
		return cout;
	}
};

Nastava operator+(const Nastava& left, const Nastava& right) {
	Nastava nova(left);
	for (size_t i = 0; i < right._brojStudenata; i++)
		nova += right._prisutni[i];					// operator += svakako ne dozvoljava ponavljanje studenata
	return nova;
}

class Predmet
{
	char* _naziv;
	int _godinaStudija;
	Kolekcija<Nastava, 90> _odrzanaNastava;

	int Pronadji(const char* datum, char tip) {
		for (size_t i = 0; i < _odrzanaNastava.GetTrenutno(); i++)
		{
			if ((strcmp(_odrzanaNastava.GetElement(i).GetDatum(), datum) == 0) &&
				_odrzanaNastava.GetElement(i).GetTip() == tip) {
				return i;
			}
		}
		return -1;
	}
public:
	Predmet(const char* naziv, int godinaStudija) : _godinaStudija(godinaStudija)
	{
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Predmet()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}

	//Funkciju za evidenciju održane nastave na predmetu.
	void EvidentirajNastavu(const Nastava& n) {
		_odrzanaNastava.DodajElement(n);
	}

	/*Funkciju koja na osnovu datuma i tipa održane nastave dodaje prisustvo studentu sa proslijedenim brojem indeksa i brojem sati.
	Voditi racuna da broj sati prisustva ne može biti veci od broja sati održane nastave.*/
	void DodajPrisustvoStudentu(const char* datum, char tip, const char* brIndeksa, int brojSati) {
		int pronadjen = Pronadji(datum, tip);
		if (pronadjen == -1) {
			cout << "\tERROR :: Datum ili tip nastave nije validan!\n";
			return;
		}
		if (_odrzanaNastava.GetElement(pronadjen).GetOdrzanoSati() < brojSati) {
			cout << "\tERROR :: Nemoguce dodati broj sati prisustva veci od broja sati na nastavi!\n";
			return;
		}
		Prisustvo temp(brIndeksa, brojSati);
		_odrzanaNastava.GetElement(pronadjen) += temp;
	}

	/*Funkciju koja na osnovu datuma i tipa održane nastave uklanja prisustvo studentu sa proslijedenim brojem indeksa.*/
	void UkloniPrisustvoStudentu(const char* datum, char tip, const char* brIndeksa) {
		int nastava = Pronadji(datum, tip);
		if (nastava == -1) {
			cout << "\tERROR :: Datum ili tip nastave nije validan!\n";
			return;
		}
		_odrzanaNastava.GetElement(nastava) -= brIndeksa;
	}

	/*Funkciju koja na osnovu broja indeksa studenta vraca sve podatke o njegovom prisustvu na nastavi*/
	Prisustvo* GetPrisustvoStudenta(const char* brIndeksa) {
		Prisustvo* temp = nullptr;
		for (size_t i = 0; i < _odrzanaNastava.GetTrenutno(); i++)
		{
			for (size_t j = 0; j < _odrzanaNastava.GetElement(i).GetBrojStudenata(); j++)
			{
				if (strcmp(_odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojIndeksa(), brIndeksa) == 0)
				{
					char* brojIndeksa = AlocirajNizKaraktera(_odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojIndeksa());
					int brojPrisustva = _odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojSati();
					temp = new Prisustvo(brojIndeksa, brojPrisustva);
					break;
				}
			}
		}
		return temp;
	}

	/*Funkciju koja na osnovu tipa nastave (P ili V) vrši ispis prisustva za sve studente (sumarno) u datom formatu:
	IB140001 30/40 75%, gdje je prva kolona "broj indeksa", druga kolona "broj sati prisustva / ukupan broj sati održane nastave" i
	treca kolona "procenat prisustva studenta na nastavi".*/
	void Print(char tip) {
		for (size_t i = 0; i < _odrzanaNastava.GetTrenutno(); i++)
		{
			if (_odrzanaNastava.GetElement(i).GetTip() == tip) {
				for (size_t j = 0; j < _odrzanaNastava.GetElement(i).GetBrojStudenata(); j++) {
					cout << _odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojIndeksa();
					cout << "\t" << _odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojSati();
					cout << "/" << _odrzanaNastava.GetElement(i).GetOdrzanoSati();
					float procenat = _odrzanaNastava.GetElement(i).GetPrisustvo(j).GetBrojSati() * 100 / _odrzanaNastava.GetElement(i).GetOdrzanoSati();
					cout << "\t" << procenat << "%" << endl;
				}
			}
		}
	}

	friend ostream& operator<<(ostream&, const Predmet&);
};

ostream& operator<<(ostream& cout, const Predmet& p)
{
	cout << "Predmet: " << p._naziv << endl;
	cout << "Godina studija: " << p._godinaStudija << endl;

	return cout;
}

#define endline cout<<endl
#define endlines cout<<endl<<endl

int main()
{

#pragma region Testiranje kolekcije

	Kolekcija<int, 10> kolekcijaInt;
	for (size_t i = 0; i < 15; i++)
		kolekcijaInt.DodajElement(i);
	kolekcijaInt.UkloniElement(5);
	cout << kolekcijaInt << endl;

#pragma endregion
	//Testirati sve dostupne funkcionalnosti klasa Prisustvo i Nastava.
#pragma region Testiranje prisustva

	Prisustvo p1("170094", 0);
	Prisustvo p2("170095", 2);
	Prisustvo p3("170096", 4);
	Prisustvo p4("170097", 6);
	p4 = p4 + 7;
	cout << p4;		// p4 bi trebalo imati p4 + 7 = 13 sati prisustva
	p4 += 10;		// sada bi trebalo imati 23 sata prisustva
	cout << p4 << endl;

#pragma endregion
#pragma region Testiranje nastave

	Nastava n1("8.11.2018", 'P', 50);
	Nastava n2("11.4.2018", 'V', 20);
	Nastava n3("30.8.2018", 'V', 30);
	Nastava n4("29.3.2018", 'P', 35);
	n1 += p2;
	n1 += p4;
	n1 += p1;
	n1 += p3;
	n1 += p3;

	n1 -= "170095"; // uklanja se student p2
	n1 -= "170090"; // ne postoji

	n2 += p2;
	n2 += p4;

	n3 += p1;
	cout << n1;

	cout << n1["170096"];
	cout << n1["170094"];
	endline;

	n1("170096", 5);	// ne bi trebalo povecati jer vec ima evidentirane sate
	n1("170094", 6);	// ovom bi trebalo povecati jer nema evidentirane sate

	Nastava n1_n2 = n1 + n2;	// sada bi ova trebala imati 4 studenta jer n2 ima onog sto smo uklonili iz n1
	endline;
	cout << "\t-> Nastava n1 + n2 <-" << n1_n2 << endl;


#pragma endregion
#pragma region Testiranje predmeta

	Predmet programiranjeIII("Programiranje III", 2);
	programiranjeIII.EvidentirajNastavu(n1);
	programiranjeIII.EvidentirajNastavu(n2);

	programiranjeIII.DodajPrisustvoStudentu("8.11.2018", 'P', "170049", 1);			// Datum i tip su od nastave n1
	programiranjeIII.DodajPrisustvoStudentu("8.11.2018", 'P', "170085", 1);
	programiranjeIII.DodajPrisustvoStudentu("8.11.2016", 'P', "170049", 1);			// Datum nije validan

	programiranjeIII.UkloniPrisustvoStudentu("8.11.2018", 'P', "170049");
	programiranjeIII.UkloniPrisustvoStudentu("8.11.2018", 'P', "170049");			// Ne bi trebalo dva puta ukloniti

	Prisustvo* prisustvo = programiranjeIII.GetPrisustvoStudenta("170085");
	if (prisustvo != nullptr)
		cout << "\nTrazeno prisustvo: " << *prisustvo << endl;
	else cout << "\nTrazeno prisustvo: / Nije pronadjen /\n";
	endline;

	programiranjeIII.Print('P');
	endline;
	programiranjeIII.UkloniPrisustvoStudentu("8.11.2018", 'P', "170096");
	endline;
	programiranjeIII.Print('P');

#pragma endregion

	delete prisustvo;
	prisustvo = nullptr;

	return 0;
}
