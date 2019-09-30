void setup() {   // the setup function runs once when you press reset or power the board
pinMode(12, OUTPUT);  // initialize digital pin 12 as an output.
}
void loop() {   // the loop function runs over and over again forever
digitalWrite(12, HIGH); // turn the LED on (HIGH is the voltage level)
delay(1000);    // wait for a second
digitalWrite(12, LOW);  // turn the LED off by making the voltage LOW
delay(1000);    // wait for a second
}
