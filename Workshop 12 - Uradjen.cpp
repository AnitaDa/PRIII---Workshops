#include<iostream>
#include<string>
#include<regex>
#include<Windows.h>
#include<tuple>

using namespace std;

auto crt = "\n---------------------------------------------------------\n";

char* AlocirajKaraktere(char* t) {
	if (t == nullptr) return nullptr;
	char* te = new char[strlen(t) + 1];
	strcpy_s(te, strlen(t) + 1, t);
	return te;
}

//Koristiti klasu NedozvoljenaOperacija za bacanje izuzetaka u cijelom programu
class NedozvoljenaOperacija : public exception
{
	int _linijaKoda;
public:
	NedozvoljenaOperacija(const char* poruka, int linijaKoda) : exception(poruka)
	{
		_linijaKoda = linijaKoda;
	}

	void Info() const
	{
		cout << "Greska u liniji koda " << _linijaKoda << ": " << exception::what() << endl;
	}
};

template<class T1, class T2>
class FITKolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	//Konstruktor, destruktor i ostale funkcije potrebne za rad sa klasom
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& orig) {
		_trenutno = orig._trenutno;
		if (_trenutno == 0) { _elementi1 = nullptr; _elementi2 = nullptr; }
		else {
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (int i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = orig._elementi1[i];
				_elementi2[i] = orig._elementi2[i];
			}
		}
	}
	FITKolekcija& operator=(const FITKolekcija& right) {
		if (this != &right) {
			_trenutno = right._trenutno;
			if (_elementi1 != nullptr) delete[] _elementi1;
			if (_elementi2 != nullptr) delete[] _elementi2;
			if (_trenutno == 0) {
				_elementi1 = nullptr;
				_elementi2 = nullptr;
			}
			else {
				_elementi1 = new T1[_trenutno];
				_elementi2 = new T2[_trenutno];
				for (int i = 0; i < _trenutno; i++)
				{
					_elementi1[i] = right._elementi1[i];
					_elementi2[i] = right._elementi2[i];
				}
			}
		}
		return *this;
	}
	void AddElement(const T1& el1, const T2& el2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutno++;
	}
	pair<T1, T2> RemoveElementByIndex(int index) {

		if (index < 0 || index >= _trenutno) throw NedozvoljenaOperacija("Index nije validan!", __LINE__);

		T1 first = _elementi1[index];
		T2 second = _elementi2[index];

		T1* temp = new T1[_trenutno - 1];
		T2* temp2 = new T2[_trenutno - 1];

		int j = 0;
		for (int i = 0; i < _trenutno; i++)
		{
			if (i != index)
			{
				temp[j] = _elementi1[i];
				temp2[j++] = _elementi2[i];
			}
		}

		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp;
		_elementi2 = temp2;
		_trenutno--;

		return make_pair(first, second);
	}
	int GetTrenutno() const { return _trenutno; }
	bool FindT2(const T2& element) {
		for (int i = 0; i < _trenutno; i++)
			if (_elementi2[i] == element) return true;
		return false;
	}
	bool FindT1(const T1& element) {
		for (int i = 0; i < _trenutno; i++)
			if (_elementi1[i] == element) return true;
		return false;
	}
	int GetIndexT1(const T1& element) {
		for (int i = 0; i < _trenutno; i++)
			if (_elementi1[i] == element) return i;
		return -1;
	}
	int GetIndexT2(const T2& element) {
		for (int i = 0; i < _trenutno; i++)
			if (_elementi1[i] == element) return i;
		return -1;
	}
	T2 GetElement2(int index) {
		return _elementi2[index];	// Vec ima provjera prije poziva ove funkcije, tako da se ne moze desiti da index ne bude validan
	}
	T1 GetElement1(int index) {
		return _elementi1[index];
	}
	T1& GetElement1Ref(int index) {
		return _elementi1[index];
	}
	T2& GetElement2Ref(int index) {
		return _elementi2[index];
	}
	void SetElement2(int index, const T2& noviElement) {
		_elementi2[index] = noviElement;
	}
	void Set_all_t2(const T2& noviElement) {
		for (int i = 0; i < _trenutno; i++)
			_elementi2[i] = noviElement;
	}
	bool operator==(const FITKolekcija<T1, T2>& right) {

		if (_trenutno != right._trenutno) return false;

		bool isti = true;
		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] != right._elementi1[i]) isti = false;
			if (_elementi2[i] != right._elementi2[i]) isti = false;
		}

		return isti;
	}
};

enum Kategorija { GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge };
const char* KategorijaChar[] = { "Game Challenge", "Innovation Challenge", "Mobile Challenge", "SQL Challenge", "Coding Challenge" };

