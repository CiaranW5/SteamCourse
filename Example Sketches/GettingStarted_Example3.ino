const int MIN_RGB = 9;
const int MAX_RGB = 11;
const int RED_LED = 12;
const int BLUE_LED = 13;

const int SW1 = 2;     // the number of the pushbutton pin
const int SW2 = 3;     // the number of the pushbutton pin
// const int can be an alternative to #define
// HERE WE HAVE THE FIRST FUNCTION DEFINED BY US
void RGB_Numbers(int j)
{
  switch (j)
    {
      case(0): 
        digitalWrite(MIN_RGB, LOW);
        digitalWrite(MIN_RGB+1, LOW);
        digitalWrite(MAX_RGB, LOW);
        break;
      case(1): 
        digitalWrite(MIN_RGB, HIGH);
        digitalWrite(MIN_RGB+1, LOW);
        digitalWrite(MAX_RGB, LOW);
        break;
      case(2): 
        digitalWrite(MIN_RGB, LOW);
        digitalWrite(MIN_RGB+1, HIGH);
        digitalWrite(MAX_RGB, LOW);
        break;
      case(3): 
        digitalWrite(MIN_RGB, HIGH);
        digitalWrite(MIN_RGB+1, HIGH);
        digitalWrite(MAX_RGB, LOW);
        break;
      case(4): 
        digitalWrite(MIN_RGB, LOW);
        digitalWrite(MIN_RGB+1, LOW);
        digitalWrite(MAX_RGB, HIGH);
        break;
      case(5): 
        digitalWrite(MIN_RGB, HIGH);
        digitalWrite(MIN_RGB+1, LOW);
        digitalWrite(MAX_RGB, HIGH);
        break;
      case(6): 
        digitalWrite(MIN_RGB, LOW);
        digitalWrite(MIN_RGB+1, HIGH);
        digitalWrite(MAX_RGB, HIGH);
        break;
      case(7): 
        digitalWrite(MIN_RGB, HIGH);
        digitalWrite(MIN_RGB+1, HIGH);
        digitalWrite(MAX_RGB, HIGH);
        break;
      default:
        digitalWrite(MIN_RGB, LOW);
        digitalWrite(MIN_RGB+1, LOW);
        digitalWrite(MAX_RGB, LOW);
        break;  
    }
}

void setup() {
  // put your setup code here, to run once:

  int i;
  for (i = MIN_RGB; i <= BLUE_LED; i++)
    pinMode(i, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int i, delay_time = 500;

  int buttonState2 = 0;         // variable for reading the pushbutton status

  buttonState2 = digitalRead(SW1);

  if (buttonState2 == 1)
    for (i = 0; i <= ((MAX_RGB-MIN_RGB) + 1); i++)
    {
      if (i != 0)
        digitalWrite(MIN_RGB + i - 1, LOW);
      if (i != (2 + 1))
        digitalWrite(i + MIN_RGB, HIGH);
      delay(delay_time);
    }
  else
    for (i = 0; i <= 7; i++)
    {
      RGB_Numbers(i);
      delay(delay_time);
    }
}
