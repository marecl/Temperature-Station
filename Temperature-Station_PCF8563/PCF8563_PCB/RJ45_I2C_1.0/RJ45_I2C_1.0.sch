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
L power:GND #PWR02
U 1 1 5ADE0BC0
P 2650 7000
F 0 "#PWR02" H 2650 6750 50  0001 C CNN
F 1 "GND" H 2655 6827 50  0000 C CNN
F 2 "" H 2650 7000 50  0001 C CNN
F 3 "" H 2650 7000 50  0001 C CNN
	1    2650 7000
	1    0    0    -1  
$EndComp
Text GLabel 4200 6600 2    50   Input ~ 0
dht_1
Text GLabel 4200 6700 2    50   Input ~ 0
dht_2
Text GLabel 4450 4300 2    50   Input ~ 0
dht_3
Text GLabel 4450 4400 2    50   Input ~ 0
dht_4
Text GLabel 4450 4500 2    50   Input ~ 0
dht_5
Text GLabel 4450 4600 2    50   Input ~ 0
dht_6
Text GLabel 3250 5700 2    50   Input ~ 0
SCL
Text GLabel 3250 5600 2    50   Input ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x05 J8
U 1 1 5ADE7775
P 4750 5200
F 0 "J8" H 4830 5242 50  0000 L CNN
F 1 "Out" H 4830 5151 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 4750 5200 50  0001 C CNN
F 3 "~" H 4750 5200 50  0001 C CNN
	1    4750 5200
	1    0    0    -1  
$EndComp
Text GLabel 4550 5100 0    50   Input ~ 0
SDA
Text GLabel 4550 5000 0    50   Input ~ 0
SCL
Text GLabel 4550 5200 0    50   Input ~ 0
1wire
Text GLabel 4550 5400 0    50   Input ~ 0
3V3
$Comp
L power:GND #PWR08
U 1 1 5ADE7BC9
P 4250 5300
F 0 "#PWR08" H 4250 5050 50  0001 C CNN
F 1 "GND" H 4255 5127 50  0000 C CNN
F 2 "" H 4250 5300 50  0001 C CNN
F 3 "" H 4250 5300 50  0001 C CNN
	1    4250 5300
	1    0    0    -1  
$EndComp
Text GLabel 3550 5900 2    50   Input ~ 0
3V3
Text GLabel 2850 4000 2    50   Input ~ 0
3V3
Text GLabel 4200 6500 2    50   Input ~ 0
dht_0
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U1
U 1 1 5AF6068E
P 2650 5500
F 0 "U1" H 1709 5546 50  0000 R CNN
F 1 "ATmega328-PU" H 1709 5455 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 2650 5500 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet.pdf" H 2650 5500 50  0001 C CNN
	1    2650 5500
	1    0    0    -1  
$EndComp
Text GLabel 4450 4700 2    50   Input ~ 0
dht_7
Wire Wire Line
	3250 4300 4350 4300
Wire Wire Line
	4450 4400 4100 4400
Wire Wire Line
	3250 4500 3850 4500
Wire Wire Line
	4450 4600 3600 4600
Wire Wire Line
	3250 4700 3350 4700
Wire Wire Line
	3350 4200 3350 4700
Connection ~ 3350 4700
Wire Wire Line
	3350 4700 4450 4700
Wire Wire Line
	3600 4200 3600 4600
Connection ~ 3600 4600
Wire Wire Line
	3600 4600 3250 4600
Wire Wire Line
	3850 4200 3850 4500
Connection ~ 3850 4500
Wire Wire Line
	3850 4500 4450 4500
Wire Wire Line
	4100 4200 4100 4400
Connection ~ 4100 4400
Wire Wire Line
	4100 4400 3250 4400
Wire Wire Line
	4350 4200 4350 4300
Connection ~ 4350 4300
Wire Wire Line
	4350 4300 4450 4300
$Comp
L power:GND #PWR09
U 1 1 5AF63F82
P 4750 3900
F 0 "#PWR09" H 4750 3650 50  0001 C CNN
F 1 "GND" H 4755 3727 50  0000 C CNN
F 2 "" H 4750 3900 50  0001 C CNN
F 3 "" H 4750 3900 50  0001 C CNN
	1    4750 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5AF645B3
