EESchema Schematic File Version 4
LIBS:Muxtemp-cache
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
Text GLabel 6500 5050 2    50   Input ~ 0
3V3
$Comp
L Device:R R1
U 1 1 5AF665E6
P 6100 5250
F 0 "R1" H 6170 5296 50  0000 L CNN
F 1 "4k7" H 6170 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 6030 5250 50  0001 C CNN
F 3 "~" H 6100 5250 50  0001 C CNN
	1    6100 5250
	0    1    1    0   
$EndComp
Text GLabel 6750 4950 3    50   Input ~ 0
1wire
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J1
U 1 1 5AF93E8A
P 5700 4850
F 0 "J1" H 6100 5115 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 6100 5024 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 6350 4950 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 6350 4850 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 6350 4750 50  0001 L CNN "Description"
F 5 "" H 6350 4650 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 6350 4550 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 6350 4450 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 6350 4350 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 6350 4250 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 6350 4150 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 6350 4050 50  0001 L CNN "Arrow Price/Stock"
	1    5700 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5AF93F27
P 5600 4950
F 0 "#PWR07" H 5600 4700 50  0001 C CNN
F 1 "GND" H 5605 4777 50  0000 C CNN
F 2 "" H 5600 4950 50  0001 C CNN
F 3 "" H 5600 4950 50  0001 C CNN
	1    5600 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5050 5700 5250
Wire Wire Line
	5700 5250 5950 5250
Wire Wire Line
	6250 5250 6500 5250
Wire Wire Line
	6500 5250 6500 5050
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J2
U 1 1 5AFA4115
P 7150 4850
F 0 "J2" H 7550 5115 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 7550 5024 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 7800 4950 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 7800 4850 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 7800 4750 50  0001 L CNN "Description"
F 5 "" H 7800 4650 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 7800 4550 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 7800 4450 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 7800 4350 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 7800 4250 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 7800 4150 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 7800 4050 50  0001 L CNN "Arrow Price/Stock"
	1    7150 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5AFA411B
P 7050 4950
F 0 "#PWR08" H 7050 4700 50  0001 C CNN
F 1 "GND" H 7055 4777 50  0000 C CNN
F 2 "" H 7050 4950 50  0001 C CNN
F 3 "" H 7050 4950 50  0001 C CNN
	1    7050 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 5050 7150 5250
Wire Wire Line
	7150 5250 7400 5250
Wire Wire Line
	7700 5250 7950 5250
Wire Wire Line
	7950 5250 7950 5050
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J3
U 1 1 5AFA64CF
P 8600 4850
F 0 "J3" H 9000 5115 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 9000 5024 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 9250 4950 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 9250 4850 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 9250 4750 50  0001 L CNN "Description"
F 5 "" H 9250 4650 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 9250 4550 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 9250 4450 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 9250 4350 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 9250 4250 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 9250 4150 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 9250 4050 50  0001 L CNN "Arrow Price/Stock"
	1    8600 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5AFA64D5
P 8500 4950
F 0 "#PWR09" H 8500 4700 50  0001 C CNN
F 1 "GND" H 8505 4777 50  0000 C CNN
F 2 "" H 8500 4950 50  0001 C CNN
F 3 "" H 8500 4950 50  0001 C CNN
	1    8500 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5050 9450 5050
Wire Wire Line
	8600 5050 8600 5250
Wire Wire Line
	8600 5250 8850 5250
Wire Wire Line
	9150 5250 9400 5250
Wire Wire Line
	9400 5250 9400 5050
Connection ~ 9400 5050
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J4
U 1 1 5AFA64F5
P 10050 4850
F 0 "J4" H 10450 5115 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 10450 5024 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 10700 4950 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 10700 4850 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 10700 4750 50  0001 L CNN "Description"
F 5 "" H 10700 4650 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 10700 4550 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 10700 4450 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 10700 4350 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 10700 4250 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 10700 4150 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 10700 4050 50  0001 L CNN "Arrow Price/Stock"
	1    10050 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5050 10050 5250
