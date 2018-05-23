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
Text GLabel 6050 3850 0    50   Input ~ 0
1wire
Text GLabel 6050 4750 0    50   Input ~ 0
3V3
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
Text GLabel 1750 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J1
U 1 1 5AF93E8A
P 950 1700
F 0 "J1" H 1350 1965 50  0000 C CNN
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
L power:GND #PWR07
U 1 1 5AF93F27
P 850 1800
F 0 "#PWR07" H 850 1550 50  0001 C CNN
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
Text GLabel 3100 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J2
U 1 1 5AFA4115
P 2300 1700
F 0 "J2" H 2700 1965 50  0000 C CNN
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
L power:GND #PWR08
U 1 1 5AFA411B
P 2200 1800
F 0 "#PWR08" H 2200 1550 50  0001 C CNN
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
Text GLabel 4450 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J3
U 1 1 5AFA64CF
P 3650 1700
F 0 "J3" H 4050 1965 50  0000 C CNN
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
L power:GND #PWR09
U 1 1 5AFA64D5
P 3550 1800
F 0 "#PWR09" H 3550 1550 50  0001 C CNN
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
Text GLabel 5800 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J4
U 1 1 5AFA64F5
P 5000 1700
F 0 "J4" H 5400 1965 50  0000 C CNN
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
L power:GND #PWR04
U 1 1 5AFA64FB
P 5900 1700
F 0 "#PWR04" H 5900 1450 50  0001 C CNN
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
Text GLabel 7150 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J5
U 1 1 5AFA7E57
P 6350 1700
F 0 "J5" H 6750 1965 50  0000 C CNN
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
L power:GND #PWR05
U 1 1 5AFA7E5D
P 7250 1700
F 0 "#PWR05" H 7250 1450 50  0001 C CNN
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
Text GLabel 8500 1800 2    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J6
U 1 1 5AFA7E7D
P 7700 1700
F 0 "J6" H 8100 1965 50  0000 C CNN
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
L power:GND #PWR06
U 1 1 5AFA7E83
P 8600 1700
F 0 "#PWR06" H 8600 1450 50  0001 C CNN
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
L MHRJJ66NFRA:MHRJJ66NFRA J7
U 1 1 5AFA7EA1
P 950 2650
F 0 "J7" H 1350 2915 50  0000 C CNN
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
L power:GND #PWR013
U 1 1 5AFA7EA7
P 1850 2650
F 0 "#PWR013" H 1850 2400 50  0001 C CNN
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
L MHRJJ66NFRA:MHRJJ66NFRA J8
U 1 1 5AFA7EC5
P 2300 2650
F 0 "J8" H 2700 2915 50  0000 C CNN
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
L power:GND #PWR014
U 1 1 5AFA7ECB
P 3200 2650
F 0 "#PWR014" H 3200 2400 50  0001 C CNN
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
	850  1800 950  1800
Wire Wire Line
	2200 1800 2300 1800
Wire Wire Line
	3550 1800 3650 1800
$Comp
L power:GND #PWR03
U 1 1 5AF94E3F
P 4550 1700
F 0 "#PWR03" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5AF951DB
P 4900 1800
F 0 "#PWR010" H 4900 1550 50  0001 C CNN
F 1 "GND" H 4905 1627 50  0000 C CNN
F 2 "" H 4900 1800 50  0001 C CNN
F 3 "" H 4900 1800 50  0001 C CNN
	1    4900 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 1800 5000 1800
$Comp
L power:GND #PWR011
U 1 1 5AF982BE
P 6250 1800
F 0 "#PWR011" H 6250 1550 50  0001 C CNN
F 1 "GND" H 6255 1627 50  0000 C CNN
F 2 "" H 6250 1800 50  0001 C CNN
F 3 "" H 6250 1800 50  0001 C CNN
	1    6250 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1800 6350 1800
$Comp
L power:GND #PWR012
U 1 1 5AF9B49C
P 7600 1800
F 0 "#PWR012" H 7600 1550 50  0001 C CNN
F 1 "GND" H 7605 1627 50  0000 C CNN
F 2 "" H 7600 1800 50  0001 C CNN
F 3 "" H 7600 1800 50  0001 C CNN
	1    7600 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1800 7700 1800
$Comp
L power:GND #PWR015
U 1 1 5AF9EBD5
P 850 2750
F 0 "#PWR015" H 850 2500 50  0001 C CNN
F 1 "GND" H 855 2577 50  0000 C CNN
F 2 "" H 850 2750 50  0001 C CNN
F 3 "" H 850 2750 50  0001 C CNN
	1    850  2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2750 950  2750
$Comp
L power:GND #PWR016
U 1 1 5AFA1EA7
P 2200 2750
F 0 "#PWR016" H 2200 2500 50  0001 C CNN
F 1 "GND" H 2205 2577 50  0000 C CNN
F 2 "" H 2200 2750 50  0001 C CNN
F 3 "" H 2200 2750 50  0001 C CNN
	1    2200 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2750 2300 2750
