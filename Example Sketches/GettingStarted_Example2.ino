/* Derived from Blink once again
  This sketch uses the Multifunction Board with
  RGB Leds: using D9 to D11 and single LEDs on D12 and D13
*/

#define MIN_RGB 9
#define MAX_RGB 11
#define RED_LED 12
#define BLUE_LED 13

#define SW1 2
#define SW2 3

void setup() {
  // put your setup code here, to run once:
  // Note that this version uses numbers on the code - bad practice
  int i;
  for (i=9; i<=13;i++)
    pinMode(i, OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  pinMode(5, OUTPUT);  // This is the Buzzer pin
}

void loop() {
  // put your main code here, to run repeatedly:

  int i, delay_time;

  delay_time = 1000;

  for (i = 0; i <= 3; i++)
    {
      if (i != 0)
        digitalWrite(MIN_RGB + i-1, LOW);
      if (i != 3)
        digitalWrite(MIN_RGB + i, HIGH);
      delay(delay_time);  
    }
}
