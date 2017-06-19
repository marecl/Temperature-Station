#Temperature Station  
##I made this project for my father who needed to log central heating activity  

Definitions:
	* CONFIGFILE [name] - file with WiFi authentication credientals  
	* IPSETFILE [name] - IP obtaining settings  
	* TEMPLATE [name] - File containing sensor names and addresses  
	* NTPSERV [address] - NTP server address. IP or name. Can not be server pool  
	* MAX_SENSORS [number] - how many sensors can be connected (currently set to 16)  

Every temperature log can be found by /archiwum/YYYYMM/DD.csv  

Folder SDfiles  
	Copy everything to the root od SDcard  
	Enter data to PASS.TXT  
	Change settings in IP_SET.TXT  

IP_SET:  
	mode=[static/dhcp]  
	ip=[ip]  
	gateway=[gateway]  
	subnet=[subnet]  

TEMPLATE.TXT  
	[NAME]=[full DS18B20 address]  
Remember system reads first MAX_SENSORS rows.  
Logging order will be just like declared in file.  
This will log all valid sensors (i.e. if MAX_SENSORS is 16 and there are 5 valid data rows, these 5 will be logged)  

If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  
	
Please note everything must be correct, there are no validation mechanisms yet!  
After selecting dhcp mode it will create file DHCP.TXT which will contain obtained IP address and so on.  


DS3231 version is going to be updated much later but works fine.  

If there is Error[500] use another browser (this dosen't work with Edge)  
	
		!!!TODO!!!  

	1. One JSON instead of many configs  
	2. Hide important files (add SSL support)  
	3. WWW based updater  
	4. Automatic daylight saving  
	5. Update DS3231 version to newest sketch  
	6. Create program to associate sensors addresses and names (saved to template.txt)  
	7. Make full-SMD version (without NodeMCU, just ESP-12E)  	