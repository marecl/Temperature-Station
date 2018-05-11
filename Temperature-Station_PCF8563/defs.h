/*#define SD_D D0 //GPIO16
  #define SDA D1 //GPIO5
  #define SCL D2 //GPIO4
  #define OW_PORT D3 //GPIO0
  #define SD_CS D4 //GPIO2*/


#define SD_D A0
#define SDA 5
#define SCL 4
#define OW_PORT 2
#define SD_CS 16

#define countof(a) (sizeof(a) / sizeof(a[0]))

bool isMember(byte _1[], JsonArray & compArr);
int isMember(JsonArray & locArr, JsonObject & remObj);
String addrToString(uint8_t _addr[8]);
String addrToString(JsonArray& _addr);

const char setsFile[] PROGMEM = "/set.dat";
const char sensFile[] PROGMEM = "/sensors.txt";

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

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
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < 8; b++) {
      if (_1[b] != compArr[a]["a"][b].as<byte>())
        break;
      else if (b == 7) return true;
    }
  }
  return false;
}

int isMember(JsonArray & locArr, JsonObject & remObj) {
  for (uint8_t locsiz = 0; locsiz < locArr.size(); locsiz++) {
    for (uint8_t a = 0; a < 8; a++) {
      if (locArr[locsiz]["a"][a].as<byte>() != remObj["a"][a].as<byte>())
        break;
      else if (a == 7) return locsiz;
    }
  }
  return -1;
}


String addrToString(uint8_t _addr[8]) {
  String out = "";
  for (uint8_t a = 0; a < 8; a++)
    out += String(_addr[a]);
  return out;
}

String addrToString(JsonArray& _addr) {
  String out = "";
  for (uint8_t a = 0; (a < 8 && a < _addr.size()); a++)
    out += _addr[a].as<String>();
  return out;
}

void saveJson(JsonObject & toSave) {
  if (SD.exists(FPSTR(sensFile)))
    SD.remove(FPSTR(sensFile));
  File root = SD.open(FPSTR(sensFile), FILE_WRITE);
  toSave.prettyPrintTo(root);
  root.flush();
  root.close();
  return;
}

String printDateTime(Czas & timeobj) {
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
