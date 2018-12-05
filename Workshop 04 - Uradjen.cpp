#include <iostream>
#include <iomanip>
using namespace std;

#define endline cout<<endl;
#define endlines cout<<endl<<endl;

char* AlocirajNizKaraktera(const char* v) {
	if (v == nullptr) return nullptr;
	char* temp = new char[strlen(v) + 1];
	strcpy_s(temp, strlen(v) + 1, v);
	return temp;
}

class Vrijeme
{
	int* _sati;
	int* _minute;
	int* _sekunde;
public:
	Vrijeme(int sati, int minute, int sekunde = 0) {
		_sati = new int(sati);
		_minute = new int(minute);
		_sekunde = new int(sekunde);
	}
	Vrijeme(const Vrijeme& right) {
		_sati = new int(*right._sati);
		_minute = new int(*right._minute);
		_sekunde = new int(*right._sekunde);
	}
	Vrijeme& operator=(const Vrijeme& right) {
		if (this != &right) {
			if (_sati != nullptr) delete _sati;
			if (_minute != nullptr) delete _minute;
			if (_sekunde != nullptr) delete _sekunde;
			_sati = new int(*right._sati);
			_minute = new int(*right._minute);
			_sekunde = new int(*right._sekunde);
		}
		return *this;
	}
	~Vrijeme() {
		delete _sati; _sati = nullptr;
		delete _minute; _minute = nullptr;
		delete _sekunde; _sekunde = nullptr;
	}
	int GetVrijemeUSekundama() const {
		int sekunde = *_sekunde;
		int minute = *_minute;
		int sati = *_sati;

		while (sati != 0) {
			sati--;
			minute += 60;
		}
		while (minute != 0) {
			minute--;
			sekunde += 60;
		}
		return sekunde;
	}

	void PovecajVrijeme(int minute) {
		*_minute += minute;
		while (*_minute > 60) {
			(*_sati)++;
			*_minute -= 60;
		}
		if (*_sati > 24) *_sati = 0;
	}
	Vrijeme operator+(int minute) {
		Vrijeme temp(*this);
		*temp._minute += minute;
		while (*temp._minute > 60) {
			(*temp._sati)++;
			*temp._minute -= 60;
		}
		if (*temp._sati > 24) *temp._sati = 0;
		return temp;
	}
	bool operator<(const Vrijeme& right) const {
		if (*this->_sati < *right._sati) return true;
		if (*this->_sati > *right._sati) return false;
		if (*this->_sati == *right._sati) {
			if (*this->_minute < *right._minute) return true;
			if (*this->_minute > *right._minute) return false;
			if (*this->_minute == *right._minute) {
				if (*this->_sekunde < *right._sekunde) return true;
				if (*this->_sekunde > *right._sekunde) return false;
				if (*this->_sekunde == *right._sekunde) return false;
			}
		}
	}
	bool operator>(const Vrijeme& right) const {
		return !(*this < right);
	}
	bool operator==(const Vrijeme& right) const {
		return *this->_sati == *right._sati &&
			*this->_minute == *right._minute &&
			*this->_sekunde == *right._sekunde;
	}

	//Preklopiti operator << za ispis ojekata tipa Vrijeme
	friend ostream& operator<<(ostream& cout, const Vrijeme& time) {
		return cout << *time._sati << ":" << *time._minute << ":" << *time._sekunde;
	}

};

