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
int main()
{
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
		if (!pierwszy) out = ",\n\t\t";
		else pierwszy = false;
		out += "[\"" + name + "\"," + addr + "]";
		settings << out;
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
	settings << "\t\t\"ip\": \"";
	if (name == "static") {
		dhcp = false;
		cout << "IP address: ";
		cin >> name;
		settings << name;
	}
	else dhcp = true;
	settings << "\",\n";

	settings << "\t\t\"gateway\": \"";
	if (!dhcp) {
		cout << "Gateway IP: ";
		cin >> name;
		settings << name;
	}
	settings << "\",\n";

	settings << "\t\t\"subnet\": \"";
	if (!dhcp) {
		cout << "Subnet mask: ";
		cin >> name;
		settings << name;
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
	cout << endl << valid_sensors << endl;
	settings << "\n\t],\n";
	settings << "\t\"saved_ap\": " << valid_sensors << endl;
	settings << "}";
	settings.close();
	while (1);
	return 0;
}