//Klasu proglasiti apstraktnom
class Osoba
{
protected:
	char* _ime;
	char* _prezime;
	int _godiste;
public:
	Osoba(const char* ime, const char* prezime, int godiste)
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		_godiste = godiste;
	}

	virtual ~Osoba()
	{
		delete[] _ime;
		_ime = nullptr;

		delete[] _prezime;
		_prezime = nullptr;
	}

	virtual char* GetIme() const = 0;
	virtual char* GetPrezime() const = 0;
	virtual int GetGodiste() const = 0;
	virtual void SetImePrezime(const char*, const char*) = 0;
	virtual void Print() const = 0;

	bool operator==(const Osoba& right) {
		return strcmp(_ime, right._ime) == 0
			&& strcmp(_prezime, right._prezime) == 0
			&& _godiste == right._godiste;
	}

};

//Prepoznati nasljedjivanje izmedju odgovarajuci klasa
//I ucesnici i clanovi komisije trebaju imati podatke za pristup sistemu (email i password)
class Ucesnik : public Osoba
{
	char* _email;
	string password; //Najmanje 5 karaktera, kombinacija brojeva i velikih slova
	string _obrazovnaInstitucija;

	static bool CheckPassword(string pw) {

		bool karakteri = false;
		bool brojevi = false;
		bool malaSlova = false;
		bool velikaSlova = false;

		pw.size() > 5 ? karakteri = true : karakteri = false;

		regex reg_pattern_charM("[a-z]+");
		regex reg_pattern_charV("[A-Z]+");
		regex reg_pattern_charN("[1-9]+");

		brojevi = regex_search(pw, reg_pattern_charN);
		malaSlova = regex_search(pw, reg_pattern_charM);
		velikaSlova = regex_search(pw, reg_pattern_charV);

		return brojevi && malaSlova && velikaSlova && karakteri;
	}

public:
	//Potrebne funkcije za rad sa klasom
	Ucesnik(const char* ime, const char* prezime, int godiste, const char* email, string pw, string ois)
		: Osoba(ime, prezime, godiste)
	{
		_email = AlocirajKaraktere((char*)email);
		if (CheckPassword(pw)) {
			cout << "\t\tPassword je validan!\n";
			password = pw;
		}
		else {
			cout << ime << ": Password nije validan!\n";
			password = "Default1";
		}
		_obrazovnaInstitucija = ois;
	}
	Ucesnik() : Osoba("Prazno", "Prazno", 1900) {
		_email = AlocirajKaraktere((char*)"Empty");
	}
	Ucesnik(const Ucesnik& right) : Osoba(right._ime, right._prezime, right._godiste)
	{
		_email = AlocirajKaraktere(right._email);
		password = right.password;
		_obrazovnaInstitucija = right._obrazovnaInstitucija;
	}
	Ucesnik& operator=(const Ucesnik& right) {
		if (this != &right) {
			this->SetImePrezime(right._ime, right._prezime);
			this->_godiste = right._godiste;
			if (_email != nullptr) delete[] _email;
			_email = AlocirajKaraktere(right._email);
			password = right.password;
			_obrazovnaInstitucija = right._obrazovnaInstitucija;
		}
		return *this;
	}
	~Ucesnik() {
		delete[] _email; _email = nullptr;
	}

	char* GetIme() const {
		return Osoba::_ime;
	}
	char* GetPrezime() const {
		return Osoba::_prezime;
	}
	int GetGodiste() const {
		return Osoba::_godiste;
	}
	void SetImePrezime(const char* ime, const char* prezime) {
		delete[] Osoba::_ime; Osoba::_ime = AlocirajKaraktere((char*)ime);
		delete[] Osoba::_prezime; Osoba::_prezime = AlocirajKaraktere((char*)prezime);
	}
	void Print() const {
		cout << "\t" << Osoba::_ime << " " << Osoba::_prezime << " - e-mail: " << _email << endl;
	}

	bool operator!=(const Ucesnik& right) {
		return !(*this == right);
	}
	bool operator==(const Ucesnik& right) {
		return strcmp(_ime, right._ime) == 0 &&
			strcmp(_prezime, right._prezime) == 0 &&
			_godiste == right._godiste;
	}

};

class ClanKomisije : public Osoba
{
	char* _pozicija; //Radno mjesto u firmi
	string _firma;
public:
	//Potrebne funkcije za rad sa klasom

