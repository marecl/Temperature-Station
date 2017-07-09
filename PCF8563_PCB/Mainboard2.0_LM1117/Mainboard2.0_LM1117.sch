EESchema Schematic File Version 2
LIBS:Mainboard2.0_LM1117-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ESP8266
LIBS:Mainboard2.0_LM1117-cache
EELAYER 25 0
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
Text GLabel 1900 6550 0    60   Input ~ 0
MOSI
Text GLabel 1900 6650 0    60   Input ~ 0
MISO
Text GLabel 1900 6750 0    60   Input ~ 0
SCK
Text GLabel 5050 4900 2    60   Input ~ 0
CS
Text GLabel 1900 6850 0    60   Input ~ 0
1wire
Text GLabel 1900 6950 0    60   Input ~ 0
SDA
Text GLabel 1900 7050 0    60   Input ~ 0
SCL
Text GLabel 3250 4700 0    60   Input ~ 0
GPIO16
$Comp
L R R5
U 1 1 591DCE97
P 2550 6200
F 0 "R5" V 2630 6200 50  0000 C CNN
F 1 "4k7" V 2550 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2480 6200 50  0001 C CNN
F 3 "" H 2550 6200 50  0001 C CNN
	1    2550 6200
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 591DCEFA
P 2700 6200
F 0 "R6" V 2780 6200 50  0000 C CNN
F 1 "4k7" V 2700 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2630 6200 50  0001 C CNN
F 3 "" H 2700 6200 50  0001 C CNN
	1    2700 6200
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 591DCF1C
P 2850 6200
F 0 "R7" V 2930 6200 50  0000 C CNN
F 1 "4k7" V 2850 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2780 6200 50  0001 C CNN
F 3 "" H 2850 6200 50  0001 C CNN
	1    2850 6200
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 J5
U 1 1 591DD18C
P 6200 6800
F 0 "J5" H 6200 7000 50  0000 C CNN
F 1 "CONN_01X03" V 6300 6800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm_SMD_Pin1Left" H 6200 6800 50  0001 C CNN
F 3 "" H 6200 6800 50  0001 C CNN
	1    6200 6800
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X03 J1
U 1 1 591DD1E5
P 6200 6300
F 0 "J1" H 6200 6500 50  0000 C CNN
F 1 "CONN_01X03" V 6300 6300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm_SMD_Pin1Left" H 6200 6300 50  0001 C CNN
F 3 "" H 6200 6300 50  0001 C CNN
	1    6200 6300
	0    -1   -1   0   
$EndComp
Text GLabel 5850 7000 0    60   Input ~ 0
3V3
Text GLabel 5850 7100 0    60   Input ~ 0
1wire
$Comp
L GND #PWR01
U 1 1 591DD301
P 5850 7200
F 0 "#PWR01" H 5850 6950 50  0001 C CNN
F 1 "GND" H 5850 7050 50  0000 C CNN
F 2 "" H 5850 7200 50  0001 C CNN
F 3 "" H 5850 7200 50  0001 C CNN
	1    5850 7200
	0    1    1    0   
$EndComp
$Comp
L SD_Card J4
U 1 1 591DD53A
P 8650 1900
F 0 "J4" H 8000 2450 50  0000 C CNN
F 1 "SD_Card" H 9250 1350 50  0000 C CNN
F 2 "Moje:SDCARD-STANDARD" H 8850 2250 50  0001 C CNN
F 3 "" H 8650 1900 50  0001 C CNN
	1    8650 1900
	1    0    0    -1  
$EndComp
Text GLabel 7750 1600 0    60   Input ~ 0
CS
Text GLabel 7750 1700 0    60   Input ~ 0
MOSI
$Comp
L GND #PWR02
U 1 1 591DDAB3
P 7750 1800
F 0 "#PWR02" H 7750 1550 50  0001 C CNN
F 1 "GND" H 7750 1650 50  0000 C CNN
F 2 "" H 7750 1800 50  0001 C CNN
F 3 "" H 7750 1800 50  0001 C CNN
	1    7750 1800
	0    1    1    0   
