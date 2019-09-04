/*
   Library which is made to hold all settings of Temperature Station
   There is no way (I know) to make it return passwords / other sensitive data
*/

/*
  Insecure, uncomment only for debugging
  Will show every action in settingsHandler including passwords
*/
#define S_DEBUG

class settingsHandler {
  public:
    settingsHandler();
    ~settingsHandler();
    void save(const char*);
    bool load(const char*);
    void config(const char*, const char*);
    void configAP(const char*, const char*);
    void ssid(const char*);
    void ssidAP(const char*);
    void configIP(IPAddress, IPAddress, IPAddress);
    void configIP(const char*, const char*, const char*);
    void useDHCP(bool);
    bool useDHCP();
    void name(const char*);
    void configUser(const char*, const char*);
    bool authenticate(const char*, const char*);
    bool beginWiFi();
    bool beginAP();
    void beginOTA(uint16_t = 8266);
    bool webAuthenticate(ESP8266WebServer*);
    bool reset(const char*, const char*, const char*);
    IPAddress localIP();
    IPAddress gatewayIP();
    IPAddress subnetMask();
    const char* ssid();
    const char* ssidAP();
    const char* name();
    const char* username();
    String IPtoString(IPAddress);
    IPAddress stringToIP(const char*);
#ifdef S_DEBUG
    void serialDebug(HardwareSerial*);
#endif

  private:
    void setField(char*, const char*, uint8_t);
    char* _name;
    char* _ssid;
    char* _pass;
    char* _passap;
    char* _ssidap;
    char* _user;
    char* _pwd;
    bool _dhcp;
    IPAddress _ip;
    IPAddress _gw;
    IPAddress _mask;
#ifdef S_DEBUG
    HardwareSerial *_debug;
#endif
};
