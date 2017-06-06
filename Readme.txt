#define CONFIGFILE [name] - file with WiFi authentication credientals
#define IPSETFILE [name] - IP obtaining settings
#define NTPSERV [address] - NTP server address. IP or name. Can not be server pool.

#define _tempX_ [nazwa] thermometer name
byte _tempX_ = {[thermometer address from 1w_addr_finder])
double _tX_ - temperature variable for specified sensor
Replace X with sensor ID (like temp1, temp2, tempFF etc.)

To add more than 4 sensors you need to check all SD related operations

Every temperature log can be found by /archiwum/YYYYMM/DD.csv

void loop():
	_tX_ = sensors.getTempC(_tempX_) - zrzucenie temperatury czujnika o numerze X
	root.print((String)_tX_ + ";") - zrzucenie temperatury do pliku (z przewidzeniem nastepnego elementu)
	root.print((String)_tX_ + "\r\n") - zrzucenie temperatury do pliku (ostatni element)
	

Folder SDfiles
	Copy everything to the root od SDcard
	Enter data to PASS.TXT
	Change settings in IP_SET.TXT 

IP_SET:
	mode=[static/dhcp]
	ip=[ip]
	gateway=[gateway]
	subnet=[subnet]
Please note everything must be correct, there are no validation mechanisms yet!
After selecting dhcp mode it will create file DHCP.TXT which will contain obtained IP address and so on.
	
DS3231 version is going to be updated much later but works fine.

If there is Error[500] use another browser (this dosen't work with Edge)
	
		!!!TODO!!!

	1.Sensor data taken from JSON
	2. Hide important files
	3. WWW based updater
	4. Automatic daylight saving
	6. Update DS3231 version to newest sketch


	
	