$EndComp
Text GLabel 7750 2000 0    60   Input ~ 0
SCK
$Comp
L GND #PWR03
U 1 1 591DDB59
P 7750 2100
F 0 "#PWR03" H 7750 1850 50  0001 C CNN
F 1 "GND" H 7750 1950 50  0000 C CNN
F 2 "" H 7750 2100 50  0001 C CNN
F 3 "" H 7750 2100 50  0001 C CNN
	1    7750 2100
	0    1    1    0   
$EndComp
Text GLabel 7750 2200 0    60   Input ~ 0
MISO
$Comp
L R R4
U 1 1 591DDBFF
P 1950 6200
F 0 "R4" V 2030 6200 50  0000 C CNN
F 1 "10k" V 1950 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 1880 6200 50  0001 C CNN
F 3 "" H 1950 6200 50  0001 C CNN
	1    1950 6200
	1    0    0    -1  
$EndComp
Text GLabel 10000 1700 2    60   Input ~ 0
SD_D
$Comp
L R R1
U 1 1 591DE219
P 2100 6200
F 0 "R1" V 2180 6200 50  0000 C CNN
F 1 "10k" V 2100 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2030 6200 50  0001 C CNN
F 3 "" H 2100 6200 50  0001 C CNN
	1    2100 6200
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 591DE506
P 2250 6200
F 0 "R2" V 2330 6200 50  0000 C CNN
F 1 "10k" V 2250 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2180 6200 50  0001 C CNN
F 3 "" H 2250 6200 50  0001 C CNN
	1    2250 6200
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 591DE541
P 2400 6200
F 0 "R3" V 2480 6200 50  0000 C CNN
F 1 "10k" V 2400 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2330 6200 50  0001 C CNN
F 3 "" H 2400 6200 50  0001 C CNN
	1    2400 6200
	1    0    0    -1  
$EndComp
Text GLabel 1800 6050 0    60   Input ~ 0
3V3
$Comp
L PCF8563 U2
U 1 1 591DECCB
P 8450 5750
F 0 "U2" H 8150 6100 50  0000 L CNN
F 1 "PCF8563" H 8550 6100 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 8450 5750 50  0001 C CNN
F 3 "" H 8450 5750 50  0001 C CNN
	1    8450 5750
	1    0    0    -1  
$EndComp
Text GLabel 8850 5550 2    60   Input ~ 0
SCL
Text GLabel 8850 5650 2    60   Input ~ 0
SDA
$Comp
L Crystal Y1
U 1 1 591DEFF4
P 7950 5750
F 0 "Y1" H 7950 5900 50  0000 C CNN
F 1 "Crystal" H 7950 5600 50  0000 C CNN
F 2 "Crystals:Crystal_AT310_d3.0mm_l10.0mm_Horizontal" H 7950 5750 50  0001 C CNN
F 3 "" H 7950 5750 50  0001 C CNN
	1    7950 5750
	0    1    1    0   
$EndComp
$Comp
L D D3
U 1 1 591DF21A
P 8300 5000
F 0 "D3" H 8300 5100 50  0000 C CNN
F 1 "1N4148" H 8300 4900 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF" H 8300 5000 50  0001 C CNN
F 3 "" H 8300 5000 50  0001 C CNN
	1    8300 5000
	-1   0    0    1   
$EndComp
$Comp
L D D1
U 1 1 591DF401
P 8450 4700
F 0 "D1" H 8450 4800 50  0000 C CNN
F 1 "1N4148" H 8450 4600 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF" H 8450 4700 50  0001 C CNN
F 3 "" H 8450 4700 50  0001 C CNN
	1    8450 4700
	0    -1   -1   0   
$EndComp
Text GLabel 8450 4550 1    60   Input ~ 0
3V3
$Comp
L GND #PWR04
U 1 1 591DF8F9
P 8450 6150
F 0 "#PWR04" H 8450 5900 50  0001 C CNN
F 1 "GND" H 8450 6000 50  0000 C CNN
F 2 "" H 8450 6150 50  0001 C CNN
F 3 "" H 8450 6150 50  0001 C CNN
	1    8450 6150
	1    0    0    -1  
