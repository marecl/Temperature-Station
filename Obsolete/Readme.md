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

Every temperature log can be found by /archiwum/YYYYMM/DD.csv  