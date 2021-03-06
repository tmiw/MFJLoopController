EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "MFJ Wi-Fi Loop Controller"
Date ""
Rev "1.2.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Converter_DCDC:TMR-1212 U3
U 1 1 5F8E2AC8
P 4700 1900
F 0 "U3" H 4700 2367 50  0000 C CNN
F 1 "TVN3-1212" H 4700 2276 50  0000 C CNN
F 2 "Converter_DCDC:Converter_DCDC_TRACO_TMR-xxxx_THT" H 4700 1550 50  0001 C CNN
F 3 "https://www.tracopower.com/products/tmr2.pdf" H 4700 1400 50  0001 C CNN
	1    4700 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5F8E6F26
P 3000 1900
F 0 "C4" H 3115 1946 50  0000 L CNN
F 1 "22uF 25V" H 3115 1855 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 3038 1750 50  0001 C CNN
F 3 "~" H 3000 1900 50  0001 C CNN
	1    3000 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:L L2
U 1 1 5F8E8642
P 3400 1700
F 0 "L2" V 3590 1700 50  0000 C CNN
F 1 "4.7uH" V 3499 1700 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L5.3mm_D2.2mm_P10.16mm_Horizontal_Vishay_IM-1" H 3400 1700 50  0001 C CNN
F 3 "~" H 3400 1700 50  0001 C CNN
	1    3400 1700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5F8ECC5D
P 2350 2200
F 0 "#PWR06" H 2350 1950 50  0001 C CNN
F 1 "GND" H 2355 2027 50  0000 C CNN
F 2 "" H 2350 2200 50  0001 C CNN
F 3 "" H 2350 2200 50  0001 C CNN
	1    2350 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2100 2350 2200
$Comp
L power:+12V #PWR05
U 1 1 5F8EDD8C
P 2350 1600
F 0 "#PWR05" H 2350 1450 50  0001 C CNN
F 1 "+12V" H 2365 1773 50  0000 C CNN
F 2 "" H 2350 1600 50  0001 C CNN
F 3 "" H 2350 1600 50  0001 C CNN
	1    2350 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1700 2350 1600
$Comp
L Device:C C1
U 1 1 5F906717
P 2550 3700
F 0 "C1" H 2665 3746 50  0000 L CNN
F 1 "10uF 100V" H 2665 3655 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 2588 3550 50  0001 C CNN
F 3 "~" H 2550 3700 50  0001 C CNN
	1    2550 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5F90743B
P 3050 3500
F 0 "L1" V 3240 3500 50  0000 C CNN
F 1 "5.6uH" V 3149 3500 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L5.3mm_D2.2mm_P10.16mm_Horizontal_Vishay_IM-1" H 3050 3500 50  0001 C CNN
F 3 "~" H 3050 3500 50  0001 C CNN
	1    3050 3500
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C5
U 1 1 5F907C85
P 3500 3700
F 0 "C5" H 3615 3746 50  0000 L CNN
F 1 "2.2uF 100V" H 3615 3655 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 3538 3550 50  0001 C CNN
F 3 "~" H 3500 3700 50  0001 C CNN
	1    3500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3550 2900 3550
Wire Wire Line
	2900 3550 2900 3500
Wire Wire Line
	3200 3500 3200 3550
Wire Wire Line
	3200 3550 3500 3550
Wire Wire Line
	4300 3550 3500 3550
Connection ~ 3500 3550
Wire Wire Line
	4600 3850 3500 3850
Connection ~ 3500 3850
Wire Wire Line
	3500 3850 2550 3850
$Comp
L power:+12V #PWR01
U 1 1 5F90CF63
P 2050 3450
F 0 "#PWR01" H 2050 3300 50  0001 C CNN
F 1 "+12V" H 2065 3623 50  0000 C CNN
F 2 "" H 2050 3450 50  0001 C CNN
F 3 "" H 2050 3450 50  0001 C CNN
	1    2050 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5F90DA04
P 2050 4000
F 0 "#PWR02" H 2050 3750 50  0001 C CNN
F 1 "GND" H 2055 3827 50  0000 C CNN
F 2 "" H 2050 4000 50  0001 C CNN
F 3 "" H 2050 4000 50  0001 C CNN
	1    2050 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3850 2050 3850
