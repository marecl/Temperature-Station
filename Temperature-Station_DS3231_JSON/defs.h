#define SD_D D0 //GPIO16
#define SDA D1  //GPIO5
#define SCL D2  //GPIO4
#define OW_PORT D3 //GPIO0
#define SD_CS D4 //GPIO2
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define SETTINGS_FILE (char*)"SETTINGS.TXT"

#define MAX_SENSORS 16

static bool httpserver = false;
bool use_ntp = false;

String json = "";
int zone = 0;
bool letni = true;
byte packetBuffer[48];

String workfile = "TEMP.CSV"; //If no time is available we will use this file
//I should make function which will automatically clean up records and move to correct files

int valid_sensors = 0;
int saved_ap = 0;
double _temps_[MAX_SENSORS];

IPAddress stringToIP(String input) {
  uint8_t parts[4] = {0, 0, 0, 0};
  uint8_t part = 0;
  for (uint8_t a = 0; a < input.length(); a++) {
    uint8_t b = input[a];
    if (b == '.') {
      part++;
      continue;
    }
    parts[part] *= 10;
    parts[part] += b - '0';
  }
  return IPAddress(parts[0], parts[1], parts[2], parts[3]);
}

String IPtoString(IPAddress address) {
  String out = String(address[0]) + "." + String(address[1]) + ".";
  out = out + String(address[2]) + "." + String(address[3]);
  return out;
}

String printDateTime(const RtcDateTime & dt) {
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute() );
  return datestring;
}