Wire Wire Line
	10050 5250 10300 5250
Wire Wire Line
	10600 5250 10850 5250
Wire Wire Line
	10850 5250 10850 5050
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J5
U 1 1 5AFA7E57
P 5650 5800
F 0 "J5" H 6050 6065 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 6050 5974 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 6300 5900 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 6300 5800 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 6300 5700 50  0001 L CNN "Description"
F 5 "" H 6300 5600 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 6300 5500 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 6300 5400 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 6300 5300 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 6300 5200 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 6300 5100 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 6300 5000 50  0001 L CNN "Arrow Price/Stock"
	1    5650 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 6000 5650 6200
Wire Wire Line
	5650 6200 5900 6200
Wire Wire Line
	6200 6200 6450 6200
Wire Wire Line
	6450 6200 6450 6000
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J6
U 1 1 5AFA7E7D
P 7100 5800
F 0 "J6" H 7500 6065 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 7500 5974 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 7750 5900 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 7750 5800 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 7750 5700 50  0001 L CNN "Description"
F 5 "" H 7750 5600 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 7750 5500 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 7750 5400 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 7750 5300 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 7750 5200 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 7750 5100 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 7750 5000 50  0001 L CNN "Arrow Price/Stock"
	1    7100 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 6000 7100 6200
Wire Wire Line
	7100 6200 7350 6200
Wire Wire Line
	7650 6200 7900 6200
Wire Wire Line
	7900 6200 7900 6000
Text GLabel 8550 5800 0    50   Input ~ 0
dht_6
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J7
U 1 1 5AFA7EA1
P 8550 5800
F 0 "J7" H 8950 6065 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 8950 5974 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 9200 5900 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 9200 5800 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 9200 5700 50  0001 L CNN "Description"
F 5 "" H 9200 5600 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 9200 5500 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 9200 5400 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 9200 5300 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 9200 5200 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 9200 5100 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 9200 5000 50  0001 L CNN "Arrow Price/Stock"
	1    8550 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 6000 8550 6200
Wire Wire Line
	8550 6200 8800 6200
Wire Wire Line
	9100 6200 9350 6200
Wire Wire Line
	9350 6200 9350 6000
Text GLabel 10000 5800 0    50   Input ~ 0
dht_7
$Comp
L MHRJJ66NFRA:MHRJJ66NFRA J8
U 1 1 5AFA7EC5
P 10000 5800
F 0 "J8" H 10400 6065 50  0000 C CNN
F 1 "MHRJJ66NFRA" H 10400 5974 50  0000 C CNN
F 2 "MHRJJ66NFRA:MHRJJ66NFRA" H 10650 5900 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/2/MHRJJ66NFRA.pdf" H 10650 5800 50  0001 L CNN
F 4 "RJ12 6P6C right angled socket unshielded MH Connectors 6P6C Right Angle Through Hole Female Modular Socket UTP RJ12 Connector" H 10650 5700 50  0001 L CNN "Description"
F 5 "" H 10650 5600 50  0001 L CNN "Height"
F 6 "MH CONNECTORS" H 10650 5500 50  0001 L CNN "Manufacturer_Name"
F 7 "MHRJJ66NFRA" H 10650 5400 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "8094898" H 10650 5300 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8094898" H 10650 5200 50  0001 L CNN "RS Price/Stock"
F 10 "MHRJJ66NFRA" H 10650 5100 50  0001 L CNN "Arrow Part Number"
F 11 "http://uk.rs-online.com/web/p/products/8094898" H 10650 5000 50  0001 L CNN "Arrow Price/Stock"
	1    10000 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 6000 10000 6200
Wire Wire Line
	10000 6200 10250 6200
Wire Wire Line
	10550 6200 10800 6200
Wire Wire Line
	10800 6200 10800 6000
