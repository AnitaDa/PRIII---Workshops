#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <Windows.h>

using namespace std;
	
class Kolekcija {		// Dodatno implementirano
	vector<char*> _rijeci;
public:
	void AddRijec(char* word) {
		_rijeci.push_back(word);
	}
	friend ostream& operator<<(ostream& cout, const Kolekcija& k) {
		for (int i = 0; i < k._rijeci.size(); i++)
			cout << k._rijeci[i] << "\t";
		return cout;
	}
};

class Izuzetak : public exception		// Dodatno implementirano
{
	int errLine;
public:
	Izuzetak(const char* poruka, int linijaKoda) : exception(poruka) {
		errLine = linijaKoda;
	}
	void WHAT() const {
		cout << "\t-> ERROR :: " << exception::what() << ", linija koda: " << errLine << endl;
	}
};

char* AlocirajNizKaraktera(const char* msg) {
	if (msg == nullptr) return nullptr;
	char* t = new char[strlen(msg) + 1];
	strcpy_s(t, strlen(msg) + 1, msg);
	return t;
}

class Term //Pojam
{
	char* _key;
	char* _value;

	void SetDefaultValues() {
		if (_key == nullptr) _key = AlocirajNizKaraktera("empty_key");
		if (_value == nullptr) _value = AlocirajNizKaraktera("empty_val");
	}

	void IspravanKey(const char* key) {
		for (int i = 0; i <= strlen(key); i++)
		{
			if (isdigit(key[i]) || isspace(key[i]) || ispunct(key[i]))
				throw Izuzetak("Neispravne vrijednosti parametara!", __LINE__);
		}
		_key = AlocirajNizKaraktera(key);
	}
	void IspravanValue(const char* value) {
		for (int i = 0; i <= strlen(value); i++)
		{
			if (isdigit(value[i]) || isspace(value[i]) || ispunct(value[i]))
				throw Izuzetak("Neispravne vrijednosti parametara!", __LINE__);
		}
		_value = AlocirajNizKaraktera(value);
	}

public:
	//Konstruktor
	//Atributi _key i _value ne smiju sadržavati brojeve, prazne prostore niti specijalne znakove: @, #, $.
	//U slucaju da vrijednosti proslijedenih parametara ne zadovolje postavljene uslove,
	//postaviti default-ne vrijednosti atributima klase i baciti izuzetak.
	Term(const char* key, const char* value) {
		_key = _value = nullptr;
		try {
			IspravanKey(key);
		}
		catch (const Izuzetak& ex) { ex.WHAT(); SetDefaultValues(); }

		try {
			IspravanValue(value);
		}
		catch (const Izuzetak& ex) { ex.WHAT(); SetDefaultValues(); }
	}
	Term(const Term& original) {
		_key = AlocirajNizKaraktera(original._key);
		_value = AlocirajNizKaraktera(original._value);
	}

	Term& operator=(const Term& right) {
		if (this != &right) {
			if (this->_key != nullptr) delete[] this->_key;
			this->_key = AlocirajNizKaraktera(right._key);
			if (this->_value != nullptr) delete[] this->_value;
			this->_value = AlocirajNizKaraktera(right._value);
		}
		return *this;
	}

	//Destruktor
	~Term() {
		delete[] _key; _key = nullptr;
		delete[] _value; _value = nullptr;
	}

	//Operator za ispis
	friend ostream& operator<<(ostream& cout, const Term& t) {
		return cout << "KEY: " << t._key << "\tVALUE: " << t._value;
	}

	//Operatori poredenja == i != (koristiti samo vrijednost atributa _key)
	bool operator==(const Term& right) {
		return strcmp(this->_key, right._key) == 0;
	}
	bool operator!=(const Term& right) {
		return !(*this == right);
	}
	bool operator<(const Term& right) const {
		int leftA = strlen(this->_key);
		int rightA = strlen(right._key);

		for (int i = 0; i < leftA < rightA? leftA : rightA; i++)
		{
			if (this->_key[i] < right._key[i]) return true;
			else if (this->_key[i] > right._key[i]) return false;
			else continue;
		}
	}
	char* GetKey() const { return _key; }
	char* GetValue() const { return _value; }
};

bool SortFunction(const Term& left, const Term& right) {
	return left < right;
}

class Dictionary //Rijecnik pojmova
{
	string _title;
	int _publicationYear;
	//Kljucne rijeci bosanskog jezika u atribut _key
	//Odgovarajuci prevod na engleskom jeziku u atribut _value
	vector<Term> _terms;

	void ValidTitle(string title) {
		if (!isupper(title[0])) throw Izuzetak("Naziv rjecnika nije validan!", __LINE__);
		_title = title;
	}
	void VecPostoji(const Term& term) {
		for (int i = 0; i < _terms.size(); i++)
			if (_terms[i] == term) throw Izuzetak("Pojam vec postoji!", __LINE__);
	}
	int FindKey(const char* key) {
		for (int i = 0; i < _terms.size(); i++)
			if (strcmp(_terms[i].GetKey(), key) == 0) return i;
		return -1;
	}
	
public:
	//Konstruktor
	//Naslov rijecnika treba da pocinje velikim slovom. U suprotnom baciti izuzetak i 
	//postaviti default-ne vrijednosti atributa klase.
	Dictionary(string title, int year) {
		try {
			ValidTitle(title);
		}
		catch (const Izuzetak& ex) { ex.WHAT(); _title = "Empty"; }
		_publicationYear = year;
	}

