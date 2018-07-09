EESchema Schematic File Version 4
LIBS:RJ45_Breakout-cache
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
Text GLabel 3850 5300 0    50   Input ~ 0
1wire
Text GLabel 3850 6200 0    50   Input ~ 0
3V3
Text GLabel 1750 1250 2    50   Input ~ 0
3V3
$Comp
L Device:R R1
U 1 1 5AF665E6
P 1300 1450
F 0 "R1" H 1370 1496 50  0000 L CNN
F 1 "4k7" H 1370 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1230 1450 50  0001 C CNN
F 3 "~" H 1300 1450 50  0001 C CNN
	1    1300 1450
	0    1    1    0   
$EndComp
Text GLabel 1700 1150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J1
U 1 1 5AF93E8A
P 900 1050
F 0 "J1" H 1300 1315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 1300 1224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 1550 1150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 1550 1050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 1550 950 50  0001 L CNN "Description"
F 5 "" H 1550 850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 1550 750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 1550 650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 1550 550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 1550 450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 1550 350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 1550 250 50  0001 L CNN "Arrow Price/Stock"
	1    900  1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5AF93F27
P 800 1150
F 0 "#PWR0101" H 800 900 50  0001 C CNN
F 1 "GND" H 805 977 50  0000 C CNN
F 2 "" H 800 1150 50  0001 C CNN
F 3 "" H 800 1150 50  0001 C CNN
	1    800  1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1250 1750 1250
Wire Wire Line
	900  1250 900  1450
Wire Wire Line
	900  1450 1150 1450
Wire Wire Line
	1450 1450 1700 1450
Wire Wire Line
	1700 1450 1700 1250
Text GLabel 3100 1250 2    50   Input ~ 0
3V3
Text GLabel 3050 1150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J2
U 1 1 5AFA4115
P 2250 1050
F 0 "J2" H 2650 1315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 2650 1224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 2900 1150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 2900 1050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 2900 950 50  0001 L CNN "Description"
F 5 "" H 2900 850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 2900 750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 2900 650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 2900 550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 2900 450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 2900 350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 2900 250 50  0001 L CNN "Arrow Price/Stock"
	1    2250 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5AFA411B
P 2150 1150
F 0 "#PWR0102" H 2150 900 50  0001 C CNN
F 1 "GND" H 2155 977 50  0000 C CNN
F 2 "" H 2150 1150 50  0001 C CNN
F 3 "" H 2150 1150 50  0001 C CNN
	1    2150 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 1250 3100 1250
Wire Wire Line
	2250 1250 2250 1450
Wire Wire Line
	2250 1450 2500 1450
Wire Wire Line
	2800 1450 3050 1450
Wire Wire Line
	3050 1450 3050 1250
Connection ~ 3050 1250
Text GLabel 4450 1250 2    50   Input ~ 0
3V3
Text GLabel 4400 1150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J3
U 1 1 5AFA64CF
P 3600 1050
F 0 "J3" H 4000 1315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 4000 1224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 4250 1150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 4250 1050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 4250 950 50  0001 L CNN "Description"
F 5 "" H 4250 850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 4250 750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 4250 650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 4250 550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 4250 450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 4250 350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 4250 250 50  0001 L CNN "Arrow Price/Stock"
	1    3600 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5AFA64D5
P 3500 1150
F 0 "#PWR0103" H 3500 900 50  0001 C CNN
F 1 "GND" H 3505 977 50  0000 C CNN
F 2 "" H 3500 1150 50  0001 C CNN
F 3 "" H 3500 1150 50  0001 C CNN
	1    3500 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1050 4500 1050
Wire Wire Line
	4400 1250 4450 1250
Wire Wire Line
	3600 1250 3600 1450
Wire Wire Line
	3600 1450 3850 1450
Wire Wire Line
	4150 1450 4400 1450
Wire Wire Line
	4400 1450 4400 1250
Connection ~ 4400 1250
Text GLabel 5800 1250 2    50   Input ~ 0
3V3
Text GLabel 5750 1150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J4
U 1 1 5AFA64F5
P 4950 1050
F 0 "J4" H 5350 1315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 5350 1224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 5600 1150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 5600 1050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 5600 950 50  0001 L CNN "Description"
F 5 "" H 5600 850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 5600 750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 5600 650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 5600 550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 5600 450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 5600 350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 5600 250 50  0001 L CNN "Arrow Price/Stock"
	1    4950 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5AFA64FB