$Comp
L Device:R R2
U 1 1 5AFC833D
P 7550 5250
F 0 "R2" H 7620 5296 50  0000 L CNN
F 1 "4k7" H 7620 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7480 5250 50  0001 C CNN
F 3 "~" H 7550 5250 50  0001 C CNN
	1    7550 5250
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5AFC83A7
P 9000 5250
F 0 "R3" H 9070 5296 50  0000 L CNN
F 1 "4k7" H 9070 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 8930 5250 50  0001 C CNN
F 3 "~" H 9000 5250 50  0001 C CNN
	1    9000 5250
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5AFC8409
P 10450 5250
F 0 "R4" H 10520 5296 50  0000 L CNN
F 1 "4k7" H 10520 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 10380 5250 50  0001 C CNN
F 3 "~" H 10450 5250 50  0001 C CNN
	1    10450 5250
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5AFC8481
P 6050 6200
F 0 "R5" H 6120 6246 50  0000 L CNN
F 1 "4k7" H 6120 6155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5980 6200 50  0001 C CNN
F 3 "~" H 6050 6200 50  0001 C CNN
	1    6050 6200
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5AFC84FB
P 7500 6200
F 0 "R6" H 7570 6246 50  0000 L CNN
F 1 "4k7" H 7570 6155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7430 6200 50  0001 C CNN
F 3 "~" H 7500 6200 50  0001 C CNN
	1    7500 6200
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5AFC8577
P 10400 6200
F 0 "R8" H 10470 6246 50  0000 L CNN
F 1 "4k7" H 10470 6155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 10330 6200 50  0001 C CNN
F 3 "~" H 10400 6200 50  0001 C CNN
	1    10400 6200
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5AFC8645
P 8950 6200
F 0 "R7" H 9020 6246 50  0000 L CNN
F 1 "4k7" H 9020 6155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 8880 6200 50  0001 C CNN
F 3 "~" H 8950 6200 50  0001 C CNN
	1    8950 6200
	0    1    1    0   
$EndComp
Wire Wire Line
	5600 4950 5700 4950
Wire Wire Line
	7050 4950 7150 4950
Wire Wire Line
	8500 4950 8600 4950
$Comp
L power:GND #PWR010
U 1 1 5AF951DB
P 9950 4950
F 0 "#PWR010" H 9950 4700 50  0001 C CNN
F 1 "GND" H 9955 4777 50  0000 C CNN
F 2 "" H 9950 4950 50  0001 C CNN
F 3 "" H 9950 4950 50  0001 C CNN
	1    9950 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 4950 10050 4950
$Comp
L power:GND #PWR011
U 1 1 5AF982BE
P 5550 5900
F 0 "#PWR011" H 5550 5650 50  0001 C CNN
F 1 "GND" H 5555 5727 50  0000 C CNN
F 2 "" H 5550 5900 50  0001 C CNN
F 3 "" H 5550 5900 50  0001 C CNN
	1    5550 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 5900 5650 5900
$Comp
L power:GND #PWR012
U 1 1 5AF9B49C
P 7000 5900
F 0 "#PWR012" H 7000 5650 50  0001 C CNN
F 1 "GND" H 7005 5727 50  0000 C CNN
F 2 "" H 7000 5900 50  0001 C CNN
F 3 "" H 7000 5900 50  0001 C CNN
	1    7000 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5900 7100 5900
$Comp
L power:GND #PWR015
U 1 1 5AF9EBD5
P 8450 5900
F 0 "#PWR015" H 8450 5650 50  0001 C CNN
F 1 "GND" H 8455 5727 50  0000 C CNN
F 2 "" H 8450 5900 50  0001 C CNN
F 3 "" H 8450 5900 50  0001 C CNN
	1    8450 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 5900 8550 5900
$Comp
L power:GND #PWR016
U 1 1 5AFA1EA7
P 9900 5900
F 0 "#PWR016" H 9900 5650 50  0001 C CNN
F 1 "GND" H 9905 5727 50  0000 C CNN
F 2 "" H 9900 5900 50  0001 C CNN
F 3 "" H 9900 5900 50  0001 C CNN
	1    9900 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 5900 10000 5900
