#define SD_D D0 //GPIO16
#define SDA D1 //GPIO5
#define SCL D2 //GPIO4
#define OW_PORT D3 //GPIO0
#define SD_CS D4 //GPIO2
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define SETTINGS_FILE (char*)"SETTINGS.TXT"

void bootFailHandler(int _code) {
  switch (_code) {
    default: return;
      break;
    case 1: Serial.println(F("No sensors file!"));
      break;
    case 2: Serial.println(F("SD Card detected but cannot be initialized!"));
      break;
    case 3: Serial.print(F("No card inserted\n"));
      break;
    case 4: Serial.print(F("Could not connect to WiFi!\nLog mode only\n"));
      break;
  }
}

bool isMember(byte _1[], JsonArray & compArr) {
  for (int a = 0; a < compArr.size(); a++) {
    for (int b = 0; b < 8; b++) {
      byte _2 = compArr[a]["a"][b];
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

void saveJson(JsonObject & toSave) {
  if (SD.exists(SETTINGS_FILE))
    SD.remove(SETTINGS_FILE);
  File root = SD.open(SETTINGS_FILE, FILE_WRITE);
  toSave.prettyPrintTo(root);
  root.flush();
  root.close();
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
             dt.Minute());
  return datestring;
}

IPAddress stringToIP(char* input) {
  uint8_t parts[4] = {0, 0, 0, 0};
  uint8_t part = 0;
  for (uint8_t a = 0; a < strlen(input); a++) {
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
  String out;
  for (int z = 0; z < 4; z++) {
    out += String(address[z]);
    if (z < 3)out += ".";
  }
  return out;
}

bool toBool(String input) {
  if (input[0] == '1') return true;
  else return false;
}