P 5850 1050
F 0 "#PWR0104" H 5850 800 50  0001 C CNN
F 1 "GND" H 5855 877 50  0000 C CNN
F 2 "" H 5850 1050 50  0001 C CNN
F 3 "" H 5850 1050 50  0001 C CNN
	1    5850 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5750 1050 5850 1050
Wire Wire Line
	5750 1250 5800 1250
Wire Wire Line
	4950 1250 4950 1450
Wire Wire Line
	4950 1450 5200 1450
Wire Wire Line
	5500 1450 5750 1450
Wire Wire Line
	5750 1450 5750 1250
Connection ~ 5750 1250
Text GLabel 1750 2250 2    50   Input ~ 0
3V3
Text GLabel 1700 2150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J5
U 1 1 5AFA7E57
P 900 2050
F 0 "J5" H 1300 2315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 1300 2224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 1550 2150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 1550 2050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 1550 1950 50  0001 L CNN "Description"
F 5 "" H 1550 1850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 1550 1750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 1550 1650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 1550 1550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 1550 1450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 1550 1350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 1550 1250 50  0001 L CNN "Arrow Price/Stock"
	1    900  2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5AFA7E5D
P 1800 2050
F 0 "#PWR0105" H 1800 1800 50  0001 C CNN
F 1 "GND" H 1805 1877 50  0000 C CNN
F 2 "" H 1800 2050 50  0001 C CNN
F 3 "" H 1800 2050 50  0001 C CNN
	1    1800 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 2050 1800 2050
Wire Wire Line
	1700 2250 1750 2250
Wire Wire Line
	900  2250 900  2450
Wire Wire Line
	900  2450 1150 2450
Wire Wire Line
	1450 2450 1700 2450
Wire Wire Line
	1700 2450 1700 2250
Connection ~ 1700 2250
Text GLabel 3100 2250 2    50   Input ~ 0
3V3
Text GLabel 3050 2150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J6
U 1 1 5AFA7E7D
P 2250 2050
F 0 "J6" H 2650 2315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 2650 2224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 2900 2150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 2900 2050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 2900 1950 50  0001 L CNN "Description"
F 5 "" H 2900 1850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 2900 1750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 2900 1650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 2900 1550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 2900 1450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 2900 1350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 2900 1250 50  0001 L CNN "Arrow Price/Stock"
	1    2250 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5AFA7E83
P 3150 2050
F 0 "#PWR0106" H 3150 1800 50  0001 C CNN
F 1 "GND" H 3155 1877 50  0000 C CNN
F 2 "" H 3150 2050 50  0001 C CNN
F 3 "" H 3150 2050 50  0001 C CNN
	1    3150 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 2050 3150 2050
Wire Wire Line
	3050 2250 3100 2250
Wire Wire Line
	2250 2250 2250 2450
Wire Wire Line
	2250 2450 2500 2450
Wire Wire Line
	2800 2450 3050 2450
Wire Wire Line
	3050 2450 3050 2250
Connection ~ 3050 2250
Text GLabel 4450 2250 2    50   Input ~ 0
3V3
Text GLabel 3600 2050 0    50   Input ~ 0
dht_6
Text GLabel 4400 2150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J7
U 1 1 5AFA7EA1
P 3600 2050
F 0 "J7" H 4000 2315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 4000 2224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 4250 2150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 4250 2050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 4250 1950 50  0001 L CNN "Description"
F 5 "" H 4250 1850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 4250 1750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 4250 1650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 4250 1550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 4250 1450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 4250 1350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 4250 1250 50  0001 L CNN "Arrow Price/Stock"
	1    3600 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5AFA7EA7
P 4500 2050
F 0 "#PWR0107" H 4500 1800 50  0001 C CNN
F 1 "GND" H 4505 1877 50  0000 C CNN
F 2 "" H 4500 2050 50  0001 C CNN
F 3 "" H 4500 2050 50  0001 C CNN
	1    4500 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 2050 4500 2050
Wire Wire Line
	4400 2250 4450 2250
Wire Wire Line
	3600 2250 3600 2450
Wire Wire Line
	3600 2450 3850 2450
Wire Wire Line
	4150 2450 4400 2450
Wire Wire Line
	4400 2450 4400 2250
