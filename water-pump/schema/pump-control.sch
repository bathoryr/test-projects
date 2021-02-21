EESchema Schematic File Version 4
EELAYER 30 0
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
L MCU_Microchip_ATmega:ATmega328P-PU U1
U 1 1 5FA1B57F
P 5500 3100
F 0 "U1" H 4856 3146 50  0000 R CNN
F 1 "ATmega328P-PU" H 4856 3055 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 5500 3100 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5500 3100 50  0001 C CNN
	1    5500 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FA1D89F
P 6350 3400
F 0 "R2" H 6420 3446 50  0000 L CNN
F 1 "10k" H 6420 3355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6280 3400 50  0001 C CNN
F 3 "~" H 6350 3400 50  0001 C CNN
	1    6350 3400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5FA1F4E2
P 6300 4250
F 0 "R1" H 6370 4296 50  0000 L CNN
F 1 "4k7" H 6370 4205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6230 4250 50  0001 C CNN
F 3 "~" H 6300 4250 50  0001 C CNN
	1    6300 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5FA207CC
P 5500 5300
F 0 "#PWR0101" H 5500 5050 50  0001 C CNN
F 1 "GND" H 5505 5127 50  0000 C CNN
F 2 "" H 5500 5300 50  0001 C CNN
F 3 "" H 5500 5300 50  0001 C CNN
	1    5500 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5FA211B0
P 5500 900
F 0 "#PWR0102" H 5500 750 50  0001 C CNN
F 1 "+5V" H 5515 1073 50  0000 C CNN
F 2 "" H 5500 900 50  0001 C CNN
F 3 "" H 5500 900 50  0001 C CNN
	1    5500 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5FA21CF5
P 5350 1300
F 0 "C1" V 5098 1300 50  0000 C CNN
F 1 "100n" V 5189 1300 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W4.4mm_P5.00mm" H 5388 1150 50  0001 C CNN
F 3 "~" H 5350 1300 50  0001 C CNN
	1    5350 1300
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5FA231FA
P 5350 1500
F 0 "C2" V 5300 1300 50  0000 C CNN
F 1 "100n" V 5450 1350 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W4.4mm_P5.00mm" H 5388 1350 50  0001 C CNN
F 3 "~" H 5350 1500 50  0001 C CNN
	1    5350 1500
	0    1    1    0   
$EndComp
$Comp
L RF:NRF24L01_Breakout U2
U 1 1 5FA23ED9
P 7600 1650
F 0 "U2" H 7980 1696 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 7980 1605 50  0000 L CNN
F 2 "RF_Module:nRF24L01_Breakout" H 7750 2250 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 7600 1550 50  0001 C CNN
	1    7600 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 5FA27A62
P 7450 3900
F 0 "J1" H 7530 3892 50  0000 L CNN
F 1 "pressure contact" H 7530 3801 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 7450 3900 50  0001 C CNN
F 3 "~" H 7450 3900 50  0001 C CNN
	1    7450 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 5FA2937E
P 7450 4150
F 0 "J2" H 7530 4142 50  0000 L CNN
F 1 "SSR control" H 7530 4051 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 7450 4150 50  0001 C CNN
F 3 "~" H 7450 4150 50  0001 C CNN
	1    7450 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J4
U 1 1 5FA2A391
P 7600 4500
F 0 "J4" H 7628 4476 50  0000 L CNN
F 1 "Temperature sensors" H 7628 4385 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7600 4500 50  0001 C CNN
F 3 "~" H 7600 4500 50  0001 C CNN
	1    7600 4500
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5FA2B3B3
P 6700 3800
F 0 "SW1" H 6700 4085 50  0000 C CNN
F 1 "Manual reset" H 6700 3994 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 6700 4000 50  0001 C CNN
F 3 "~" H 6700 4000 50  0001 C CNN
	1    6700 3800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J3
