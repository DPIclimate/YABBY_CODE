
// Include Libraries
#include "Arduino.h"
#include "DS18B20.h"
#include "LDR.h"
#include "LED.h"
#include "Switchable.h"
#include "SoilMoisture.h"


// Pin Definitions
#define DS18B20WP_PIN_DQ	2
#define LDR_PIN_SIG	A3
#define LEDB_PIN_VIN	5
#define LEDG_PIN_VIN	6
#define LEDR_PIN_VIN	9
#define LEDY_PIN_VIN	10
#define SOILMOISTURE5V_1_PIN_SIG	A1
#define SOILMOISTURE5V_2_PIN_SIG	A5
#define WATERLEVELSW_PIN_SIG	3



// Global variables and defines
#define THRESHOLD_ldr   100
int ldrAverageLight;
// object initialization
DS18B20 ds18b20wp(DS18B20WP_PIN_DQ);
LDR ldr(LDR_PIN_SIG);
LED ledB(LEDB_PIN_VIN);
LED ledG(LEDG_PIN_VIN);
LED ledR(LEDR_PIN_VIN);
LED ledY(LEDY_PIN_VIN);
SoilMoisture soilMoisture5v_1(SOILMOISTURE5V_1_PIN_SIG);
SoilMoisture soilMoisture5v_2(SOILMOISTURE5V_2_PIN_SIG);



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    ldrAverageLight = ldr.readAverage();
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {}
    // Disclaimer: The DS18B20 1-Wire Temperature Sensor - Waterproof is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    else if(menuOption == '2') {
    // LDR (Mini Photocell) - Test Code
    // Get current light reading, substract the ambient value to detect light changes
    int ldrSample = ldr.read();
    int ldrDiff = abs(ldrAverageLight - ldrSample);
    Serial.print(F("Light Diff: ")); Serial.println(ldrDiff);

    }
    else if(menuOption == '3') {
    // LED - Basic Blue 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
        ledB.dim(i);                      // 1. Dim Led 
        delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledB.off();                        // 3. turns off
    }
    else if(menuOption == '4') {
    // LED - Basic Green 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
        ledG.dim(i);                      // 1. Dim Led 
        delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledG.off();                        // 3. turns off
    }
    else if(menuOption == '5') {
    // LED - Basic Red 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
        ledR.dim(i);                      // 1. Dim Led 
        delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledR.off();                        // 3. turns off
    }
    else if(menuOption == '6') {
    // LED - Basic Yellow 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
        ledY.dim(i);                      // 1. Dim Led 
        delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledY.off();                        // 3. turns off
    }
    else if(menuOption == '7') {
    // Soil Moisture Sensor #1 - Test Code
    int soilMoisture5v_1Val = soilMoisture5v_1.read();
    Serial.print(F("Val: ")); Serial.println(soilMoisture5v_1Val);

    }
    else if(menuOption == '8') {
    // Soil Moisture Sensor #2 - Test Code
    int soilMoisture5v_2Val = soilMoisture5v_2.read();
    Serial.print(F("Val: ")); Serial.println(soilMoisture5v_2Val);

    }
    else if(menuOption == '9') {}
    // Disclaimer: The SST Liquid Level Sensor is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    
    
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) DS18B20 1-Wire Temperature Sensor - Waterproof"));
    Serial.println(F("(2) LDR (Mini Photocell)"));
    Serial.println(F("(3) LED - Basic Blue 5mm"));
    Serial.println(F("(4) LED - Basic Green 5mm"));
    Serial.println(F("(5) LED - Basic Red 5mm"));
    Serial.println(F("(6) LED - Basic Yellow 5mm"));
    Serial.println(F("(7) Soil Moisture Sensor #1"));
    Serial.println(F("(8) Soil Moisture Sensor #2"));
    Serial.println(F("(9) SST Liquid Level Sensor"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {
            if(c == '1') 
    			Serial.println(F("Now Testing DS18B20 1-Wire Temperature Sensor - Waterproof - note that this component doesn't have a test code"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing LDR (Mini Photocell)"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing LED - Basic Green 5mm"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing LED - Basic Red 5mm"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing LED - Basic Yellow 5mm"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing Soil Moisture Sensor #1"));
    		else if(c == '8') 
    			Serial.println(F("Now Testing Soil Moisture Sensor #2"));
    		else if(c == '9') 
    			Serial.println(F("Now Testing SST Liquid Level Sensor - note that this component doesn't have a test code"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/