$Comp
L power:GND #PWR02
U 1 1 5AFA55F8
P 3200 1700
F 0 "#PWR02" H 3200 1450 50  0001 C CNN
F 1 "GND" H 3205 1527 50  0000 C CNN
F 2 "" H 3200 1700 50  0001 C CNN
F 3 "" H 3200 1700 50  0001 C CNN
	1    3200 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 1700 3200 1700
$Comp
L power:GND #PWR01
U 1 1 5AFABC81
P 1850 1700
F 0 "#PWR01" H 1850 1450 50  0001 C CNN
F 1 "GND" H 1855 1527 50  0000 C CNN
F 2 "" H 1850 1700 50  0001 C CNN
F 3 "" H 1850 1700 50  0001 C CNN
	1    1850 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 1700 1850 1700
Text GLabel 7700 1700 0    50   Input ~ 0
dht_5
Text GLabel 6350 1700 0    50   Input ~ 0
dht_4
Text GLabel 5000 1700 0    50   Input ~ 0
dht_3
Text GLabel 3650 1700 0    50   Input ~ 0
dht_2
Text GLabel 2300 1700 0    50   Input ~ 0
dht_1
Text GLabel 950  1700 0    50   Input ~ 0
dht_0
$Comp
L Regulator_Linear:LM1117-3.3 U1
U 1 1 5B065B0C
P 2750 5000
F 0 "U1" H 2750 5242 50  0000 C CNN
F 1 "LM1117-3.3" H 2750 5151 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2750 5000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 2750 5000 50  0001 C CNN
	1    2750 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5B065D6A
P 2150 5150
F 0 "C1" H 2265 5196 50  0000 L CNN
F 1 "100nF" H 2265 5105 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 2188 5000 50  0001 C CNN
F 3 "~" H 2150 5150 50  0001 C CNN
	1    2150 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5B065E70
P 3550 5150
F 0 "C2" H 3665 5196 50  0000 L CNN
F 1 "100nF" H 3665 5105 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 3588 5000 50  0001 C CNN
F 3 "~" H 3550 5150 50  0001 C CNN
	1    3550 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5B06601C
P 2750 5400
F 0 "#PWR019" H 2750 5150 50  0001 C CNN
F 1 "GND" H 2755 5227 50  0000 C CNN
F 2 "" H 2750 5400 50  0001 C CNN
F 3 "" H 2750 5400 50  0001 C CNN
	1    2750 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5300 2750 5300
Connection ~ 2750 5300
Wire Wire Line
	2750 5300 2750 5400
Wire Wire Line
	2150 5000 2400 5000
Text GLabel 2100 5000 0    50   Input ~ 0
Vin
Text GLabel 3600 5000 2    50   Input ~ 0
3V3
$Comp
L Device:D D1
U 1 1 5B06AB1D
P 2750 4600
F 0 "D1" H 2750 4816 50  0000 C CNN
F 1 "1N4007" H 2750 4725 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 2750 4600 50  0001 C CNN
F 3 "~" H 2750 4600 50  0001 C CNN
	1    2750 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 5300 3050 5300
Wire Wire Line
	2600 4600 2400 4600
Wire Wire Line
	2400 4600 2400 5000
Connection ~ 2400 5000
Wire Wire Line
	2400 5000 2450 5000
Wire Wire Line
	2900 4600 3050 4600
Wire Wire Line
	3050 4600 3050 5000
Connection ~ 3050 5000
$Comp
L Device:D D2
U 1 1 5B072CA2
P 3050 5150
F 0 "D2" V 3004 5229 50  0000 L CNN
F 1 "1N4007" V 3095 5229 50  0000 L CNN
F 2 "Diode_SMD:D_SMA" H 3050 5150 50  0001 C CNN
F 3 "~" H 3050 5150 50  0001 C CNN
	1    3050 5150
	0    1    1    0   
$EndComp
Text GLabel 6050 4950 0    50   Input ~ 0
Vin
Text GLabel 6050 3950 0    50   Input ~ 0
dht_0
Text GLabel 6050 4050 0    50   Input ~ 0
dht_1
Text GLabel 6050 4150 0    50   Input ~ 0
dht_2
Text GLabel 6050 4250 0    50   Input ~ 0
dht_3
Text GLabel 6050 4350 0    50   Input ~ 0
dht_4
Text GLabel 6050 4450 0    50   Input ~ 0
dht_5
Text GLabel 6050 4550 0    50   Input ~ 0
dht_6
Text GLabel 6050 4650 0    50   Input ~ 0
dht_7
Connection ~ 3050 5300
Wire Wire Line
	3050 5300 3550 5300
Wire Wire Line
	5750 4000 6050 4000
Wire Wire Line
	6050 3950 6050 4000
