EESchema Schematic File Version 4
LIBS:Mainboard_2.0-cache
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
Text GLabel 4600 4250 0    60   Input ~ 0
MOSI
Text GLabel 4600 4350 0    60   Input ~ 0
MISO
Text GLabel 4600 4450 0    60   Input ~ 0
SCK
Text GLabel 6550 2250 2    60   Input ~ 0
CS
Text GLabel 4600 4550 0    60   Input ~ 0
1wire
Text GLabel 4600 4650 0    60   Input ~ 0
SDA
Text GLabel 4600 4750 0    60   Input ~ 0
SCL
$Comp
L Mainboard_2.0-rescue:R R5
U 1 1 591DCE97
P 5250 3900
F 0 "R5" V 5330 3900 50  0000 C CNN
F 1 "4k7" V 5250 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5180 3900 50  0001 C CNN
F 3 "" H 5250 3900 50  0001 C CNN
	1    5250 3900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:R R6
U 1 1 591DCEFA
P 5400 3900
F 0 "R6" V 5480 3900 50  0000 C CNN
F 1 "4k7" V 5400 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5330 3900 50  0001 C CNN
F 3 "" H 5400 3900 50  0001 C CNN
	1    5400 3900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:R R7
U 1 1 591DCF1C
P 5550 3900
F 0 "R7" V 5630 3900 50  0000 C CNN
F 1 "4k7" V 5550 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5480 3900 50  0001 C CNN
F 3 "" H 5550 3900 50  0001 C CNN
	1    5550 3900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:CONN_01X03 J5
U 1 1 591DD18C
P 2950 6300
F 0 "J5" H 2950 6500 50  0000 C CNN
F 1 "CONN_01X03" V 3050 6300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm_SMD_Pin1Left" H 2950 6300 50  0001 C CNN
F 3 "" H 2950 6300 50  0001 C CNN
	1    2950 6300
	0    -1   -1   0   
$EndComp
$Comp
L Mainboard_2.0-rescue:CONN_01X03 J1
U 1 1 591DD1E5
P 2950 5800
F 0 "J1" H 2950 6000 50  0000 C CNN
F 1 "CONN_01X03" V 3050 5800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm_SMD_Pin1Left" H 2950 5800 50  0001 C CNN
F 3 "" H 2950 5800 50  0001 C CNN
	1    2950 5800
	0    -1   -1   0   
$EndComp
Text GLabel 2600 6500 0    60   Input ~ 0
3V3
Text GLabel 2600 6600 0    60   Input ~ 0
1wire
$Comp
L Mainboard_2.0-rescue:GND #PWR01
U 1 1 591DD301
P 2750 6700
F 0 "#PWR01" H 2750 6450 50  0001 C CNN
F 1 "GND" H 2750 6550 50  0000 C CNN
F 2 "" H 2750 6700 50  0001 C CNN
F 3 "" H 2750 6700 50  0001 C CNN
	1    2750 6700
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:SD_Card J4
U 1 1 591DD53A
P 8650 1900
F 0 "J4" H 8000 2450 50  0000 C CNN
F 1 "SD_Card" H 9250 1350 50  0000 C CNN
F 2 "dodatki:SDCARD-STANDARD" H 8850 2250 50  0001 C CNN
F 3 "" H 8650 1900 50  0001 C CNN
	1    8650 1900
	1    0    0    -1  
$EndComp
Text GLabel 7750 1600 0    60   Input ~ 0
CS
Text GLabel 7750 1700 0    60   Input ~ 0
MOSI
Text GLabel 7750 2000 0    60   Input ~ 0
SCK
$Comp
L Mainboard_2.0-rescue:GND #PWR02
U 1 1 591DDB59
P 7350 2550
F 0 "#PWR02" H 7350 2300 50  0001 C CNN
F 1 "GND" H 7350 2400 50  0000 C CNN
F 2 "" H 7350 2550 50  0001 C CNN
F 3 "" H 7350 2550 50  0001 C CNN
	1    7350 2550
	1    0    0    -1  