	ClanKomisije(const char* ime = "", const char* prezime = "", int godiste = 2000, const char* pozicija = "", string firma = "")
		: Osoba(ime, prezime, godiste)
	{
		_pozicija = AlocirajKaraktere((char*)pozicija);
		_firma = firma;
	}
	~ClanKomisije() {
		delete[] _pozicija; _pozicija = nullptr;
	}
	ClanKomisije(const ClanKomisije& original)
		: Osoba(original._ime, original._prezime, original._godiste)
	{
		_pozicija = AlocirajKaraktere(original._pozicija);
		_firma = original._firma;
	}
	ClanKomisije& operator=(const ClanKomisije& right) {
		if (this != &right) {
			delete[] _ime;
			delete[] _prezime;
			delete[] _pozicija;
			_ime = AlocirajKaraktere(right._ime);
			_prezime = AlocirajKaraktere(right._prezime);
			_pozicija = AlocirajKaraktere(right._pozicija);
			_firma = right._firma;
			_godiste = right._godiste;
		}
		return *this;
	}
	char* GetIme() const {
		return Osoba::_ime;
	}
	char* GetPrezime() const {
		return Osoba::_prezime;
	}
	int GetGodiste() const {
		return Osoba::_godiste;
	}
	void SetImePrezime(const char* ime, const char* prezime) {
		delete[] Osoba::_ime; Osoba::_ime = AlocirajKaraktere((char*)ime);
		delete[] Osoba::_prezime; Osoba::_prezime = AlocirajKaraktere((char*)prezime);
	}
	void Print() const {
		cout << "\t" << Osoba::_ime << " " << Osoba::_prezime << " - pozicija: " << _pozicija << endl;
	}

	bool operator==(const ClanKomisije& right) {
		return strcmp(_pozicija, right._pozicija) == 0 && _firma == right._firma
			&& Osoba::operator==(right);
	}

};

//Prijave koje ne ukljucuju izradu i predaju projekata (kategorije: SQLChallenge i CodingChallenge)
class Prijava {
protected:
	string _datum; //Podrzani format: dd.MM.yyyy
	Kategorija _kategorija;
	FITKolekcija<Ucesnik, bool> _tim; //Tim se sastoji od jednog ili vise ucesnika, drugi paramtetar oznacava predstavnika tima
private:
	bool PostojiPredstavnik() {
		if (_tim.FindT2(true)) return true;
		return false;
	}
	bool VecPostoji(const Ucesnik& u) {
		if (_tim.FindT1(u)) return true;
		return false;
	}
	int GetIndexUcesnika(const Ucesnik& u) {
		return _tim.GetIndexT1(u);
	}

	static bool CheckDatum(string datum) {

		regex datum_pattern("([0-3])?([1-9]{1})(\\.{1})([0-1]?)([1-9]{1})(\\.)([[:digit:]]{4})(\\.)");

		if (regex_match(datum, datum_pattern)) return true;
		else return false;
	}


public:
	//Konstruktor
	Prijava(string datum, Kategorija k) {
		if (CheckDatum(datum)) {
			cout << "Datum je validan!\n";
			_datum = datum;
		}
		else {
			cout << "Datum nije validan!\n";
			_datum = "18.12.2017.";			// Neki random datum..
		}
		_kategorija = k;
	}

	//Funkciju za dodavanje clanova tima uz postivanje ogranicenja maksimalno dozvoljenog broja clanova u timu.
	//SQLChallenge i CodingChallenge: 2
	//GameChallenge i MobileChallenge: 3
	//InnovationChallenge: 4
	//Onemoguciti dodavanje istog ucesnika vise puta. Tim moze imati najvise jednog predstavnika.
	void DodajClanaTima(const Ucesnik& u, bool predstavnik) {

		if (predstavnik)
			if (PostojiPredstavnik())
				throw NedozvoljenaOperacija("Predstavnik vec postoji!", __LINE__);

		// RULES
		if (_kategorija == SQLChallenge || _kategorija == CodingChallenge)
			if (_tim.GetTrenutno() == 2)
				throw NedozvoljenaOperacija("CodingChallenge i SQLChallenge ne mogu imati vise od dva clana!", __LINE__);

		if (_kategorija == GameChallenge || _kategorija == MobileChallenge)
			if (_tim.GetTrenutno() == 3)
				throw NedozvoljenaOperacija("GameChallenge i MobileChallenge ne mogu imati vise od tri clana!", __LINE__);

		if (_kategorija == InnovationChallenge)
			if (_tim.GetTrenutno() == 4)
				throw NedozvoljenaOperacija("InnovationChallenge ne moze imati vise od cetiri clana!", __LINE__);

		if (VecPostoji(u))
			throw NedozvoljenaOperacija("Ucesnik vec postoji!", __LINE__);

		_tim.AddElement(u, predstavnik);
	}

	//Funkciju za promjenu predstavnika tima.
	virtual void PromijeniPredstavnika(const Ucesnik& ucesnik) {
		int index = GetIndexUcesnika(ucesnik);
		if (index == -1) throw NedozvoljenaOperacija("Ucesnik ne postoji!", __LINE__);
		if (_tim.GetElement2(index) == true)
			throw NedozvoljenaOperacija("Ucesnik je vec predstavnik tima!", __LINE__);
		_tim.Set_all_t2(false);
		_tim.SetElement2(index, true);
		cout << "Predstavnik uspjesno promijenjen!\n";
	}

