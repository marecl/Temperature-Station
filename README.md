# Temperature Station  
## I made this project for my father who needed to log activity of central heater  

Please use PCF8563_JSON version.  

If there is Error[500] use another browser (8563 donsent work with Edge but DS32321 does idk why)  
	
		!!!TODO!!!  

	Main sketch:  
		1. Add SSL support (including login page)  
		2. WWW based updater  
		3. Automatic daylight saving  
		4. Encrypt WiFi password   
		6. Display name if available on /sensors  
		7. Change name of sensor on /sensors (new name saved to json file)  
		8. Set date and time on the webpage  

	Czas.h library:  
		1. Make daylight saving automatic and move dls to private  
		2. Return actual date and time as epoch  

	Hardware:   
		1. Make full-SMD version (without NodeMCU, just ESP-12E)  