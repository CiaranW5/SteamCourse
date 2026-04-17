#include <Servo.h>

#define SERVO_PIN 7

Servo servo;
int angle = 30;
bool forward = true;

void setup() {
  
  servo.attach(SERVO_PIN);
  servo.write(angle);

  Serial.begin(9600);

}

void loop() {

  unsigned int analogIn;

  analogIn = analogRead(A0);

  angle = ((unsigned long)analogIn*180)/1023;
  servo.write(angle);

  
  Serial.print("Angle = ");
  Serial.print(angle);
  Serial.println (" degrees");

  delay(100);
  
}