class Let
{
	static const  int _oznakaSize = 10;
	char _oznaka[_oznakaSize];
	char* _odrediste;
	int _brIzlazneKapije;
	Vrijeme _vrijemePolijetanja; //sati i minute
	int _trajanje; //izraženo u minutama
	int _kasnjenje; //izraženo u minutama

public:
	//Potrebne konstruktor i destruktor funkcije
	Let(const char* ozn, const char* odrediste, int brKapije, int trajanje, Vrijeme v = Vrijeme(0, 0))
		: _vrijemePolijetanja(v) {
		strcpy_s(_oznaka, ozn);
		_odrediste = AlocirajNizKaraktera(odrediste);
		_brIzlazneKapije = brKapije;
		_trajanje = trajanje;
		_kasnjenje = 0;
	}
	Let(const Let& right) : _vrijemePolijetanja(right._vrijemePolijetanja) {
		strcpy_s(_oznaka, right._oznaka);
		if (_odrediste != nullptr) delete[] _odrediste;
		_odrediste = AlocirajNizKaraktera(right._odrediste);
		_brIzlazneKapije = right._brIzlazneKapije;
		_trajanje = right._trajanje;
		_kasnjenje = right._kasnjenje;
	}
	Let& operator=(const Let& right) {
		if (this != &right) {
			strcpy_s(_oznaka, right._oznaka);
			_odrediste = AlocirajNizKaraktera(right._odrediste);
			_brIzlazneKapije = right._brIzlazneKapije;
			_trajanje = right._trajanje;
			_kasnjenje = right._kasnjenje;
			_vrijemePolijetanja = right._vrijemePolijetanja;
		}
		return *this;
	}
	~Let() {
		delete[] _odrediste;
		_odrediste = nullptr;
	}

	char* GetOznaku() const { return (char*)_oznaka; }

	//Funkciju sa jednim parametrom koja postavlja informaciju o eventualnom kašnjenju na vrijednost zadanu parametrom.
	void SetKasnjenje(int kasnjenje) {
		_kasnjenje = kasnjenje;
	}

	/*Preklopiti unarni operator "!" putem kojeg je moguce saznati da li odgovarajuci let kasni ili ne (vratiti logicku
	vrijednost "true" u slucaju kašnjenja, u suprotnom vratiti "false").*/
	bool operator!() const {
		return _kasnjenje;
	}

	//Funkciju koja kao rezultat vraca trajanje leta.
	int GetTrajanje() const {
		return _trajanje + _kasnjenje;
	}

	/*Preklopiti operator + na nacin da omogucava sabiranje objekata tipa "Let" i cijelog broja, pri
	cemu se kao rezultat dobiva novi objekat tipa "Let", u kojem je vrijeme polijetanja pomjereno
	unaprijed za iznos odreden drugim sabirkom (racunato u minutama).
	Takoder preklopiti i operator "+=" koji osigurava da izraz oblika "X += Y uvijek ima isto znacenje kao i izraz
	"X = X + Y" */
	Let operator+(int minute) {
		Let novi(*this);
		novi._vrijemePolijetanja.PovecajVrijeme(minute);
		return novi;
	}
	Let& operator+=(int minute) {
		this->_vrijemePolijetanja.PovecajVrijeme(minute);
		return *this;
	}

	/* Funkciju kojom je moguce saznati ocekivano vrijeme polijetanja kada se uracuna iznos kašnjenja
	u odnosu na predvideno vrijeme polijetanja (preklopiti operator + u klasi Vrijeme). */
	Vrijeme GetProsjecnoVrijeme() {
		if (_kasnjenje == 0) return _vrijemePolijetanja;
		else {
			Vrijeme temp(_vrijemePolijetanja + _kasnjenje);
			return temp;
		}
	}
	Vrijeme GetVrijemeTrajanjaLeta() {
		int trajanje = _trajanje;
		int sati = 0, minuti = 0;
		while (trajanje > 60) {
			sati++;
			trajanje -= 60;
		}
		minuti = trajanje;
		return { sati, minuti };
	}

	//Funkciju koja vraca ocekivano vrijeme slijetanja
	Vrijeme GetVrijemeSlijetanja() {
		Vrijeme temp(_vrijemePolijetanja + _kasnjenje + _trajanje);
		return temp;
	}

	/*Preklopiti operator "++" na nacin da pomijera vrijeme polaska za jedan sat unaprijed.
	  Potrebno je podržati i prefiksnu i postfiksnu verziju ovog operatora.*/

	  // LET++ operator MORA imati jedan argument
	Let& operator++(int v) {
		this->_vrijemePolijetanja.PovecajVrijeme(60);
		return *this;
	}
	// ++LET operator NE SMIJE imati nijedan argument
	Let& operator++() {
		this->_vrijemePolijetanja.PovecajVrijeme(60);
		return *this;
	}