Connection ~ 4400 2250
Text GLabel 5800 2250 2    50   Input ~ 0
3V3
Text GLabel 4950 2050 0    50   Input ~ 0
dht_7
Text GLabel 5750 2150 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J8
U 1 1 5AFA7EC5
P 4950 2050
F 0 "J8" H 5350 2315 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 5350 2224 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 5600 2150 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 5600 2050 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 5600 1950 50  0001 L CNN "Description"
F 5 "" H 5600 1850 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 5600 1750 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 5600 1650 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 5600 1550 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 5600 1450 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 5600 1350 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 5600 1250 50  0001 L CNN "Arrow Price/Stock"
	1    4950 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5AFA7ECB
P 5850 2050
F 0 "#PWR0108" H 5850 1800 50  0001 C CNN
F 1 "GND" H 5855 1877 50  0000 C CNN
F 2 "" H 5850 2050 50  0001 C CNN
F 3 "" H 5850 2050 50  0001 C CNN
	1    5850 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5750 2050 5850 2050
Wire Wire Line
	5750 2250 5800 2250
Wire Wire Line
	4950 2250 4950 2450
Wire Wire Line
	4950 2450 5200 2450
Wire Wire Line
	5500 2450 5750 2450
Wire Wire Line
	5750 2450 5750 2250
Connection ~ 5750 2250
Connection ~ 1700 1250
$Comp
L Device:R R2
U 1 1 5AFC833D
P 2650 1450
F 0 "R2" H 2720 1496 50  0000 L CNN
F 1 "4k7" H 2720 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 2580 1450 50  0001 C CNN
F 3 "~" H 2650 1450 50  0001 C CNN
	1    2650 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5AFC83A7
P 4000 1450
F 0 "R3" H 4070 1496 50  0000 L CNN
F 1 "4k7" H 4070 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3930 1450 50  0001 C CNN
F 3 "~" H 4000 1450 50  0001 C CNN
	1    4000 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5AFC8409
P 5350 1450
F 0 "R4" H 5420 1496 50  0000 L CNN
F 1 "4k7" H 5420 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5280 1450 50  0001 C CNN
F 3 "~" H 5350 1450 50  0001 C CNN
	1    5350 1450
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5AFC8481
P 1300 2450
F 0 "R5" H 1370 2496 50  0000 L CNN
F 1 "4k7" H 1370 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 1230 2450 50  0001 C CNN
F 3 "~" H 1300 2450 50  0001 C CNN
	1    1300 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5AFC84FB
P 2650 2450
F 0 "R6" H 2720 2496 50  0000 L CNN
F 1 "4k7" H 2720 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 2580 2450 50  0001 C CNN
F 3 "~" H 2650 2450 50  0001 C CNN
	1    2650 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5AFC8577
P 5350 2450
F 0 "R8" H 5420 2496 50  0000 L CNN
F 1 "4k7" H 5420 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5280 2450 50  0001 C CNN
F 3 "~" H 5350 2450 50  0001 C CNN
	1    5350 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5AFC8645
P 4000 2450
F 0 "R7" H 4070 2496 50  0000 L CNN
F 1 "4k7" H 4070 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3930 2450 50  0001 C CNN
F 3 "~" H 4000 2450 50  0001 C CNN
	1    4000 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	800  1150 900  1150
Wire Wire Line
	2150 1150 2250 1150
Wire Wire Line
	3500 1150 3600 1150
$Comp
L power:GND #PWR0109
U 1 1 5AF94E3F
P 4500 1050
F 0 "#PWR0109" H 4500 800 50  0001 C CNN
F 1 "GND" H 4505 877 50  0000 C CNN
F 2 "" H 4500 1050 50  0001 C CNN
F 3 "" H 4500 1050 50  0001 C CNN
	1    4500 1050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5AF951DB
P 4850 1150
F 0 "#PWR0110" H 4850 900 50  0001 C CNN
F 1 "GND" H 4855 977 50  0000 C CNN
F 2 "" H 4850 1150 50  0001 C CNN
F 3 "" H 4850 1150 50  0001 C CNN
	1    4850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 1150 4950 1150
$Comp
L power:GND #PWR0111
U 1 1 5AF982BE
P 800 2150
F 0 "#PWR0111" H 800 1900 50  0001 C CNN
F 1 "GND" H 805 1977 50  0000 C CNN
F 2 "" H 800 2150 50  0001 C CNN
F 3 "" H 800 2150 50  0001 C CNN
	1    800  2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  2150 900  2150
