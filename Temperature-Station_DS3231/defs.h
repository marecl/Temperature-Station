#define SDA D1
#define SCL D2
#define OW_PORT D3
#define SD_CS D4
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' && tmp == '\n' && tmp == 255 && tmp == '\t' && tmp =='\0')
#define countof(a) (sizeof(a) / sizeof(a[0]))

#define CONFIGFILE "PASS.PWD"
#define IPSETFILE "IP_SET.TXT"
#define DHCPFILE "DHCP.TXT"
#define NTPSERV "tempus1.gum.gov.pl"
#define _temp1_ pokoj
#define _temp2_ nadworze
#define _temp3_ piec_wyjscie
#define _temp4_ piec_powrot

byte _temp1_[8] = {0x28, 0xFF, 0x3B, 0xBD, 0x72, 0x16, 0x05, 0x69};
byte _temp2_[8] = {0x28, 0xA5, 0xE2, 0x27, 0x00, 0x00, 0x80, 0x8A};
byte _temp3_[8] = {0x28, 0xFF, 0xE2, 0x33, 0x34, 0x16, 0x04, 0xB6};
byte _temp4_[8] = {0x28, 0xFF, 0xB0, 0xDC, 0x33, 0x16, 0x03, 0x8A};
double _t1_ = 0, _t2_ = 0, _t3_ = 0, _t4_ = 0;
String workfile = "TEMP.CSV";

bool dhcp = false;
bool gotip = false;
bool gotgate = false;
bool gotsub = false;
String sip = "";
String sgate = "";
String ssub = "";

unsigned int localPort = 2390;
const char* ntpServerName = NTPSERV;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
static bool hasSD = false;
static bool httpserver = false;
bool letni = true;

IPAddress stringToIP(String input) {
  int parts[4] = {0, 0, 0, 0};
  int part = 0;
  for (int a = 0; a < input.length(); a++) {
    char b = input[a];
    if (b == '.') {
      part++;
      continue;
    }
    parts[part] *= 10;
    parts[part] += b - '0';
  }
  return IPAddress(parts[0], parts[1], parts[2], parts[3]);
}

void deleteRecursive(String path) {
  File file = SD.open((char *)path.c_str());
  if (!file.isDirectory()) {
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
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