	//Operator () za dodavanje novih pojmova u rijecnik (u paru key-value).
	//Sprijeciti dodavanje istih vrijednosti u kolekciju (baciti izuzetak).
	//Pojmovi u svakom momentu treba da budu sortirani abecedno (koristiti funkciju sort iz biblioteke algorithm).
	void operator()(const Term& term) {
		try {
			VecPostoji(term);
		}
		catch (const Izuzetak& ex) { ex.WHAT(); }

		_terms.push_back(term);
		sort(_terms.begin(), _terms.end(), SortFunction);
	}

	//Operator -= za uklanjanje pojma iz rijecnika na osnovu atributa _key.
	void operator-=(const char* key) {
		int index = FindKey(key);
		if (index == -1) throw Izuzetak("Invalid key!", __LINE__);
		_terms.erase(_terms.begin() + index);
		cout << "\tKljuc " << key << " obrisan!\n";
	}

	//Funkciju koja vraca trenutni broj pojmova u rijecniku.
	int GetBrojPojmova() const { return _terms.size(); }

	//operator[] koji vraca vrijednost na osnovu kljucne rijeci (povratna vrijednost je engleska rijec-value, 
	//na osnovu bosanske-key)
	char* operator[](const char* key) {
		int index = FindKey(key);
		if(index == -1) throw Izuzetak("Invalid value!", __LINE__);
		return _terms[index].GetValue();
	}

	//Funkciju koja vraca kolekciju kljuceva (bosanskih rijeci) generisanih slucajnim odabirom.
	//Generisanje slucajnog broja izmedu min i max: int randNum = rand()%(max-min + 1) + min;
	//Funkcija kao parametar prima broj rijeci koje je potrebno generisati.
	Kolekcija GenerisiRijeci(int max) {

		if (max <= 0) throw Izuzetak("Value 'max' is not valid!", __LINE__);
		Kolekcija k;
		if (max > _terms.size()) 
			max = _terms.size();
	
		int v = _terms.size();

		int* positions = new int[v];
		for (int i = 0; i < max; i++)
		{
			positions[i] = rand() % v + 0;
			for (int j = 0; j < i; j++)
			{
				if (positions[i] == positions[j]) { i--; break; }
			}
		}

		for (int i = 0; i < max; i++)
		{
			int j = positions[i];
			char* temp = _terms[j].GetKey();
			k.AddRijec(temp);
		}
		
		return k;
	}

	void PrintTerms() const {
		for (int i = 0; i < _terms.size(); i++)
			cout << _terms[i] << endl;
	}

	vector<Term>& GetTerms() { return _terms; }

};

//Osmisliti i implementirati klasu Competitor (Takmicar) na nacin da se upotrebom objekta klase Dictionary
//svakom takmicaru dodijele odredena pitanja (rijeci bosanskog jezika) - koristiti implementiranu funkciju klase Dictionary.
//Svakom pitanju je potrebno pridružiti odgovor takmicara (kolekcija _odgovori).
//Obavezno implementirati funkciju koja vrši kalkulaciju osvojenih bodova (1 tacan odgovor = 1 bod).
//Po potrebi dopuniti klasu dodatnim atributima.
class Competitor //Takmicar
{
	string _ime;
	string _prezime;
	string _username; //ime.prezime
	string _password; //minimalno 6 karaktera, kombinacija velikih i malih slova, 
					  //te najmanje jedan specijalni karakter iz liste: #%$@?! 
	vector<string> _pitanja;
	vector<string> _odgovori;
	Dictionary& _dict;
	int osvojeniBodovi;
	int maxBodovi;
	bool uradjen;
	double procenat;