$EndComp
Text GLabel 7750 2200 0    60   Input ~ 0
MISO
$Comp
L Mainboard_2.0-rescue:R R4
U 1 1 591DDBFF
P 4650 3900
F 0 "R4" V 4730 3900 50  0000 C CNN
F 1 "10k" V 4650 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4580 3900 50  0001 C CNN
F 3 "" H 4650 3900 50  0001 C CNN
	1    4650 3900
	1    0    0    -1  
$EndComp
Text GLabel 9850 1700 2    60   Input ~ 0
SD_D
$Comp
L Mainboard_2.0-rescue:R R1
U 1 1 591DE219
P 4800 3900
F 0 "R1" V 4880 3900 50  0000 C CNN
F 1 "10k" V 4800 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4730 3900 50  0001 C CNN
F 3 "" H 4800 3900 50  0001 C CNN
	1    4800 3900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:R R2
U 1 1 591DE506
P 4950 3900
F 0 "R2" V 5030 3900 50  0000 C CNN
F 1 "10k" V 4950 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4880 3900 50  0001 C CNN
F 3 "" H 4950 3900 50  0001 C CNN
	1    4950 3900
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:R R3
U 1 1 591DE541
P 5100 3900
F 0 "R3" V 5180 3900 50  0000 C CNN
F 1 "10k" V 5100 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5030 3900 50  0001 C CNN
F 3 "" H 5100 3900 50  0001 C CNN
	1    5100 3900
	1    0    0    -1  
$EndComp
Text GLabel 4500 3750 0    60   Input ~ 0
3V3
$Comp
L Mainboard_2.0-rescue:PCF8563 U2
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
L Mainboard_2.0-rescue:Crystal Y1
U 1 1 591DEFF4
P 7950 5750
F 0 "Y1" H 7950 5900 50  0000 C CNN
F 1 "Crystal" H 7950 5600 50  0000 C CNN
F 2 "Crystals:Crystal_SMD_SeikoEpson_MC406-4pin_9.6x4.1mm" H 7950 5750 50  0001 C CNN
F 3 "" H 7950 5750 50  0001 C CNN
	1    7950 5750
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:D D3
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
L Mainboard_2.0-rescue:D D1
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
L Mainboard_2.0-rescue:GND #PWR03
U 1 1 591DF8F9
P 8450 6150
F 0 "#PWR03" H 8450 5900 50  0001 C CNN
F 1 "GND" H 8450 6000 50  0000 C CNN
F 2 "" H 8450 6150 50  0001 C CNN
F 3 "" H 8450 6150 50  0001 C CNN
	1    8450 6150
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:LM1117-3.3-RESCUE-Mainboard_2.0 U1
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
L Mainboard_2.0-rescue:D D2
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
L Mainboard_2.0-rescue:C C1
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
L Mainboard_2.0-rescue:CP C2
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
L Mainboard_2.0-rescue:C C3
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
L Mainboard_2.0-rescue:CP C4
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
L Mainboard_2.0-rescue:CONN_01X02 J6
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
L Mainboard_2.0-rescue:GND #PWR04
U 1 1 591E7037
P 9550 2200
F 0 "#PWR04" H 9550 1950 50  0001 C CNN
F 1 "GND" H 9550 2050 50  0000 C CNN
F 2 "" H 9550 2200 50  0001 C CNN
F 3 "" H 9550 2200 50  0001 C CNN
	1    9550 2200
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:CONN_01X02 J7
U 1 1 591F80FA
P 7650 3300
F 0 "J7" H 7650 3450 50  0000 C CNN
F 1 "CONN_01X02" V 7750 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm_SMD_Pin1Left" H 7650 3300 50  0001 C CNN
F 3 "" H 7650 3300 50  0001 C CNN
	1    7650 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 6000 2850 6500
Wire Wire Line
	2950 6000 2950 6500
Wire Wire Line
	3050 6000 3050 6500
Wire Wire Line
	2850 6500 2600 6500
Wire Wire Line
	2950 6600 2600 6600
Connection ~ 2950 6500
Connection ~ 3050 6500
Wire Wire Line
	4600 4350 4950 4350
Wire Wire Line
	4600 4250 4800 4250
Wire Wire Line
	4800 4050 4800 4250
Connection ~ 4800 4250
Wire Wire Line
	4950 4050 4950 4350
