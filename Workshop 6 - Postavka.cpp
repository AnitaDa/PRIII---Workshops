#include<iostream>
using namespace std;

template<class T>
class Kolekcija{
	T* _elementi;
	int _trenutno;

	//Implementirati potrebne funkcije za rad sa generičkom klasom
};

class Clan
{
	const int _clanskiBroj; //automatski generisati i spriječiti dupliranje
	char*_imePrezime;
	char _telefon[20];
	int _brojIznajmljenihKnjiga;
public:

	Clan(char* imePrezime, char* telefon)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		strncpy_s(_telefon, telefon, _TRUNCATE);
		_brojIznajmljenihKnjiga = 0;

	}

	~Clan()
	{
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}

	friend ostream& operator<<(ostream&, const Clan&);
};

ostream& operator<<(ostream& cout, const Clan& c)
{
	cout << "\nPodaci o clanu\n";
	cout << "Clanski broj: " << c._clanskiBroj << endl;
	cout << "Ime i prezime: " << c._imePrezime << endl;
	cout << "Telefon: " << c._telefon << endl;

	return cout;
}

class Knjiga
{
	char _ISBN[20];
	char* _naslov;
	int _godinaIzdanja;
	Clan* _clan; //Podaci o članu kod kojeg se knjiga trenutno nalazi. Pohraniti samo adresu postojećeg člana.
	int _brojDanaZauzetosti;

	static bool ProvjeriISBN(char* ISBN)
	{
		//Funkcija vrši provjeru validnosti ISBN broja knjige
		//Podržani su 13-znamenkasti ISBN brojevi zapisani u formatu: 978-953-220-545-9
		//Pravilo: Broj treba da sadrži 4 znaka '-', bez slovnih oznaka (mala i velika slova)

		//Dodatno naknadno provjeriti da između pojedinih znakova '-' budu zapisana 3 broja, a 
		//iza posljednjeg samo jedan
	}

public:

	//Potrebne konstruktor i destruktor funkcije
	//Knjiga inicijalno nije zadužena

	//Funkciju ZaduziKnjigu koja treba da postavi podatke o clanu kod kojeg se knjiga trenutno nalazi
	//Voditi računa da se originalnom objektu tipa Clan uveća broj iznajmljenih knjiga za jedan.

	//Funkciju RazduziKnjigu koja knjigu označava kao slobodnu, odnosno uklanja podatke o clanu kod kojeg se ona trenutno nalazi.

	//Funkciju ProvjeriZaduzenost koja vraća logičku vrijednost "true" ukoliko je knjiga zadužena, u suprotnom vraća "false"

};

class Polica
{
	int& _brojPolice;
	int _kapacitet;
	Kolekcija<Knjiga> _knjige;
public:

	//Potrebne konstruktor i destruktor funkcije

	//Funkciju za dodavanje nove knjige na policu. Voditi računa o kapacitetu police prilikom dodavanja.

	//Funkciju za uklanjanje knjige sa police.

	//Info funkciju koja ispisuje podatke o svim slobodnim knjigama, svim zaduženim knjigama, i knjigama koje su na čitanju duže
	//od n dana, pri čemu se n zadaje kao parametar

};

class Bibilioteka
{
	char* _naziv;
	Kolekcija<Polica*> _police;
	Kolekcija<Clan*> _clanovi;
public:

	//Potrebne konstruktor i destruktor funkcije

	//Operator+= dodavanje novog člana biblioteke.

	//Funkciju za dodavanje nove police u biblioteku.

	//Operator() za dodavanje nove knjige na policu čiji je broj proslijeđen kao parametar.

	//Funkciju za uklanjanje knjige sa police čiji je broj proslijeđen kao parametar.

	//Funkciju za izdavanje knjige određenom članu. Kao parametri se šalju članski broj i ISBN broj knjige.
	//Provjeriti prvenstveno da li je član učlanjen u biblioteku.
	//Voditi računa o tome da li je knjiga već zauzeta. Član može imati najviše dvije zadužene knjige.

	//Funkciju koja razdužuje knjigu sa proslijeđenim ISBN brojem.

	//Operator++ koji povećava broj dana zauzetosti svih zaduženih knjiga za jedan.

	//Funkciju koja pronalazi i ispisuje sve slobodne knjige na određenoj polici (proslijediti broj police kao parametar).

	//Funkciju koja pronalazi knjigu/e na osnovu naslova i/ili pisca i ispisuje sve podatke, kao i to da li je knjiga/e trenutno zadužena.

	//Funkciju koja pronalazi sve knjige koje su zadužene duže od 14 dana i ispisuje podatke o članovima kod kojih se nalaze.
};

int main()
{
	//Testirati sve dostupne funkcionalnosti klase Biblioteka
	//Dodati najmanje 5 polica sa po 5 knjiga u objekat klase biblioteka, te učlaniti barem 10 članova.

	return 0;
}