P 4350 6100
F 0 "#PWR07" H 4350 5850 50  0001 C CNN
F 1 "GND" H 4355 5927 50  0000 C CNN
F 2 "" H 4350 6100 50  0001 C CNN
F 3 "" H 4350 6100 50  0001 C CNN
	1    4350 6100
	1    0    0    -1  
$EndComp
Text GLabel 1800 1900 2    50   Input ~ 0
3V3
$Comp
L Device:R R1
U 1 1 5AF665E6
P 1350 2100
F 0 "R1" H 1420 2146 50  0000 L CNN
F 1 "4k7" H 1420 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1280 2100 50  0001 C CNN
F 3 "~" H 1350 2100 50  0001 C CNN
	1    1350 2100
	0    1    1    0   
$EndComp
Text GLabel 950  1700 0    50   Input ~ 0
dht_0
Text GLabel 1750 1800 2    50   Input ~ 0
1wire
$Comp
L Connector_Generic:Conn_01x04 J9
U 1 1 5AF8872A
P 4150 5750
F 0 "J9" H 4230 5742 50  0000 L CNN
F 1 "Prog" H 4230 5651 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 4150 5750 50  0001 C CNN
F 3 "~" H 4150 5750 50  0001 C CNN
	1    4150 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 6100 3250 6050
Wire Wire Line
	3250 6050 3950 6050
Wire Wire Line
	3950 6050 3950 5950
Wire Wire Line
	3250 6000 3900 6000
Wire Wire Line
	3900 6000 3900 5850
Wire Wire Line
	3900 5850 3950 5850
Wire Wire Line
	3250 5800 3250 5900
Wire Wire Line
	3250 5800 3600 5800
Wire Wire Line
	3600 5800 3600 5750
Wire Wire Line
	3600 5750 3950 5750
$Comp
L power:GND #PWR05
U 1 1 5AF9107F
P 3950 5650
F 0 "#PWR05" H 3950 5400 50  0001 C CNN
F 1 "GND" H 3955 5477 50  0000 C CNN
F 2 "" H 3950 5650 50  0001 C CNN
F 3 "" H 3950 5650 50  0001 C CNN
	1    3950 5650
	0    1    1    0   
$EndComp
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J0
U 1 1 5AF93E8A
P 950 1700
F 0 "J0" H 1350 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 1350 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 1600 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 1600 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 1600 1600 50  0001 L CNN "Description"
F 5 "" H 1600 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 1600 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 1600 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 1600 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 1600 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 1600 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 1600 900 50  0001 L CNN "Arrow Price/Stock"
	1    950  1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5AF93F27
P 850 1800
F 0 "#PWR0101" H 850 1550 50  0001 C CNN
F 1 "GND" H 855 1627 50  0000 C CNN
F 2 "" H 850 1800 50  0001 C CNN
F 3 "" H 850 1800 50  0001 C CNN
	1    850  1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 1900 1800 1900
Wire Wire Line
	950  1900 950  2100
Wire Wire Line
	950  2100 1200 2100
Wire Wire Line
	1500 2100 1750 2100
Wire Wire Line
	1750 2100 1750 1900
Text GLabel 3150 1900 2    50   Input ~ 0
3V3
Text GLabel 2300 1700 0    50   Input ~ 0
dht_1
Text GLabel 3100 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J1
U 1 1 5AFA4115
P 2300 1700
F 0 "J1" H 2700 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 2700 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 2950 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 2950 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 2950 1600 50  0001 L CNN "Description"
F 5 "" H 2950 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 2950 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 2950 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 2950 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 2950 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 2950 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 2950 900 50  0001 L CNN "Arrow Price/Stock"
	1    2300 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5AFA411B
P 2200 1800
F 0 "#PWR0102" H 2200 1550 50  0001 C CNN
F 1 "GND" H 2205 1627 50  0000 C CNN
F 2 "" H 2200 1800 50  0001 C CNN
F 3 "" H 2200 1800 50  0001 C CNN
	1    2200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1900 3150 1900
Wire Wire Line
	2300 1900 2300 2100
Wire Wire Line
	2300 2100 2550 2100
Wire Wire Line
	2850 2100 3100 2100
Wire Wire Line
	3100 2100 3100 1900