$EndComp
$Comp
L LM1117-3.3-RESCUE-Mainboard2.0_LM1117 U1
U 1 1 591E00AF
P 8950 3800
F 0 "U1" H 9050 3550 50  0000 C CNN
F 1 "LM1117-3.3" H 8950 4050 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3Lead_TabPin2" H 8950 3800 50  0001 C CNN
F 3 "" H 8950 3800 50  0001 C CNN
	1    8950 3800
	1    0    0    -1  
$EndComp
$Comp
L D D2
U 1 1 591E10E9
P 8250 3150
F 0 "D2" H 8250 3250 50  0000 C CNN
F 1 "1N4007" H 8250 3050 50  0000 C CNN
F 2 "Diodes_SMD:D_SMA" H 8250 3150 50  0001 C CNN
F 3 "" H 8250 3150 50  0001 C CNN
	1    8250 3150
	-1   0    0    1   
$EndComp
$Comp
L C C1
U 1 1 591E1A2B
P 8250 3450
F 0 "C1" H 8275 3550 50  0000 L CNN
F 1 "10nF" H 8275 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 8288 3300 50  0001 C CNN
F 3 "" H 8250 3450 50  0001 C CNN
	1    8250 3450
	0    1    1    0   
$EndComp
$Comp
L CP C2
U 1 1 591E1ABE
P 8250 3650
F 0 "C2" H 8275 3750 50  0000 L CNN
F 1 "10uF" H 8275 3550 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-A_EIA-3216-18_Hand" H 8288 3500 50  0001 C CNN
F 3 "" H 8250 3650 50  0001 C CNN
	1    8250 3650
	0    1    1    0   
$EndComp
$Comp
L C C3
U 1 1 591E1D93
P 9550 3950
F 0 "C3" H 9575 4050 50  0000 L CNN
F 1 "10nF" H 9575 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9588 3800 50  0001 C CNN
F 3 "" H 9550 3950 50  0001 C CNN
	1    9550 3950
	1    0    0    -1  
$EndComp
$Comp
L CP C4
U 1 1 591E1E6F
P 9750 3950
F 0 "C4" H 9775 4050 50  0000 L CNN
F 1 "10uF" H 9775 3850 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-A_EIA-3216-18_Hand" H 9788 3800 50  0001 C CNN
F 3 "" H 9750 3950 50  0001 C CNN
	1    9750 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 J6
U 1 1 591E39F7
P 7950 4800
F 0 "J6" H 7950 4950 50  0000 C CNN
F 1 "CONN_01X02" V 8050 4800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm_SMD_Pin1Left" H 7950 4800 50  0001 C CNN
F 3 "" H 7950 4800 50  0001 C CNN
	1    7950 4800
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR05
U 1 1 591E7037
P 9550 2200
F 0 "#PWR05" H 9550 1950 50  0001 C CNN
F 1 "GND" H 9550 2050 50  0000 C CNN
F 2 "" H 9550 2200 50  0001 C CNN
F 3 "" H 9550 2200 50  0001 C CNN
	1    9550 2200
	1    0    0    -1  
$EndComp
$Comp
L R RJ1
U 1 1 591F36DF
P 2150 6750
F 0 "RJ1" V 2230 6750 50  0000 C CNN
F 1 "0" V 2150 6750 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2080 6750 50  0001 C CNN
F 3 "" H 2150 6750 50  0001 C CNN
	1    2150 6750
	0    1    1    0   
$EndComp
Text GLabel 4350 6450 0    60   Input ~ 0
3V3
Text GLabel 4650 6450 2    60   Input ~ 0
3V3
$Comp
L R RJ2
U 1 1 591F46BE
P 4500 6450
F 0 "RJ2" V 4580 6450 50  0000 C CNN
F 1 "0" V 4500 6450 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4430 6450 50  0001 C CNN
F 3 "" H 4500 6450 50  0001 C CNN
	1    4500 6450
	0    1    1    0   