	string set_username(string ime, string prezime) {
		return ime + '.' + prezime;
	}
	bool check_password(string pw) {
		if (pw.size() < 6) return false;

		bool spec = false;
		bool digit = false;
		bool upper = false;
		bool lower = false;

		for (int i = 0; i < pw.size(); i++)
		{
			if (isupper(pw[i])) upper = true;
			if (islower(pw[i])) lower = true;
			if (isdigit(pw[i])) digit = true;
			if (ispunct(pw[i])) spec = true;
		}
		return spec && digit && upper && lower;
	}

public:
	Competitor(string password, Dictionary& dict) : _dict(dict) {
		system("CLS");
		cout << "Vase ime: ";
		string ime = "";
		getline(cin, ime);
		_ime = ime;

		cout << "Vase prezime: ";
		string prezime = "";
		getline(cin, prezime);
		_prezime = prezime;

		_username = set_username(ime, prezime);
		if (check_password(password)) _password = password;
		else _password = "123abC!";
		osvojeniBodovi = 0;
		maxBodovi = _dict.GetBrojPojmova();
		uradjen = false;
		procenat = 0;
	}
	void StartTest() {
		system("CLS");
		cout << "Ime i prezime kandidata: " << _ime << " " << _prezime << endl;
		cout << "Username: " << _username << endl << endl;
		vector<Term> _terms = _dict.GetTerms();
		for (int i = 0; i < 5/*_terms.size()*/; i++)
		{
			char temp[100];
			cout << "\tRijec: " << _terms[i].GetKey() << endl;
			_pitanja.push_back(_terms[i].GetKey());
			cout << "\tPrevod: ";
			cin.getline(temp, 100);
			char* t = AlocirajNizKaraktera(temp);
			_odgovori.push_back(t);
			if (strcmp(t, _terms[i].GetValue()) == 0) {
				cout << "\n\tTacno!\n";
				Sleep(300);
				osvojeniBodovi++;
			}
			else {
				cout << "\n\tNetacno!\n";
				Sleep(300);
			}
			delete[] t; t = nullptr;
		}
		system("CLS");
		cout << "\tOsvojeni bodovi: " << osvojeniBodovi << "/" << _pitanja.size() << endl;
		procenat = osvojeniBodovi * 100.0 / _pitanja.size();
		cout << "\tOsvojeni procenat: " << procenat << "%" << endl;
		Sleep(2000);
		uradjen = true;
	}
	void Results() {
		if (!uradjen) {
			cout << "\tTest mora biti uradjen!\n";
			return;
		}
		vector<Term> _terms = _dict.GetTerms();
		cout << "\n\n\t Odgovori ::\n\n";
		for (int i = 0; i < _pitanja.size(); i++)
		{
			cout << "Rijec: \t\t" << _terms[i].GetKey() << endl;
			cout << "Tacan odgovor: \t" << _terms[i].GetValue() << endl;
			cout << "Vas odgovor: \t" << _odgovori[i] << endl << endl;
		}
	}
	double GetProcenat() const { return procenat; }
	friend ostream& operator<<(ostream& cout, const Competitor& c) {
		return cout << "Najbolji takmicar: " << c._ime << " " << c._prezime << ", osvojeni procenat: " << c.procenat << "%" << endl;
	}
	string GetImePrezime() const { return _ime + ' ' + _prezime; }
};

Competitor& GetNajboljegTakmicara(vector<Competitor>& v) {
	int maxProcenat = v[0].GetProcenat();
	int index = 0;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i].GetProcenat() > maxProcenat) {
			maxProcenat = v[i].GetProcenat();
			index = i;
		}
	}
	return v[index];
}

int main()
{
	srand(time(nullptr));
	//Kreirati rijecnik pojmova.
	//Kreirati niz objekata tipa Competitor i simulirati takmicenje u poznavanju rijeci engleskog jezika.
	//Pronaci najboljeg medu takmicarima (najveci broj bodova).

#pragma region Term

	Term t("racunar", "computer");
	Term t1("monitor", "monitor");
	Term t2("telefon", "phone");
	Term t3("zvucnik", "speaker");
	Term t4("slika", "picture");
	Term t5("staklo", "glass");
	Term t6("kljuc", "key");
	Term t7("alat", "tool");

#pragma endregion

#pragma region Dictionary

	Dictionary d1("TITLE", 2018);
	d1(t); d1(t1);	d1(t2);	
	d1(t3);	d1(t4);	d1(t5);	
	d1(t6);	d1(t7);
	d1(Term("tastatura", "keyboard"));
	d1(Term("papir", "paper"));
	d1(Term("kamera", "camera"));
	d1(Term("ljepilo", "glue"));
	d1(Term("sapun", "soap"));
	d1(Term("pristup", "access"));
	d1(Term("rijec", "word"));
	d1(Term("ruka", "hand"));
	d1(Term("noga", "leg"));
	d1(Term("glavobolja", "headache"));

	d1 -= "racunar";
	d1 -= "slika";

	d1.PrintTerms();
	cout << "\nBroj pojmova u rijecniku :: " << d1.GetBrojPojmova() << endl;

	cout << d1["zvucnik"] << endl << endl;

	cout << "\tGENERISANE RIJECI:\n";
	Kolekcija k;
	try {
		k = d1.GenerisiRijeci(5);
	} catch (const Izuzetak& ex) { ex.WHAT(); }
	cout << k;

#pragma endregion

#pragma region Competitor

	vector<Competitor> _takmicari;
	_takmicari.push_back(Competitor("12Ajdin!", d1));
	_takmicari.push_back(Competitor("!!2Ajdin%", d1));
	_takmicari.push_back(Competitor("Senad1--", d1));

	for (int i = 0; i < _takmicari.size(); i++)
	{
		cout << "\t\tTakmicar " << i + 1 << endl;
		_takmicari[i].StartTest();
	}
	for (int i = 0; i < _takmicari.size(); i++)
	{
		cout << "REZULTATI :: Takmicar " << _takmicari[i].GetImePrezime() << endl;
		_takmicari[i].Results();
	}

	Competitor najbolji = GetNajboljegTakmicara(_takmicari);
	cout << najbolji << endl;

#pragma endregion

	return 0;
}