Connection ~ 4950 4350
Wire Wire Line
	5100 4050 5100 4450
Connection ~ 5100 4450
Wire Wire Line
	4500 3750 4650 3750
Connection ~ 4800 3750
Connection ~ 4950 3750
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
	8450 4850 8450 5000
Connection ~ 8450 5000
Wire Wire Line
	7850 3350 7850 3450
Wire Wire Line
	8400 3450 8650 3450
Connection ~ 8650 3450
Wire Wire Line
	8650 3650 8400 3650
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
	9550 2000 9550 2100
Connection ~ 9550 2100
Wire Wire Line
	9250 3800 9550 3800
Connection ~ 9550 3800
Wire Wire Line
	4600 4450 5100 4450
Connection ~ 9750 4100
Text GLabel 7750 1900 0    60   Input ~ 0
3V3
Text GLabel 9900 3800 2    60   Input ~ 0
3V3
$Comp
L Mainboard_2.0-rescue:GND #PWR05
U 1 1 594541DB
P 10000 4100
F 0 "#PWR05" H 10000 3850 50  0001 C CNN
F 1 "GND" H 10000 3950 50  0000 C CNN
F 2 "" H 10000 4100 50  0001 C CNN
F 3 "" H 10000 4100 50  0001 C CNN
	1    10000 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 4100 8950 4100
Connection ~ 8950 4100
$Comp
L Mainboard_2.0-rescue:ESP-12E U4
U 1 1 59610456
P 5650 2050
F 0 "U4" H 5650 1950 50  0000 C CNN
F 1 "ESP-12E" H 5650 2150 50  0000 C CNN
F 2 "ESP8266:ESP-12E_SMD" H 5650 2050 50  0001 C CNN
F 3 "" H 5650 2050 50  0001 C CNN
	1    5650 2050
	1    0    0    -1  
$EndComp
Text GLabel 4750 2450 0    60   Input ~ 0
3V3
$Comp
L Mainboard_2.0-rescue:GND #PWR06
U 1 1 59611A33
P 6550 2450
F 0 "#PWR06" H 6550 2200 50  0001 C CNN
F 1 "GND" H 6550 2300 50  0000 C CNN
F 2 "" H 6550 2450 50  0001 C CNN
F 3 "" H 6550 2450 50  0001 C CNN
	1    6550 2450
	1    0    0    -1  
$EndComp
Text GLabel 5700 4550 2    60   Input ~ 0
GPIO0
Text GLabel 5700 4650 2    60   Input ~ 0
GPIO5
Text GLabel 5700 4750 2    60   Input ~ 0
GPIO4
Text GLabel 5700 4450 2    60   Input ~ 0
GPIO14
Text GLabel 5700 4350 2    60   Input ~ 0
GPIO12
Text GLabel 5700 4250 2    60   Input ~ 0
GPIO13
Text GLabel 4750 2350 0    60   Input ~ 0
GPIO13
Text GLabel 4750 2250 0    60   Input ~ 0
GPIO12
Text GLabel 4750 2150 0    60   Input ~ 0
GPIO14
Text GLabel 6550 1950 2    60   Input ~ 0
GPIO5
Text GLabel 6550 2050 2    60   Input ~ 0
GPIO4
Connection ~ 5100 3750
Connection ~ 5250 3750
Connection ~ 5400 3750
Wire Wire Line
	4600 4550 5250 4550
Wire Wire Line
	4600 4650 5400 4650
Wire Wire Line
	4600 4750 5550 4750
Wire Wire Line
	5250 4050 5250 4550
Connection ~ 5250 4550
Wire Wire Line
	5400 4050 5400 4650
Connection ~ 5400 4650
Connection ~ 5550 4750
Wire Wire Line
	7850 3250 7850 3150
Wire Wire Line
	7850 3150 8100 3150
Wire Wire Line
	8400 3150 8650 3150
Text GLabel 6550 1750 2    60   Input ~ 0
Tx
Text GLabel 6550 1850 2    60   Input ~ 0
Rx
$Comp
L Mainboard_2.0-rescue:R R10
U 1 1 59617D63
P 4300 1950
F 0 "R10" V 4380 1950 50  0000 C CNN
F 1 "1k" V 4300 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4230 1950 50  0001 C CNN
F 3 "" H 4300 1950 50  0001 C CNN
	1    4300 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 1950 4750 1950