$Comp
L power:GND #PWR0112
U 1 1 5AF9B49C
P 2150 2150
F 0 "#PWR0112" H 2150 1900 50  0001 C CNN
F 1 "GND" H 2155 1977 50  0000 C CNN
F 2 "" H 2150 2150 50  0001 C CNN
F 3 "" H 2150 2150 50  0001 C CNN
	1    2150 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2150 2250 2150
$Comp
L power:GND #PWR0113
U 1 1 5AF9EBD5
P 3500 2150
F 0 "#PWR0113" H 3500 1900 50  0001 C CNN
F 1 "GND" H 3505 1977 50  0000 C CNN
F 2 "" H 3500 2150 50  0001 C CNN
F 3 "" H 3500 2150 50  0001 C CNN
	1    3500 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2150 3600 2150
$Comp
L power:GND #PWR0114
U 1 1 5AFA1EA7
P 4850 2150
F 0 "#PWR0114" H 4850 1900 50  0001 C CNN
F 1 "GND" H 4855 1977 50  0000 C CNN
F 2 "" H 4850 2150 50  0001 C CNN
F 3 "" H 4850 2150 50  0001 C CNN
	1    4850 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2150 4950 2150
$Comp
L power:GND #PWR0115
U 1 1 5AFA55F8
P 3150 1050
F 0 "#PWR0115" H 3150 800 50  0001 C CNN
F 1 "GND" H 3155 877 50  0000 C CNN
F 2 "" H 3150 1050 50  0001 C CNN
F 3 "" H 3150 1050 50  0001 C CNN
	1    3150 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 1050 3150 1050
$Comp
L power:GND #PWR0116
U 1 1 5AFABC81
P 1800 1050
F 0 "#PWR0116" H 1800 800 50  0001 C CNN
F 1 "GND" H 1805 877 50  0000 C CNN
F 2 "" H 1800 1050 50  0001 C CNN
F 3 "" H 1800 1050 50  0001 C CNN
	1    1800 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 1050 1800 1050
Text GLabel 2250 2050 0    50   Input ~ 0
dht_5
Text GLabel 900  2050 0    50   Input ~ 0
dht_4
Text GLabel 4950 1050 0    50   Input ~ 0
dht_3
Text GLabel 3600 1050 0    50   Input ~ 0
dht_2
Text GLabel 2250 1050 0    50   Input ~ 0
dht_1
Text GLabel 900  1050 0    50   Input ~ 0
dht_0
$Comp
L Regulator_Linear:LM1117-3.3 U1
U 1 1 5B065B0C
P 3400 4100
F 0 "U1" H 3400 4342 50  0000 C CNN
F 1 "LM1117-3.3" H 3400 4251 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3400 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 3400 4100 50  0001 C CNN
	1    3400 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5B065D6A
P 2800 4250
F 0 "C1" H 2915 4296 50  0000 L CNN
F 1 "100nF" H 2915 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 2838 4100 50  0001 C CNN
F 3 "~" H 2800 4250 50  0001 C CNN
	1    2800 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5B065E70
P 4200 4250
F 0 "C2" H 4315 4296 50  0000 L CNN
F 1 "100nF" H 4315 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4238 4100 50  0001 C CNN
F 3 "~" H 4200 4250 50  0001 C CNN
	1    4200 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5B06601C
P 3400 4500
F 0 "#PWR0117" H 3400 4250 50  0001 C CNN
F 1 "GND" H 3405 4327 50  0000 C CNN
F 2 "" H 3400 4500 50  0001 C CNN
F 3 "" H 3400 4500 50  0001 C CNN
	1    3400 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4400 3400 4400
Connection ~ 3400 4400
Wire Wire Line
	3400 4400 3400 4500
Wire Wire Line
	2800 4100 3050 4100
Text GLabel 2750 4100 0    50   Input ~ 0
Vin
Text GLabel 4250 4100 2    50   Input ~ 0
3V3
$Comp
L Device:D D1
U 1 1 5B06AB1D
P 3400 3700
F 0 "D1" H 3400 3916 50  0000 C CNN
F 1 "1N4007" H 3400 3825 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 3400 3700 50  0001 C CNN
F 3 "~" H 3400 3700 50  0001 C CNN
	1    3400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4400 3700 4400
Wire Wire Line
	3250 3700 3050 3700
Wire Wire Line
	3050 3700 3050 4100