$Comp
L power:GND #PWR01
U 1 1 5AFABC81
P 6500 4850
F 0 "#PWR01" H 6500 4600 50  0001 C CNN
F 1 "GND" H 6505 4677 50  0000 C CNN
F 2 "" H 6500 4850 50  0001 C CNN
F 3 "" H 6500 4850 50  0001 C CNN
	1    6500 4850
	-1   0    0    1   
$EndComp
Text GLabel 7100 5800 0    50   Input ~ 0
dht_5
Text GLabel 5650 5800 0    50   Input ~ 0
dht_4
Text GLabel 10050 4850 0    50   Input ~ 0
dht_3
Text GLabel 8600 4850 0    50   Input ~ 0
dht_2
Text GLabel 7150 4850 0    50   Input ~ 0
dht_1
Text GLabel 5700 4850 0    50   Input ~ 0
dht_0
$Comp
L power:GND #PWR021
U 1 1 5B06601C
P 4950 3200
F 0 "#PWR021" H 4950 2950 50  0001 C CNN
F 1 "GND" V 4955 3027 50  0000 C CNN
F 2 "" H 4950 3200 50  0001 C CNN
F 3 "" H 4950 3200 50  0001 C CNN
	1    4950 3200
	0    -1   -1   0   
$EndComp
Text GLabel 5500 3100 2    50   Input ~ 0
3V3
Text GLabel 8150 2350 0    50   Input ~ 0
dht_0
Text GLabel 8150 2450 0    50   Input ~ 0
dht_1
Text GLabel 8150 2550 0    50   Input ~ 0
dht_2
Text GLabel 8150 2650 0    50   Input ~ 0
dht_3
Text GLabel 8150 2750 0    50   Input ~ 0
dht_4
Text GLabel 8150 2850 0    50   Input ~ 0
dht_5
Text GLabel 8150 2950 0    50   Input ~ 0
dht_6
Text GLabel 8150 3050 0    50   Input ~ 0
dht_7
Wire Wire Line
	7850 2400 8150 2400
Wire Wire Line
	8150 2350 8150 2400
$Comp
L Device:R R9
U 1 1 5B08A8A2
P 7700 2400
F 0 "R9" V 7700 2750 50  0000 C CNN
F 1 "33k" V 7700 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2400 50  0001 C CNN
F 3 "~" H 7700 2400 50  0001 C CNN
	1    7700 2400
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5B08E015
P 7550 3350
F 0 "#PWR018" H 7550 3100 50  0001 C CNN
F 1 "GND" H 7555 3177 50  0000 C CNN
F 2 "" H 7550 3350 50  0001 C CNN
F 3 "" H 7550 3350 50  0001 C CNN
	1    7550 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3100 8150 3100
Wire Wire Line
	8150 3100 8150 3050
Wire Wire Line
	7850 3000 8150 3000
Wire Wire Line
	8150 3000 8150 2950
Wire Wire Line
	7850 2900 8150 2900
Wire Wire Line
	8150 2900 8150 2850
Wire Wire Line
	7850 2800 8150 2800
Wire Wire Line
	8150 2800 8150 2750
Wire Wire Line
	7850 2700 8150 2700
Wire Wire Line
	8150 2700 8150 2650
Wire Wire Line
	7850 2600 8150 2600
Wire Wire Line
	8150 2600 8150 2550
Wire Wire Line
	7850 2500 8150 2500
Wire Wire Line
	8150 2500 8150 2450
$Comp
L Device:Fuse F1
U 1 1 5B0ABB62
P 5350 3100
F 0 "F1" V 5153 3100 50  0000 C CNN
F 1 "500mA" V 5244 3100 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" V 5280 3100 50  0001 C CNN
F 3 "~" H 5350 3100 50  0001 C CNN
	1    5350 3100
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5B075ADF
P 7700 2500
F 0 "R10" V 7700 2850 50  0000 C CNN
F 1 "33k" V 7700 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2500 50  0001 C CNN
F 3 "~" H 7700 2500 50  0001 C CNN
	1    7700 2500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5B075B27