Text GLabel 4150 1950 0    60   Input ~ 0
3V3
Text GLabel 3200 1750 2    60   Input ~ 0
Tx
Text GLabel 3200 1650 2    60   Input ~ 0
Rx
Connection ~ 4650 3750
Text GLabel 4600 4150 0    60   Input ~ 0
SD_D
Wire Wire Line
	5550 4050 5550 4750
Wire Wire Line
	4600 4150 4650 4150
Wire Wire Line
	4650 4050 4650 4150
Connection ~ 4650 4150
Text GLabel 4750 1750 0    60   Input ~ 0
RESET
$Comp
L Mainboard_2.0-rescue:SW_DIP_x01-RESCUE-Mainboard_2.0 SW1
U 1 1 5962EC4D
P 3550 4600
F 0 "SW1" H 3550 4750 50  0000 C CNN
F 1 "RESET" H 3550 4450 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_DIP_x1_W5.08mm_Slide_Copal_CHS-A" H 3550 4600 50  0001 C CNN
F 3 "" H 3550 4600 50  0001 C CNN
	1    3550 4600
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:R R11
U 1 1 5962EF38
P 3550 4050
F 0 "R11" V 3630 4050 50  0000 C CNN
F 1 "1k" V 3550 4050 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 3480 4050 50  0001 C CNN
F 3 "" H 3550 4050 50  0001 C CNN
	1    3550 4050
	1    0    0    -1  
$EndComp
Text GLabel 3550 3900 1    60   Input ~ 0
3V3
$Comp
L Mainboard_2.0-rescue:GND #PWR07
U 1 1 5962FD13
P 3550 4900
F 0 "#PWR07" H 3550 4650 50  0001 C CNN
F 1 "GND" H 3550 4750 50  0000 C CNN
F 2 "" H 3550 4900 50  0001 C CNN
F 3 "" H 3550 4900 50  0001 C CNN
	1    3550 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 4200 3550 4300
Text GLabel 3550 4250 0    60   Input ~ 0
RESET
Text GLabel 6550 2150 2    60   Input ~ 0
GPIO0
Wire Wire Line
	8650 3150 8650 3450
$Comp
L Mainboard_2.0-rescue:SW_DIP_x01-RESCUE-Mainboard_2.0 SW2
U 1 1 596B81BC
P 2800 4600
F 0 "SW2" H 2800 4750 50  0000 C CNN
F 1 "FLASH" H 2800 4450 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_DIP_x1_W5.08mm_Slide_Copal_CHS-A" H 2800 4600 50  0001 C CNN
F 3 "" H 2800 4600 50  0001 C CNN
	1    2800 4600
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:R R12
U 1 1 596B81C2
P 2800 4050
F 0 "R12" V 2880 4050 50  0000 C CNN
F 1 "1k" V 2800 4050 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 2730 4050 50  0001 C CNN
F 3 "" H 2800 4050 50  0001 C CNN
	1    2800 4050
	1    0    0    -1  
$EndComp
Text GLabel 2800 3900 1    60   Input ~ 0
3V3
$Comp
L Mainboard_2.0-rescue:GND #PWR08
U 1 1 596B81C9
P 2800 4900
F 0 "#PWR08" H 2800 4650 50  0001 C CNN
F 1 "GND" H 2800 4750 50  0000 C CNN
F 2 "" H 2800 4900 50  0001 C CNN
F 3 "" H 2800 4900 50  0001 C CNN
	1    2800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4200 2800 4300
Text GLabel 2800 4250 0    60   Input ~ 0
GPIO0
Text Notes 6100 5800 0    60   ~ 12
91SMX(IQD)/MC406(seiko epson)\ncrystals can be used
$Comp
L Mainboard_2.0-rescue:R RJ1
U 1 1 597795BC
P 9700 1700
F 0 "RJ1" V 9780 1700 50  0000 C CNN
F 1 "0" V 9700 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 9630 1700 50  0001 C CNN
F 3 "" H 9700 1700 50  0001 C CNN
	1    9700 1700
	0    1    1    0   