	bool operator==(const Prijava& right) {
		return _datum == right._datum && _kategorija == right._kategorija && _tim == right._tim;
	}
	friend ostream& operator<<(ostream& cout, Prijava& p) {
		cout << endl << p._datum << "\t" << KategorijaChar[p._kategorija] << endl;
		cout << "\tTeam:\n";
		if (p._tim.GetTrenutno() == 0)
			cout << "\tTEAM JE PRAZAN.\n\n";
		for (int i = 0; i < p._tim.GetTrenutno(); i++)
		{
			cout << "\t\t" << i + 1 << ": " << p._tim.GetElement1(i).GetIme() << " " << p._tim.GetElement1(i).GetPrezime() << ", " <<
				p._tim.GetElement1(i).GetGodiste() << endl;
		}
		return cout;
	}

	Kategorija& GetKategoriju() { return _kategorija; }
	virtual void PredstaviPrijavu() {
		cout << crt << "Nasa prijava je objavljena datuma " << _datum << ", iz kategorije " << KategorijaChar[_kategorija] << ".\n";
		cout << "Nas tim se sastoji od sljedecih clanova:\n";
		for (int i = 0; i < _tim.GetTrenutno(); i++)
			_tim.GetElement1(i).Print();
		cout << "\nNadamo se da vam se svidio nas projekat. Unaprijed hvala!\n\n";
	}
	string GetDatum() const { return _datum; }
	virtual FITKolekcija<Ucesnik, bool>& GetTeam() {
		return _tim;
	}

};

//Prijave koje ukljucuju izradu projekata (kategorije: GameChallenge, InnovationChallenge i MobileChallenge)
class PrijavaProjekta : public Prijava {
	char* _nazivProjekta;
	string _opisProjekta;
	bool _finalizirano; //Atribut oznacava da li je projekat zavrsen i predan u predvidjenom roku
	bool _prezentacija; //Projekti koji se prezentuju i ocjenjuju
public:
	//Konstruktor i destruktor
	PrijavaProjekta(string datum, Kategorija k, const char* naziv, string opis, bool prez)
		: Prijava(datum, k)
	{
		_nazivProjekta = AlocirajKaraktere((char*)naziv);
		_opisProjekta = opis;
		_finalizirano = false;
		_prezentacija = prez;
	}
	virtual ~PrijavaProjekta() {
		delete[] _nazivProjekta; _nazivProjekta = nullptr;
	}
	PrijavaProjekta(const PrijavaProjekta& orig)
		: Prijava(orig._datum, orig._kategorija)
	{
		_nazivProjekta = AlocirajKaraktere(orig._nazivProjekta);
		_opisProjekta = orig._opisProjekta;
		_finalizirano = orig._finalizirano;
		_prezentacija = orig._prezentacija;
		_tim = orig._tim;
	}
	PrijavaProjekta& operator=(const PrijavaProjekta& right) {
		if (this != &right) {
			delete[] _nazivProjekta;
			_nazivProjekta = AlocirajKaraktere(right._nazivProjekta);
			_opisProjekta = right._opisProjekta;
			_finalizirano = right._finalizirano;
			_prezentacija = right._prezentacija;
			_datum = right._datum;
			_kategorija = right._kategorija;
			_tim = right._tim;
		}
		return *this;
	}

	//Ostale potrebne funkcije za rad sa klasom
	void SetFinalizirano(bool prezentacija) { _finalizirano = true; _prezentacija = prezentacija; }
	bool operator==(const PrijavaProjekta& right)
	{
		return strcmp(_nazivProjekta, right._nazivProjekta) == 0 && _opisProjekta == right._opisProjekta
			&& _finalizirano == right._finalizirano && _prezentacija == right._prezentacija
			&& Prijava::operator==(right);
	}
	friend ostream& operator<<(ostream& cout, const PrijavaProjekta& p) {
		cout << "Naziv projekta: " << p._nazivProjekta << endl;
		cout << "Opis projekta: " << p._opisProjekta << endl;
		cout << (Prijava&)p;
		return cout;
	}

	bool Finaliziran_Prezentiran() const { return _finalizirano && _prezentacija; }

	void PredstaviPrijavu() {
		string s = _finalizirano && _prezentacija ? " je finaliziran i prezentiran " : " nazalost, nije finaliziran ";
		cout << crt << "Postovani, nas naziv projekta je " << _nazivProjekta << ", i nas projekat" << s << endl;
		cout << "Nas projekat je iz kategorije " << KategorijaChar[_kategorija] << endl;
		cout << "Opis naseg projekta je " << _opisProjekta << endl;
		cout << "Nas tim se sastoji od sljedecih clanova:\n";
		for (int i = 0; i < _tim.GetTrenutno(); i++)
			_tim.GetElement1(i).Print();
		if (_finalizirano && _prezentacija) cout << "\nNadamo se da ce Vam se svidjeti nas projekat.\n";
	}
	FITKolekcija<Ucesnik, bool>& GetTeam() {
		return _tim;
	}
};

bool KategorijaSort(Prijava* left, Prijava* right) {
	return left->GetKategoriju() < right->GetKategoriju();
}

