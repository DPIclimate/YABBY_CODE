/*
 * This code has be designed to run the DPI Schools Program 'The Yabby unit' teacher gauide example build.
 * Once the circuit board has been built following the design provided in the 'Teacher guide', the following code can be uploaded and executed.
 * The lights and sensors should react as follows:
 * The white LED will turn on if the temperature is greater than 25 degrees celsius or less than 20 degrees celsius
 * The lux photocell readings are linked to the red LED. When it is dark the red LED will become brighter and when it is light the red LED will bcome dull
 * The blue LED will turn on if the moisture sensor reads a moisture content is less than 60%
 * The liquid level sensor is connected to the yellow LED. the yellow LED will turn on if the liquid level sensor is in contact with a liquid. The yellow LED will turn off
 * if the liquid level sensor is not in contact with witha liquid. 
 * A single green LED will turn on if the pH value (read by the pH probe) is lower than or equal to 6.5, both Green LED will turn on if the pH value is higher than 7.5
 * 
 * This code also has the ability to calibrate the pH probe.
 * 
 * The code has been commented to allow users to understand what each section of code is controlling. 
 * 
 * If any changes need to be made to the code they should be done here in the main code file.
 * Warning any alterations to the libaries files could cause error that will affect all the code 
 * and make it unable to execute.
 * 
 * last edited by Charlene Chamberlain, Digital Laboratory Technician DPI Climate Branch
 * 30/09/2019
 */

// Include Libraries
/*
 * These are libaries that contain things such as code and configuration files
 * that the program is going to need and use. If you are using a library with code
 * or settings that are in a another file that is not refernced here 
 * the code will not be able to find it and will be unable to execute.
 */
#include "Arduino.h" //includes the library for Arduino devices
#include "DS18B20.h" //include the library for the tempature sensor
#include "LDR.h" //include the library for the light sensor
#include "LED.h" //inclide libaray to run LEDs
#include "Switchable.h" //icludes switching libaray
#include "SoilMoisture.h" // include library for soil moisture probe
#include "DFRobot_PH.h"
#include <EEPROM.h>

// Pin Definitions
/*
 * This section is used to define which pins on the board 
 * the LED and sensors are connected to. This allows the arduino board to comunicate with the correct sensors or LED through the specified pins.
 */
#define DS18B20WP_PIN_DQ	2               //the temperature sensor is connected to digital pin 2
#define LDR_PIN_SIG	A1                    //the photo/light sensor is connected to pin A3
#define LEDB_PIN_VIN	5                   //the Blue LED is connected to digital pin 5
#define LEDG_PIN_VIN	6                   //the Green LED 1 is connected to digital pin 6
#define LEDR_PIN_VIN	10//9               //the Red LED is connected to digital pin 10
#define LEDY_PIN_VIN	11                  //the Yellow LED is connected to digital pin 10
#define LEDG2_PIN_VIN 8                   //the Green LED 2 is connected to digital pin 12
#define LEDW_PIN_VIN 3                    //the white LED is connected to digital pin 13
#define SOILMOISTURE5V_1_PIN_SIG	A0      //the soil moisture probe is connected to A1 (Analog Pin 1)
#define PH_PIN_SIG	A2                    //the pH probe is connected to pin A5 (Analog Pin 5)
#define WATERLEVELSW_PIN_SIG	4           //the liquid level sensor is connected to digital pin 3

DFRobot_PH ph;

// Global variables and defines
/*
 * This section defines global variables.
 * Global variables can be seen by all sections of code, 
 * such as if you were to reference LEDpin within the checkTemp() method you would get the integer 10. 
 */
#define THRESHOLD_ldr   100
int ldrAverageLight;  // int stands for integer and is used to store a number value. This instance creates an integer to represent the average light level for the LDR/light sensor
int LEDbrightness;    //create an integer to represent the brightness of the LED
int LEDpin = 10;      //create an integer to represent the red led assigned to pin 10

int num; // create an integer to represent the variable num
unsigned long previousMillis = 0; //unsigned long are long varaibles that are used for large integer storage. They can stor up to 32 bits, unlike a normal long variable an unsigned long will not store negative numbers.

const long interval = 2000; //const stands for constant. It means the value of the variable is constant and can not be changed. This creates a constant long called interval and assignes it the value of 2000.