Connection ~ 3050 4100
Wire Wire Line
	3050 4100 3100 4100
Wire Wire Line
	3550 3700 3700 3700
Wire Wire Line
	3700 3700 3700 4100
Connection ~ 3700 4100
$Comp
L Device:D D2
U 1 1 5B072CA2
P 3700 4250
F 0 "D2" V 3654 4329 50  0000 L CNN
F 1 "1N4007" V 3745 4329 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 3700 4250 50  0001 C CNN
F 3 "~" H 3700 4250 50  0001 C CNN
	1    3700 4250
	0    1    1    0   
$EndComp
Text GLabel 3850 6400 0    50   Input ~ 0
Vin
Text GLabel 3850 5400 0    50   Input ~ 0
dht_0
Text GLabel 3850 5500 0    50   Input ~ 0
dht_1
Text GLabel 3850 5600 0    50   Input ~ 0
dht_2
Text GLabel 3850 5700 0    50   Input ~ 0
dht_3
Text GLabel 3850 5800 0    50   Input ~ 0
dht_4
Text GLabel 3850 5900 0    50   Input ~ 0
dht_5
Text GLabel 3850 6000 0    50   Input ~ 0
dht_6
Text GLabel 3850 6100 0    50   Input ~ 0
dht_7
Connection ~ 3700 4400
Wire Wire Line
	3700 4400 4200 4400
Wire Wire Line
	3550 5450 3850 5450
Wire Wire Line
	3850 5400 3850 5450
$Comp
L Device:R R10
U 1 1 5B08A8A2
P 3400 5450
F 0 "R10" V 3400 5800 50  0000 C CNN
F 1 "33k" V 3400 5950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5450 50  0001 C CNN
F 3 "~" H 3400 5450 50  0001 C CNN
	1    3400 5450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5B08E015
P 3250 6400
F 0 "#PWR0118" H 3250 6150 50  0001 C CNN
F 1 "GND" H 3255 6227 50  0000 C CNN
F 2 "" H 3250 6400 50  0001 C CNN
F 3 "" H 3250 6400 50  0001 C CNN
	1    3250 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 6150 3850 6150
Wire Wire Line
	3850 6150 3850 6100
Wire Wire Line
	3550 6050 3850 6050
Wire Wire Line
	3850 6050 3850 6000
Wire Wire Line
	3550 5950 3850 5950
Wire Wire Line
	3850 5950 3850 5900
Wire Wire Line
	3550 5850 3850 5850
Wire Wire Line
	3850 5850 3850 5800
Wire Wire Line
	3550 5750 3850 5750
Wire Wire Line
	3850 5750 3850 5700
Wire Wire Line
	3550 5650 3850 5650
Wire Wire Line
	3850 5650 3850 5600
Wire Wire Line
	3550 5550 3850 5550
Wire Wire Line
	3850 5550 3850 5500
Wire Wire Line
	2750 4100 2800 4100
Connection ~ 2800 4100
Wire Wire Line
	4200 4100 4250 4100
Connection ~ 4200 4100
$Comp
L Device:Fuse F1
U 1 1 5B0ABB62
P 3950 4100
F 0 "F1" V 3753 4100 50  0000 C CNN
F 1 "500mA" V 3844 4100 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" V 3880 4100 50  0001 C CNN
F 3 "~" H 3950 4100 50  0001 C CNN
	1    3950 4100
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 5B075ADF
P 3400 5550
F 0 "R11" V 3400 5900 50  0000 C CNN
F 1 "33k" V 3400 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5550 50  0001 C CNN
F 3 "~" H 3400 5550 50  0001 C CNN
	1    3400 5550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5B075B27
P 3400 5650
F 0 "R12" V 3400 6000 50  0000 C CNN
F 1 "33k" V 3400 6150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5650 50  0001 C CNN
F 3 "~" H 3400 5650 50  0001 C CNN
	1    3400 5650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R13
U 1 1 5B075B6D
P 3400 5750
F 0 "R13" V 3400 6100 50  0000 C CNN
F 1 "33k" V 3400 6250 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5750 50  0001 C CNN
F 3 "~" H 3400 5750 50  0001 C CNN
	1    3400 5750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 5B075BB5
P 3400 5850
F 0 "R14" V 3400 6200 50  0000 C CNN
F 1 "33k" V 3400 6350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5850 50  0001 C CNN
F 3 "~" H 3400 5850 50  0001 C CNN
	1    3400 5850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5B075BFF