tuple<int, int, int> GetDMY(string datum) {

	vector<string> dmy(3);			// dmy - day month year
	int j = 0;

	for (int i = 0; i < datum.size(); i++)
	{
		if (datum[i] == '.')
			j++;
		else
			dmy[j] += datum[i];
	}

	return make_tuple(stoi(dmy[0]), stoi(dmy[1]), stoi(dmy[2]));
}

class FITCodingChallenge
{
	int godina;
	string _prijaveDo; //Podrzani format unosa: dd.MM.yyyy (koristiti regex)
	vector<Prijava*> _prijave;
	FITKolekcija<ClanKomisije, Kategorija> _komisija;
	FITKolekcija<Prijava*, FITKolekcija<ClanKomisije*, double>> _ocjene;

	static bool CheckDatum(string datum) {

		regex datum_pattern("([0-3])?([1-9]{1})(\\.{1})([0-1]?)([1-9]{1})(\\.)([[:digit:]]{4})(\\.)");

		if (regex_match(datum, datum_pattern)) return true;
		else return false;
	}
	bool ProvjeriRok(string datum) {

		auto prijaveDo = GetDMY(_prijaveDo);
		auto _datum = GetDMY(datum);

		if (get<2>(prijaveDo) > get<2>(_datum)) return true;
		if (get<2>(prijaveDo) < get<2>(_datum)) return false;
		if (get<2>(prijaveDo) == get<2>(_datum)) {
			if (get<1>(prijaveDo) > get<1>(_datum)) return true;
			if (get<1>(prijaveDo) < get<1>(_datum)) return false;
			if (get<1>(prijaveDo) == get<1>(_datum)) {
				if (get<0>(prijaveDo) > get<0>(_datum)) return true;
				if (get<0>(prijaveDo) < get<0>(_datum)) return false;
				if (get<0>(prijaveDo) == get<0>(_datum)) return false;
			}
		}
	}

	bool VecPostojiPrijava(const Prijava& prijava) {

		for (int i = 0; i < _prijave.size(); i++) {

			Prijava* no_const = const_cast<Prijava*>(&prijava);
			PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(no_const);
			PrijavaProjekta* pok2 = dynamic_cast<PrijavaProjekta*>(_prijave[i]);

			if (pok != nullptr && pok2 != nullptr) {
				if (*pok2 == *pok) return true;
			}
			else {
				if (*_prijave[i] == prijava) return true;
			}
		}

		return false;
	}
	int GetIndexPrijave(Prijava& prijava) {

		PrijavaProjekta* p = dynamic_cast<PrijavaProjekta*>(&prijava);
		if (p == nullptr) return -1;

		for (int i = 0; i < _prijave.size(); i++) {

			PrijavaProjekta* p1 = dynamic_cast<PrijavaProjekta*>(_prijave[i]);
			if (p1 != nullptr) {
				if (*p == *p1)
					return i;
			}
		}

		return -1;
	}
	bool CheckKomisiju(const ClanKomisije& clan, Kategorija k) {
		for (int i = 0; i < _komisija.GetTrenutno(); i++)
		{
			if (_komisija.GetElement2(i) == k) {
				if (_komisija.GetElement1(i) == clan)
					return true;
			}
		}
		return false;
	}
	void SortirajPoKategoriji() {
		sort(_prijave.begin(), _prijave.end(), KategorijaSort);
	}

public:
	//Konstruktor
	FITCodingChallenge(int godina, string _do) {
		this->godina = godina;
		if (CheckDatum(_do)) {
			cout << "Datum je validan!";
			_prijaveDo = _do;
		}
		else {
			cout << "Datum nije validan!\n";
			_prijaveDo = "19.12.2018.";		// Danasnji datum
		}
	}

	//Operator += za dodavanje novih prijava. Isti ucesnik ne moze imati vise prijava u sklopu iste kategorije.
	//Postovati ogranicenje roka za prijave (_prijaveDo).
	//Voditi racuna o alokaciji memorije za prijave (ne zadrzavati adresu proslijedjenog objekta)
	void operator+=(const Prijava& prijava) {

		if (!ProvjeriRok(prijava.GetDatum())) {
			cout << "Rok za prijavu je prosao!\n";
			return;
		}

		if (VecPostojiPrijava(prijava)) {
			cout << "\tPrijava vec postoji!\n";
			return;
		}

		Prijava* no_const = const_cast<Prijava*>(&prijava);
		PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(no_const);

		if (pok != nullptr) {
			_prijave.push_back(new PrijavaProjekta(*pok));
			cout << "\tPrijava projekta uspjesno dodana!\n";
		}

		else {
			_prijave.push_back(new Prijava(prijava));
			cout << "\tPrijava uspjesno dodana!\n";
		}
	}

	//Funkciju koja odredjenu prijavu oznacava kao finaliziranu i odabranu za prezentaciju ili ne.
	void OznaciPrijavuFinaliziranom(Prijava& prijava, bool prezentacija) {
		int i = GetIndexPrijave(prijava);
		if (i == -1) {
			cout << "\tGreska!\n";
			return;
		}
		PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(_prijave[i]);
		if (pok != nullptr) {
			pok->SetFinalizirano(prezentacija);
			cout << "\tPrijava oznacena finaliziranom!\n";
			return;
		}

	}