U 1 1 5FA2DCE2
P 7550 3200
F 0 "J3" H 7578 3176 50  0000 L CNN
F 1 "Display" H 7578 3085 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 7550 3200 50  0001 C CNN
F 3 "~" H 7550 3200 50  0001 C CNN
	1    7550 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 900  5500 1000
Connection ~ 5500 1300
Wire Wire Line
	5500 1300 5500 1500
Connection ~ 5500 1500
Wire Wire Line
	5500 1500 5500 1600
Wire Wire Line
	5600 1600 5600 1500
Wire Wire Line
	5600 1500 5500 1500
NoConn ~ 4900 1900
Wire Wire Line
	4450 5100 4450 1500
Wire Wire Line
	4450 1500 4950 1500
Wire Wire Line
	5200 1500 5200 1300
Connection ~ 5200 1500
Connection ~ 5500 5100
Wire Wire Line
	5500 5100 4450 5100
Wire Wire Line
	6100 3800 6250 3800
Wire Wire Line
	6100 3900 7250 3900
Wire Wire Line
	6100 4000 6800 4000
Wire Wire Line
	6800 4000 6800 4150
Wire Wire Line
	6800 4150 7250 4150
Wire Wire Line
	6100 4100 6300 4100
Wire Wire Line
	6700 4100 6700 4500
Wire Wire Line
	6700 4500 6900 4500
Connection ~ 6300 4100
Wire Wire Line
	6300 4100 6700 4100
Wire Wire Line
	6300 4400 6300 4550
Wire Wire Line
	6300 4550 6500 4550
Wire Wire Line
	6500 4550 6500 4500
$Comp
L power:+5V #PWR0104
U 1 1 5FA3F050
P 6500 4500
F 0 "#PWR0104" H 6500 4350 50  0001 C CNN
F 1 "+5V" H 6600 4600 50  0000 C CNN
F 2 "" H 6500 4500 50  0001 C CNN
F 3 "" H 6500 4500 50  0001 C CNN
	1    6500 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 5FA40FF3
P 6650 2850
F 0 "#PWR0105" H 6650 2700 50  0001 C CNN
F 1 "+5V" H 6750 2950 50  0000 C CNN
F 2 "" H 6650 2850 50  0001 C CNN
F 3 "" H 6650 2850 50  0001 C CNN
	1    6650 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3400 6200 3400
Wire Wire Line
	6100 3200 7350 3200
Wire Wire Line
	6100 3300 7350 3300
Wire Wire Line
	6500 3400 6650 3400
NoConn ~ 7100 1950
Text Label 6100 3300 0    50   ~ 0
SCL
Text Label 6100 3200 0    50   ~ 0
SDA
Text Label 6100 2000 0    50   ~ 0
CE
Text Label 6100 2100 0    50   ~ 0
CSN
Text Label 6100 2200 0    50   ~ 0
MOSI
Text Label 6100 2300 0    50   ~ 0
MISO
Text Label 6100 2400 0    50   ~ 0
SCK
$Comp
L power:GND #PWR0107
U 1 1 5FA4FCB2
P 7600 2400
F 0 "#PWR0107" H 7600 2150 50  0001 C CNN
F 1 "GND" H 7605 2227 50  0000 C CNN
F 2 "" H 7600 2400 50  0001 C CNN
F 3 "" H 7600 2400 50  0001 C CNN
	1    7600 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2000 6250 2000
Wire Wire Line
	6100 2100 6250 2100
Wire Wire Line
	6100 2200 6250 2200
Wire Wire Line
	6100 2300 6250 2300
Wire Wire Line
	6100 2400 6250 2400
Wire Wire Line
	7100 1350 6950 1350
Wire Wire Line
	7100 1450 6950 1450
Wire Wire Line
	7100 1550 6950 1550
Wire Wire Line
	7100 1650 6950 1650
Wire Wire Line
	7100 1850 6950 1850