Wire Wire Line
	2050 3850 2050 4000
Connection ~ 2550 3850
Wire Wire Line
	2550 3550 2050 3550
Wire Wire Line
	2050 3550 2050 3450
Connection ~ 2550 3550
Text Label 5250 3550 0    50   ~ 0
POWER_5V
Wire Wire Line
	4900 3550 5250 3550
$Comp
L Relay_SolidState:LCC110 U5
U 1 1 5F910C85
P 9700 1350
F 0 "U5" H 9700 1917 50  0000 C CNN
F 1 "LCC120" H 9700 1826 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 9700 850 50  0001 C CIN
F 3 "http://www.ixysic.com/home/pdfs.nsf/www/LCC110.pdf/$file/LCC110.pdf" H 9700 1350 50  0001 L CNN
	1    9700 1350
	1    0    0    -1  
$EndComp
$Comp
L Relay_SolidState:LCC110 U6
U 1 1 5F91387C
P 9700 2500
F 0 "U6" H 9700 3067 50  0000 C CNN
F 1 "LCC120" H 9700 2976 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 9700 2000 50  0001 C CIN
F 3 "http://www.ixysic.com/home/pdfs.nsf/www/LCC110.pdf/$file/LCC110.pdf" H 9700 2500 50  0001 L CNN
	1    9700 2500
	1    0    0    -1  
$EndComp
$Comp
L Relay_SolidState:LCC110 U7
U 1 1 5F916630
P 9700 3700
F 0 "U7" H 9700 4267 50  0000 C CNN
F 1 "LCC120" H 9700 4176 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 9700 3200 50  0001 C CIN
F 3 "http://www.ixysic.com/home/pdfs.nsf/www/LCC110.pdf/$file/LCC110.pdf" H 9700 3700 50  0001 L CNN
	1    9700 3700
	1    0    0    -1  
$EndComp
$Comp
L Relay_SolidState:LCC110 U8
U 1 1 5F9194BD
P 9700 4850
F 0 "U8" H 9700 5417 50  0000 C CNN
F 1 "LCC120" H 9700 5326 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 9700 4350 50  0001 C CIN
F 3 "http://www.ixysic.com/home/pdfs.nsf/www/LCC110.pdf/$file/LCC110.pdf" H 9700 4850 50  0001 L CNN
	1    9700 4850
	1    0    0    -1  
$EndComp
Text Label 9100 1150 2    50   ~ 0
GPIO_TUNE_UP
Text Label 8950 3500 2    50   ~ 0
GPIO_TUNE_DOWN
Wire Wire Line
	9100 1150 9400 1150
Wire Wire Line
	9400 1550 9400 2300
Wire Wire Line
	8950 3500 9400 3500
Wire Wire Line
	9400 3900 9400 4650
$Comp
L Device:R R8
U 1 1 5F91FFDE
P 9050 2700
F 0 "R8" V 8843 2700 50  0000 C CNN
F 1 "100" V 8934 2700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8980 2700 50  0001 C CNN
F 3 "~" H 9050 2700 50  0001 C CNN
	1    9050 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 2700 9200 2700
$Comp
L power:GND #PWR015
U 1 1 5F923502
P 8750 2800
F 0 "#PWR015" H 8750 2550 50  0001 C CNN
F 1 "GND" H 8755 2627 50  0000 C CNN
F 2 "" H 8750 2800 50  0001 C CNN
F 3 "" H 8750 2800 50  0001 C CNN
	1    8750 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 2800 8750 2700
Wire Wire Line
	8750 2700 8900 2700
$Comp
L Device:R R7
U 1 1 5F924C1B
P 9000 5050
F 0 "R7" V 8793 5050 50  0000 C CNN
F 1 "100" V 8884 5050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8930 5050 50  0001 C CNN
F 3 "~" H 9000 5050 50  0001 C CNN
	1    9000 5050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5F926E43