	//Operator () za dodavanje clanova komisije po kategorijama. Sprijeciti dodavanje istih clanova u sklopu jedne kategorije.
	void operator()(const ClanKomisije& clan, Kategorija k) {
		if (CheckKomisiju(clan, k)) {
			cout << "\tNemoguce dodati clana komisije! Clan je vec dodan!\n";
			return;
		}
		_komisija.AddElement(clan, k);
		cout << "Clan " << clan.GetIme() << " " << clan.GetPrezime() << " uspjesno dodan u komisiju!\n";
	}

	//Funkciju za ispis svih prijava po kategorijama. Funkcija neobavezno prima parametar kategoriju.
	//Ispis treba da ukljuci i podatke o ucesnicima, odnosno clanovima tima.
	//Ukoliko kategorija nije proslijedjena, podaci treba da budu grupisani po kategorijama.
	void IspisiPrijavePoKategorijama(Kategorija* k = nullptr) {

		if (k == nullptr) {
			SortirajPoKategoriji();

			for (int i = 0; i < _prijave.size(); i++)
			{
				PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(_prijave[i]);
				if (pok != nullptr)
					cout << crt << *pok;
				else
					cout << crt << *_prijave[i];
			}
		}
		else {
			for (int i = 0; i < _prijave.size(); i++)
			{
				PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(_prijave[i]);
				if (pok != nullptr) {
					if (pok->GetKategoriju() == *k)
						cout << crt << *pok;
				}
				else
					if (_prijave[i]->GetKategoriju() == *k)
						cout << crt << *_prijave[i];
			}
		}

	}

	//Funkciju za ocjenu projekata (radova) od strane clanova komisije (kolekcija _ocjene).
	//Kolekcija sadrzi samo pokazivace (bez alokacije nove memorije) na postojece prijave i vec definisane clanove komisije.
	//Voditi racuna da se ne mogu ocijeniti projekti koji nisu finalizirani i nisu odabrani za prezentaciju, niti da ocjenu mogu dati clanovi komisije koji
	//nisu pridruzeni odgovarajucoj kategoriji prijave. Svaki clan komisije daje ocjenu za svaki projekat, odnosno rad, u
	//kategoriji gdje je dodijeljen. Ocjene se krecu u rasponu od 1 do 10 za projekte, a za radove u kategoriji SQL ili CodingChallenge
	//u rasponu osvojenih bodova od 0 do 100. Pretpostaviti da su svi ucesnici prijavljeni u kategorijama SQL i CodingChallenge predali radove.
	void OcijeniRadove() {
		for (int i = 0; i < _prijave.size(); i++)
		{
			_prijave[i]->PredstaviPrijavu();
			for (int j = 0; j < _komisija.GetTrenutno(); j++)
			{
				if (_komisija.GetElement2(j) == _prijave[i]->GetKategoriju()) {
					PrijavaProjekta* pok = dynamic_cast<PrijavaProjekta*>(_prijave[i]);
					if (pok != nullptr) {
						if (pok->Finaliziran_Prezentiran()) {
							cout << "\nKolega " << _komisija.GetElement1(j).GetIme() << ", molimo vas da ocijenite rad: ";

							double ocjena = rand() % 100 + 0;
							//	cin >> ocjena;

							FITKolekcija<ClanKomisije*, double> temp;
							temp.AddElement(&_komisija.GetElement1Ref(j), ocjena);
							_ocjene.AddElement(_prijave[i], temp);
							Sleep(400);
						}
						else {
							cout << "Nazalost, rad ne moze biti ocijenjen od strane " << _komisija.GetElement1(j).GetIme() << endl;
							Sleep(500);
						}
					}
					else {
						cout << "\nKolega " << _komisija.GetElement1(j).GetIme() << ", molimo vas da ocijenite rad: ";

						double ocjena = rand() % 100 + 0;
						//	cin >> ocjena;

						FITKolekcija<ClanKomisije*, double> temp;
						temp.AddElement(&_komisija.GetElement1Ref(j), ocjena);
						_ocjene.AddElement(_prijave[i], temp);
						Sleep(400);
					}
				}
			}
		}
	}