char input = 0; //char stands for character and is used to represent a single character e.g. 'a'. In this case it creates a char variable called input and assignes it the value zero.

char input2 = 0; // Creates tha char variable called input2 and assignes it the value of zero.

// define vars for testing menu
const int timeout = 10000;    // creates a constant integer called time out and defines a timeout of 10 sec
char menuOption = 0;          // creates a character variable called menu options and assignes the value zero
long time0;                   // creates a long variable called time0

// float is used to define a floating point number. A number that has a decimal point
float voltage,phValue,temperature;  //create three variables that are floating point numbers that are named, voltage, pHValue, temperature.

// Object initialization
DS18B20 ds18b20wp(DS18B20WP_PIN_DQ);
LDR ldr(LDR_PIN_SIG);
LED ledB(LEDB_PIN_VIN);
LED ledG1(LEDG_PIN_VIN);
LED ledR(LEDR_PIN_VIN);
LED ledY(LEDY_PIN_VIN);
LED ledG2(LEDG2_PIN_VIN);
LED ledW(LEDW_PIN_VIN);
SoilMoisture soilMoisture5v_1(SOILMOISTURE5V_1_PIN_SIG);
SoilMoisture ph_2(PH_PIN_SIG);

#define SensorPin PH_PIN_SIG            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20 //ph sample intival 20 milliseconds
#define printInterval 800 //ph print interval 800 milliseconds
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;       //store array index

const int STATE_MENU = 0;         //constant integer  for the STATE_MENU variable will always be zero 
const int STATE_CALIBRATION = 1;  //constant integer for STATE_CALIBRATION variable will always be one
int currentState = STATE_MENU;   // create an integer variable called currentState that is equal to the value to the STATE_MENU variable

// Setup the essentials for your circuit to work. It runs first time every time your circuit is powered with electricity.
void setup() {
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(115200);  
    
    ph.begin();
    
    while (!Serial) ; // wait for serial port to connect. Needed for native USB (local USB connection)
    Serial.println("start"); //Serial.println means that anything inside the brackets and quote makes will be printed to the serial monitor. For example Serial.println("hello world") will print hello world to the serial monitor
    
    ldrAverageLight = ldr.readAverage();  
}

void checkTemp(){
    // Read DS18B20 temp sensor value in degrees celsius. for degrees fahrenheit use ds18b20wp.ReadTempF()
    float ds18b20wpTempC = ds18b20wp.readTempC(); // (float) is used to identify a floating point number/ a number that has a decimal point
    // Print the value form the temperature sensor to the serial monitor
    Serial.print(F("Temp: ")); Serial.print(ds18b20wpTempC); Serial.println(F(" [C]"));
    
/*
 * the following is an 'if else' loop. An if else statment is a conditional loop. if the first statements conditions are meant then the command executes 
 */

    //if the tempature is greater then or equal to 25.0 degrees celsius
    if(ds18b20wpTempC >= 25.0){
      // turn on white LED
      ledW.on(); 
    }
    
    //if the tempature is less then or equal to 20.0 degrees celsius
    else if(ds18b20wpTempC <= 20.0){
      //turn white LED on
      ledW.on();
    }
    
    //if the tempature does not meet any of the previous conditions
    //turn the white LED off
    else{
      ledW.off();
    }
}

/*
 * The checkLightLevel function gets an interger from the sensor this intiger represensts the number of lux/ what the light level is.
 */
void checkLightLevel(){
 //read integer from the sensor 
 int ldrSample = ldr.read();
    int ldrDiff = abs(ldrAverageLight - ldrSample);
    //print the light level to the serial monitor
    Serial.print("Light level: ");
    Serial.println(ldrSample); 

    ldrSample = 1023 - ldrSample;
    LEDbrightness = map(ldrSample, 0, 1023, 0, 255);
    analogWrite(LEDpin, LEDbrightness);
}

/*
 *  The checkSoilProbe function gets an integer from the sensor.
 *  the integer value is then printed to the serial monitor 
 *  if the integer is less the 60 the red LED will turn on.
 */
void checkSoilProbe(){
    //read the value from the soil moisture probe
    int soilMoisture5v_1Val = soilMoisture5v_1.read();
    //print the soil moisture value to the serial monitor
    Serial.print(F("Soil: ")); Serial.println(soilMoisture5v_1Val);
    // if the soil moisture value is less then 60 
    if(soilMoisture5v_1Val < 60){
      //turn on red LED
        ledB.on();                                          
    }
    //if the soil moisture value does not meet the previous condition 
    else{
      //turn red LED off
      ledB.off();   
    } 
}