$Comp
L Device:R R9
U 1 1 5B08A8A2
P 5600 4000
F 0 "R9" V 5600 4350 50  0000 C CNN
F 1 "33k" V 5600 4500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4000 50  0001 C CNN
F 3 "~" H 5600 4000 50  0001 C CNN
	1    5600 4000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5B08E015
P 5450 4950
F 0 "#PWR017" H 5450 4700 50  0001 C CNN
F 1 "GND" H 5455 4777 50  0000 C CNN
F 2 "" H 5450 4950 50  0001 C CNN
F 3 "" H 5450 4950 50  0001 C CNN
	1    5450 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4700 6050 4700
Wire Wire Line
	6050 4700 6050 4650
Wire Wire Line
	5750 4600 6050 4600
Wire Wire Line
	6050 4600 6050 4550
Wire Wire Line
	5750 4500 6050 4500
Wire Wire Line
	6050 4500 6050 4450
Wire Wire Line
	5750 4400 6050 4400
Wire Wire Line
	6050 4400 6050 4350
Wire Wire Line
	5750 4300 6050 4300
Wire Wire Line
	6050 4300 6050 4250
Wire Wire Line
	5750 4200 6050 4200
Wire Wire Line
	6050 4200 6050 4150
Wire Wire Line
	5750 4100 6050 4100
Wire Wire Line
	6050 4100 6050 4050
Wire Wire Line
	2100 5000 2150 5000
Connection ~ 2150 5000
Wire Wire Line
	3550 5000 3600 5000
Connection ~ 3550 5000
$Comp
L Device:Fuse F1
U 1 1 5B0ABB62
P 3300 5000
F 0 "F1" V 3103 5000 50  0000 C CNN
F 1 "500mA" V 3194 5000 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" V 3230 5000 50  0001 C CNN
F 3 "~" H 3300 5000 50  0001 C CNN
	1    3300 5000
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5B075ADF
P 5600 4100
F 0 "R10" V 5600 4450 50  0000 C CNN
F 1 "33k" V 5600 4600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4100 50  0001 C CNN
F 3 "~" H 5600 4100 50  0001 C CNN
	1    5600 4100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5B075B27
P 5600 4200
F 0 "R11" V 5600 4550 50  0000 C CNN
F 1 "33k" V 5600 4700 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4200 50  0001 C CNN
F 3 "~" H 5600 4200 50  0001 C CNN
	1    5600 4200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5B075B6D
P 5600 4300
F 0 "R12" V 5600 4650 50  0000 C CNN
F 1 "33k" V 5600 4800 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4300 50  0001 C CNN
F 3 "~" H 5600 4300 50  0001 C CNN
	1    5600 4300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R13
U 1 1 5B075BB5
P 5600 4400
F 0 "R13" V 5600 4750 50  0000 C CNN
F 1 "33k" V 5600 4900 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4400 50  0001 C CNN
F 3 "~" H 5600 4400 50  0001 C CNN
	1    5600 4400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 5B075BFF
P 5600 4500
F 0 "R14" V 5600 4850 50  0000 C CNN
F 1 "33k" V 5600 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4500 50  0001 C CNN
F 3 "~" H 5600 4500 50  0001 C CNN
	1    5600 4500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5B075C4B
P 5600 4600
F 0 "R15" V 5600 4950 50  0000 C CNN
F 1 "33k" V 5600 5100 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4600 50  0001 C CNN
F 3 "~" H 5600 4600 50  0001 C CNN
	1    5600 4600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5B075C99
P 5600 4700
F 0 "R16" V 5600 5050 50  0000 C CNN
F 1 "33k" V 5600 5200 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5530 4700 50  0001 C CNN
F 3 "~" H 5600 4700 50  0001 C CNN
	1    5600 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 5000 3550 5000
Wire Wire Line
	3050 5000 3150 5000
$Comp
L Connector_Generic:Conn_01x12 J9
U 1 1 5B076B25
P 6250 4350
F 0 "J9" H 6329 4342 50  0000 L CNN
F 1 "Flex" H 6329 4251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x16_P2.54mm_Vertical" H 6250 4350 50  0001 C CNN
F 3 "~" H 6250 4350 50  0001 C CNN
	1    6250 4350
	1    0    0    -1  
$EndComp
Connection ~ 5450 4700
Wire Wire Line
	5450 4600 5450 4700
Connection ~ 5450 4600
Wire Wire Line
	5450 4500 5450 4600
Connection ~ 5450 4500
Wire Wire Line
	5450 4400 5450 4500
Connection ~ 5450 4400
Wire Wire Line
	5450 4300 5450 4400
Connection ~ 5450 4300
Wire Wire Line
	5450 4200 5450 4300
Connection ~ 5450 4200
Wire Wire Line
	5450 4100 5450 4200
Connection ~ 5450 4100
Wire Wire Line
	5450 4000 5450 4100
Wire Wire Line
	6050 4850 5450 4850
Wire Wire Line
	5450 4700 5450 4850
Wire Wire Line
	5450 4850 5450 4950
Connection ~ 5450 4850
$EndSCHEMATC