	/*Preklopiti relacione operatore "<" i ">" koji ispituju koji let nastupa ranije, odnosno kasnije.
	  Operator "<" vraca logicku vrijednost "true" ukoliko polijetanje leta sa lijeve strane nastupa
	  prije polijetanje leta sa desne strane, a u suprotnom vraca logicku vrijednost "false".
	  Analogno vrijedi za operator ">". Prilikom uporedivanja treba uzeti u obzir i ocekivano vrijeme kašnjenja,
	  a ne samo planirano vrijeme polijetanja.*/
	bool operator<(Let& right) {
		Vrijeme lijevo(this->_vrijemePolijetanja + this->_kasnjenje);
		Vrijeme desno(right._vrijemePolijetanja + right._kasnjenje);
		return lijevo < desno;
	}
	bool operator>(Let& right) {
		return !(this->_vrijemePolijetanja < right._vrijemePolijetanja);
	}

	/*Preklopiti operator "<<" koji treba da podrži ispis objekata tipa "Let" na ekran. U slucaju da
	se radi o polijetanju bez kašnjenja, ispis bi trebao da izgleda kako slijedi:

		JFK 156 Atalanta    12:50   19:30   5

	Podaci predstavljaju redom: oznaku leta, naziv odredišta, vrijeme polijetanja, ocekivano
	vrijeme slijetanja i broj izlazne kapije.

	U slucaju da se radi o letu koji kasni, ispis bi trebao izgledati kako slijedi:

		ZGK 932 Zagreb    15:50 (Planirano 15:30, Kasni 20 min)*/
	friend ostream& operator<<(ostream& cout, Let& let) {
		cout << let._oznaka << " " << " " << let._odrediste << "\t" << let.GetProsjecnoVrijeme() << "\t" << let.GetVrijemeSlijetanja() << "\t" << let._brIzlazneKapije;
		if (let._kasnjenje > 0) cout << "  [Kasni " << let._kasnjenje << " min]" << endl;
		else cout << endl;
		return cout;
	}
};


class RasporedLetova
{
private:
	int _brojRegistrovanihLetova;
	const int _maksimalanBrojLetova;
	Let** _letovi;

	void SortirajLetove() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (size_t i = 0; i < _brojRegistrovanihLetova - 1; i++)
			{
				if (_letovi[i]->GetProsjecnoVrijeme() < _letovi[i + 1]->GetProsjecnoVrijeme()) {

					Let* temp = new Let(*_letovi[i]);
					delete _letovi[i];
					_letovi[i] = new Let(*_letovi[i + 1]);

					delete _letovi[i + 1];
					_letovi[i + 1] = new Let(*temp);

					delete temp;
					temp = nullptr;

					promjena = true;
				}
			}
		}
	}