P 7700 2600
F 0 "R11" V 7700 2950 50  0000 C CNN
F 1 "33k" V 7700 3150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2600 50  0001 C CNN
F 3 "~" H 7700 2600 50  0001 C CNN
	1    7700 2600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5B075B6D
P 7700 2700
F 0 "R12" V 7700 3050 50  0000 C CNN
F 1 "33k" V 7700 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2700 50  0001 C CNN
F 3 "~" H 7700 2700 50  0001 C CNN
	1    7700 2700
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R13
U 1 1 5B075BB5
P 7700 2800
F 0 "R13" V 7700 3150 50  0000 C CNN
F 1 "33k" V 7700 3350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2800 50  0001 C CNN
F 3 "~" H 7700 2800 50  0001 C CNN
	1    7700 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 5B075BFF
P 7700 2900
F 0 "R14" V 7700 3250 50  0000 C CNN
F 1 "33k" V 7700 3450 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 2900 50  0001 C CNN
F 3 "~" H 7700 2900 50  0001 C CNN
	1    7700 2900
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5B075C4B
P 7700 3000
F 0 "R15" V 7700 3350 50  0000 C CNN
F 1 "33k" V 7700 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 3000 50  0001 C CNN
F 3 "~" H 7700 3000 50  0001 C CNN
	1    7700 3000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5B075C99
P 7700 3100
F 0 "R16" V 7700 3450 50  0000 C CNN
F 1 "33k" V 7700 3650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 3100 50  0001 C CNN
F 3 "~" H 7700 3100 50  0001 C CNN
	1    7700 3100
	0    -1   -1   0   
$EndComp
Connection ~ 7550 3100
Wire Wire Line
	7550 3000 7550 3100
Connection ~ 7550 3000
Wire Wire Line
	7550 2900 7550 3000
Connection ~ 7550 2900
Wire Wire Line
	7550 2800 7550 2900
Connection ~ 7550 2800
Wire Wire Line
	7550 2700 7550 2800
Connection ~ 7550 2700
Wire Wire Line
	7550 2600 7550 2700
Connection ~ 7550 2600
Wire Wire Line
	7550 2500 7550 2600
Connection ~ 7550 2500
Wire Wire Line
	7550 2400 7550 2500
Wire Wire Line
	7550 3100 7550 3200
Text GLabel 2100 1500 2    50   Input ~ 0
3V3_L
$Comp
L power:GND #PWR022
U 1 1 5B3B302D
P 2000 4500
F 0 "#PWR022" H 2000 4250 50  0001 C CNN
F 1 "GND" H 2005 4327 50  0000 C CNN
F 2 "" H 2000 4500 50  0001 C CNN
F 3 "" H 2000 4500 50  0001 C CNN
	1    2000 4500
	1    0    0    -1  
$EndComp
Text GLabel 2600 1900 2    50   Input ~ 0
dht_3
Text GLabel 2600 1800 2    50   Input ~ 0
dht_4
Text GLabel 2600 4200 2    50   Input ~ 0
dht_5
Text GLabel 2600 4100 2    50   Input ~ 0
dht_6
Text GLabel 2600 4000 2    50   Input ~ 0
dht_7
Text GLabel 2600 2200 2    50   Input ~ 0
dht_0
Text GLabel 2600 2100 2    50   Input ~ 0
dht_1
Text GLabel 2600 2000 2    50   Input ~ 0
dht_2
Text GLabel 2600 3100 2    50   Input ~ 0
SDA
Text GLabel 2600 3200 2    50   Input ~ 0
SCL
$Comp
L Device:R R19
U 1 1 5B3B44F9
P 2750 3300
F 0 "R19" V 2850 3300 50  0000 C CNN
F 1 "10k" V 2950 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2680 3300 50  0001 C CNN
F 3 "~" H 2750 3300 50  0001 C CNN
	1    2750 3300
	0    1    1    0   