P 8700 5200
F 0 "#PWR014" H 8700 4950 50  0001 C CNN
F 1 "GND" H 8705 5027 50  0000 C CNN
F 2 "" H 8700 5200 50  0001 C CNN
F 3 "" H 8700 5200 50  0001 C CNN
	1    8700 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5050 9150 5050
Wire Wire Line
	8850 5050 8700 5050
Wire Wire Line
	8700 5050 8700 5200
NoConn ~ 10000 1050
NoConn ~ 10000 1250
NoConn ~ 10000 2200
NoConn ~ 10000 2400
NoConn ~ 10000 3400
NoConn ~ 10000 3600
NoConn ~ 10000 4550
NoConn ~ 10000 4750
Text Label 10450 1450 0    50   ~ 0
ISOLATED_12V
Text Label 10450 3800 0    50   ~ 0
ISOLATED_12V
Wire Wire Line
	10000 1450 10450 1450
Wire Wire Line
	10000 3800 10450 3800
Text Label 10450 5150 0    50   ~ 0
ISOLATED_GND
Text Label 10450 2800 0    50   ~ 0
ISOLATED_GND
Wire Wire Line
	10000 2800 10450 2800
Wire Wire Line
	10000 5150 10450 5150
Text Label 10450 1650 0    50   ~ 0
BIAS_L
Text Label 10450 2600 0    50   ~ 0
BIAS_R
Text Label 10450 4000 0    50   ~ 0
BIAS_R
Text Label 10450 4950 0    50   ~ 0
BIAS_L
Wire Wire Line
	10000 1650 10450 1650
Wire Wire Line
	10000 2600 10450 2600
Wire Wire Line
	10000 4000 10450 4000
Wire Wire Line
	10000 4950 10450 4950
Text Label 7450 3900 0    50   ~ 0
BIAS_R
Text Label 7400 4300 0    50   ~ 0
BIAS_L
$Comp
L Connector:Barrel_Jack J4
U 1 1 5F940219
P 6750 5100
F 0 "J4" H 6521 5058 50  0000 R CNN
F 1 "DC+12V" H 6521 5149 50  0000 R CNN
F 2 "Connector_BarrelJack:BarrelJack_CUI_PJ-063AH_Horizontal_CircularHoles" H 6800 5060 50  0001 C CNN
F 3 "~" H 6800 5060 50  0001 C CNN
	1    6750 5100
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5F942969
P 7050 5500
F 0 "#PWR012" H 7050 5250 50  0001 C CNN
F 1 "GND" H 7055 5327 50  0000 C CNN
F 2 "" H 7050 5500 50  0001 C CNN
F 3 "" H 7050 5500 50  0001 C CNN
	1    7050 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 5200 7050 5500
$Comp
L power:+12V #PWR011
U 1 1 5F944B48
P 7050 4850
F 0 "#PWR011" H 7050 4700 50  0001 C CNN
F 1 "+12V" H 7065 5023 50  0000 C CNN
F 2 "" H 7050 4850 50  0001 C CNN
F 3 "" H 7050 4850 50  0001 C CNN
	1    7050 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4850 7050 5000
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 5F94D535
P 6650 4250
F 0 "J2" H 6568 3925 50  0000 C CNN
F 1 "Bias Tee" H 6568 4016 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 6650 4250 50  0001 C CNN
F 3 "~" H 6650 4250 50  0001 C CNN
	1    6650 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6850 4150 7050 4150
Wire Wire Line
	6850 4300 6850 4250
$Comp
L MCU_Module:WeMos_D1_mini U4
U 1 1 5F95893A
P 6850 1750
F 0 "U4" H 7250 950 50  0000 C CNN
F 1 "WeMos_D1_mini" H 7300 850 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 6850 600 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 5000 600 50  0001 C CNN
	1    6850 1750
	1    0    0    -1  
$EndComp
Text Label 6950 800  0    50   ~ 0
POWER_3.3V
Wire Wire Line
	6950 800  6950 950 