$EndComp
Text GLabel 5700 4150 2    60   Input ~ 0
SD_D
Text GLabel 4750 2050 0    60   Input ~ 0
SD_D
$Comp
L Mainboard_2.0-rescue:R RJ2
U 1 1 5977D282
P 2900 6700
F 0 "RJ2" V 2980 6700 50  0000 C CNN
F 1 "0" V 2900 6700 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 2830 6700 50  0001 C CNN
F 3 "" H 2900 6700 50  0001 C CNN
	1    2900 6700
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:GND #PWR09
U 1 1 5977F4E8
P 3050 6500
F 0 "#PWR09" H 3050 6250 50  0001 C CNN
F 1 "GND" H 3050 6350 50  0000 C CNN
F 2 "" H 3050 6500 50  0001 C CNN
F 3 "" H 3050 6500 50  0001 C CNN
	1    3050 6500
	0    -1   -1   0   
$EndComp
$Comp
L Mainboard_2.0-rescue:R RJ3
U 1 1 5977B25F
P 7350 2400
F 0 "RJ3" V 7430 2400 50  0000 C CNN
F 1 "0" V 7350 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 7280 2400 50  0001 C CNN
F 3 "" H 7350 2400 50  0001 C CNN
	1    7350 2400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 1800 7350 2100
Wire Wire Line
	7350 2100 7750 2100
Wire Wire Line
	7350 1800 7750 1800
Connection ~ 7350 2100
$Comp
L Mainboard_2.0-rescue:GND #PWR010
U 1 1 5977B655
P 7350 2100
F 0 "#PWR010" H 7350 1850 50  0001 C CNN
F 1 "GND" H 7350 1950 50  0000 C CNN
F 2 "" H 7350 2100 50  0001 C CNN
F 3 "" H 7350 2100 50  0001 C CNN
	1    7350 2100
	0    1    1    0   
$EndComp
$Comp
L Mainboard_2.0-rescue:GND #PWR011
U 1 1 59835083
P 3200 1850
F 0 "#PWR011" H 3200 1600 50  0001 C CNN
F 1 "GND" H 3200 1700 50  0000 C CNN
F 2 "" H 3200 1850 50  0001 C CNN
F 3 "" H 3200 1850 50  0001 C CNN
	1    3200 1850
	1    0    0    -1  
$EndComp
$Comp
L Mainboard_2.0-rescue:CONN_01X03 J2
U 1 1 598350CE
P 3000 1750
F 0 "J2" H 3000 1950 50  0000 C CNN
F 1 "CONN_01X03" V 3100 1750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm_SMD_Pin1Left" H 3000 1750 50  0001 C CNN
F 3 "" H 3000 1750 50  0001 C CNN
	1    3000 1750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2950 6500 2950 6600
Wire Wire Line
	3050 6500 3050 6700
Wire Wire Line
	4800 4250 5700 4250
Wire Wire Line
	4950 4350 5700 4350
Wire Wire Line
	5100 4450 5700 4450
Wire Wire Line
	4800 3750 4950 3750
Wire Wire Line
	4950 3750 5100 3750
Wire Wire Line
	8450 5000 8450 5350
Wire Wire Line
	8650 3450 8650 3650
Wire Wire Line
	8650 3650 8650 3800
Wire Wire Line
	7850 3650 7850 4100
Wire Wire Line
	7850 3450 7850 3650
Wire Wire Line
	9750 3800 9900 3800
Wire Wire Line
	9550 4100 9750 4100
Wire Wire Line
	9550 2100 9550 2200
Wire Wire Line
	9550 3800 9750 3800
Wire Wire Line
	9750 4100 10000 4100
Wire Wire Line
	8950 4100 9550 4100
Wire Wire Line
	5100 3750 5250 3750
Wire Wire Line
	5250 3750 5400 3750
Wire Wire Line
	5400 3750 5550 3750
Wire Wire Line
	5250 4550 5700 4550
Wire Wire Line
	5400 4650 5700 4650
Wire Wire Line
	5550 4750 5700 4750
Wire Wire Line
	4650 3750 4800 3750
Wire Wire Line
	4650 4150 5700 4150
Wire Wire Line
	7350 2100 7350 2250
$EndSCHEMATC