$EndComp
$Comp
L R RJ3
U 1 1 591F4C30
P 4500 6650
F 0 "RJ3" V 4580 6650 50  0000 C CNN
F 1 "0" V 4500 6650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4430 6650 50  0001 C CNN
F 3 "" H 4500 6650 50  0001 C CNN
	1    4500 6650
	0    1    1    0   
$EndComp
$Comp
L R RJ4
U 1 1 591F59BA
P 4500 6850
F 0 "RJ4" V 4580 6850 50  0000 C CNN
F 1 "0" V 4500 6850 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4430 6850 50  0001 C CNN
F 3 "" H 4500 6850 50  0001 C CNN
	1    4500 6850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR06
U 1 1 591F5A39
P 4650 6850
F 0 "#PWR06" H 4650 6600 50  0001 C CNN
F 1 "GND" H 4650 6700 50  0000 C CNN
F 2 "" H 4650 6850 50  0001 C CNN
F 3 "" H 4650 6850 50  0001 C CNN
	1    4650 6850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR07
U 1 1 591F5A9B
P 4350 6850
F 0 "#PWR07" H 4350 6600 50  0001 C CNN
F 1 "GND" H 4350 6700 50  0000 C CNN
F 2 "" H 4350 6850 50  0001 C CNN
F 3 "" H 4350 6850 50  0001 C CNN
	1    4350 6850
	0    1    1    0   
$EndComp
$Comp
L CONN_01X02 J7
U 1 1 591F80FA
P 7650 3300
F 0 "J7" H 7650 3450 50  0000 C CNN
F 1 "CONN_01X02" V 7750 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm_SMD_Pin1Left" H 7650 3300 50  0001 C CNN
F 3 "" H 7650 3300 50  0001 C CNN
	1    7650 3300
	-1   0    0    1   
$EndComp
$Comp
L R RJ6
U 1 1 591F89CE
P 9700 1700
F 0 "RJ6" V 9780 1700 50  0000 C CNN
F 1 "0" V 9700 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 9630 1700 50  0001 C CNN
F 3 "" H 9700 1700 50  0001 C CNN
	1    9700 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 6500 6100 7000
Wire Wire Line
	6200 6500 6200 7100
Wire Wire Line
	6300 6500 6300 7200
Wire Wire Line
	6100 7000 5850 7000
Wire Wire Line
	6200 7100 5850 7100
Connection ~ 6200 7000
Wire Wire Line
	6300 7200 5850 7200
Connection ~ 6300 7000
Wire Wire Line
	1900 6650 3000 6650
Wire Wire Line
	1900 6550 3000 6550
Wire Wire Line
	2100 6350 2100 6550
Connection ~ 2100 6550
Wire Wire Line
	2250 6350 2250 6650
Connection ~ 2250 6650
Wire Wire Line
	2400 6350 2400 6750
Connection ~ 2400 6750
Wire Wire Line
	1800 6050 2850 6050
Connection ~ 2100 6050
Connection ~ 2250 6050
Wire Wire Line
	8050 5950 7950 5950
Wire Wire Line
	7950 5950 7950 5900
Wire Wire Line
	7950 5600 7950 5550
Wire Wire Line
	7950 5550 8050 5550
Wire Wire Line
	7650 5000 7650 6150
Wire Wire Line
	7650 6150 8450 6150
Wire Wire Line
	8450 4850 8450 5350
Connection ~ 8450 5000
Wire Wire Line
	8650 3150 8650 3800
Wire Wire Line
	7850 3350 7850 4100
Wire Wire Line
	8400 3450 8650 3450
Connection ~ 8650 3450
Wire Wire Line
	8400 3650 8650 3650
Connection ~ 8650 3650
Wire Wire Line
	8100 3650 7850 3650
Connection ~ 7850 3650
Wire Wire Line
	8100 3450 7850 3450
Connection ~ 7850 3450
Connection ~ 9750 3800
Connection ~ 9550 4100
Wire Wire Line
	7650 5000 7900 5000
Wire Wire Line
	8000 5000 8150 5000
Wire Wire Line
	9550 2000 9550 2200
Connection ~ 9550 2100
Wire Wire Line
	9250 3800 9900 3800
Connection ~ 9550 3800
Wire Wire Line
	2300 6750 3000 6750
