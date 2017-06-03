#define CONFIGFILE [nazwa] - plik z którego odczytane zostana dane sieci WiFi (ssid i haslo)
#define IPSETFILE [nazwa] - plik z ustawieniami sieci
	!!! Nazwa max 8 znaków, rozszerzenie max 3 znaki !!!
#define NTPSERV [adres] - adres serwera z którego bedzie brana data (pojedynczy serwer - nie POOL)

#define _tempX_ [nazwa] nazwa dla danego termometru
byte _tempX_ = {[adres z one_wire_address_finder]}
double _tX_ - utworzenie zmiennej dla danego numeru czujnika

Every temperature log can be found by /archiwum/YYYYMM/DD.csv

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
	
Folder SDroot
	Wrzucic wszystko z folderu na karte SD.
	Uzupelnic plik PASS.TXT
	Zmienic ustawienia w IP_SET.TXT

IP_SET:
	mode=[static/dhcp]
	ip=[ip]
	gateway=[gateway]
	subnet=[subnet]
Please note everything must be correct, there are no validation mechanisms yet!
After selecting dhcp mode it will create file DHCP.TXT which will contain obtained IP address and so on.
	
DS3231 version is going to be updated much later.
	
		!!!Do zrobienia!!!
	1. Adresy czujek i nazwy brane z JSONa
	2. Ukrywanie wrazliwych plikow
	3. Aktualizacja softu ze strony WWW (httpUpdate)
	4. Naprawic edycje, upload i usuwanie plikow na stronie
	5. Auto czas letni
	6. Aktualizacja DS3231


	
	