Connection ~ 3100 1900
Text GLabel 4500 1900 2    50   Input ~ 0
3V3
Text GLabel 3650 1700 0    50   Input ~ 0
dht_2
Text GLabel 4450 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J2
U 1 1 5AFA64CF
P 3650 1700
F 0 "J2" H 4050 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 4050 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 4300 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 4300 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 4300 1600 50  0001 L CNN "Description"
F 5 "" H 4300 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 4300 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 4300 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 4300 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 4300 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 4300 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 4300 900 50  0001 L CNN "Arrow Price/Stock"
	1    3650 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5AFA64D5
P 3550 1800
F 0 "#PWR0103" H 3550 1550 50  0001 C CNN
F 1 "GND" H 3555 1627 50  0000 C CNN
F 2 "" H 3550 1800 50  0001 C CNN
F 3 "" H 3550 1800 50  0001 C CNN
	1    3550 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1700 4550 1700
Wire Wire Line
	4450 1900 4500 1900
Wire Wire Line
	3650 1900 3650 2100
Wire Wire Line
	3650 2100 3900 2100
Wire Wire Line
	4200 2100 4450 2100
Wire Wire Line
	4450 2100 4450 1900
Connection ~ 4450 1900
Text GLabel 5850 1900 2    50   Input ~ 0
3V3
Text GLabel 5000 1700 0    50   Input ~ 0
dht_3
Text GLabel 5800 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J3
U 1 1 5AFA64F5
P 5000 1700
F 0 "J3" H 5400 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 5400 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 5650 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 5650 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 5650 1600 50  0001 L CNN "Description"
F 5 "" H 5650 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 5650 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 5650 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 5650 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 5650 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 5650 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 5650 900 50  0001 L CNN "Arrow Price/Stock"
	1    5000 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5AFA64FB
P 5900 1700
F 0 "#PWR0104" H 5900 1450 50  0001 C CNN
F 1 "GND" H 5905 1527 50  0000 C CNN
F 2 "" H 5900 1700 50  0001 C CNN
F 3 "" H 5900 1700 50  0001 C CNN
	1    5900 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 1700 5900 1700
Wire Wire Line
	5800 1900 5850 1900
Wire Wire Line
	5000 1900 5000 2100
Wire Wire Line
	5000 2100 5250 2100
Wire Wire Line
	5550 2100 5800 2100
Wire Wire Line
	5800 2100 5800 1900
Connection ~ 5800 1900
Text GLabel 7200 1900 2    50   Input ~ 0
3V3
Text GLabel 6350 1700 0    50   Input ~ 0
dht_4
Text GLabel 7150 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J4
U 1 1 5AFA7E57
P 6350 1700
F 0 "J4" H 6750 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 6750 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 7000 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 7000 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 7000 1600 50  0001 L CNN "Description"
F 5 "" H 7000 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 7000 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 7000 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 7000 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 7000 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 7000 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 7000 900 50  0001 L CNN "Arrow Price/Stock"
	1    6350 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5AFA7E5D
P 7250 1700
F 0 "#PWR0105" H 7250 1450 50  0001 C CNN
F 1 "GND" H 7255 1527 50  0000 C CNN
F 2 "" H 7250 1700 50  0001 C CNN
F 3 "" H 7250 1700 50  0001 C CNN
	1    7250 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	7150 1700 7250 1700
Wire Wire Line
	7150 1900 7200 1900
Wire Wire Line
	6350 1900 6350 2100
Wire Wire Line
	6350 2100 6600 2100
Wire Wire Line
	6900 2100 7150 2100
Wire Wire Line
	7150 2100 7150 1900
Connection ~ 7150 1900
Text GLabel 8550 1900 2    50   Input ~ 0
3V3
Text GLabel 7700 1700 0    50   Input ~ 0
dht_5
Text GLabel 8500 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J5
U 1 1 5AFA7E7D
P 7700 1700
F 0 "J5" H 8100 1965 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 8100 1874 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 8350 1800 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 8350 1700 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 8350 1600 50  0001 L CNN "Description"
F 5 "" H 8350 1500 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 8350 1400 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 8350 1300 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 8350 1200 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 8350 1100 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 8350 1000 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 8350 900 50  0001 L CNN "Arrow Price/Stock"
	1    7700 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5AFA7E83
