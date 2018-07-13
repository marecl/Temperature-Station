EESchema Schematic File Version 4
LIBS:Slave_PCB-cache
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
L ESP8266:ESP-01v090 U1
U 1 1 5B0DB50F
P 4500 3550
F 0 "U1" H 4500 4065 50  0000 C CNN
F 1 "ESP-01v090" H 4500 3974 50  0000 C CNN
F 2 "ESP8266:ESP-01" H 4500 3550 50  0001 C CNN
F 3 "http://l0l.org.uk/2014/12/esp8266-modules-hardware-guide-gotta-catch-em-all/" H 4500 3550 50  0001 C CNN
	1    4500 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5B1FD54A
P 6100 3400
F 0 "#PWR0101" H 6100 3150 50  0001 C CNN
F 1 "GND" V 6105 3272 50  0000 R CNN
F 2 "" H 6100 3400 50  0001 C CNN
F 3 "" H 6100 3400 50  0001 C CNN
	1    6100 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 3400 6100 3400
$Comp
L Device:R R3
U 1 1 5B1FD59B
P 5950 3900
F 0 "R3" H 5750 3950 50  0000 L CNN
F 1 "4k7" H 5700 3850 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5880 3900 50  0001 C CNN
F 3 "~" H 5950 3900 50  0001 C CNN
	1    5950 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5B1FD5F4
P 6050 3900
F 0 "R4" H 6150 3950 50  0000 L CNN
F 1 "4k7" H 6120 3855 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5980 3900 50  0001 C CNN
F 3 "~" H 6050 3900 50  0001 C CNN
	1    6050 3900
	1    0    0    -1  
$EndComp
Text GLabel 6100 3500 2    50   Input ~ 0
SDA
Text GLabel 6100 3600 2    50   Input ~ 0
SCL
Text GLabel 5850 4050 0    50   Input ~ 0
3V3
Wire Wire Line
	5850 4050 5950 4050
Connection ~ 5950 4050
Wire Wire Line
	5950 4050 6050 4050
Text GLabel 3250 3700 0    50   Input ~ 0
3V3
$Comp
L Device:R R2
U 1 1 5B1FDC54
P 3400 3600
F 0 "R2" V 3400 3900 50  0000 C CNN
F 1 "10k" V 3400 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 3600 50  0001 C CNN
F 3 "~" H 3400 3600 50  0001 C CNN
	1    3400 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3250 3600 3250 3700
Wire Wire Line
	3250 3700 3450 3700
$Comp
L Device:R R1
U 1 1 5B1FE147
P 3400 3500
F 0 "R1" V 3400 3800 50  0000 C CNN
F 1 "10k" V 3400 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" V 3330 3500 50  0001 C CNN
F 3 "~" H 3400 3500 50  0001 C CNN
	1    3400 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3250 3500 3250 3600
Connection ~ 3250 3600
Wire Wire Line
	5450 3600 6050 3600
Wire Wire Line
	5450 3500 5950 3500
Wire Wire Line
	5950 3500 5950 3750
Connection ~ 5950 3500
Wire Wire Line
	5950 3500 6100 3500
Wire Wire Line
	6050 3600 6050 3750
Connection ~ 6050 3600
Wire Wire Line
	6050 3600 6100 3600
Text GLabel 3550 3400 0    50   Input ~ 0
hRX
Text GLabel 5450 3700 2    50   Input ~ 0
hTX
$Comp
L power:GND #PWR0102
U 1 1 5B1FF264
P 4250 5100
F 0 "#PWR0102" H 4250 4850 50  0001 C CNN
F 1 "GND" H 4255 4927 50  0000 C CNN
F 2 "" H 4250 5100 50  0001 C CNN
F 3 "" H 4250 5100 50  0001 C CNN
	1    4250 5100
	1    0    0    -1  
$EndComp
Text GLabel 4250 4600 0    50   Input ~ 0
SDA
Text GLabel 4250 4700 0    50   Input ~ 0
SCL
Text GLabel 4250 4900 0    50   Input ~ 0
hRX
Text GLabel 4250 4800 0    50   Input ~ 0
hTX
$Comp
L Connector_Generic:Conn_01x06 J1
U 1 1 5B1FF737
P 4450 4800
F 0 "J1" H 4529 4792 50  0000 L CNN
F 1 "COM" H 4529 4701 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4450 4800 50  0001 C CNN
F 3 "~" H 4450 4800 50  0001 C CNN
	1    4450 4800
	1    0    0    -1  
$EndComp
Text GLabel 4250 5000 0    50   Input ~ 0
3V3
$Comp
L Device:C C1
U 1 1 5B20244E
P 3450 3850
F 0 "C1" H 3565 3896 50  0000 L CNN
F 1 "100nF" H 3565 3805 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W2.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 3488 3700 50  0001 C CNN
F 3 "~" H 3450 3850 50  0001 C CNN
	1    3450 3850
	1    0    0    -1  
$EndComp
Connection ~ 3450 3700
Wire Wire Line
	3450 3700 3550 3700
$Comp
L power:GND #PWR0103
U 1 1 5B20257C
P 3450 4000
F 0 "#PWR0103" H 3450 3750 50  0001 C CNN
F 1 "GND" H 3455 3827 50  0000 C CNN
F 2 "" H 3450 4000 50  0001 C CNN
F 3 "" H 3450 4000 50  0001 C CNN
	1    3450 4000
	1    0    0    -1  
$EndComp
$EndSCHEMATC