Connection ~ 9750 4100
Wire Wire Line
	9850 1700 10000 1700
Text GLabel 7750 1900 0    60   Input ~ 0
3V3
$Comp
L R RJ5
U 1 1 59332C6E
P 4500 7050
F 0 "RJ5" V 4580 7050 50  0000 C CNN
F 1 "0" V 4500 7050 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4430 7050 50  0001 C CNN
F 3 "" H 4500 7050 50  0001 C CNN
	1    4500 7050
	0    1    1    0   
$EndComp
$Comp
L GND #PWR08
U 1 1 59332CEF
P 4350 7050
F 0 "#PWR08" H 4350 6800 50  0001 C CNN
F 1 "GND" H 4350 6900 50  0000 C CNN
F 2 "" H 4350 7050 50  0001 C CNN
F 3 "" H 4350 7050 50  0001 C CNN
	1    4350 7050
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 59332D63
P 4650 7050
F 0 "#PWR09" H 4650 6800 50  0001 C CNN
F 1 "GND" H 4650 6900 50  0000 C CNN
F 2 "" H 4650 7050 50  0001 C CNN
F 3 "" H 4650 7050 50  0001 C CNN
	1    4650 7050
	0    -1   -1   0   
$EndComp
Text GLabel 9900 3800 2    60   Input ~ 0
3V3
$Comp
L GND #PWR010
U 1 1 594541DB
P 10000 4100
F 0 "#PWR010" H 10000 3850 50  0001 C CNN
F 1 "GND" H 10000 3950 50  0000 C CNN
F 2 "" H 10000 4100 50  0001 C CNN
F 3 "" H 10000 4100 50  0001 C CNN
	1    10000 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 4100 10000 4100
Text GLabel 4650 6650 2    60   Input ~ 0
3V3
Text GLabel 4350 6650 0    60   Input ~ 0
3V3
Connection ~ 8950 4100
$Comp
L ESP-12E U4
U 1 1 59610456
P 4150 4700
F 0 "U4" H 4150 4600 50  0000 C CNN
F 1 "ESP-12E" H 4150 4800 50  0000 C CNN
F 2 "ESP8266:ESP-12E_SMD" H 4150 4700 50  0001 C CNN
F 3 "" H 4150 4700 50  0001 C CNN
	1    4150 4700
	1    0    0    -1  
$EndComp
Text GLabel 3250 5100 0    60   Input ~ 0
3V3
$Comp
L GND #PWR011
U 1 1 59611A33
P 5050 5100
F 0 "#PWR011" H 5050 4850 50  0001 C CNN
F 1 "GND" H 5050 4950 50  0000 C CNN
F 2 "" H 5050 5100 50  0001 C CNN
F 3 "" H 5050 5100 50  0001 C CNN
	1    5050 5100
	1    0    0    -1  
$EndComp
Text GLabel 3000 6850 2    60   Input ~ 0
GPIO0
Text GLabel 3000 6950 2    60   Input ~ 0
GPIO5
Text GLabel 3000 7050 2    60   Input ~ 0
GPIO4
Text GLabel 3000 6750 2    60   Input ~ 0
GPIO14
Text GLabel 3000 6650 2    60   Input ~ 0
GPIO12
Text GLabel 3000 6550 2    60   Input ~ 0
GPIO13
Text GLabel 3250 5000 0    60   Input ~ 0
GPIO13
Text GLabel 3250 4900 0    60   Input ~ 0
GPIO12
Text GLabel 3250 4800 0    60   Input ~ 0
GPIO14
Text GLabel 5050 4800 2    60   Input ~ 0
GPIO0
Text GLabel 5050 4600 2    60   Input ~ 0
GPIO5
Text GLabel 5050 4700 2    60   Input ~ 0
GPIO4
Connection ~ 2400 6050
Connection ~ 2550 6050
Connection ~ 2700 6050
Wire Wire Line
	1900 6850 3000 6850
Wire Wire Line
	1900 6950 3000 6950
Wire Wire Line
	1900 7050 3000 7050
Wire Wire Line
	2550 6350 2550 6850
