/*
  Blink - modified for the multifunction shield

  Turns two LEDs on for a time period, then off for the same perion, repeatedly.

  
  Based on the this example code in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

#define RED_LED2 12
#define BLUE_LED1 13

#define BLINK_PERIOD 200
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(BLUE_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(BLUE_LED1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(RED_LED2, HIGH);  // turn the LED on (LOW is the voltage level)
  delay(BLINK_PERIOD);                      // wait for a second
  digitalWrite(BLUE_LED1, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(RED_LED2, LOW);  // turn the LED off (HIGH is the voltage level)
  delay(BLINK_PERIOD);                      // wait for a second
}