P 8600 1700
F 0 "#PWR0106" H 8600 1450 50  0001 C CNN
F 1 "GND" H 8605 1527 50  0000 C CNN
F 2 "" H 8600 1700 50  0001 C CNN
F 3 "" H 8600 1700 50  0001 C CNN
	1    8600 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	8500 1700 8600 1700
Wire Wire Line
	8500 1900 8550 1900
Wire Wire Line
	7700 1900 7700 2100
Wire Wire Line
	7700 2100 7950 2100
Wire Wire Line
	8250 2100 8500 2100
Wire Wire Line
	8500 2100 8500 1900
Connection ~ 8500 1900
Text GLabel 1800 2850 2    50   Input ~ 0
3V3
Text GLabel 950  2650 0    50   Input ~ 0
dht_6
Text GLabel 1750 2750 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J6
U 1 1 5AFA7EA1
P 950 2650
F 0 "J6" H 1350 2915 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 1350 2824 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 1600 2750 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 1600 2650 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 1600 2550 50  0001 L CNN "Description"
F 5 "" H 1600 2450 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 1600 2350 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 1600 2250 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 1600 2150 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 1600 2050 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 1600 1950 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 1600 1850 50  0001 L CNN "Arrow Price/Stock"
	1    950  2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5AFA7EA7
P 1850 2650
F 0 "#PWR0107" H 1850 2400 50  0001 C CNN
F 1 "GND" H 1855 2477 50  0000 C CNN
F 2 "" H 1850 2650 50  0001 C CNN
F 3 "" H 1850 2650 50  0001 C CNN
	1    1850 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 2650 1850 2650
Wire Wire Line
	1750 2850 1800 2850
Wire Wire Line
	950  2850 950  3050
Wire Wire Line
	950  3050 1200 3050
Wire Wire Line
	1500 3050 1750 3050
Wire Wire Line
	1750 3050 1750 2850
Connection ~ 1750 2850
Text GLabel 3150 2850 2    50   Input ~ 0
3V3
Text GLabel 2300 2650 0    50   Input ~ 0
dht_7
Text GLabel 3100 2750 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J7
U 1 1 5AFA7EC5
P 2300 2650
F 0 "J7" H 2700 2915 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 2700 2824 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 2950 2750 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 2950 2650 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 2950 2550 50  0001 L CNN "Description"
F 5 "" H 2950 2450 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 2950 2350 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 2950 2250 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 2950 2150 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 2950 2050 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 2950 1950 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 2950 1850 50  0001 L CNN "Arrow Price/Stock"
	1    2300 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5AFA7ECB
P 3200 2650
F 0 "#PWR0108" H 3200 2400 50  0001 C CNN
F 1 "GND" H 3205 2477 50  0000 C CNN
F 2 "" H 3200 2650 50  0001 C CNN
F 3 "" H 3200 2650 50  0001 C CNN
	1    3200 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 2650 3200 2650
Wire Wire Line
	3100 2850 3150 2850
Wire Wire Line
	2300 2850 2300 3050
Wire Wire Line
	2300 3050 2550 3050
Wire Wire Line
	2850 3050 3100 3050
Wire Wire Line
	3100 3050 3100 2850
Connection ~ 3100 2850
Connection ~ 1750 1900
$Comp
L Device:R R10
U 1 1 5AFC77C0
P 3600 4050
F 0 "R10" H 3670 4096 50  0000 L CNN
F 1 "10k" H 3670 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3530 4050 50  0001 C CNN
F 3 "~" H 3600 4050 50  0001 C CNN
	1    3600 4050
	-1   0    0    1   
$EndComp
Wire Wire Line
	3350 3900 3600 3900
$Comp
L Device:R R9
U 1 1 5AFC78A8
P 3350 4050
F 0 "R9" H 3420 4096 50  0000 L CNN
F 1 "10k" H 3420 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3280 4050 50  0001 C CNN
F 3 "~" H 3350 4050 50  0001 C CNN
	1    3350 4050
	-1   0    0    1   
$EndComp
Connection ~ 3600 3900
Wire Wire Line
	3600 3900 3850 3900