NoConn ~ 7250 1250
NoConn ~ 7250 1650
NoConn ~ 7250 1750
NoConn ~ 7250 2150
NoConn ~ 6450 1750
NoConn ~ 6450 1650
NoConn ~ 6450 1350
$Comp
L power:GND #PWR010
U 1 1 5F967C23
P 6850 2900
F 0 "#PWR010" H 6850 2650 50  0001 C CNN
F 1 "GND" H 6855 2727 50  0000 C CNN
F 2 "" H 6850 2900 50  0001 C CNN
F 3 "" H 6850 2900 50  0001 C CNN
	1    6850 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2550 6850 2900
Text Label 7650 1350 0    50   ~ 0
GPIO_TUNE_UP
Text Label 7650 1850 0    50   ~ 0
GPIO_TUNE_DOWN
Wire Wire Line
	7250 1350 7650 1350
Wire Wire Line
	7250 1850 7650 1850
Text Label 7650 1450 0    50   ~ 0
ADC_SCL
Text Label 7650 1550 0    50   ~ 0
ADC_SDA
Wire Wire Line
	7250 1450 7650 1450
Wire Wire Line
	7650 1550 7250 1550
Text Label 5500 6400 2    50   ~ 0
ADC_SCL
Text Label 5500 6500 2    50   ~ 0
ADC_SDA
$Comp
L power:GND #PWR09
U 1 1 5F981ECE
P 4850 6300
F 0 "#PWR09" H 4850 6050 50  0001 C CNN
F 1 "GND" H 4855 6127 50  0000 C CNN
F 2 "" H 4850 6300 50  0001 C CNN
F 3 "" H 4850 6300 50  0001 C CNN
	1    4850 6300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x03 J3
U 1 1 5F9A31D0
P 6700 6150
F 0 "J3" H 6618 5825 50  0000 C CNN
F 1 "SWR Bridge" H 6618 5916 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 6700 6150 50  0001 C CNN
F 3 "~" H 6700 6150 50  0001 C CNN
	1    6700 6150
	-1   0    0    1   
$EndComp
Text Label 7350 6250 0    50   ~ 0
FWD_POWER
Wire Wire Line
	6900 6150 7350 6150
Wire Wire Line
	6900 6250 7350 6250
Wire Wire Line
	6900 6050 7350 6050
Text Label 1400 6700 2    50   ~ 0
REV_PWR
Text Label 1400 6050 2    50   ~ 0
FWD_POWER
$Comp
L Device:R R1
U 1 1 5F9BB543
P 1800 6050
F 0 "R1" V 1593 6050 50  0000 C CNN
F 1 "22K" V 1684 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1730 6050 50  0001 C CNN
F 3 "~" H 1800 6050 50  0001 C CNN
	1    1800 6050
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F9BDB10
P 1800 6700
F 0 "R2" V 1593 6700 50  0000 C CNN
F 1 "22K" V 1684 6700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1730 6700 50  0001 C CNN
F 3 "~" H 1800 6700 50  0001 C CNN
	1    1800 6700
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 6050 1650 6050
Wire Wire Line
	1400 6700 1650 6700
$Comp
L Device:R R4
U 1 1 5F9CAF2C
P 2150 6850
F 0 "R4" H 2220 6896 50  0000 L CNN
F 1 "18K" H 2220 6805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 6850 50  0001 C CNN
F 3 "~" H 2150 6850 50  0001 C CNN
	1    2150 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5F9CBC59
P 2150 6200
F 0 "R3" H 2220 6246 50  0000 L CNN
F 1 "18K" H 2220 6155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 6200 50  0001 C CNN
F 3 "~" H 2150 6200 50  0001 C CNN
	1    2150 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6050 1950 6050
Wire Wire Line
	1950 6700 2150 6700
$Comp
L power:GND #PWR013
U 1 1 5F9D97C5
P 7350 6050
F 0 "#PWR013" H 7350 5800 50  0001 C CNN
F 1 "GND" H 7355 5877 50  0000 C CNN
F 2 "" H 7350 6050 50  0001 C CNN
F 3 "" H 7350 6050 50  0001 C CNN
	1    7350 6050
	-1   0    0    1   