/*
 * The checkLiquidLevel funtion gets an integer from the sensor.
 * the integer returened will be a 1 or a 0. Means that the sensor is wet or dry. 
 * if the sensor is dry the yellow LED will turn off
 * if the sensor is wet the yellow LED will turn on
 */
void checkLiquidLevel(){
   int isWet = digitalRead(WATERLEVELSW_PIN_SIG);
   //if the liquid level sensor is wet
    if ( isWet ) {
      //turn yellow LED off
    ledY.on();
    // if the liquid level sensor is dry
  } else {
    //turn yellow LED on
    ledY.off();
  }
}

/*
 * The checkPHSensor function obtains two longs from the sensor.
 * Using these numbers a caculation is preformed that gives you two 
 * floating point numbers the pH voltage and the pH value.
 * If the pH value is equal to or less then 6.5 the Green1 LED will 
 * turn on 
 * if the pH value is equal to or greater then 7.5 Geen1 LED and Green2  
 * LED will both been turned on. If the pH value is between 6.5 and 7.5 
 * both green LEDs will be turned off. 
 * This function uses another fuction called 'averagearray' that is located towards the end of this program
 */
void checkPHSensor(){     // a static variable is one that can only be seen inside one function. unlike global variables that can be seen by all functions
    static unsigned long samplingTime = millis(); // millis() returns the number of milliseconds that have passed since you started running the current project. this line assigns that value to a variable called samplingtime
    static unsigned long printTime = millis(); // creates a static unsigned long variable called printTime and assigns it the value of the number of milliseconds that have passed since the program started   
    static float pHValue,voltage; //creates two static float varaibles called pHvalue and voltage. a Float is a number with a decimal point

    // if the number of milliseconds since the program started minus the value of sampling Time is greater then the value of samplingInterval execute the code in the brackets
    if(millis()-samplingTime > samplingInterval){
        pHArray[pHArrayIndex++]=analogRead(SensorPin); //
        if(pHArrayIndex==ArrayLenth)pHArrayIndex=0; //if 
        voltage = avergearray(pHArray, ArrayLenth)*5.0/1024; //avaragearray is a function that is located later in the project. voltage equals values form the average array multiplied by 5.0 and divided by 1024
        pHValue = 3.5*voltage+Offset; // pHValue equals 3.5 multiplied by the value of the variable voltage plus the value of the  Offset
        samplingTime=millis(); // assign the number of milliseconds since the program started to the variable samplingTime
    }
    
    //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    if(millis() - printTime > printInterval){
    Serial.print("PH Voltage:"); //print 'PH Voltage' to the serial monitor
          Serial.print(voltage,2); //print the value assigned to the variable voltage to the serial monitor
          Serial.print("    pH value: "); // print 'pH value' to the serial monitor
    Serial.println(pHValue,2); // print the value assigned to the variable pHValue to the serial monitor
          
          printTime=millis(); //assign the number of milliseconds since the program started to the varaible printTime
          // if the pH value is less then or equal to 6.5 Green LED 1 will turn on
          if(pHValue <= 6.5){
            ledG1.on();
              
          } //end of if statment
          // if the pH value is greater then or equal to 7.5 Green LED 1 and 2 will turn on
          else if(pHValue >=7.5){
            ledG1.on();
            ledG2.on();
          }//end of if else statement
          //else if neither of the previous conditions are meet bot Green LED will be turned off 
          else{
            ledG1.off();
            ledG2.off();
          }//end of else statement
    }
}

/*
 * The check menu fuction waits for the user to enter 'm' in the serial monitor. It then prints the menu options for the user to see
 */
void checkMenu(){
  //if input for the serial monitor is available
 if(Serial.available()){
  //the input from the serial monitor is read and assigned to the variable input
    input = Serial.read();
    //if the input matches 'm' the menu is printed
    if(input == 'm'){
      Serial.println("              MENU             ");
      Serial.println("===============================");
      Serial.println("Test tempature sensor.........t");
      Serial.println("Test water level sensor ......l");
      Serial.println("Test soil moisture probe .....s");
      Serial.println("Test light sesnor.............e");
      Serial.println("Test white LED................w");
      Serial.println("Test blue LED ................b");
      Serial.println("Test green LED 1 .............g");
      Serial.println("Test green LED 2 .............f");
      Serial.println("Test yellow LED ..............y");
      Serial.println("Test red LED .................r");
      Serial.println("Calibrate ph sesnor ..........c");
      Serial.println("");
      //the program waits 6000 milliseconds / 6 seconds before continuing. This was done to allow users time to read the menu and enter a option
      delay(6000);
    } // end of if loop
 }
}