P 3400 5950
F 0 "R15" V 3400 6300 50  0000 C CNN
F 1 "33k" V 3400 6450 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 5950 50  0001 C CNN
F 3 "~" H 3400 5950 50  0001 C CNN
	1    3400 5950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5B075C4B
P 3400 6050
F 0 "R16" V 3400 6400 50  0000 C CNN
F 1 "33k" V 3400 6550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 6050 50  0001 C CNN
F 3 "~" H 3400 6050 50  0001 C CNN
	1    3400 6050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R17
U 1 1 5B075C99
P 3400 6150
F 0 "R17" V 3400 6500 50  0000 C CNN
F 1 "33k" V 3400 6650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 6150 50  0001 C CNN
F 3 "~" H 3400 6150 50  0001 C CNN
	1    3400 6150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 4100 4200 4100
Wire Wire Line
	3700 4100 3800 4100
$Comp
L Connector_Generic:Conn_01x12 J12
U 1 1 5B076B25
P 4050 5800
F 0 "J12" H 4129 5792 50  0000 L CNN
F 1 "Flex" H 4129 5701 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x12_P2.54mm_Vertical" H 4050 5800 50  0001 C CNN
F 3 "~" H 4050 5800 50  0001 C CNN
	1    4050 5800
	1    0    0    -1  
$EndComp
Connection ~ 3250 6150
Wire Wire Line
	3250 6050 3250 6150
Connection ~ 3250 6050
Wire Wire Line
	3250 5950 3250 6050
Connection ~ 3250 5950
Wire Wire Line
	3250 5850 3250 5950
Connection ~ 3250 5850
Wire Wire Line
	3250 5750 3250 5850
Connection ~ 3250 5750
Wire Wire Line
	3250 5650 3250 5750
Connection ~ 3250 5650
Wire Wire Line
	3250 5550 3250 5650
Connection ~ 3250 5550
Wire Wire Line
	3250 5450 3250 5550
Wire Wire Line
	3850 6300 3250 6300
Wire Wire Line
	3250 6150 3250 6300
Wire Wire Line
	3250 6300 3250 6400
Connection ~ 3250 6300
$Comp
L power:GND #PWR0119
U 1 1 5ADE0BC0
P 8900 4600
F 0 "#PWR0119" H 8900 4350 50  0001 C CNN
F 1 "GND" H 8905 4427 50  0000 C CNN
F 2 "" H 8900 4600 50  0001 C CNN
F 3 "" H 8900 4600 50  0001 C CNN
	1    8900 4600
	1    0    0    -1  
$EndComp
Text GLabel 9500 4200 2    50   Input ~ 0
dht_1
Text GLabel 9500 4300 2    50   Input ~ 0
dht_2
Text GLabel 9500 1900 2    50   Input ~ 0
dht_3
Text GLabel 9500 2000 2    50   Input ~ 0
dht_4
Text GLabel 9500 2100 2    50   Input ~ 0
dht_5
Text GLabel 9500 2200 2    50   Input ~ 0
dht_6
Text GLabel 9500 3300 2    50   Input ~ 0
SCL
Text GLabel 9500 3200 2    50   Input ~ 0
SDA
Text GLabel 7500 4000 0    50   Input ~ 0
SDA
Text GLabel 7500 3900 0    50   Input ~ 0
SCL
Text GLabel 7500 3800 0    50   Input ~ 0
3V3
$Comp
L power:GND #PWR0120
U 1 1 5ADE7BC9
P 7200 3700
F 0 "#PWR0120" H 7200 3450 50  0001 C CNN
F 1 "GND" H 7205 3527 50  0000 C CNN
F 2 "" H 7200 3700 50  0001 C CNN
F 3 "" H 7200 3700 50  0001 C CNN
	1    7200 3700
	1    0    0    -1  
$EndComp
Text GLabel 9800 3500 2    50   Input ~ 0
3V3
Text GLabel 9100 1600 2    50   Input ~ 0
3V3
Text GLabel 9500 4100 2    50   Input ~ 0
dht_0
Text GLabel 9500 2300 2    50   Input ~ 0
dht_7
$Comp
L Connector_Generic:Conn_01x04 J10
U 1 1 5AF8872A
P 10400 3400
F 0 "J10" H 10480 3392 50  0000 L CNN
F 1 "Prog" H 10480 3301 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 10400 3400 50  0001 C CNN
F 3 "~" H 10400 3400 50  0001 C CNN
	1    10400 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 3400 9500 3500
