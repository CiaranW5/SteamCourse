#include <Servo.h>

#define TRIG_PIN 7
#define ECHO_PIN 8
#define SERVO_PIN A3

#define HCSR04_TIMEOUT 25000

Servo servo;
int angle = 30;
bool forward = true;

const float speedOfSound = 0.0343;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(SERVO_PIN);
  servo.write(angle);

  Serial.begin(9600);

}


float get_distance(void) {

  long duration = 0;
  float distance = 0;
  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, HCSR04_TIMEOUT); 
  distance = (duration * speedOfSound) / 2;

  return distance;
}

void loop() {

  // Move servo
  float distance;

  servo.write(angle);

  if (forward) angle++;
  else angle--;

  if (angle >= 150) forward = false;
  if (angle <= 30) forward = true;

  delay(100);

  distance = get_distance();

  Serial.print("Angle = ");
  Serial.print(angle);
  Serial.print(" degrees");
  Serial.print("\tDistance = ");
  Serial.print(distance);
  Serial.println(" cm");

}