class settingsHandler {
  public:
    settingsHandler();
    ~settingsHandler();
    void serialDebug(HardwareSerial*);
    void save(const char*);
    bool load(const char*);
    void config(const char*, const char*);
    void configAP(const char*, const char*);
    char* ssid();
    char* ssidAP();
    void ssid(const char*);
    void ssidAP(const char*);
    void configIP(IPAddress, IPAddress, IPAddress);
    void configIP(const char*, const char*, const char*);
    void useDHCP(bool);
    bool useDHCP();
    void name(const char*);
    char* name();
    void setPassword(const char*);
    void setUsername(const char*);
    char* username();
    bool authenticate(const char*, const char*);
    bool beginWiFi();
    void beginAP();
    void configUpdateServer(ESP8266WebServer*, ESP8266HTTPUpdateServer*);
    bool reset(const char*, const char*);
    IPAddress localIP();
    IPAddress gatewayIP();
    IPAddress subnetMask();

  private:
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
    IPAddress stringToIP(const char*);
    HardwareSerial *_debug;
    void setField(char*, const char*, uint8_t);
};
