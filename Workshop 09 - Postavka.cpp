#include<iostream>
using namespace std;

template<class T1, class T2>
class List {
	T1* _elementi1;
	T2* _elementi2;
	const int _max;
	int _trenutno;
public:
	List(int max) : _max(max)
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
	}

	T2& GetElement2(int index)
	{
		if (index >= 0 && index < _trenutno)
			return _elementi2[index];
		//U suprotnom baciti izuzetak

	}

	int GetTrenutno()const { return _trenutno; }

	friend ostream& operator<<(ostream&, const List&);
};

template<class T1, class T2>
ostream& operator<<(ostream& cout, const List<T1, T2>& list)
{
	for (int i = 0; i < list._trenutno; i++)
	{
		cout << list._elementi1[i] << "\t" << list._elementi2[i] << endl;
	}

	return cout;
}

class Pitanje
{
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

	virtual ~Pitanje() = 0
	{
		delete[] _tekst;
		_tekst = nullptr;

		delete[] _oblast;
		_oblast = nullptr;
	}

	virtual void Info()
	{
		cout << "Pitanje: " << _tekst << endl;
	}
};

//Pitanje na kojem studenti upisuju odgovore.
//Pohranjuje se tačan odgovor od strane edukatora.
class EsejskoPitanje : public Pitanje {
	char* _tacanOdgovor;
	char* _odgovorStudenta;
public:
	//Konstruktor i destruktor

	//Funkciju za izmjenu svih atributa klase
};

//Pitanje koje može imati više tačnih odgovora. Student bira ponudjene opcije. 
class MCMAPitanje : public Pitanje
{
	List<char*, bool> _odgovori; //ponudjeni odgovori sa oznakama tacnih odgovora
	char* _oznaceniOdgovoriStudenta; //cuvaju se redni brojevi (odvojeni zarezom) ponudjenih odgovora koje je odabrao student, npr. 1,2,6
public:

	//Napomena: Podatke o odgovorima cuvati u zasebnoj memoriji i istu dealocirati unutar same klase
	//Konstruktor i destruktor
	//Broj bodova inicijalno postaviti na 0 i uvecavati svakim oznacenim tacnim odgovorom.

	//Funkciju za izmjenu podataka o pitanju

	//Funkciju za dodavanje odgovora u listu koja treba da osigura da su najmanje 2 (od max) odgovora oznacena kao tacna.
	//Ukoliko se pokusa dodati i posljednji ili pretposljednji odgovor bez da je uslov ispunjen, baciti izuzetak. Dok se 
	//ne dodaju svi predvidjeni odgovori pitanje ne treba biti upotrebljivo ukoliko prethodni uslov nije ispunjen (ne prikazivati
	//pitanja na ispisu).

	//Funkciju za uklanjanje odgovora na osnovu rednog broja u listi. Ukoliko se uklanja odgovor koji je oznacen kao tacan i 
	//pri tome u listi ostaje samo još jedan odgovor oznacen kao tacan, pitanje postaje neupotrebljivo dok se ne doda jos jedan
	//tacan odgovor.

	//Funkciju za promjenu odgovora na odredjenom rednom broju

};

//Pitanje koje može imati jedan tačan odgovor. Student bira jednu od ponudjenih opcija.
class MCSAPitanje : public MCMAPitanje
{
public:
	//Konstruktor
	//Broj bodova se postavlja na vrijednost proslijedjenog parametra.

	//Preklopljene verzije funkcija za dodavanje i uklanjanje odgovora koje dozvoljavaju da samo jedan odgovor bude oznacen kao tacan.
	//Svi prethodno navedeni uslovi trebaju biti ispunjeni, ali u ovom slučaju sa iskljucivo jednim tacnim odgovorom.
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

	//Dodati ostale potrebne funkcije za rad sa klasom
};

class Test {
	char _brojIndeksa[10]; //broj indeksa studenta za kojeg je formiran test
	Pitanje* _pitanja[30]; // test studenta sadrzi najvise 30 pitanja razlicitog tipa
	double _maksimalnoBodova;
	double _osvojenoBodova;
	double _rezultat; //izrazen u procentima
	int ocjena;
public:
	//Omoguciti formiranje testa sa razlicitim tipom pitanja, te dodati funkcije za unos, provjeru tacnih odgovora i ocjenu studenta.
	//Po potrebi dopuniti i prethodne klase novim atributima/funkcijama.

};

int main()
{
	//Testirati sve implementirane funkcionalnosti programa. 
	//Kreirati najmanje jedan test sa 10 pitanja.

	return 0;
}