	//Funkcija koja na osnovu imena i prezimena clana komisije ispisuje podatke o njegovim ocjenama ukljucujuci
	//prijave na koje se one odnose.
	void IspisiPodatkeClanaKomisije(const char* ime, const char* prezime) {
		bool postoji = false;
		for (int i = 0; i < _ocjene.GetTrenutno(); i++)
		{
			for (int j = 0; j < _ocjene.GetElement2(i).GetTrenutno(); j++)
			{
				if ((strcmp(_ocjene.GetElement2(i).GetElement1(j)->GetIme(), ime) == 0)
					&& (strcmp(_ocjene.GetElement2(i).GetElement1(j)->GetPrezime(), prezime) == 0))
				{
					postoji = true;
					cout << "\nZa prijavu " << i + 1 << "-" << KategorijaChar[_ocjene.GetElement1(i)->GetKategoriju()] <<
						", clan komisije " << ime << " " << prezime << " je dao ocjenu " << _ocjene.GetElement2(i).GetElement2(j);
				}
			}
		}
		if (!postoji)
			cout << "\tIme i prezime clana komisije nije validno!\n";
	}

	//Funkcija koja na osnovu imena i prezimena ucesnika ispisuje podatke o njegovim prijavama na takmicenju, zajedno sa 
	//ocjenama, ukoliko su one definisane za prijavu/e datog ucenika.
	void IspisiPodatkeUcesnika(const char* ime, const char* prezime) {
		cout << "\n\n";
		bool postoji = false;

		for (int i = 0; i < _ocjene.GetTrenutno(); i++)
		{
			for (int j = 0; j < _ocjene.GetElement1(i)->GetTeam().GetTrenutno(); j++) {

				if (strcmp(_ocjene.GetElement1(i)->GetTeam().GetElement1(j).GetIme(), ime) == 0
					&& strcmp(_ocjene.GetElement1(i)->GetTeam().GetElement1(j).GetPrezime(), prezime) == 0) {

					postoji = true;

					cout << "Ucesnik " << ime << " " << prezime << " se takmicio u kategoriji " << KategorijaChar[_ocjene.GetElement1(i)->GetKategoriju()] <<
						" sa ocjenama:\n ";

					for (int k = 0; k < _ocjene.GetElement2(j).GetTrenutno(); k++)
					{
						cout << "\t" << _ocjene.GetElement2(i).GetElement1(k)->GetIme() << " " <<
							_ocjene.GetElement2(i).GetElement1(k)->GetPrezime() << " : " <<
							_ocjene.GetElement2(i).GetElement2(k) << endl;
					}
				}
			}
		}
	}

	//Funkciju za rangiranje kandidata na osnovu kategorije proslijedjene kao parametar.
	//Funkcija treba da na osnovu ocjena clanova komisije ispise rang liste timova zajedno sa prosjecnim ocjenama njihovih projekata (radova).
	//Dakle, konacna rang lista ukljucuje nazive timova (u zavisnosti od kategorije i naziv projekta), te prosjecnu ocjenu.
	void RangLista(Kategorija kategorija) {

		cout << "\n\n\t\tRang lista za kategoriju " << KategorijaChar[kategorija] << ":\n\n";

		for (int i = 0; i < _prijave.size(); i++)
		{
			double suma = 0;
			int brojac = 0;
			for (int j = 0; j < _ocjene.GetTrenutno(); j++)
			{
				if (_prijave[i] == _ocjene.GetElement1(j)) {	// Porede se adrese
					for (int k = 0; k < _ocjene.GetElement2(j).GetTrenutno(); k++)
					{
						if (_ocjene.GetElement1(j)->GetKategoriju() == kategorija) {
							suma += _ocjene.GetElement2(j).GetElement2(k);
							brojac++;
						}
					}
				}
			}
			if (brojac != 0) {
				cout << "Team " << i + 1 << " - Osvojeni bodovi: ";
				double konacno = suma / brojac;
				cout << konacno << "%\n";
			}
		}
	}
};