NoConn ~ 6100 2500
NoConn ~ 6100 1900
NoConn ~ 6100 2600
NoConn ~ 6100 2800
NoConn ~ 6100 2900
NoConn ~ 6100 3000
NoConn ~ 6100 3100
NoConn ~ 6100 3600
NoConn ~ 6100 3700
NoConn ~ 6100 4300
Text Label 6950 1350 0    50   ~ 0
MOSI
Text Label 6950 1450 0    50   ~ 0
MISO
Text Label 6950 1550 0    50   ~ 0
SCK
Text Label 6950 1650 0    50   ~ 0
CSN
Text Label 6950 1850 0    50   ~ 0
CE
Wire Wire Line
	7600 2250 7600 2300
Wire Wire Line
	5500 5100 5500 5300
Wire Wire Line
	5500 4600 5500 5100
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5FA8045E
P 4000 1300
F 0 "#FLG0101" H 4000 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 1473 50  0000 C CNN
F 2 "" H 4000 1300 50  0001 C CNN
F 3 "~" H 4000 1300 50  0001 C CNN
	1    4000 1300
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5FA8139A
P 4000 750
F 0 "#FLG0102" H 4000 825 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 923 50  0000 C CNN
F 2 "" H 4000 750 50  0001 C CNN
F 3 "~" H 4000 750 50  0001 C CNN
	1    4000 750 
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 5FA90F97
P 6850 1150
F 0 "C3" H 6968 1196 50  0000 L CNN
F 1 "10u" H 6968 1105 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D7.5mm_P2.50mm" H 6888 1000 50  0001 C CNN
F 3 "~" H 6850 1150 50  0001 C CNN
	1    6850 1150
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U3
U 1 1 5FA932A8
P 6500 1000
F 0 "U3" H 6500 1242 50  0000 C CNN
F 1 "AMS1117-3.3" H 6500 1151 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6500 1200 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 6600 750 50  0001 C CNN
	1    6500 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 1000 5900 1000
Connection ~ 5500 1000
Wire Wire Line
	5500 1000 5500 1300
Wire Wire Line
	6800 1000 6850 1000
Wire Wire Line
	7600 1000 7600 1050
Connection ~ 6850 1000
Wire Wire Line
	6850 1000 7200 1000
Wire Wire Line
	6500 1300 6850 1300
Wire Wire Line
	6850 2300 7600 2300
Wire Wire Line
	6850 1300 6850 2300
Connection ~ 6850 1300
Connection ~ 7600 2300
Wire Wire Line
	7600 2300 7600 2400
Wire Wire Line
	7250 4000 7150 4000
Wire Wire Line
	7150 4000 7150 3800
Wire Wire Line
	7150 3400 7350 3400
Wire Wire Line
	7250 4250 7150 4250
Wire Wire Line
	7150 4250 7150 4000
Connection ~ 7150 4000
Wire Wire Line
	7150 4600 7150 4250
Wire Wire Line
	7150 4600 7400 4600
Connection ~ 7150 4250
Wire Wire Line
	7150 4600 7150 4800
Connection ~ 7150 4600
$Comp
L Connector:Conn_01x02_Female J6
U 1 1 5FA65E45
P 7550 3600
F 0 "J6" H 7578 3576 50  0000 L CNN
F 1 "Manual reset" H 7578 3485 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7550 3600 50  0001 C CNN
F 3 "~" H 7550 3600 50  0001 C CNN
	1    7550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3700 7150 3700
Connection ~ 7150 3700
Wire Wire Line
	7150 3700 7150 3400
Wire Wire Line
	7350 3600 6250 3600
Wire Wire Line
	6250 3600 6250 3800
Connection ~ 6250 3800
Wire Wire Line
	6250 3800 6500 3800
Wire Wire Line
	5500 5100 6200 5100
Wire Wire Line
	6900 3800 7150 3800
Connection ~ 7150 3800
Wire Wire Line
	7150 3800 7150 3700
