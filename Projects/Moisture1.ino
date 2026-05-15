#define MOISTURE A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int moistureReading = 0;

  moistureReading = analogRead(MOISTURE);
  Serial.print ("Moisture Reading = ");
  Serial.println(moistureReading);
  
  delay(500);
}