/*
 * testLED is triggerd when a user selects one of the menu options. the function reads any input for the serial monitor and executes the corresponding 
 * code e.g. enter 'b' to test blue LED
 */
void testLED(){
  //if input from the serial monitor is available
  if(Serial.available()){
    //read input for the serial monitor and label it as the variable 'input'
    input = Serial.read();

    //if the input form the serial monitor is equal to b
    if (input=='b'){
      //set variable num to zero
    num = 0;
    //while the variable num is less then 20
      while(num < 20){ // while loop start
    //print 'now testing blue led' to the serial monitor
      Serial.println("now testing blue led");
         //turn blue LED on                    
        ledB.on();
        //wait for 150 milliseconds
        delay(150);                               
     //turn blue LED off                                        
    ledB.off(); 
    //wait for 150 milliseconds
    delay(150);
    //add one to the value of num
      num ++;
      //end of loop. return to the start of the while loop.
      }
    }
    //else if the input from the serial mointor is an 'r'
    else if (input=='r'){
      //set the value of variable num to zero 
      num = 0;
      //while the value of num is less then 20
      while(num < 20){ //start of while loop
      //print 'now testing red led' to the serial monitor
      Serial.println("now testing red led");
       //turn red led on
        ledR.on();
        //wait for 150 milliseconds
        delay(150);                               
     //turn red led off                                         
    ledR.off(); 
    //wait for 150 milliseconds
    delay(150);
    //add one to the value of num
      num ++;
      //end of loop. return to the start of the while loop.
      }
      //when the vaule of num is equal to or greater then 20 exit the loop
    }
    //else if the input from the serial monitor is 'y'
    else if (input=='y'){
      //set the value of num to zero
           num = 0;
           //while the value of the variable num is less the 20
      while(num < 20){ //start of while loop
      //print 'now testing the yellow led' to the serial monitor
      Serial.println("now testing yellow led");
      //turn the yellow led on  
        ledY.on();
        //wait for 150 milliseconds
        delay(150);                               
    // turn yellow led off                                         
    ledY.off(); 
    //wait for 150 milliseconds
    delay(150);
    //add one to the value of num
      num ++;
      //end of loop. return to the start of the while loop
      } 
      //when the vaule of num is equal to or greater then 20 exit the loop
    } 
       
    //else if the input for the serial mointor is 'w'
    else if (input=='w'){
      //set the value of the variable num to zero
    num = 0;
    //while the value of num is less then 20
      while(num < 20){ //start of while loop
        //print 'now testing white led' to the serial monitor
      Serial.println("now testing white led");
        // turn white led on
        ledW.on();
        //wait 150 milliseconds
        delay(150);                               
    // turn white led off                                          
    ledW.off(); 
    //wait 150 milliseconds
    delay(150);
    //add one to the value of num 
      num ++;
      //end of loop return to the start of the while loop
      }
      //when the vaule of num is equal to or greater then 20 exit the loop
    }
    
    //else if the input from the serial monitor is 'g'
    else if (input=='g'){
      //set the value of num to zero
     num = 0;
     //while the vaule of num is less then 20
      while(num < 20){ //start of while loop
      //print 'now testing green 1 led' to the serial moinitor
      Serial.println("now testing Green 1 led");
       //turn on ledG1 
        ledG1.on();
        //wait for 150 milliseconds
        delay(150);                            
     //turn ledG1 off                                         
    ledG1.off(); 
    //wait 150 milliseconds
    delay(150);
    //add one to the value of num
      num ++;
      //end of loop. return to the start of the while loop
      }
      //when the vaule of num is equal to or greater then 20 exit the loop
    }
    //else if the input tot the serial monitor is 'f'
    else if (input=='f'){
      //set the value of num to zero
      num = 0;
      //while the value of num is less then 20
      while(num < 20){ //start of while loop
      //print to 'now testing green 2 led' to the serial moinitor
      Serial.println("now testing green 2 led");
        // turn the second green led on
        ledG2.on();
        //wait 150 milliseconds
        delay(150);                               
     //turn the second green led off                                         
    ledG2.off(); 
    //wait for 150 milliseconds
    delay(150);
    //add one to the value of num
      num ++;
      //end of loop. return to the start of the while loop 
      }
      //when the vaule of num is equal to or greater then 20 exit the loop
    }

    //else if the input from the serial monitor is 's'
    else if (input == 's'){
      //create an blank integer, the value that is read from the soil moisture probe is then assigned to that integer 
      int soilMoisture5v_1Val = soilMoisture5v_1.read();
      // print 'testing soil moisture probe' to the serial monitor 
      Serial.println("testing soil moisture probe");
      //print 'val' and the vaule read from the soil moisture probe to the serial monitor
    Serial.print(F("Val: ")); Serial.println(soilMoisture5v_1Val);
    }

    //else if the input to the serial monitor is 'e'
    else if (input == 'e'){
      //create an intger and assign it the value read from the light sensor
      int ldrSample = ldr.read();
    // print 'light level' to the serial monitor
    Serial.print("Light level: ");
    //print the value read from the light sensor
    Serial.println(ldrSample);   
    }

    //else if the input to the serial monitor is 'l'
    else if (input == 'l'){
      //create an integer that will store the value read from the water level sensor  
     int isWet = digitalRead(WATERLEVELSW_PIN_SIG); 
     //print 'water level sensor tested' to serial monitor
     Serial.println("water level sensor tested");
     //print the value read from the water level sensor
     //this value will be a 1 or 0 indicating a true or false value
     Serial.println(isWet);
    }
    //if the input to the serial monitor is a 't'
    else if (input == 't'){
      //create a float variable and assign it the value read from the tempature sensor
      float ds18b20wpTempC = ds18b20wp.readTempC(); // (float) is used to identify a floating point number/ a number that has a decimal point
    // Print the temperature to the serial monitor
    Serial.print(F("Temp: ")); Serial.print(ds18b20wpTempC); Serial.println(F(" [C]"));
    } 
    //if the input to the serial monitor is 'c'      
    else if (input == 'c'){
      //change the program to the state that allows the user to calibrate the ph probe
      Serial.println("change state to calibration");
      currentState = STATE_CALIBRATION;
    }
  } 
}