$Comp
L Converter_ACDC:HLK-PM01 PS1
U 1 1 5FA6B6EB
P 4450 1000
F 0 "PS1" H 4450 1325 50  0000 C CNN
F 1 "HLK-PM01" H 4450 1234 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_HiLink_HLK-PMxx" H 4450 700 50  0001 C CNN
F 3 "http://www.hlktech.net/product_detail.php?ProId=54" H 4850 650 50  0001 C CNN
	1    4450 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 900  5150 900 
Wire Wire Line
	5150 900  5150 1000
Wire Wire Line
	5150 1000 5500 1000
Wire Wire Line
	4850 1100 4950 1100
Wire Wire Line
	4950 1100 4950 1500
Connection ~ 4950 1500
Wire Wire Line
	4950 1500 5200 1500
$Comp
L Connector:Screw_Terminal_01x03 J5
U 1 1 5FA71ED4
P 2650 1050
F 0 "J5" H 2730 1042 50  0000 L CNN
F 1 "230V AC" H 2730 951 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-3-5.08_1x03_P5.08mm_Horizontal" H 2650 1050 50  0001 C CNN
F 3 "~" H 2650 1050 50  0001 C CNN
	1    2650 1050
	-1   0    0    -1  
$EndComp
$Comp
L Device:Fuse_Small F1
U 1 1 5FA73702
P 3050 850
F 0 "F1" H 3050 1035 50  0000 C CNN
F 1 ".5A" H 3050 944 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" H 3050 943 50  0001 C CNN
F 3 "~" H 3050 850 50  0001 C CNN
	1    3050 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 2850 6650 3400
$Comp
L power:+3.3V #PWR0103
U 1 1 5FA8FAC0
P 7200 850
F 0 "#PWR0103" H 7200 700 50  0001 C CNN
F 1 "+3.3V" H 7215 1023 50  0000 C CNN
F 2 "" H 7200 850 50  0001 C CNN
F 3 "" H 7200 850 50  0001 C CNN
	1    7200 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0106
U 1 1 5FA907EB
P 7150 2850
F 0 "#PWR0106" H 7150 2700 50  0001 C CNN
F 1 "+3.3V" H 7165 3023 50  0000 C CNN
F 2 "" H 7150 2850 50  0001 C CNN
F 3 "" H 7150 2850 50  0001 C CNN
	1    7150 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 850  7200 1000
Connection ~ 7200 1000
Wire Wire Line
	7200 1000 7600 1000
Wire Wire Line
	7150 2850 7150 3100
Wire Wire Line
	7150 3100 7350 3100
$Comp
L Connector:Conn_01x02_Female J7
U 1 1 5FAA8CD9
P 7600 4700
F 0 "J7" H 7628 4676 50  0000 L CNN
F 1 "Temperature sensors" H 7628 4585 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7600 4700 50  0001 C CNN
F 3 "~" H 7600 4700 50  0001 C CNN
	1    7600 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4800 7150 4800
Connection ~ 7150 4800
Wire Wire Line
	7150 4800 7150 5100
Wire Wire Line
	7400 4700 6900 4700
Wire Wire Line
	6900 4700 6900 4500
Connection ~ 6900 4500
Wire Wire Line
	6900 4500 7400 4500
$Comp
L Device:CP C4
U 1 1 5FB0BA90
P 5900 1150
F 0 "C4" H 6018 1196 50  0000 L CNN
F 1 "100u" H 6018 1105 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D7.5mm_P2.50mm" H 5938 1000 50  0001 C CNN
F 3 "~" H 5900 1150 50  0001 C CNN
	1    5900 1150
	1    0    0    -1  
$EndComp
Connection ~ 5900 1000
Wire Wire Line
	5900 1000 5500 1000
Wire Wire Line
	5900 1300 6500 1300
Connection ~ 6500 1300
$Comp
L Device:C_Small C5
U 1 1 5FB1C8B8
P 3550 1000
F 0 "C5" H 3500 1250 50  0000 L CNN
F 1 ".1u" H 3550 800 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L18.0mm_W6.0mm_P15.00mm_FKS3_FKP3" H 3550 1000 50  0001 C CNN
F 3 "~" H 3550 1000 50  0001 C CNN
	1    3550 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:Varistor RV1
