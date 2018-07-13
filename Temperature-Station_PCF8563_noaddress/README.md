# This version is getting all updates  

It's using single JSON (saved as text) file to store all settings.  

Warning: This is still under development.  
You will need to use LwIP 1.4 Higher Bandwidth to compile due to UDP problems.  
  
Features:  
	1. Single configuration file  
	2. (quite) Easy configuration 
	3. Stable

	SDfiles:  
		Copy index.htm to the root of SDcard.  
		
	SETTINGS.TXT:  
		*To Do*  
	
# Warning on first run! (workaround)   
If You need to get sensor addresses and You are generating settings for the first time:  
		1. Asked for name and address enter whatever you want    
		(write 000,000,000,000,000,000,000,000)
		2. Enter \quit.
		3. Configure network
			3.1. IP address will be saved to settings file  
		4. Connect to Your station [ip]/sensors   
		5. Remove 000,000(and so on) sensor and name sensors shown in table.  
		Checkbox checked - this sensor's name will be removed.  

SettingsGenerator has got validation mechanisms. They're working for me.  
When invalid address (sensor or IP) is entered it will ask you to write it again. Simple.  

Order of logged temperatures is just as order in file.  
If you change this file remember to rename your current log file.  
For logging changes will be applied immediately, but header will remain the same until new file is generated.  
 