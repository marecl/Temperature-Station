# Temperature Station  
## I made this project for my father who needed to log activity of central heater  

Please use PCF8563_JSON version.  
I2C expander is ATmega board which will extend amount of sensors that can be connected (WIP).  

If there is Error[500] use another browser (8563 donsent work with Edge but DS32321 does idk why)  
	
		!!!TODO!!!  

	Main sketch:  
		1. Add SSL support (including login page)  
		2. WWW based updater  
		3. Automatic daylight saving  
		4. Encrypt WiFi password   
		5. Set date and time on the webpage  
		6. Save each file as JSON and show as CSV  
		7. SettingsGenerator for everything but sensors

	Czas.h library:  
		1. Return actual date and time as epoch  

	Hardware:   
		1. Make full-SMD version (without NodeMCU, just ESP-12E)  