$Comp
L power:GND #PWR0121
U 1 1 5AF9107F
P 10200 3300
F 0 "#PWR0121" H 10200 3050 50  0001 C CNN
F 1 "GND" H 10205 3127 50  0000 C CNN
F 2 "" H 10200 3300 50  0001 C CNN
F 3 "" H 10200 3300 50  0001 C CNN
	1    10200 3300
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5AFC7D0E
P 9650 3500
F 0 "R9" V 9700 3850 50  0000 L CNN
F 1 "10k" V 9600 3850 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 9580 3500 50  0001 C CNN
F 3 "~" H 9650 3500 50  0001 C CNN
	1    9650 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 3700 7500 3700
$Comp
L Connector_Generic:Conn_01x12 J9
U 1 1 5B0AE8BD
P 7700 2150
F 0 "J9" H 7780 2142 50  0000 L CNN
F 1 "Conn_01x12" H 7780 2051 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x12_P2.54mm_Vertical" H 7700 2150 50  0001 C CNN
F 3 "~" H 7700 2150 50  0001 C CNN
	1    7700 2150
	1    0    0    -1  
$EndComp
Text GLabel 7500 1850 0    50   Input ~ 0
dht_1
Text GLabel 7500 1950 0    50   Input ~ 0
dht_2
Text GLabel 7500 1750 0    50   Input ~ 0
dht_0
Text GLabel 7500 2050 0    50   Input ~ 0
dht_3
Text GLabel 7500 2150 0    50   Input ~ 0
dht_4
Text GLabel 7500 2250 0    50   Input ~ 0
dht_5
Text GLabel 7500 2350 0    50   Input ~ 0
dht_6
Text GLabel 7500 2450 0    50   Input ~ 0
dht_7
Text GLabel 7500 2550 0    50   Input ~ 0
3V3
Text GLabel 7500 1650 0    50   Input ~ 0
1wire
$Comp
L power:GND #PWR0122
U 1 1 5B0B3D30
P 7200 2650
F 0 "#PWR0122" H 7200 2400 50  0001 C CNN
F 1 "GND" H 7205 2477 50  0000 C CNN
F 2 "" H 7200 2650 50  0001 C CNN
F 3 "" H 7200 2650 50  0001 C CNN
	1    7200 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 2650 7500 2650
Text GLabel 7500 2750 0    50   Input ~ 0
Vin
Wire Wire Line
	9500 3400 10200 3400
Wire Wire Line
	9500 3600 10150 3600
Wire Wire Line
	10150 3600 10150 3500
Wire Wire Line
	10150 3500 10200 3500
Wire Wire Line
	9500 3700 10200 3700
Wire Wire Line
	10200 3700 10200 3600
Wire Wire Line
	9000 1600 9100 1600
Wire Wire Line
	8900 1600 9000 1600
Connection ~ 9000 1600
Connection ~ 9500 3400
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U2
U 1 1 5AF6068E
P 8900 3100
F 0 "U2" H 7959 3146 50  0000 R CNN
F 1 "ATmega328-PU" H 7959 3055 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 8900 3100 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet.pdf" H 8900 3100 50  0001 C CNN
	1    8900 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NO_Small JP1
U 1 1 5B11B726
P 9600 3100
F 0 "JP1" H 9750 3300 50  0000 C CNN
F 1 "bypass1Wire" H 9750 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.50mm_HandSolder" H 9600 3100 50  0001 C CNN
F 3 "~" H 9600 3100 50  0001 C CNN
	1    9600 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5B11B9A1
P 9900 3100
F 0 "#PWR0123" H 9900 2850 50  0001 C CNN
F 1 "GND" H 9905 2927 50  0000 C CNN
F 2 "" H 9900 3100 50  0001 C CNN
F 3 "" H 9900 3100 50  0001 C CNN
	1    9900 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 3100 9900 3100
Text GLabel 7500 4100 0    50   Input ~ 0
1wire
$Comp
L Connector_Generic:Conn_01x05 J11
U 1 1 5ADE7775
P 7700 3900
F 0 "J11" H 7780 3942 50  0000 L CNN
F 1 "Out" H 7780 3851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Horizontal" H 7700 3900 50  0001 C CNN
F 3 "~" H 7700 3900 50  0001 C CNN
	1    7700 3900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
