# This version is getting all updates  

It's using single JSON (saved as text) file to store all settings.  

Features:  
	1. Single configuration file  
	2. (quite) Easy configuration 
	3. Stable

	Definitions:  
		* SETTINGS_FILE [name] - I don't think it needs explaination  
		* MAX_SENSORS (number) - how many sensors will be used (16 is enough I think)  

	SDfiles:  
		Copy index.htm to the root of SDcard.  
		Use SettingsGenerator to create settings file which is needed to run.  
		You can fill SETTINGS_BLANK.txt using SETTINGS_SAMPLE.txt or simply copy and rename sample file.  
		
	SETTINGS.TXT:  
		"sensor": this object stores sensor name and address. Address stored as decimal!  
				["sensor"][sensor number][0] - sensor name  
				["sensor"][sensor number][1-8] - sensor address as decimal  
		"valid_sensors": how many sensors are set (above)  
		"ip": IP settings  
			"mode": dhcp, static or dhcp_stat. The third one saves DHCP settings and changes itself to static for one-time IP configuration.  
			"ip": desired/obtained IP address  
			"gateway": gateway  
			"subnet": subnet mask  
		"wlan": table of many SSIDs and passphrases. Station will try to connect to every access point declared in file. Order of connecting is just as given.  
				If you don't want to use WiFi fill ssid and password with -1 (if filling settings manually)
		"saved_ap": number of access points saved in file.  
		"ntp_server": Self explainatory but don't use pools.  
		"use_ntp": self explainatory, probably going to mature this later  
		"timezone": also self explainatory. 
	
# Warning on first run!   
If You need to get sensor addresses and You are generating settings for the first time:  
		1. Asked for name and address enter whatever you want    
		(remember address consists of 8 bytes; 8 integers smaller than 255 separated by commas)  
		2. Enter \quit to finish adding sensors.  
		3. Configure network
			3.1. IP address will be saved to settings file  
		4. Connect to Your station [ip]/sensors   
		5. Launch SettingsGenerator  
		6. Identify Your sensors by touching them or removing one of them (temperature change will be shown when page is refreshed!)  
		7. Give that sensor a name (maybe physically label sensor with that name and address) and copy-paste address from site to SettingsGenerator  

SettingsGenerator has got validation mechanisms. They're working for me.  
When invalid address (sensor or IP) is entered it will ask you to write it again. Simple.  

Order of logged temperatures is just as order in file.  
If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  

	Known bug: I have no idea why, but when I put my board on power supply (dosen't matter if turned on or off) it's going nuts and loses time. Pretty strange, need to make another board.  