$EndComp
Text GLabel 4600 1450 0    50   Input ~ 0
3V3_L
$Comp
L power:GND #PWR019
U 1 1 5B3C5966
P 4600 1550
F 0 "#PWR019" H 4600 1300 50  0001 C CNN
F 1 "GND" V 4600 1350 50  0000 C CNN
F 2 "" H 4600 1550 50  0001 C CNN
F 3 "" H 4600 1550 50  0001 C CNN
	1    4600 1550
	0    1    1    0   
$EndComp
Text GLabel 4600 1850 0    50   Input ~ 0
SDA
Text GLabel 4600 1950 0    50   Input ~ 0
SCL
Text GLabel 4600 1650 0    50   Input ~ 0
1wire
Text GLabel 4600 1350 0    50   Input ~ 0
Vin
$Comp
L Device:Jumper_NO_Small JP9
U 1 1 5B3DFEA5
P 2700 2800
F 0 "JP9" H 2700 3000 50  0000 C CNN
F 1 "1wireBypass" H 2850 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 2700 2800 50  0001 C CNN
F 3 "~" H 2700 2800 50  0001 C CNN
	1    2700 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5B3E05D1
P 2800 2800
F 0 "#PWR020" H 2800 2550 50  0001 C CNN
F 1 "GND" V 2805 2627 50  0000 C CNN
F 2 "" H 2800 2800 50  0001 C CNN
F 3 "" H 2800 2800 50  0001 C CNN
	1    2800 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:Jumper_NO_Small JP1
U 1 1 5B3E3946
P 6600 4950
F 0 "JP1" H 6650 5100 50  0000 C CNN
F 1 "1wireBypass" H 6650 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 6600 4950 50  0001 C CNN
F 3 "~" H 6600 4950 50  0001 C CNN
	1    6600 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4950 6750 4950
Text GLabel 7950 5050 2    50   Input ~ 0
3V3
Text GLabel 8200 4950 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR02
U 1 1 5B4137E3
P 7950 4850
F 0 "#PWR02" H 7950 4600 50  0001 C CNN
F 1 "GND" H 7955 4677 50  0000 C CNN
F 2 "" H 7950 4850 50  0001 C CNN
F 3 "" H 7950 4850 50  0001 C CNN
	1    7950 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP2
U 1 1 5B4137E9
P 8050 4950
F 0 "JP2" H 8100 5100 50  0000 C CNN
F 1 "1wireBypass" H 8100 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 8050 4950 50  0001 C CNN
F 3 "~" H 8050 4950 50  0001 C CNN
	1    8050 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 4950 8200 4950
Text GLabel 9400 5050 2    50   Input ~ 0
3V3
Text GLabel 9650 4950 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR03
U 1 1 5B418C4D
P 9400 4850
F 0 "#PWR03" H 9400 4600 50  0001 C CNN
F 1 "GND" H 9405 4677 50  0000 C CNN
F 2 "" H 9400 4850 50  0001 C CNN
F 3 "" H 9400 4850 50  0001 C CNN
	1    9400 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP3
U 1 1 5B418C53
P 9500 4950
F 0 "JP3" H 9550 5100 50  0000 C CNN
F 1 "1wireBypass" H 9550 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 9500 4950 50  0001 C CNN
F 3 "~" H 9500 4950 50  0001 C CNN
	1    9500 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 4950 9650 4950
Text GLabel 10850 5050 2    50   Input ~ 0
3V3
Text GLabel 11100 4950 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR04
U 1 1 5B426029
P 10850 4850
F 0 "#PWR04" H 10850 4600 50  0001 C CNN
F 1 "GND" H 10855 4677 50  0000 C CNN
F 2 "" H 10850 4850 50  0001 C CNN
F 3 "" H 10850 4850 50  0001 C CNN
	1    10850 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP4
