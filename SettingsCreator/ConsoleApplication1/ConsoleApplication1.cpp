// ConsoleApplication1.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

string addr = "";
string name = "";
string out = "";
int valid_sensors = 0;
bool pierwszy = true;
bool dhcp = false;

bool verify_addr(string address) {
	int x = 0;
	string verify = "";
	int temp;
	int verified = 0;
	while (x < address.length()) {
		verify = "";
		temp = 0;
		while (address[x] != ',' && x != address.length()) {
			verify += address[x];
			x++;
		}
		x++;
		if (verify != "") {
			const char * verifyc = verify.c_str();
			temp = atoi(verifyc);
		}
		else return false;
		if (temp > 255) return false;
		verified++;
	}
	if (verified != 8) return false;
	return true;
}

bool verify_ip(string address) {
	string verify = "";
	int temp;
	int verified = 0;
	int x = 0;
	while (x < address.length()) {
		verify = "";
		temp = 0;
		while (address[x] != '.' && x != address.length()) {
			verify += address[x];
			x++;
		}
		x++;
		if (verify != "") {
			const char * verifyc = verify.c_str();
			temp = atoi(verifyc);
		}
		if (temp > 255) return false;
		verified++;
	}
	if (verified != 4) return false;
	return true;
}

int main() {
	ofstream settings;
	settings.open("settings.txt", ios::out);
	settings << "{\n\t\"sensor\": [\n\t\t";
	cout << "Enter \\quit to finish entering sensor names\n";
	while (1) {
		cout << "Name: ";
		cin >> name;
		if (name == "\\quit") {
			if (valid_sensors == 0) {
				cout << "There must be at least one sensor!\n"; continue;
			}
			break;
		}
		cout << "Address: ";
		cin >> addr;
		if (!verify_addr(addr)) {
			cout << "Incorrect address. Enter name and address again.\n";
			continue;
		}

		if (!pierwszy) settings << ",\n\t\t";
		else pierwszy = false;
		out = "[\"" + name + "\"";
		out += "," + addr;
		out += "]";
		settings << out;
		settings.flush();
		out = "";
		valid_sensors++;
	}
	settings << "\n\t],\n";
	settings << "\t\"valid_sensors\": " << valid_sensors << ",\n";

	settings << "\t\"ip\": {\n\t\t";
	cout << "IP mode (static/dhcp): ";
	cin >> name;
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	settings << "\"mode\": " << "\"" << name << "\",\n";
	if (name == "static")
		dhcp = false;
	else dhcp = true;

	settings << "\t\t\"ip\": \"";

	while (!dhcp) {
		cout << "IP address: ";
		cin >> name;
		if (!verify_ip(name)) {
			cout << "Incorrect IP address!\n";
			continue;
		}
		settings << name;
		break;
	}
	settings << "\",\n";
	settings << "\t\t\"gateway\": \"";

	while (!dhcp) {
		cout << "Gateway IP: ";
		cin >> name;
		if (!verify_ip(name)) {
			cout << "Incorrect gateway IP!\n";
			continue;
		}
		settings << name;
		break;
	}

	settings << "\",\n";
	settings << "\t\t\"subnet\": \"";

	while (!dhcp) {
		cout << "Subnet mask: ";
		cin >> name;
		if (!verify_ip(name)) {
			cout << "Incorrect subnet mask!\n";
			continue;
		}
		settings << name;
		break;
	}
	settings << "\"\n\t},\n";

	settings << "\t\"wlan\": [\n\t\t";
	cout << "Enter network credentails (type \\quit to finish\n";
	pierwszy = true;
	valid_sensors = 0;
	while (1) {
		cout << "SSID: ";
		cin >> name;
		if (name == "\\quit") {
			if (valid_sensors == 0) {
				cout << "No Access Points given. Understandable.\n";
				settings << "[\"-1\", \"-1\"]";
				valid_sensors = 1;
			}
			break;
		}
		if (!pierwszy) settings << ",\n\t\t[";
		else pierwszy = false;
		settings << "[\"" << name << "\", ";
		cout << "Password: ";
		cin >> name;
		settings << "\"" << name << "\"]";
		valid_sensors++;
	}
	settings << "\n\t],\n";
	settings << "\t\"saved_ap\": " << valid_sensors << ",\n";

	cout << "Enter NTP server name (i.e. tempus1.gum.gov.pl): ";
	cin >> name;
	settings << "\t\"ntp_server\": " << name << ",\n";

	while (1) {
		cout << "Do You wish to automatically update date and time on boot? (Y/N): ";
		name = "";
		cin >> name;
		if (name == "" || name == "Y" || name == "y") dhcp = true; //multipurpose variables :D
		else if (name == "n" || name == "N") dhcp = false;
		else continue;
		break;
	}
	settings << "\t\"use_ntp\": ";
	if (dhcp) settings << "true,\n";
	else settings << "false,\n";

	cout << "Enter Your timezone: ";
	cin >> valid_sensors;
	settings << "\t\"timezone\": " << valid_sensors;

	settings << "\n}";
	settings.flush();
	settings.close();
	while (1);
	return 0;
}