$Comp
L Device:R R11
U 1 1 5AFC79D7
P 3850 4050
F 0 "R11" H 3920 4096 50  0000 L CNN
F 1 "10k" H 3920 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3780 4050 50  0001 C CNN
F 3 "~" H 3850 4050 50  0001 C CNN
	1    3850 4050
	-1   0    0    1   
$EndComp
Connection ~ 3850 3900
Wire Wire Line
	3850 3900 4100 3900
$Comp
L Device:R R12
U 1 1 5AFC7A25
P 4100 4050
F 0 "R12" H 4170 4096 50  0000 L CNN
F 1 "10k" H 4170 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4030 4050 50  0001 C CNN
F 3 "~" H 4100 4050 50  0001 C CNN
	1    4100 4050
	-1   0    0    1   
$EndComp
Connection ~ 4100 3900
Wire Wire Line
	4100 3900 4350 3900
$Comp
L Device:R R13
U 1 1 5AFC7A71
P 4350 4050
F 0 "R13" H 4420 4096 50  0000 L CNN
F 1 "10k" H 4420 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4280 4050 50  0001 C CNN
F 3 "~" H 4350 4050 50  0001 C CNN
	1    4350 4050
	-1   0    0    1   
$EndComp
Connection ~ 4350 3900
Wire Wire Line
	4350 3900 4750 3900
$Comp
L Device:R R14
U 1 1 5AFC7D0E
P 3400 5900
F 0 "R14" V 3450 6250 50  0000 L CNN
F 1 "10k" V 3350 6250 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5900 50  0001 C CNN
F 3 "~" H 3400 5900 50  0001 C CNN
	1    3400 5900
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 5AFC7D87
P 3550 6250
F 0 "R15" H 3650 6400 50  0000 L CNN
F 1 "10k" H 3650 6300 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3480 6250 50  0001 C CNN
F 3 "~" H 3550 6250 50  0001 C CNN
	1    3550 6250
	-1   0    0    1   
$EndComp
$Comp
L Device:R R16
U 1 1 5AFC7DE5
P 3850 6250
F 0 "R16" H 3920 6296 50  0000 L CNN
F 1 "10k" H 3920 6205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3780 6250 50  0001 C CNN
F 3 "~" H 3850 6250 50  0001 C CNN
	1    3850 6250
	-1   0    0    1   
$EndComp
$Comp
L Device:R R17
U 1 1 5AFC7E37
P 4150 6250
F 0 "R17" H 4220 6296 50  0000 L CNN
F 1 "10k" H 4220 6205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 4080 6250 50  0001 C CNN
F 3 "~" H 4150 6250 50  0001 C CNN
	1    4150 6250
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5AFC833D
P 2700 2100
F 0 "R2" H 2770 2146 50  0000 L CNN
F 1 "4k7" H 2770 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 2630 2100 50  0001 C CNN
F 3 "~" H 2700 2100 50  0001 C CNN
	1    2700 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5AFC83A7
P 4050 2100
F 0 "R3" H 4120 2146 50  0000 L CNN
F 1 "4k7" H 4120 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3980 2100 50  0001 C CNN
F 3 "~" H 4050 2100 50  0001 C CNN
	1    4050 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5AFC8409
P 5400 2100
F 0 "R4" H 5470 2146 50  0000 L CNN
F 1 "4k7" H 5470 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5330 2100 50  0001 C CNN
F 3 "~" H 5400 2100 50  0001 C CNN
	1    5400 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5AFC8481
P 6750 2100
F 0 "R5" H 6820 2146 50  0000 L CNN
F 1 "4k7" H 6820 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6680 2100 50  0001 C CNN
F 3 "~" H 6750 2100 50  0001 C CNN
	1    6750 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5AFC84FB
P 8100 2100
F 0 "R6" H 8170 2146 50  0000 L CNN
F 1 "4k7" H 8170 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 8030 2100 50  0001 C CNN
F 3 "~" H 8100 2100 50  0001 C CNN
	1    8100 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5AFC8577
P 2700 3050
F 0 "R8" H 2770 3096 50  0000 L CNN
F 1 "4k7" H 2770 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 2630 3050 50  0001 C CNN
F 3 "~" H 2700 3050 50  0001 C CNN
	1    2700 3050
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5AFC8645
P 1350 3050
F 0 "R7" H 1420 3096 50  0000 L CNN
F 1 "4k7" H 1420 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1280 3050 50  0001 C CNN
F 3 "~" H 1350 3050 50  0001 C CNN
	1    1350 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	4250 5300 4550 5300
