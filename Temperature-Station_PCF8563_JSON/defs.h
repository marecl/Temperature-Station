#define SD_D D0
#define SDA D1
#define SCL D2
#define OW_PORT D3
#define SD_CS D4
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == 0)
#define countof(a) (sizeof(a) / sizeof(a[0]))

#define DHCPFILE "DHCP.TXT"
#define SETTINGS_FILE "SETTINGS.TXT"
#define NTPSERV "tempus1.gum.gov.pl"

#define MAX_SENSORS 16

static bool hasSD = false;
static bool httpserver = false;

String json = "";

const char* ntpServerName = NTPSERV;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

unsigned int localPort = 2390;

String workfile = "TEMP.CSV";

int valid_sensors = 0;
int saved_ap = 0;
double _temps_[MAX_SENSORS];

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
