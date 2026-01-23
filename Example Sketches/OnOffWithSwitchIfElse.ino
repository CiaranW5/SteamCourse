/*
  Turns an LED on or off based on the setting of a PushButton (Momentary) switch.

  The #define block is used to give names to the digital pin numbers.
  This makes your sketch (program) easier to read and maintain
*/

#define MM_SWITCH 2
#define BB_LED 13

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin BB_LED as an output.
  
  pinMode(BB_LED, OUTPUT);
  pinMode(MM_SWITCH, INPUT);
}
// the loop function runs over and over again forever
void loop() {
  
  int SwitchState = 0;

  SwitchState = digitalRead(MM_SWITCH);

  if (SwitchState == HIGH)
    {
      digitalWrite(BB_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    }
  else
    {
      digitalWrite(BB_LED, LOW);   // turn the LED off by making the voltage LOW
    }
}
