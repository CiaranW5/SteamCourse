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
// It's also ok to have
// const int MIN_RGB = 9;
// const int MAX_RGB = 11; and so on

void setup() {
 
  int i;
  for (i=MIN_RGB; i<=BLUE_LED;i++)
    pinMode(i, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

}

void loop() {
  
  int i, delay_time = 1000;

  for (i = 0; i <= 2; i++)
    {
      if (i==0)
        {
          digitalWrite((MIN_RGB + 2), LOW);
        }
      else
        {
          digitalWrite(i + MIN_RGB-1, LOW);
        }
        
      digitalWrite(i + MIN_RGB, HIGH);
      delay(delay_time);  
    }
}