public:
	RasporedLetova(int maksimalanBrojLetova = 5) : _maksimalanBrojLetova(maksimalanBrojLetova),
		_letovi(new Let*[_maksimalanBrojLetova]), _brojRegistrovanihLetova(0)
	{}
	~RasporedLetova() {
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			delete _letovi[i];
			_letovi[i] = nullptr;
		}
		delete[] _letovi;
		_letovi = nullptr;
		_brojRegistrovanihLetova = 0;
	}

	/*Preklopiti operator "+=" na nacin da registruje novi let u raspored. Raspored letova u svakom momentu treba biti sortiran
	  prema vremenu polijetanja. Voditi racuna o maksimalnom broju letova.
	  Za potrebe poredenja vremena polaska letova preklopiti odgovarajuce operatore u klasi Vrijeme.*/
	void operator+=(const Let& let) {
		if (_brojRegistrovanihLetova == _maksimalanBrojLetova) {
			cout << "Dostigli ste max broj letova!\n";
			return;
		}

		_letovi[_brojRegistrovanihLetova++] = new Let(let);
		SortirajLetove();
	}
	void operator-=(const char* oznaka) {
		int indeks = -1;
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->GetOznaku(), oznaka) == 0) {
				indeks = i;
				break;
			}
		}
		if (indeks == -1) {
			cout << "\tERROR :: Oznaka nije validna!\n";
			return;
		}
		for (size_t i = indeks; i < _brojRegistrovanihLetova - 1; i++)
		{
			delete _letovi[i];
			_letovi[i] = nullptr;
			_letovi[i] = new Let(*_letovi[i + 1]);
			if (_letovi[i + 1] != nullptr) {
				delete _letovi[i + 1];
				_letovi[i + 1] = nullptr;
			}
		}
		_brojRegistrovanihLetova--;
	}
	/*Preklopiti operator "-=" na nacin da ukljanja registrovani let sa oznakom definisanom putem desnog operanda.
	 Pri tome sacuvati redoslijed prethodno registrovanih letova. */

	 //Preklopiti operator "[]" na nacin da vraca objekat tipa Let na osnovu proslijedene pozicije u rasporedu (pozicije krecu od 1).
	Let* operator[](int indeks) {
		indeks--;
		if (indeks < 0 || indeks > _brojRegistrovanihLetova)
			return nullptr;
		else return _letovi[indeks];
	}

	/*Preklopiti operator "()" na nacin da letu sa definisanom oznakom pomjeri vrijeme polijetanja za vrijednost drugog parametra izraženog
	 u minutama. */
	void operator()(const char* oznaka, int minute) {
		int indeks = -1;
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->GetOznaku(), oznaka) == 0) {
				indeks = i;
				break;
			}
		}
		if (indeks == -1) {
			cout << "\tERROR :: Oznaka nije validna!\n";
			return;
		}
		*_letovi[indeks] += minute;
	}


	//Funkciju koja daje prosjecno trajanje svih pohranjenih letova.
	float GetProsjecnoTrajanjeLetova() {
		float prosjek = 0;
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			int trenutno = _letovi[i]->GetVrijemeTrajanjaLeta().GetVrijemeUSekundama();
			prosjek += trenutno;
		}
		return abs((float)(prosjek / 60) / 60);		// vraca rezultat u satima
	}

	/*Preklopiti operator "<<" koji ispisuje kompletan spisak svih letova, sortiran po ocekivanim vremenima polazaka.
	  Za potrebe poredenja vremena polaska letova preklopiti odgovarajuce operatore u klasi Vrijeme. */
	friend ostream& operator<<(ostream& cout, const RasporedLetova& r) {
		cout << "\n\n\t:: RASPORED LETOVA ::\n\n";
		for (size_t i = 0; i < r._brojRegistrovanihLetova; i++)
			cout << *r._letovi[i];
		return cout;
	}

};

int main()
{
#pragma region Testiranje vremena

	Vrijeme vrijeme1(18, 40);
	Vrijeme vrijeme2(19, 45);
	vrijeme2 = vrijeme1;
	cout << vrijeme1 << endl << vrijeme2 << endl;
	Vrijeme vrijeme3(19, 45);
	cout << vrijeme3 << endl;

#pragma endregion
#pragma region Testiranje leta

	Let let("SAM", "Sarajevo - Mostar", 1, 120, vrijeme1);		// 18:40
	Let let2("SAT", "Sarajevo - Travnik", 2, 100, vrijeme3);
	Let let3("MSA", "Mostar - Sarajevo", 3, 85, vrijeme1);
	let3.SetKasnjenje(15);

	Let let_copy = let + 61;
	let.SetKasnjenje(4);	// let ima kasnjenje od 4 minute

	cout << let;
	cout << let_copy;		// pomjereno za 61 minutu

	let_copy += 30;			// pomjereno za dodatnih 30 minuta

	cout << let_copy;
	endline
		cout << "Prosjecno vrijeme polaska sa uracunatim kasnjenjem: " << let.GetProsjecnoVrijeme();
	endline
		cout << "Vrijeme slijetanja sa uracunatim kasnjenjem: " << setprecision(2) << let.GetVrijemeSlijetanja() << endl;
	endline

		cout << let;
	endline

		let++;
	++let;

	cout << "\n\tNakon povecanja za 2 sata:\n\n" << let;

#pragma endregion
#pragma region Testiranje rasporeda letova

	RasporedLetova raspored(5);
	raspored += let;
	raspored += let2;
	raspored += let3;

	raspored -= "SAT";

	Let* temp = raspored[1];
	if (temp != nullptr) cout << *temp;

	raspored("SAM", 15);		// SAM je na indeksu 1

	endlines;
	cout << "Prosjecno trajanje svih letova: " << raspored.GetProsjecnoTrajanjeLetova() << "h";
	endlines;

	cout << raspored;

#pragma endregion

	return 0;
}
