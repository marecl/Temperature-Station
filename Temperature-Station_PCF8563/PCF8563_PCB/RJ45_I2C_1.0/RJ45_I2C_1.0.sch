EESchema Schematic File Version 4
LIBS:RJ45_I2C_1.0-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR04
U 1 1 5ADE0BC0
P 2600 5150
F 0 "#PWR04" H 2600 4900 50  0001 C CNN
F 1 "GND" H 2605 4977 50  0000 C CNN
F 2 "" H 2600 5150 50  0001 C CNN
F 3 "" H 2600 5150 50  0001 C CNN
	1    2600 5150
	1    0    0    -1  
$EndComp
Text GLabel 3200 4750 2    50   Input ~ 0
dht_1
Text GLabel 3200 4850 2    50   Input ~ 0
dht_2
Text GLabel 3200 2450 2    50   Input ~ 0
dht_3
Text GLabel 3200 2550 2    50   Input ~ 0
dht_4
Text GLabel 3200 2650 2    50   Input ~ 0
dht_5
Text GLabel 3200 2750 2    50   Input ~ 0
dht_6
Text GLabel 3200 3850 2    50   Input ~ 0
SCL
Text GLabel 3200 3750 2    50   Input ~ 0
SDA
Text GLabel 4950 3500 0    50   Input ~ 0
SDA
Text GLabel 4950 3400 0    50   Input ~ 0
SCL
Text GLabel 4950 3300 0    50   Input ~ 0
3V3
$Comp
L power:GND #PWR01
U 1 1 5ADE7BC9
P 4650 3200
F 0 "#PWR01" H 4650 2950 50  0001 C CNN
F 1 "GND" H 4655 3027 50  0000 C CNN
F 2 "" H 4650 3200 50  0001 C CNN
F 3 "" H 4650 3200 50  0001 C CNN
	1    4650 3200
	1    0    0    -1  
$EndComp
Text GLabel 3500 4050 2    50   Input ~ 0
3V3
Text GLabel 2800 2150 2    50   Input ~ 0
3V3
Text GLabel 3200 4650 2    50   Input ~ 0
dht_0
Text GLabel 3200 2850 2    50   Input ~ 0
dht_7
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5AF8872A
P 4100 3950
F 0 "J3" H 4180 3942 50  0000 L CNN
F 1 "Prog" H 4180 3851 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 4100 3950 50  0001 C CNN
F 3 "~" H 4100 3950 50  0001 C CNN
	1    4100 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3950 3200 4050
$Comp
L power:GND #PWR03
U 1 1 5AF9107F
P 3900 3850
F 0 "#PWR03" H 3900 3600 50  0001 C CNN
F 1 "GND" H 3905 3677 50  0000 C CNN
F 2 "" H 3900 3850 50  0001 C CNN
F 3 "" H 3900 3850 50  0001 C CNN
	1    3900 3850
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5AFC7D0E
P 3350 4050
F 0 "R1" V 3400 4400 50  0000 L CNN
F 1 "10k" V 3300 4400 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3280 4050 50  0001 C CNN
F 3 "~" H 3350 4050 50  0001 C CNN
	1    3350 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	4650 3200 4950 3200
$Comp
L Connector_Generic:Conn_01x12 J1
U 1 1 5B0AE8BD
P 5150 2050
F 0 "J1" H 5230 2042 50  0000 L CNN
F 1 "Conn_01x12" H 5230 1951 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x12_P2.54mm_Vertical" H 5150 2050 50  0001 C CNN
F 3 "~" H 5150 2050 50  0001 C CNN
	1    5150 2050
	1    0    0    -1  
$EndComp
Text GLabel 4950 1750 0    50   Input ~ 0
dht_1
Text GLabel 4950 1850 0    50   Input ~ 0
dht_2
Text GLabel 4950 1650 0    50   Input ~ 0
dht_0
Text GLabel 4950 1950 0    50   Input ~ 0
dht_3
Text GLabel 4950 2050 0    50   Input ~ 0
dht_4
Text GLabel 4950 2150 0    50   Input ~ 0
dht_5
Text GLabel 4950 2250 0    50   Input ~ 0
dht_6
Text GLabel 4950 2350 0    50   Input ~ 0
dht_7
Text GLabel 4950 2450 0    50   Input ~ 0
3V3
Text GLabel 4950 1550 0    50   Input ~ 0
1wire
$Comp
L power:GND #PWR02
U 1 1 5B0B3D30
P 4650 2550
F 0 "#PWR02" H 4650 2300 50  0001 C CNN
F 1 "GND" H 4655 2377 50  0000 C CNN
F 2 "" H 4650 2550 50  0001 C CNN
F 3 "" H 4650 2550 50  0001 C CNN
	1    4650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2550 4950 2550
Text GLabel 4950 2650 0    50   Input ~ 0
Vin
Wire Wire Line
	3200 3950 3900 3950
Wire Wire Line
	3200 4150 3850 4150
Wire Wire Line
	3850 4150 3850 4050
Wire Wire Line
	3850 4050 3900 4050
Wire Wire Line
	3200 4250 3900 4250
Wire Wire Line
	3900 4250 3900 4150
Wire Wire Line
	2700 2150 2800 2150
Wire Wire Line
	2600 2150 2700 2150
Connection ~ 2700 2150
Connection ~ 3200 3950
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U1
U 1 1 5AF6068E
P 2600 3650
F 0 "U1" H 1659 3696 50  0000 R CNN
F 1 "ATmega328-PU" H 1659 3605 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 2600 3650 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet.pdf" H 2600 3650 50  0001 C CNN
	1    2600 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NO_Small JP1
U 1 1 5B11B726
P 3300 3650
F 0 "JP1" H 3450 3850 50  0000 C CNN
F 1 "bypass1Wire" H 3450 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 3300 3650 50  0001 C CNN
F 3 "~" H 3300 3650 50  0001 C CNN
	1    3300 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5B11B9A1
P 3600 3650
F 0 "#PWR0101" H 3600 3400 50  0001 C CNN
F 1 "GND" H 3605 3477 50  0000 C CNN
F 2 "" H 3600 3650 50  0001 C CNN
F 3 "" H 3600 3650 50  0001 C CNN
	1    3600 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3650 3600 3650
Text GLabel 4950 3600 0    50   Input ~ 0
1wire
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 5ADE7775
P 5150 3400
F 0 "J2" H 5230 3442 50  0000 L CNN
F 1 "Out" H 5230 3351 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 5150 3400 50  0001 C CNN
F 3 "~" H 5150 3400 50  0001 C CNN
	1    5150 3400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
