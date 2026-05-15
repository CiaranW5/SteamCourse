/* Plant Watering System
  A3 - Moisture Sensor
  D7 Pump Control (Relay Control)
  A1 LDR reading
*/

#define PUMP 7
#define MOISTURE_INPUT A3
#define LDR_OUT A1
#define STATUS 12

void setup() {

  Serial.begin(9600);
  pinMode (PUMP, OUTPUT);
  pinMode (STATUS, OUTPUT);
  // put your setup code here, to run once:
  delay (1000);   // Wait for a moment
}


void loop() {

  int moisture_reading;
  int ldr_reading;

  int pump_control;

  moisture_reading = analogRead(MOISTURE_INPUT);
  ldr_reading = analogRead(LDR_OUT);

  Serial.print("Moisture Reading = ");
  Serial.println(moisture_reading);

  if ((moisture_reading < 200) && (ldr_reading < 500)) {
    Serial.println("ON");
    digitalWrite (PUMP, HIGH);
    digitalWrite (STATUS, HIGH);
    delay (1000);
    digitalWrite (PUMP, LOW);
    digitalWrite (STATUS, LOW);
  }
  else {
    digitalWrite (PUMP, LOW);
    digitalWrite (STATUS, LOW);
    delay(1000);      // else clause probably not needed apart from the delay
  }
  Serial.print("LDR Reading = ");
  Serial.println(ldr_reading);

  delay(5000);

}
