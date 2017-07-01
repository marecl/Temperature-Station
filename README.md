# Temperature Station  
## I made this project for my father who needed to log activity of central heater  

<<<<<<< HEAD
	THESE DEFINITIONS/FILES ARE NOT FOR JSON VERSION.  
	FOR JSON VERSION PLEASE SEE README IN THERE!  
	
	Definitions:  
=======
	Definitions (not JSON version):  
>>>>>>> origin/master
		* DHCPFILE [name] - when DHCP is set, there will be written obtained IP address  
		* CONFIGFILE [name] - file with WiFi authentication credentials  
		* IPSETFILE [name] - IP obtaining settings    
		* TEMPLATE [name] - File containing sensor names and addresses  
		* NTPSERV [address] - NTP server address. IP or name. Can not be server pool  
		* MAX_SENSORS [number] - how many sensors can be connected (currently set to 16)  

<<<<<<< HEAD
	Folder SDfiles:  
		1. Copy everything to the root od SDcard  
		2. Enter data to PASS.TXT  
		3. Change settings in IP_SET.TXT  
		4. Some sketches may not support everything yet, I'm going to update them. Someday.  

=======
	JSON:  
		* DHCPFILE [name] - when DHCP is set, there will be written obtained IP address  
		* SETTINGS_FILE [name] - I don't think it needs explaination  

	Folder SDfiles:  
		Copy everything to the root od SDcard  
		Enter data to PASS.TXT  
		Change settings in IP_SET.TXT  
		Some sketches may not support everything yet, I'm going to update them. Someday.  

>>>>>>> origin/master
	IP_SET:  
		mode=[static/dhcp]  
		ip=[ip]  
		gateway=[gateway]  
		subnet=[subnet]  
<<<<<<< HEAD
		
	TEMPLATE.TXT:  
		[NAME]=[full DS18B20 address as HEX]  
=======

	TEMPLATE.TXT  
		[NAME]=[full DS18B20 address]  
>>>>>>> origin/master
	Remember system reads first MAX_SENSORS rows.  

Logging order will be just like declared in file.  
This will log all valid sensors (i.e. if MAX_SENSORS is 16 and there are 5 valid data rows, these 5 will be logged)  

If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  
	
<<<<<<< HEAD
=======
Please note everything must be correct, there are no validation mechanisms yet!  
After selecting dhcp mode it will create file DHCP.TXT which will contain obtained IP address and so on.  

	SETTINGS.TXT (JSON version only):  
		"sensor": this object stores sensor name and address. Address stored as decimal!  
				["sensor"][sensor number][0] - sensor name  
				["sensor"][sensor number][1-8] - sensor address  
		"valid_sensors": how many sensors are set (above)  
		"ip": (as in IP_SET but everything in quotes (""))  
		"wlan": (as in PASS.PWD, but again, everything in quotes (""))  
	
Can be generated using PC application and [ip]/sensors.  
Or you can do it yourself.  

>>>>>>> origin/master
DS3231 and PCF8563 versions are going to be updated much later but both work fine (not tested yet).  
Currently only PCF8563_JSON version is being updated. Please use this one.  

If there is Error[500] use another browser (this dosen't work with Edge)  

Every temperature log can be found by /archiwum/YYYYMM/DD.csv  
	
		!!!TODO!!!  

<<<<<<< HEAD
Main sketch:  
	1.Add SSL support (including login page)  
	2.WWW based updater  
	3.Automatic daylight saving  
	4.Encrypt WiFi password   
	5.Enable/disable NTP sync (saved in JSON)  
	6.Display name if available on /sensors  
	7.Change name of sensor on /sensors (new name saved to json file)  
	8.Move NTP server to JSON  
	9.Set date and time on the webpage  
	
PC program:   
	1.Add validation mechanisms to settings generator  
	
Czas.h library:  
	1.Access to variables ONLY via functions  
	2.Make proper destructor to free memory when work is done  
	3.Make daylight saving automatic and move dls to private  
	4.Return actual date and time as epoch  
	
Hardware:  
	1.Update DS3231 and PCF8563 version to newest sketch  
	2.Make full-SMD version (without NodeMCU, just ESP-12E)  
=======
	Main sketch:  
		1. Add SSL support (including login page)  
		2. WWW based updater  
		3. Automatic daylight saving  
		4. Encrypt WiFi password  
		5. Many WiFi stations credentials (if one fails, connects to another)   
		6. Display name if available on /sensors  
		7. Change name of sensor on /sensors (new name saved to json file)  
		8. Move NTP server to JSON  
		9. Set date and time on the webpage  
		10. Enable/disable NTP sync (saved in JSON)  

	PC program:   
		1. Add validation mechanisms to settings generator  

	Czas.h library:  
		1. Access to variables ONLY via functions  
		2. Make proper destructor to free memory when work is done  
		3. Make daylight saving automatic and move dls to private  
		4. Add date->epoch converter

	Hardware:  
		1. Update DS3231 version to newest sketch  
		2. Make full-SMD version (without NodeMCU, just ESP-12E)  
>>>>>>> origin/master