Wire Wire Line
	850  1800 950  1800
Wire Wire Line
	2200 1800 2300 1800
Wire Wire Line
	3550 1800 3650 1800
$Comp
L power:GND #PWR0109
U 1 1 5AF94E3F
P 4550 1700
F 0 "#PWR0109" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5AF951DB
P 4900 1800
F 0 "#PWR0110" H 4900 1550 50  0001 C CNN
F 1 "GND" H 4905 1627 50  0000 C CNN
F 2 "" H 4900 1800 50  0001 C CNN
F 3 "" H 4900 1800 50  0001 C CNN
	1    4900 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 1800 5000 1800
$Comp
L power:GND #PWR0111
U 1 1 5AF982BE
P 6250 1800
F 0 "#PWR0111" H 6250 1550 50  0001 C CNN
F 1 "GND" H 6255 1627 50  0000 C CNN
F 2 "" H 6250 1800 50  0001 C CNN
F 3 "" H 6250 1800 50  0001 C CNN
	1    6250 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1800 6350 1800
$Comp
L power:GND #PWR0112
U 1 1 5AF9B49C
P 7600 1800
F 0 "#PWR0112" H 7600 1550 50  0001 C CNN
F 1 "GND" H 7605 1627 50  0000 C CNN
F 2 "" H 7600 1800 50  0001 C CNN
F 3 "" H 7600 1800 50  0001 C CNN
	1    7600 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1800 7700 1800
$Comp
L power:GND #PWR0113
U 1 1 5AF9EBD5
P 850 2750
F 0 "#PWR0113" H 850 2500 50  0001 C CNN
F 1 "GND" H 855 2577 50  0000 C CNN
F 2 "" H 850 2750 50  0001 C CNN
F 3 "" H 850 2750 50  0001 C CNN
	1    850  2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2750 950  2750
$Comp
L power:GND #PWR0114
U 1 1 5AFA1EA7
P 2200 2750
F 0 "#PWR0114" H 2200 2500 50  0001 C CNN
F 1 "GND" H 2205 2577 50  0000 C CNN
F 2 "" H 2200 2750 50  0001 C CNN
F 3 "" H 2200 2750 50  0001 C CNN
	1    2200 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2750 2300 2750
$Comp
L power:GND #PWR0115
U 1 1 5AFA55F8
P 3200 1700
F 0 "#PWR0115" H 3200 1450 50  0001 C CNN
F 1 "GND" H 3205 1527 50  0000 C CNN
F 2 "" H 3200 1700 50  0001 C CNN
F 3 "" H 3200 1700 50  0001 C CNN
	1    3200 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 1700 3200 1700
$Comp
L power:GND #PWR0116
U 1 1 5AFABC81
P 1850 1700
F 0 "#PWR0116" H 1850 1450 50  0001 C CNN
F 1 "GND" H 1855 1527 50  0000 C CNN
F 2 "" H 1850 1700 50  0001 C CNN
F 3 "" H 1850 1700 50  0001 C CNN
	1    1850 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 1700 1850 1700
Connection ~ 3250 5800
Wire Wire Line
	2650 4000 2750 4000
Connection ~ 2750 4000
Wire Wire Line
	2750 4000 2850 4000
Wire Wire Line
	3250 6500 4150 6500
Wire Wire Line
	3250 6700 3550 6700
Wire Wire Line
	3250 6600 3850 6600
Wire Wire Line
	3550 6100 3850 6100
Connection ~ 3850 6100
Wire Wire Line
	3550 6400 3550 6700
Connection ~ 3550 6700
Wire Wire Line
	3550 6700 4200 6700
Connection ~ 4150 6100
Wire Wire Line
	4150 6100 4350 6100
Wire Wire Line
	3850 6100 4150 6100
Wire Wire Line
	4150 6400 4150 6500
Connection ~ 4150 6500
Wire Wire Line
	4150 6500 4200 6500
Wire Wire Line
	3850 6400 3850 6600
Connection ~ 3850 6600
Wire Wire Line
	3850 6600 4200 6600
Text GLabel 3250 6200 2    50   Input ~ 0
1wire
$EndSCHEMATC