void returnMenu(){
   //if input for the serial monitor is available
 while(Serial.available()){
  //the input from the serial monitor is read and assigned to the variable input
    input2 = Serial.read();
   
    //if the input matches 'm' the menu is printed
    if(input2 == 'x'){
      Serial.println("returning to menu state");
      currentState = STATE_MENU;
    }
}
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() {
  if(currentState==STATE_MENU){ // if the currentState variable is equal to the value of the STATE_MENU the following commands will be run 
    unsigned long currentMillis = millis(); // millis() returns the number of milliseconds that have passed since you started running the current project. This line assigns that value to a variable called currentMillis.
    if(currentMillis - previousMillis >= interval){ // if the number of current milliseconds minus the precious number of milli seconds is greater then or equal to the value of interval trigger the code in the if brackets
      previousMillis = currentMillis; //change the value of previousMillis to equal the value of currentMillis
      checkSoilProbe(); //trigger the function checkSoilProbe. this will run the code in the checkSoilProbe function.
      checkPHSensor();  //trigger the function checkPHSensor. this will run the code in the checkPHSensor function.
      checkLightLevel(); //trigger the function checkLightLevel. this will run the code in the checkLightLevel function.
      checkLiquidLevel(); //trigger the function checkLiquidLevel. this will run the code in the checkLiquidLevel function.
      checkTemp();        //trigger the function checkTemp. this will run the code in the checkTemp function.
      Serial.println();                                                                 //print a blank line to the serial monitor
      Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
      Serial.println("                Enter 'm' to display menu             ");         //print Enter 'm' to display menu to serial monitor
      Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
      Serial.println();                                                                 //print a blank line to the serial monitor
      checkMenu();       //trigger the checkMenu function. This will run the code in the checkMenu function and print the menu options
      testLED();        //trigger the testLEd function. this will run the code in the testLED function. however unlike the other function will only trigger when the user has entered an option from the menu.
    }           
  }
  //change state from state_menu to state_calibrtaion
  //this allow the program to change/access the pH calibration part of the code
  else if(currentState==STATE_CALIBRATION){
    //A static variable is one that is only visible inside one function. Unlike global variables that are visible to all functions
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();
        temperature = ds18b20wp.readTempC();         // read the value from the temperature sensor
        voltage = analogRead(PH_PIN_SIG)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("temperature:");             //print 'temperature' to the serial monitor
        Serial.print(temperature,1);              // print the value read for the temperature sensor to the serial monitor
        Serial.print("^C  pH:");                  //print 'pH' to the serial monitor
        Serial.println(phValue,2);               //print the value calculated for the ph value to the serial monitor
        
        
    } // end of if loop

    // create a character variable called cmdRecivedChar
    char cmdReceivedChar;
    // create a static unsigned long variable called cndReceivedTimeOut and assign it the value of the number of milliseconds that have passed since the start of the program
    static unsigned long cmdReceivedTimeOut = millis();
    //
    byte _cmdReceivedBufferIndex;
    //
    char   _cmdReceivedBuffer[ReceivedBufferLength];
    //while input from the serial monitor is available
    while(Serial.available()>0){ //while the input from the serial monitor is greater then zero
        if(millis() - cmdReceivedTimeOut > 500U){ //if the number of milliseconds  since the program started minus the value of cmdReceivedTimeOut is greater then 500U
            _cmdReceivedBufferIndex = 0; //value of cmdReceivedBufferIndex is equal to zero
            memset(_cmdReceivedBuffer,0,(ReceivedBufferLength));
        }
        cmdReceivedTimeOut = millis(); // assign the variable cmdReceivedTimeOut the number of milliseconds that have passed since the program started as its value
        cmdReceivedChar = Serial.read(); // assign the value read from the serial monitor to the mdReceivedChar 
        if (cmdReceivedChar == '\n' || _cmdReceivedBufferIndex==ReceivedBufferLength-1){
            _cmdReceivedBufferIndex = 0;
            strupr(_cmdReceivedBuffer);

            if(strstr(_cmdReceivedBuffer, "X") != NULL){ // if the value read from the serial monitor is 'x'
              Serial.println(""); // print a blank line to the serial monitor
              Serial.println("returning to menu state"); //print 'returning to menu state' to the serial monitor
              currentState = STATE_MENU; // return to menu state, exit calibraion state
            }
            else{
              ph.calibration(voltage,temperature,_cmdReceivedBuffer); //else run the ph.clabration function (in a file out side of main code body)
            }
        }else{
            _cmdReceivedBuffer[_cmdReceivedBufferIndex] = cmdReceivedChar;
            _cmdReceivedBufferIndex++;
        }
    }
  }
}

 /*   
  *    an Array is a collection of variables that are assigned an index number.
  *    this function is used to find the average for the ph sensor readings.
  *    For the sensor to take an accurate reading it takes a number of reads very quickly (within a few seconds)
  *    This fuction is then used to calculate the average of the those readings
  */
