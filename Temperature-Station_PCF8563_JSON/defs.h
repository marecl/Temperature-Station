#define SD_D D0 //GPIO16
#define SDA D1 //GPIO5
#define SCL D2 //GPIO4
#define OW_PORT D3 //GPIO0
#define SD_CS D4 //GPIO2
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define SETTINGS_FILE (char*)"SETTINGS.TXT"

#define MAX_SENSORS 16

static bool httpserver = false;
bool use_ntp = true;
int zone = 0;
bool letni = true;
String json = "";

byte packetBuffer[48];

String workfile = "TEMP.CSV";
int valid_sensors = 0;
int saved_ap = 0;

void bootFailHandler(int _code) {
  switch (_code) {
    default: return; break;
    case 1: Serial.println(F("No settings file!")); break;
    case 2: Serial.println(F("SD Card detected but cannot be initialized!"));
      break;
    case 3: Serial.print(F("No card inserted\n")); break;
    case 4: Serial.println(F("Invalid JSON file"));
      Serial.println(F("Come back with valid one. Rebooting...")); break;
    case 5: Serial.print(F("Could not connect to WiFi!\n"));
      Serial.print(F("Log mode only\n")); break;
  }
  delay(1000);
  ESP.restart();
}

bool isMember(byte _1[], JsonObject& compObj, int _size) {
  for (int a = 0; a < _size; a++) {
    for (int b = 0; b < 8; b++) {
      byte _2 = compObj["sensor"][a][b + 1];
      if (_1[b] != _2)
        break;
      else if (b == 7) return true;
    }
  }
  return false;
}

String addrToString(uint8_t _addr[8]) {
  String out = "";
  for (int a = 0; a < 8; a++)
    out += String(_addr[a]);
  return out;
}

void saveJson(JsonObject &toSave) {
  if (SD.exists(SETTINGS_FILE))
    SD.remove(SETTINGS_FILE);
  File root = SD.open(SETTINGS_FILE, FILE_WRITE);
  toSave.prettyPrintTo(root);
  root.flush();
  root.close();
}

String printDateTime(Czas& timeobj) {
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             timeobj.day(),
             timeobj.month(),
             timeobj.year(),
             timeobj.hour(),
             timeobj.minute());
  return datestring;
}

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