U 1 1 5B42602F
P 10950 4950
F 0 "JP4" H 11000 5100 50  0000 C CNN
F 1 "1wireBypass" H 11000 5000 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 10950 4950 50  0001 C CNN
F 3 "~" H 10950 4950 50  0001 C CNN
	1    10950 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	11050 4950 11100 4950
Text GLabel 6450 6000 2    50   Input ~ 0
3V3
Text GLabel 6700 5900 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR05
U 1 1 5B439115
P 6450 5800
F 0 "#PWR05" H 6450 5550 50  0001 C CNN
F 1 "GND" H 6455 5627 50  0000 C CNN
F 2 "" H 6450 5800 50  0001 C CNN
F 3 "" H 6450 5800 50  0001 C CNN
	1    6450 5800
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP5
U 1 1 5B43911B
P 6550 5900
F 0 "JP5" H 6600 6050 50  0000 C CNN
F 1 "1wireBypass" H 6600 5950 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 6550 5900 50  0001 C CNN
F 3 "~" H 6550 5900 50  0001 C CNN
	1    6550 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 5900 6700 5900
Text GLabel 10800 6000 2    50   Input ~ 0
3V3
Text GLabel 11050 5900 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR014
U 1 1 5B443AAB
P 10800 5800
F 0 "#PWR014" H 10800 5550 50  0001 C CNN
F 1 "GND" H 10805 5627 50  0000 C CNN
F 2 "" H 10800 5800 50  0001 C CNN
F 3 "" H 10800 5800 50  0001 C CNN
	1    10800 5800
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP8
U 1 1 5B443AB1
P 10900 5900
F 0 "JP8" H 10950 6050 50  0000 C CNN
F 1 "1wireBypass" H 10950 5950 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 10900 5900 50  0001 C CNN
F 3 "~" H 10900 5900 50  0001 C CNN
	1    10900 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	11000 5900 11050 5900
Text GLabel 9350 6000 2    50   Input ~ 0
3V3
Text GLabel 9600 5900 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR013
U 1 1 5B448DA5
P 9350 5800
F 0 "#PWR013" H 9350 5550 50  0001 C CNN
F 1 "GND" H 9355 5627 50  0000 C CNN
F 2 "" H 9350 5800 50  0001 C CNN
F 3 "" H 9350 5800 50  0001 C CNN
	1    9350 5800
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP7
U 1 1 5B448DAB
P 9450 5900
F 0 "JP7" H 9500 6050 50  0000 C CNN
F 1 "1wireBypass" H 9500 5950 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 9450 5900 50  0001 C CNN
F 3 "~" H 9450 5900 50  0001 C CNN
	1    9450 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5900 9600 5900
Text GLabel 8150 3150 0    50   Input ~ 0
1wire
$Comp
L Device:R R17
U 1 1 5B452E98
P 7700 3200
F 0 "R17" V 7700 3550 50  0000 C CNN
F 1 "100k" V 7700 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 7630 3200 50  0001 C CNN
F 3 "~" H 7700 3200 50  0001 C CNN
	1    7700 3200
	0    -1   -1   0   
$EndComp
Connection ~ 7550 3200
Wire Wire Line
	7550 3200 7550 3350
Wire Wire Line
	7850 3200 8150 3200
Wire Wire Line
	8150 3200 8150 3150
Text GLabel 7900 6000 2    50   Input ~ 0
3V3
Text GLabel 8150 5900 3    50   Input ~ 0
1wire
$Comp
L power:GND #PWR06
U 1 1 5B4594A2
P 7900 5800
F 0 "#PWR06" H 7900 5550 50  0001 C CNN
F 1 "GND" H 7905 5627 50  0000 C CNN
F 2 "" H 7900 5800 50  0001 C CNN
F 3 "" H 7900 5800 50  0001 C CNN
	1    7900 5800
	-1   0    0    1   