$EndComp
Text Label 7350 6150 0    50   ~ 0
REV_PWR
$Comp
L power:GND #PWR03
U 1 1 5F9E221C
P 2150 6350
F 0 "#PWR03" H 2150 6100 50  0001 C CNN
F 1 "GND" H 2155 6177 50  0000 C CNN
F 2 "" H 2150 6350 50  0001 C CNN
F 3 "" H 2150 6350 50  0001 C CNN
	1    2150 6350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F9E2B56
P 2150 7000
F 0 "#PWR04" H 2150 6750 50  0001 C CNN
F 1 "GND" H 2155 6827 50  0000 C CNN
F 2 "" H 2150 7000 50  0001 C CNN
F 3 "" H 2150 7000 50  0001 C CNN
	1    2150 7000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5F9E683D
P 2500 6200
F 0 "C2" H 2615 6246 50  0000 L CNN
F 1 "4.7nF" H 2615 6155 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 2538 6050 50  0001 C CNN
F 3 "~" H 2500 6200 50  0001 C CNN
	1    2500 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6050 2500 6050
Connection ~ 2150 6050
Wire Wire Line
	2500 6350 2150 6350
Connection ~ 2150 6350
$Comp
L Device:C C3
U 1 1 5F9F18E4
P 2500 6850
F 0 "C3" H 2615 6896 50  0000 L CNN
F 1 "4.7nF" H 2615 6805 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 2538 6700 50  0001 C CNN
F 3 "~" H 2500 6850 50  0001 C CNN
	1    2500 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 7000 2150 7000
Connection ~ 2150 7000
Wire Wire Line
	2150 6700 2500 6700
Connection ~ 2150 6700
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5FA10368
P 7350 5200
F 0 "#FLG02" H 7350 5275 50  0001 C CNN
F 1 "PWR_FLAG" H 7350 5373 50  0000 C CNN
F 2 "" H 7350 5200 50  0001 C CNN
F 3 "~" H 7350 5200 50  0001 C CNN
	1    7350 5200
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5FA10F96
P 7350 5000
F 0 "#FLG01" H 7350 5075 50  0001 C CNN
F 1 "PWR_FLAG" H 7350 5173 50  0000 C CNN
F 2 "" H 7350 5000 50  0001 C CNN
F 3 "~" H 7350 5000 50  0001 C CNN
	1    7350 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 5000 7050 5000
Connection ~ 7050 5000
Wire Wire Line
	7350 5200 7050 5200
Connection ~ 7050 5200
$Comp
L Regulator_Switching:R-78C5.0-1.0 U2
U 1 1 5FA23FD4
P 4600 3550
F 0 "U2" H 4600 3792 50  0000 C CNN
F 1 "R-785.0-1.0" H 4600 3701 50  0000 C CNN
F 2 "Converter_DCDC:Converter_DCDC_RECOM_R-78E-0.5_THT" H 4650 3300 50  0001 L CIN
F 3 "https://www.recom-power.com/pdf/Innoline/R-78Cxx-1.0.pdf" H 4600 3550 50  0001 C CNN
	1    4600 3550
	1    0    0    -1  
$EndComp
Text Label 6750 800  2    50   ~ 0
POWER_5V
Wire Wire Line
	6750 800  6750 950 
Wire Wire Line
	2350 1700 3000 1700
Wire Wire Line
	3000 1700 3000 1750
Wire Wire Line
	2350 2100 3000 2100
Wire Wire Line
	3000 2100 3000 2050
$Comp
L Connector:Conn_01x10_Female J1
U 1 1 5FB62215
P 5850 6600
F 0 "J1" H 5878 6576 50  0000 L CNN
F 1 "Adafruit ADS1115" H 5878 6485 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 5850 6600 50  0001 C CNN
F 3 "~" H 5850 6600 50  0001 C CNN
	1    5850 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 6300 5050 6300
Wire Wire Line
	5650 6200 5500 6200
Wire Wire Line
	5650 6400 5500 6400
Wire Wire Line
	5650 6500 5500 6500
Wire Wire Line
	5650 6600 5050 6600
Wire Wire Line
	5050 6600 5050 6300
Connection ~ 5050 6300
Wire Wire Line
	5050 6300 4850 6300
Wire Wire Line
	2500 6650 2500 6700
