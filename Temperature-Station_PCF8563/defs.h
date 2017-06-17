#define SD_D D0
#define SDA D1
#define SCL D2
#define OW_PORT D3
#define SD_CS D4
#define rtcadd 0x51
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' && tmp == '\n' && tmp == 255 && tmp == '\t' && tmp =='\0')
#define countof(a) (sizeof(a) / sizeof(a[0]))

#define CONFIGFILE "PASS.PWD"
#define IPSETFILE "IP_SET.TXT"
#define DHCPFILE "DHCP.TXT"
#define TEMPLATE "TEMPLATE.TXT"
#define NTPSERV "tempus1.gum.gov.pl"
#define _TIMEZONE_ 1 //UTC +1

#define _temp1_ pokoj
#define _temp2_ nadworze
#define _temp3_ piec_wyjscie
#define _temp4_ piec_powrot
byte _temp1_[8] = {0x28, 0xFF, 0x3B, 0xBD, 0x72, 0x16, 0x05, 0x69};
byte _temp2_[8] = {0x28, 0xA5, 0xE2, 0x27, 0x00, 0x00, 0x80, 0x8A};
byte _temp3_[8] = {0x28, 0xFF, 0xE2, 0x33, 0x34, 0x16, 0x04, 0xB6};
byte _temp4_[8] = {0x28, 0xFF, 0xB0, 0xDC, 0x33, 0x16, 0x03, 0x8A};

static bool hasSD = false;
static bool httpserver = false;

bool letni = true;

bool dhcp = false;
bool gotip = false;
bool gotgate = false;
bool gotsub = false;
String sip = "";
String sgate = "";
String ssub = "";

const char* ntpServerName = NTPSERV;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

unsigned int localPort = 2390;

double _t1_ = 0, _t2_ = 0, _t3_ = 0, _t4_ = 0;
String workfile = "TEMP.CSV";
bool usetemplate = false;

const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int czas[7]; //sek,min,godz, dzmsc,dztyg,msc,rok
byte bcdToDec(byte value) {
  return ((value / 16) * 10 + value % 16);
}

byte decToBcd(byte value) {
  return (value / 10 * 16 + value % 10);
}

void readPCF8563() {
  Wire.beginTransmission(0x51);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x51, 7);
  czas[0] = bcdToDec(Wire.read() & B01111111); // sek
  czas[1] = bcdToDec(Wire.read() & B01111111); // min
  czas[2] = bcdToDec(Wire.read() & B00111111); //godz
  czas[3] = bcdToDec(Wire.read() & B00111111); //dz msc
  czas[4] = bcdToDec(Wire.read() & B00000111); //dz tyg
  czas[5] = bcdToDec(Wire.read() & B00011111); // msc
  czas[6] = bcdToDec(Wire.read()); //rok
  czas[6] += 2000;
}

void setPCF8563(uint32_t t) {
  t -= 946684800UL;
  t = t + (3600 * _TIMEZONE_);
  if (letni) t += 3600; //Czas letni
  uint8_t ss = t % 60;
  t /= 60;
  uint8_t mm = t % 60;
  t /= 60;
  uint8_t hh = t % 24;
  uint16_t d = t / 24;
  int dt = (d % 7) - 1;
  uint8_t przest = 0;
  int r;
  int m;
  for (r = 0; ; ++r) {
    przest = r % 4 == 0;
    if (d < 365 + przest) break;
    d -= 365 + przest;
  }
  for (m = 1; ; m++) {
    uint8_t dniwmsc = pgm_read_byte(daysInMonth + m - 1);
    if (przest && m == 2)
      dniwmsc++;
    if (d < dniwmsc)
      break;
    d -= dniwmsc;
  }
  d++;

  Wire.beginTransmission(rtcadd);
  Wire.write(0x02);
  Wire.write(decToBcd(ss)); //sek
  Wire.write(decToBcd(mm)); //min
  Wire.write(decToBcd(hh)); //godz
  Wire.write(decToBcd(d)); //dzien
  Wire.write(decToBcd(dt)); //dzien tyg (1-7)
  Wire.write(decToBcd(m)); //msc
  Wire.write(decToBcd(r)); //rok-2000
  Wire.endTransmission();
}

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