int main()
{
	//Kreirati objekat klase FITCodingChallenge, te demonstrirati sve implementirane funkcionalnosti.

#pragma region Testiranje Kolekcije

	FITKolekcija<int, string> kolekcija;
	kolekcija.AddElement(170094, "Ajdin Hukara");
	kolekcija.AddElement(170097, "Enis Mulic");
	kolekcija.AddElement(170100, "Faruk Kadic");

	cout << "Trenutno elemenata u kolekciji: " << kolekcija.GetTrenutno() << endl;

	try {
		pair<int, string> par = kolekcija.RemoveElementByIndex(0);
		cout << "Uklonjen: " << par.first << " " << par.second << endl;
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	cout << "Trenutno elemenata u kolekciji: " << kolekcija.GetTrenutno() << endl;

#pragma endregion

#pragma region Ucesnik, clan komisije i prijava

	Ucesnik ajdin("Ajdin", "Hukara", 1996, "ajdin.hukara@edu.fit.ba", "helloworld", "FIT Mostar");	// Ne valja password
	Ucesnik enis("Enis", "Mulic", 1997, "enis.mulic@edu.fit.ba", "Helloworld2", "FIT Mostar");
	Ucesnik edin("Edin", "Sehovic", 1996, "seha_brat@edu.fit.ba", "hEllow0rld3", "FIT Mostar");
	Ucesnik faruk("Faruk", "Kadic", 1997, "faruk.kadic@gmail.com", "Password01", "FIT Mostar");
	Ucesnik dino("Dino", "Muharemovic", 1992, "dino_muha@hotmail.com", "PwP4417", "PMF Sarajevo");

	ajdin.Print();

	ClanKomisije denis("Denis", "Music", 1978, "Profesor", "Fakultet Informacijskih Tehnologija");
	ClanKomisije indira("Indira", "Hamulic", 1981, "Asistent", "Fakultet Informacijskih Tehnologija");
	ClanKomisije larisa("Larisa", "Dedovic", 1988, "Asistent", "Fakultet Informacijskih Tehnologija");
	ClanKomisije jasmin("Jasmin", "Azemovic", 1982, "Dekan", "Fakultet Informacijskih Tehnologija");
	ClanKomisije adel("Adel", "Handzic", 1989, "Asistent", "Fakultet Informacijskih Tehnologija");
	denis.Print();

	Prijava prijava("6.12.2018", CodingChallenge)	// Datum nije validan, fali tacka na kraju
		, prijava2("18.12.2018.", SQLChallenge);

	prijava.DodajClanaTima(ajdin, false);
	prijava.DodajClanaTima(enis, true);
	prijava2.DodajClanaTima(edin, true);
	prijava2.DodajClanaTima(enis, false);

	try {
		prijava.DodajClanaTima(edin, false);			// Ne moze imati vise od dva clana
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	try {
		prijava.DodajClanaTima(ajdin, true);			// Predstavnik vec postoji
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	try {
		prijava.PromijeniPredstavnika(enis);			// Vec je predstavnik
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	try {
		prijava.PromijeniPredstavnika(edin);			// Nije dodan
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	try {
		prijava.PromijeniPredstavnika(ajdin);			// Uspjesno promijenjen
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }

	Prijava novaPrijava("17.3.2017.", InnovationChallenge);
	Prijava novaPrijava2("23.7.2018.", GameChallenge);

	novaPrijava.DodajClanaTima(ajdin, true);
	novaPrijava.DodajClanaTima(enis, false);
	novaPrijava.DodajClanaTima(dino, false);
	novaPrijava2.DodajClanaTima(faruk, true);
	novaPrijava2.DodajClanaTima(dino, false);
	novaPrijava2.DodajClanaTima(edin, false);

#pragma endregion

#pragma region Prijava projekta

	PrijavaProjekta pprojekta("6.12.2018.", SQLChallenge, "Projekat 1", "Opisa bez", true);
	try {
		pprojekta.DodajClanaTima(enis, true);
		pprojekta.DodajClanaTima(edin, false);
		pprojekta.DodajClanaTima(ajdin, false);
	}
	catch (const NedozvoljenaOperacija& err) { err.Info(); }
	pprojekta.PromijeniPredstavnika(edin);

#pragma endregion

#pragma region FIT Coding Challenge

	FITCodingChallenge _cchallenge(2018, "15.1.2019.");
	Prijava _prosaoRok("19.2.2019.", SQLChallenge);	// datum prijave je poslije datuma _prijaveDo

	_cchallenge += prijava;
	_cchallenge += prijava2;
	_cchallenge += pprojekta;
	_cchallenge += novaPrijava;
	_cchallenge += novaPrijava2;
	_cchallenge += _prosaoRok;	// Rok za prijavu je prosao
	_cchallenge += pprojekta;	// Ne treba dodati
	_cchallenge += prijava;		// Ne treba dodati

	_cchallenge.OznaciPrijavuFinaliziranom(prijava2, true);		// Greska!
	_cchallenge.OznaciPrijavuFinaliziranom(pprojekta, true);

	_cchallenge(denis, SQLChallenge);
	_cchallenge(larisa, SQLChallenge);
	_cchallenge(indira, CodingChallenge);
	_cchallenge(denis, SQLChallenge);
	_cchallenge(jasmin, InnovationChallenge);
	_cchallenge(jasmin, GameChallenge);
	_cchallenge(adel, InnovationChallenge);

	_cchallenge.IspisiPrijavePoKategorijama();
	_cchallenge.IspisiPrijavePoKategorijama(new Kategorija(SQLChallenge));
	_cchallenge.IspisiPrijavePoKategorijama(new Kategorija(CodingChallenge));

	_cchallenge.OcijeniRadove();
	_cchallenge.IspisiPodatkeClanaKomisije("Ajdin", "Hukara");
	_cchallenge.IspisiPodatkeClanaKomisije("Denis", "Music");

	_cchallenge.IspisiPodatkeUcesnika("Ajdin", "Hukara");
	_cchallenge.IspisiPodatkeUcesnika("Faruk", "Kadic");
	_cchallenge.IspisiPodatkeUcesnika("Edin", "Sehovic");

	_cchallenge.RangLista(CodingChallenge);
	_cchallenge.RangLista(SQLChallenge);
	_cchallenge.RangLista(InnovationChallenge);
	_cchallenge.RangLista(GameChallenge);

#pragma endregion

	return 0;
}