Connection ~ 2500 6700
Wire Wire Line
	5650 7100 5050 7100
Wire Wire Line
	5050 7100 5050 6900
Connection ~ 5050 6600
Wire Wire Line
	5650 6900 5050 6900
Connection ~ 5050 6900
Wire Wire Line
	5050 6900 5050 6600
$Comp
L Amplifier_Operational:LMC6482 U1
U 1 1 5F9E921C
P 3350 6200
F 0 "U1" H 3350 6567 50  0000 C CNN
F 1 "AD8032" H 3350 6476 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 3350 6200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmc6482.pdf" H 3350 6200 50  0001 C CNN
	1    3350 6200
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LMC6482 U1
U 2 1 5F9ED41D
P 3450 7050
F 0 "U1" H 3450 7417 50  0000 C CNN
F 1 "AD8032" H 3450 7326 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 3450 7050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmc6482.pdf" H 3450 7050 50  0001 C CNN
	2    3450 7050
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LMC6482 U1
U 3 1 5F9F1121
P 4350 5750
F 0 "U1" H 4308 5796 50  0000 L CNN
F 1 "AD8032" H 4308 5705 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 4350 5750 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmc6482.pdf" H 4350 5750 50  0001 C CNN
	3    4350 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 6300 4850 6050
Wire Wire Line
	4850 6050 4250 6050
Connection ~ 4850 6300
Text Label 4250 5300 0    50   ~ 0
POWER_3.3V
Wire Wire Line
	4250 5300 4250 5400
Wire Wire Line
	2500 6050 3050 6050
Wire Wire Line
	3050 6050 3050 6100
Connection ~ 2500 6050
Wire Wire Line
	2500 6700 3150 6700
Wire Wire Line
	3150 6700 3150 6950
Wire Wire Line
	3650 6200 3650 6550
Wire Wire Line
	3650 6550 2850 6550
Wire Wire Line
	2850 6550 2850 6300
Wire Wire Line
	2850 6300 3050 6300
Wire Wire Line
	3750 7050 3750 7400
Wire Wire Line
	3750 7400 2850 7400
Wire Wire Line
	2850 7400 2850 7150
Wire Wire Line
	2850 7150 3150 7150
$Comp
L Device:R R5
U 1 1 5FA189F1
P 4000 6200
F 0 "R5" V 4207 6200 50  0000 C CNN
F 1 "150" V 4116 6200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3930 6200 50  0001 C CNN
F 3 "~" H 4000 6200 50  0001 C CNN
	1    4000 6200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R6
U 1 1 5FA1968D
P 4000 7050
F 0 "R6" V 4207 7050 50  0000 C CNN
F 1 "150" V 4116 7050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3930 7050 50  0001 C CNN
F 3 "~" H 4000 7050 50  0001 C CNN
	1    4000 7050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 7050 3750 7050
Connection ~ 3750 7050
Wire Wire Line
	3650 6200 3850 6200
Connection ~ 3650 6200
$Comp
L Device:C C6
U 1 1 5FA710E3
P 4250 6350
F 0 "C6" H 4365 6396 50  0000 L CNN
F 1 "2.2uF" H 4365 6305 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 4288 6200 50  0001 C CNN
F 3 "~" H 4250 6350 50  0001 C CNN
	1    4250 6350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5FA72231
P 4250 7200
F 0 "C7" H 4365 7246 50  0000 L CNN
F 1 "2.2uF" H 4365 7155 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 4288 7050 50  0001 C CNN
F 3 "~" H 4250 7200 50  0001 C CNN
	1    4250 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6200 4250 6200
Wire Wire Line
	4150 7050 4250 7050
Wire Wire Line
	4250 6200 4750 6200
Wire Wire Line
	4750 6200 4750 6800
Wire Wire Line
	4750 6800 5650 6800
Connection ~ 4250 6200
Wire Wire Line
	5650 7000 4250 7000
Wire Wire Line
	4250 7000 4250 7050