$EndComp
$Comp
L Device:Jumper_NO_Small JP6
U 1 1 5B4594A8
P 8000 5900
F 0 "JP6" H 8050 6050 50  0000 C CNN
F 1 "1wireBypass" H 8050 5950 50  0000 C CNN
F 2 "Resistor_SMD:R_0612_1632Metric" H 8000 5900 50  0001 C CNN
F 3 "~" H 8000 5900 50  0001 C CNN
	1    8000 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 5900 8150 5900
$Comp
L MCU_Microchip_ATmega:ATmega328P-AU U1
U 1 1 5B47C124
P 2000 3000
F 0 "U1" H 2000 1414 50  0000 C CNN
F 1 "ATmega328P-AU" H 2000 1323 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 2000 3000 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet.pdf" H 2000 3000 50  0001 C CNN
	1    2000 3000
	1    0    0    -1  
$EndComp
Text GLabel 5450 2800 2    50   Input ~ 0
3V3_L
Text GLabel 3200 3300 2    50   Input ~ 0
3V3_L
$Comp
L Connector_Generic:Conn_01x04 J9
U 1 1 5B491CA8
P 3350 1950
F 0 "J9" H 3430 1942 50  0000 L CNN
F 1 "ATmega Prog" H 3430 1851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3350 1950 50  0001 C CNN
F 3 "~" H 3350 1950 50  0001 C CNN
	1    3350 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2200 2600 2150
Wire Wire Line
	2600 2100 2600 2050
Wire Wire Line
	2600 2000 2600 1950
$Comp
L power:GND #PWR017
U 1 1 5B499B0C
P 3150 1850
F 0 "#PWR017" H 3150 1600 50  0001 C CNN
F 1 "GND" V 3150 1650 50  0000 C CNN
F 2 "" H 3150 1850 50  0001 C CNN
F 3 "" H 3150 1850 50  0001 C CNN
	1    3150 1850
	0    1    1    0   
$EndComp
$Comp
L Connector_Specialized:Conn_01x04_Male J11
U 1 1 5B3BDD3F
P 4750 3200
F 0 "J11" H 4856 3478 50  0000 C CNN
F 1 "Power Supply" H 4856 3387 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4750 3200 50  0001 C CNN
F 3 "~" H 4750 3200 50  0001 C CNN
	1    4750 3200
	1    0    0    -1  
$EndComp
Text GLabel 5250 3300 2    50   Input ~ 0
Vin
Wire Wire Line
	4950 3100 5150 3100
Wire Wire Line
	5150 3100 5150 2800
Wire Wire Line
	5150 2800 5450 2800
Connection ~ 5150 3100
Wire Wire Line
	5150 3100 5200 3100
$Comp
L Device:R R18
U 1 1 5B3CA5CE
P 5100 3400
F 0 "R18" V 5000 3400 50  0000 C CNN
F 1 "100k" V 4900 3400 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 5030 3400 50  0001 C CNN
F 3 "~" H 5100 3400 50  0001 C CNN
	1    5100 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 3300 5250 3300
Wire Wire Line
	5250 3300 5250 3400
$Comp
L Connector_Generic:Conn_01x07 J10
U 1 1 5B3FBF0B
P 4800 1650
F 0 "J10" H 4879 1692 50  0000 L CNN
F 1 "Conn" H 4879 1601 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Horizontal" H 4800 1650 50  0001 C CNN
F 3 "~" H 4800 1650 50  0001 C CNN
	1    4800 1650
	1    0    0    1   
$EndComp
Text GLabel 2600 3000 2    50   Input ~ 0
Lock
Text GLabel 2600 2900 2    50   Input ~ 0
1wire
Wire Wire Line
	2000 1500 2100 1500
Text GLabel 4600 1750 0    50   Input ~ 0
Lock
Wire Wire Line
	2900 3300 3200 3300
Wire Wire Line
	2600 1950 3150 1950
Wire Wire Line
	2600 2050 3150 2050
Wire Wire Line
	2600 2150 3150 2150
$EndSCHEMATC