Connection ~ 2550 6850
Wire Wire Line
	2700 6350 2700 6950
Connection ~ 2700 6950
Connection ~ 2850 7050
Wire Wire Line
	7850 3250 7850 3150
Wire Wire Line
	7850 3150 8100 3150
Wire Wire Line
	8400 3150 8650 3150
$Comp
L FT232RL U3
U 1 1 59610C09
P 2400 2350
F 0 "U3" H 1750 3250 50  0000 L CNN
F 1 "FT232RL" H 2800 3250 50  0000 L CNN
F 2 "Housings_SSOP:SSOP-28_5.3x10.2mm_Pitch0.65mm" H 2400 2350 50  0001 C CNN
F 3 "" H 2400 2350 50  0001 C CNN
	1    2400 2350
	1    0    0    -1  
$EndComp
Text GLabel 5050 4400 2    60   Input ~ 0
Tx
Text GLabel 5050 4500 2    60   Input ~ 0
Rx
$Comp
L USB_B J2
U 1 1 596132B0
P 850 1950
F 0 "J2" H 650 2400 50  0000 L CNN
F 1 "USB_B" H 650 2300 50  0000 L CNN
F 2 "Connectors:USB_Micro-B" H 1000 1900 50  0001 C CNN
F 3 "" H 1000 1900 50  0001 C CNN
	1    850  1950
	1    0    0    -1  
$EndComp
Text GLabel 2400 1300 1    60   Input ~ 0
3V3
Wire Wire Line
	1600 3050 1600 3350
Wire Wire Line
	1450 3350 2600 3350
Connection ~ 2200 3350
Connection ~ 2400 3350
Connection ~ 2500 3350
$Comp
L GND #PWR012
U 1 1 59615883
P 2200 3450
F 0 "#PWR012" H 2200 3200 50  0001 C CNN
F 1 "GND" H 2200 3300 50  0000 C CNN
F 2 "" H 2200 3450 50  0001 C CNN
F 3 "" H 2200 3450 50  0001 C CNN
	1    2200 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3350 2200 3450
Wire Wire Line
	1150 1950 1600 1950
Wire Wire Line
	1150 2050 1600 2050
$Comp
L GND #PWR013
U 1 1 59615F04
P 850 2350
F 0 "#PWR013" H 850 2100 50  0001 C CNN
F 1 "GND" H 850 2200 50  0000 C CNN
F 2 "" H 850 2350 50  0001 C CNN
F 3 "" H 850 2350 50  0001 C CNN
	1    850  2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1350 2300 1300
Wire Wire Line
	2300 1300 2500 1300
Wire Wire Line
	2500 1300 2500 1350
$Comp
L R R8
U 1 1 59616677
P 1300 1750
F 0 "R8" V 1380 1750 50  0000 C CNN
F 1 "4k7" V 1300 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 1230 1750 50  0001 C CNN
F 3 "" H 1300 1750 50  0001 C CNN
	1    1300 1750
	0    1    1    0   
$EndComp
$Comp
L R R9
U 1 1 596168C6
P 1450 2500
F 0 "R9" V 1530 2500 50  0000 C CNN
F 1 "10k" V 1450 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 1380 2500 50  0001 C CNN
F 3 "" H 1450 2500 50  0001 C CNN
	1    1450 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 1750 1450 2350
Wire Wire Line
	1450 2350 1600 2350
Wire Wire Line
	1450 2650 1450 3350
Connection ~ 1600 3350
$Comp
L R R10
U 1 1 59617D63
P 2800 4600
F 0 "R10" V 2880 4600 50  0000 C CNN
F 1 "1k" V 2800 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2730 4600 50  0001 C CNN
F 3 "" H 2800 4600 50  0001 C CNN
	1    2800 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 4600 3250 4600