Connection ~ 4250 7050
$Comp
L power:GND #PWR07
U 1 1 5FA8DA78
P 4250 6500
F 0 "#PWR07" H 4250 6250 50  0001 C CNN
F 1 "GND" H 4255 6327 50  0000 C CNN
F 2 "" H 4250 6500 50  0001 C CNN
F 3 "" H 4250 6500 50  0001 C CNN
	1    4250 6500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FA8E76E
P 4250 7350
F 0 "#PWR08" H 4250 7100 50  0001 C CNN
F 1 "GND" H 4255 7177 50  0000 C CNN
F 2 "" H 4250 7350 50  0001 C CNN
F 3 "" H 4250 7350 50  0001 C CNN
	1    4250 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 6200 5500 5400
Wire Wire Line
	5500 5400 4250 5400
Connection ~ 4250 5400
Wire Wire Line
	4250 5400 4250 5450
Text Label 7650 1950 0    50   ~ 0
ADC_ALERT
Wire Wire Line
	7250 1950 7650 1950
Text Label 5500 6700 2    50   ~ 0
ADC_ALERT
Wire Wire Line
	5500 6700 5650 6700
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5FBD0C84
P 1000 800
F 0 "H1" H 1100 849 50  0000 L CNN
F 1 "MountingHole_Pad" H 1100 758 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1000 800 50  0001 C CNN
F 3 "~" H 1000 800 50  0001 C CNN
	1    1000 800 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5FBD7795
P 1000 900
F 0 "#PWR016" H 1000 650 50  0001 C CNN
F 1 "GND" H 1005 727 50  0000 C CNN
F 2 "" H 1000 900 50  0001 C CNN
F 3 "" H 1000 900 50  0001 C CNN
	1    1000 900 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5FBD8D26
P 1000 1350
F 0 "H2" H 1100 1399 50  0000 L CNN
F 1 "MountingHole_Pad" H 1100 1308 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1000 1350 50  0001 C CNN
F 3 "~" H 1000 1350 50  0001 C CNN
	1    1000 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5FBD8D2C
P 1000 1450
F 0 "#PWR017" H 1000 1200 50  0001 C CNN
F 1 "GND" H 1005 1277 50  0000 C CNN
F 2 "" H 1000 1450 50  0001 C CNN
F 3 "" H 1000 1450 50  0001 C CNN
	1    1000 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5FBDE4E2
P 1000 1850
F 0 "H3" H 1100 1899 50  0000 L CNN
F 1 "MountingHole_Pad" H 1100 1808 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1000 1850 50  0001 C CNN
F 3 "~" H 1000 1850 50  0001 C CNN
	1    1000 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5FBDE4E8
P 1000 1950
F 0 "#PWR018" H 1000 1700 50  0001 C CNN
F 1 "GND" H 1005 1777 50  0000 C CNN
F 2 "" H 1000 1950 50  0001 C CNN
F 3 "" H 1000 1950 50  0001 C CNN
	1    1000 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5FBE35D2
P 1000 2350
F 0 "H4" H 1100 2399 50  0000 L CNN
F 1 "MountingHole_Pad" H 1100 2308 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1000 2350 50  0001 C CNN
F 3 "~" H 1000 2350 50  0001 C CNN
	1    1000 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5FBE35D8
P 1000 2450
F 0 "#PWR019" H 1000 2200 50  0001 C CNN
F 1 "GND" H 1005 2277 50  0000 C CNN
F 2 "" H 1000 2450 50  0001 C CNN
F 3 "" H 1000 2450 50  0001 C CNN
	1    1000 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 2100 3650 2100
Connection ~ 3000 2100
Wire Wire Line
	3550 1700 3650 1700
Wire Wire Line
	3250 1700 3000 1700
Connection ~ 3000 1700
Text Label 5800 2300 0    50   ~ 0
ISOLATED_GND
Text Label 6050 1550 1    50   ~ 0
ISOLATED_12V
Wire Wire Line
	5800 2100 5800 2300
Wire Wire Line
	7050 3900 7050 4150
Wire Wire Line
	6850 4300 7400 4300
Wire Wire Line
	7050 3900 7450 3900
