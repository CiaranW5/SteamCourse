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

  servo.write(angle);

  if (forward) angle++;
  else angle--;

  if (angle >= 150) forward = false;
  if (angle <= 30) forward = true;

  delay(100);

  Serial.print("Angle = ");
  Serial.print(angle);
  Serial.println (" degrees");
  
}