Text GLabel 2650 4600 0    60   Input ~ 0
3V3
Text GLabel 3200 1750 2    60   Input ~ 0
Tx
Text GLabel 3200 1650 2    60   Input ~ 0
Rx
Text GLabel 3200 2050 2    60   Input ~ 0
DTR
Text GLabel 3200 1850 2    60   Input ~ 0
RTS
Text GLabel 4250 2400 0    60   Input ~ 0
DTR
Text GLabel 4250 1750 0    60   Input ~ 0
RTS
$Comp
L R R11
U 1 1 5961BEBC
P 4700 1750
F 0 "R11" V 4780 1750 50  0000 C CNN
F 1 "10k" V 4700 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4630 1750 50  0001 C CNN
F 3 "" H 4700 1750 50  0001 C CNN
	1    4700 1750
	0    1    1    0   
$EndComp
$Comp
L R R12
U 1 1 5961BFBF
P 4700 2400
F 0 "R12" V 4780 2400 50  0000 C CNN
F 1 "10k" V 4700 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4630 2400 50  0001 C CNN
F 3 "" H 4700 2400 50  0001 C CNN
	1    4700 2400
	0    1    1    0   
$EndComp
Connection ~ 1950 6050
Text GLabel 1900 6450 0    60   Input ~ 0
SD_D
Text GLabel 3000 6450 2    60   Input ~ 0
GPIO16
Wire Wire Line
	1900 6750 2000 6750
Wire Wire Line
	2850 6350 2850 7050
Wire Wire Line
	1900 6450 3000 6450
Wire Wire Line
	1950 6350 1950 6450
Connection ~ 1950 6450
Wire Wire Line
	4250 1750 4550 1750
Wire Wire Line
	4850 1750 4950 1750
Text GLabel 5250 1550 1    60   Input ~ 0
GPIO0
Wire Wire Line
	4250 2400 4550 2400
Wire Wire Line
	4850 2400 4950 2400
Wire Wire Line
	4400 1750 4400 2100
Wire Wire Line
	4400 2100 5250 2100
Wire Wire Line
	5250 2100 5250 2200
Connection ~ 4400 1750
Wire Wire Line
	5250 1950 4450 1950
Wire Wire Line
	4450 1950 4450 2400
Connection ~ 4450 2400
Text GLabel 5250 2600 3    60   Input ~ 0
RESET
Text GLabel 3250 4400 0    60   Input ~ 0
RESET
$Comp
L BC847 Q1
U 1 1 5962D32E
P 5150 1750
F 0 "Q1" H 5350 1825 50  0000 L CNN
F 1 "BC847" H 5350 1750 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5350 1675 50  0001 L CIN
F 3 "" H 5150 1750 50  0001 L CNN
	1    5150 1750
	1    0    0    -1  
$EndComp
$Comp
L BC847 Q2
U 1 1 5962D48A
P 5150 2400
F 0 "Q2" H 5350 2475 50  0000 L CNN
F 1 "BC847" H 5350 2400 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5350 2325 50  0001 L CIN
F 3 "" H 5150 2400 50  0001 L CNN
	1    5150 2400
	1    0    0    1   
$EndComp
$Comp
L SW_DIP_x01 SW1
U 1 1 5962EC4D
P 6300 2150
F 0 "SW1" H 6300 2300 50  0000 C CNN
F 1 "SW_DIP_x01" H 6300 2000 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_DIP_x1_W5.08mm_Slide_Copal_CHS-A" H 6300 2150 50  0001 C CNN
F 3 "" H 6300 2150 50  0001 C CNN
	1    6300 2150
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 5962EF38
P 6300 1600
F 0 "R13" V 6380 1600 50  0000 C CNN
F 1 "1k" V 6300 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6230 1600 50  0001 C CNN
F 3 "" H 6300 1600 50  0001 C CNN
	1    6300 1600
	1    0    0    -1  
$EndComp
Text GLabel 6300 1450 1    60   Input ~ 0
3V3
$Comp
L GND #PWR014
U 1 1 5962FD13
P 6300 2450
F 0 "#PWR014" H 6300 2200 50  0001 C CNN
F 1 "GND" H 6300 2300 50  0000 C CNN
F 2 "" H 6300 2450 50  0001 C CNN
F 3 "" H 6300 2450 50  0001 C CNN
	1    6300 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1750 6300 1850
Text GLabel 6300 1800 0    60   Input ~ 0
RESET
$EndSCHEMATC