NoConn ~ 7250 2050
NoConn ~ 4200 1900
NoConn ~ 11750 1150
$Comp
L Device:C C8
U 1 1 5FBA733D
P 3650 1900
F 0 "C8" H 3765 1946 50  0000 L CNN
F 1 "22uF 25V" H 3765 1855 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 3688 1750 50  0001 C CNN
F 3 "~" H 3650 1900 50  0001 C CNN
	1    3650 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 1750 3650 1700
Connection ~ 3650 1700
Wire Wire Line
	3650 1700 4200 1700
Wire Wire Line
	3650 2050 3650 2100
Connection ~ 3650 2100
Wire Wire Line
	3650 2100 3000 2100
Wire Wire Line
	5200 2100 5450 2100
Wire Wire Line
	5200 1700 5450 1700
$Comp
L Device:C C9
U 1 1 5FBB5B0F
P 5450 1900
F 0 "C9" H 5565 1946 50  0000 L CNN
F 1 "330nF" H 5565 1855 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 5488 1750 50  0001 C CNN
F 3 "~" H 5450 1900 50  0001 C CNN
	1    5450 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1750 5450 1700
Wire Wire Line
	5450 2050 5450 2100
Connection ~ 5450 2100
Wire Wire Line
	5450 2100 5800 2100
$Comp
L Device:L L3
U 1 1 5FBC3C58
P 5650 1700
F 0 "L3" V 5840 1700 50  0000 C CNN
F 1 "1mH" V 5749 1700 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L5.3mm_D2.2mm_P10.16mm_Horizontal_Vishay_IM-1" H 5650 1700 50  0001 C CNN
F 3 "~" H 5650 1700 50  0001 C CNN
	1    5650 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5500 1700 5450 1700
Connection ~ 5450 1700
$Comp
L Device:C C10
U 1 1 5FBC9B4B
P 6050 1900
F 0 "C10" H 6165 1946 50  0000 L CNN
F 1 "330nF" H 6165 1855 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L5.1mm_D3.1mm_P7.50mm_Horizontal" H 6088 1750 50  0001 C CNN
F 3 "~" H 6050 1900 50  0001 C CNN
	1    6050 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1700 6050 1700
Wire Wire Line
	6050 1700 6050 1750
Wire Wire Line
	6050 1550 6050 1700
Connection ~ 6050 1700
Wire Wire Line
	6050 2050 6050 2100
Wire Wire Line
	6050 2100 5800 2100
Connection ~ 5800 2100
$Comp
L Device:R R9
U 1 1 5FBDE0D5
P 4600 2750
F 0 "R9" H 4670 2796 50  0000 L CNN
F 1 "1000" H 4670 2705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4530 2750 50  0001 C CNN
F 3 "~" H 4600 2750 50  0001 C CNN
	1    4600 2750
	1    0    0    -1  
$EndComp
Text Label 4400 2550 2    50   ~ 0
ISOLATED_12V
Text Label 4400 2950 2    50   ~ 0
ISOLATED_GND
Wire Wire Line
	4400 2550 4600 2550
Wire Wire Line
	4600 2550 4600 2600
Wire Wire Line
	4600 2900 4600 2950
Wire Wire Line
	4600 2950 4400 2950
$Comp
L Device:R R10
U 1 1 5FBEAFE1
P 5000 2750
F 0 "R10" H 5070 2796 50  0000 L CNN
F 1 "1000" H 5070 2705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4930 2750 50  0001 C CNN
F 3 "~" H 5000 2750 50  0001 C CNN
	1    5000 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2550 5000 2550
Wire Wire Line
	5000 2550 5000 2600
Connection ~ 4600 2550
Wire Wire Line
	5000 2900 5000 2950
Wire Wire Line
	5000 2950 4600 2950
Connection ~ 4600 2950
Text Notes 5200 2450 2    50   ~ 0
Minimum load for isolated supply
Text Notes 5250 1300 2    50   ~ 0
Isolated power for loop control
Text Notes 4050 3200 2    50   ~ 0
Non-isolated power for ESP8266
Text Notes 3850 7650 2    50   ~ 0
SWR Bridge Buffering
Text Notes 9950 5550 2    50   ~ 0
Loop control
Text Notes 7550 3700 2    50   ~ 0
External connections
$EndSCHEMATC