double avergearray(int* arr, int number){ // averarray is a fuction that will return a double type (another form of an integer). it also has two integer parameteres supplyed to the function called arr and number
  int i; // create an integer variable called i
  int max,min; //create two interger varaiables called max and min
  double avg; // create a double varaiable called avg
  long amount=0; // create a long variable called amount with a value equal to zero
  if(number<=0){ //if the value of the number is equal to or less then zero print the following line
    Serial.println("Error number for the array to avraging!/n"); 
    return 0; // returns the value zero
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    } // end of for loop
    avg = amount/number; // avg is equal to the value of amount divided by  the value of number
    return avg; // return the value of the average
  } // end of if loop 
  else{
    if(arr[0]<arr[1]){ // if the value at array index 0 is less then the value at array index 1
      min = arr[0];max=arr[1]; // assign value at index 0 to min and index 1 to max
    }
    else{
      min=arr[1];max=arr[0]; // else assign value at index 1 to min and index 0 to max
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){       
        amount+=min;        //arr<min
        min=arr[i];
      } //end of if loop
      else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        } // end of if loop
        else{
          amount+=arr[i]; //min<=arr<=max
        }//end of else loop
      }//end of else loop
    }// end of for loop
    avg = (double)amount/(number-2); //avg equals the value of amount as a double divided by the (value of number minus two)
  }//end of else loop
  return avg; //return the value of average
}