U 1 1 5FB251E3
P 3300 1000
F 0 "RV1" H 3250 1250 50  0000 L CNN
F 1 "300/450V" H 3100 800 50  0000 L CNN
F 2 "Varistor:RV_Disc_D12mm_W3.9mm_P7.5mm" V 3230 1000 50  0001 C CNN
F 3 "~" H 3300 1000 50  0001 C CNN
	1    3300 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 950  4000 950 
Wire Wire Line
	4050 950  4050 900 
Wire Wire Line
	3950 1050 4000 1050
Wire Wire Line
	4050 1050 4050 1100
Wire Wire Line
	3550 900  3750 900 
Wire Wire Line
	3750 900  3750 950 
Wire Wire Line
	3550 1100 3750 1100
Wire Wire Line
	3750 1100 3750 1050
Wire Wire Line
	3300 1150 3550 1150
Wire Wire Line
	3550 1150 3550 1100
Connection ~ 3550 1100
Wire Wire Line
	3300 850  3550 850 
Wire Wire Line
	3550 850  3550 900 
Connection ~ 3550 900 
Wire Wire Line
	3150 850  3300 850 
Connection ~ 3300 850 
Wire Wire Line
	2850 850  2850 950 
Connection ~ 3300 1150
Wire Wire Line
	2850 1150 2850 1050
Connection ~ 2850 1150
Text Label 2800 950  0    50   ~ 0
L
Text Label 2800 1050 0    50   ~ 0
N
Text Label 2800 1150 0    50   ~ 0
N
Wire Wire Line
	2850 1150 3300 1150
Wire Wire Line
	4000 750  4000 950 
Connection ~ 4000 950 
Wire Wire Line
	4000 950  4050 950 
Wire Wire Line
	4000 1300 4000 1050
Connection ~ 4000 1050
Wire Wire Line
	4000 1050 4050 1050
Wire Wire Line
	2850 850  2950 850 
$Comp
L Device:R R3
U 1 1 5FB3C1A1
P 6200 4600
F 0 "R3" H 6270 4646 50  0000 L CNN
F 1 "330" H 6270 4555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6130 4600 50  0001 C CNN
F 3 "~" H 6200 4600 50  0001 C CNN
	1    6200 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4200 6200 4200
Wire Wire Line
	6200 4200 6200 4450
$Comp
L Device:LED_Small D1
U 1 1 5FB40DAC
P 6200 4900
F 0 "D1" V 6246 4830 50  0000 R CNN
F 1 "LED_btn" V 6155 4830 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" V 6200 4900 50  0001 C CNN
F 3 "~" V 6200 4900 50  0001 C CNN
	1    6200 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6200 4750 6200 4800
Wire Wire Line
	6200 5000 6200 5100
Connection ~ 6200 5100
Wire Wire Line
	6200 5100 7150 5100
$Comp
L Mechanical:MountingHole H1
U 1 1 5FB501F7
P 3350 1950
F 0 "H1" H 3450 1996 50  0000 L CNN
F 1 "MountingHole" H 3450 1905 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3350 1950 50  0001 C CNN
F 3 "~" H 3350 1950 50  0001 C CNN
	1    3350 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5FB50764
P 3350 2250
F 0 "H2" H 3450 2296 50  0000 L CNN
F 1 "MountingHole" H 3450 2205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3350 2250 50  0001 C CNN
F 3 "~" H 3350 2250 50  0001 C CNN
	1    3350 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Core_Ferrite_Coupled_Small_1243 L1
U 1 1 5FE66F9A
P 3850 1000
F 0 "L1" H 3850 1231 50  0000 C CNN
F 1 "10 mH" H 3850 1140 50  0000 C CNN
F 2 "Inductor_THT:L_CommonMode_Wuerth_WE-CMB-XS" H 3850 1000 50  0001 C CNN
F 3 "~" H 3850 1000 50  0001 C CNN
	1    3850 1000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
