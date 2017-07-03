# This version is getting all updates  

It's using single JSON (saved as txt) file to store all settings.  

Features:  
	1. Single configuration file  
	2. Quite easy configuration  
	3. Quite good stability  

	Definitions:  
		* DHCPFILE [name] - when DHCP is set, there will be written obtained IP address  
		* SETTINGS_FILE [name] - I don't think it needs explaination  
		* NTPSERV [host name] - NTP server. Don't use pool.  
		* MAX_SENSORS (number) - how many sensors will be used (16 is enough I think)  

	SDfiles:  
		Copy everything to the root of SDcard.  
		Use SettingsGenerator to create settings file which is needed to run.  
		
	SETTINGS.TXT:  
	"sensor": this object stores sensor name and address. Address stored as decimal!  
			["sensor"][sensor number][0] - sensor name  
			["sensor"][sensor number][1-8] - sensor address as decimal  
	"valid_sensors": how many sensors are set (above)  
	"ip": (as in IP_SET but everything in quotes (""))  
	"wlan": table of many SSIDs and passphrases. Station will try to connect to every access point
			declared in file. Order of connecting is just as given.  
	"saved_ap": number of access points saved in file.  
	
Can be generated using SettingsGenerator and [ip]/sensors.  
Or you can do it yourself.  
SettingsGenerator has got validation mechanisms. They're working for me.  
When invalid address (sensor or IP) is entered it will ask you to write it again. Simple.  

After selecting dhcp mode Station will create file DHCP.TXT which will contain obtained IP address and so on.  

Order of logged temperatures is just as order in file.  
If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  

	Known bugs:
		1. Sometimes it's losing time (year 2147 or something like that) - very rare.  
		2. From time to time it's saving temperatures (many times) when it shouldn't - quite often  