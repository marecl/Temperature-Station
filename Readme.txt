#define TEMPFILE [nazwa] - plik do którego beda zrzucane temperatury (odczytywalne w excelu).
	Jesli brak, utworzy sie automatycznie.
#define CONFIGFILE [nazwa] - plik z którego odczytane zostana dane sieci WiFi (ssid i haslo)
#define IPFILE [nazwa] - plik do którego bedzie zrzucony otrzymany adres IP
	!!! Nazwa max 8 znaków, rozszerzenie max 3 znaki !!!
#define LOGINTERVAL [milisekundy] - co ile zrzuca temperatury do pliku
#define NTPSERV [adres] - adres serwera z którego bedzie brana data (pojedynczy serwer - nie POOL)

#define _tempX_ [nazwa] nazwa dla danego termometru
byte _tempX_ = {[adres z one_wire_address_finder]}
double _tX_ - utworzenie zmiennej dla danego numeru czujnika

void setup():
	root.print("Data;Godzina;[nazwa0];[nazwa1];...;[nazwaX]") - utworzenie naglówka tabeli z danymi

void loop():
	_tX_ = sensors.getTempC(_tempX_) - zrzucenie temperatury czujnika o numerze X
	root.print((String)_tX_ + ";") - zrzucenie temperatury do pliku (z przewidzeniem nastepnego elementu)
	root.print((String)_tX_ + "\r\n") - zrzucenie temperatury do pliku (ostatni element)
	
Definicje czujnikow:
	Mozna dodawac ile sie chce, pamietac o dopasowaniu adresu z nazwa (najlepiej spisac)
	Usuwac wszystkie z powyzszych lokalizacji.
	Pamietac zeby ostatni element konczyl "\r\n" zamiast ";"
	
Archiwizacja:
	/ARCHIWUM/YYYYMM/DD.CSV
	Codziennie przed pó³noc¹ kopiowany jest plik.
	Jeœli nie ma danch - nie kopiuje.
	
Folder Pliki_Karta_SD:
	Wrzucic wszystko z folderu na karte SD.
	Uzupelnic plik pass.txt
	
	
		!!!Do zrobienia!!!
	1. Adresy czujek i nazwy brane z JSONa
	2. Adres MAC, IP i dane sieci brane z jednego pliku
	3. Aktualizacja softu ze strony WWW (httpUpdate)
	4. Drzewo plikow z archiwum
	5. Zapis od razu w pliku docelowym!!!
	
	

