# Temperature Station  
## I made this project for my father who needed to log activity of central heater  

DS3231 and PCF8563 versions are going to be updated much later but both work fine (not tested yet).  
Currently only PCF8563_JSON version is being updated. Please use this one.  

	THESE DEFINITIONS/FILES ARE NOT FOR JSON VERSION.  
	FOR JSON VERSION PLEASE SEE README IN THERE!  
	
	Definitions:  
		* DHCPFILE [name] - when DHCP is set, there will be written obtained IP address  
		* CONFIGFILE [name] - file with WiFi authentication credentials  
		* IPSETFILE [name] - IP obtaining settings    
		* TEMPLATE [name] - File containing sensor names and addresses  
		* NTPSERV [address] - NTP server address. IP or name. Can not be server pool  
		* MAX_SENSORS [number] - how many sensors can be connected (currently set to 16)  

	Folder SDfiles:  
		1. Copy everything to the root od SDcard  
		2. Enter data to PASS.TXT  
		3. Change settings in IP_SET.TXT  
		4. Some sketches may not support everything yet, I'm going to update them. Someday.  

	IP_SET:  
		mode=[static/dhcp]  
		ip=[ip]  
		gateway=[gateway]  
		subnet=[subnet]  
		
	TEMPLATE.TXT:  
		[NAME]=[full DS18B20 address as HEX]  
	Remember system reads first MAX_SENSORS rows.  

Logging order will be just like declared in file.  
This will log all valid sensors (i.e. if MAX_SENSORS is 16 and there are 5 valid data rows, these 5 will be logged)  

If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  

If there is Error[500] use another browser (this dosen't work with Edge)  

Every temperature log can be found by /archiwum/YYYYMM/DD.csv  
	
		!!!TODO!!!  

	Main sketch:  
		1.Add SSL support (including login page)  
		2.WWW based updater  
		3.Automatic daylight saving  
		4.Encrypt WiFi password   
		5.Enable/disable NTP sync (saved in JSON)  
		6.Display name if available on /sensors  
		7.Change name of sensor on /sensors (new name saved to json file)  
		8.Set date and time on the webpage  

	Czas.h library:  
		1.Access to variables ONLY via functions  
		2.Make proper destructor to free memory when work is done  
		3.Make daylight saving automatic and move dls to private  
		4.Return actual date and time as epoch  

	Hardware:  
		1.Update DS3231 and PCF8563 version to newest sketch  
		2.Make full-SMD version (without NodeMCU, just ESP-12E)  
