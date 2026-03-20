// Written by Ciaran MacNamee, based on Public Domain Code
// // 1. PIN DEFINITIONS

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 8;

// Ultrasonic Timeout
#define HCSR04_TIMEOUT 25000
#define DANGER_ZONE 20
#define WARNING_ZONE 50

const float speedOfSound = 0.034;

// Traffic light LED pins
const int ampelR = 12;  // Red LED
const int ampelY = 13; // Yellow LED
const int ampelG = 11; // Green LED

// // 2. VARIABLES FOR DISTANCE & MOVING AVERAGE
long duration = 0;
int distance = 0;

const int numReadings = 5;
int readings[numReadings]; 
int readIndex = 0;         
long total = 0;            
int averageDistance = 0;   

// // 3. TIME CONTROL VARIABLES (Global)

unsigned long lastTerminalUpdate = 0;    // For Terminal refresh timing
const int TerminalInterval = 200;        // Update Terminal every 200ms
      
// // 4. SETUP
void setup() {
  Serial.begin(9600);

  // Initial User Message
  
  Serial.println("System Ready");
  Serial.println("===============");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ampelR, OUTPUT);
  pinMode(ampelY, OUTPUT);
  pinMode(ampelG, OUTPUT);
  

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}


void loop() {
  unsigned long currentMillis = millis();
  // // 6. SENSOR MEASUREMENT
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, HCSR04_TIMEOUT); 
  distance = (duration * speedOfSound) / 2;

// distance <= 0 is an error, but set a safe distance value in this case
  if (distance <= 0) {
    distance = 200;
  }

  // // 7. CALCULATE MOVING AVERAGE
  total = total - readings[readIndex];
  readings[readIndex] = distance;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  averageDistance = total / numReadings;

  // // 8. TRAFFIC LIGHT LOGIC
  if (averageDistance <= DANGER_ZONE && averageDistance > 0) {
    digitalWrite(ampelR, HIGH); digitalWrite(ampelY, LOW); digitalWrite(ampelG, LOW);
  } 
  else if (averageDistance > DANGER_ZONE && averageDistance <= WARNING_ZONE) {
    digitalWrite(ampelR, LOW); digitalWrite(ampelY, HIGH); digitalWrite(ampelG, LOW);
  } 
  else {
    digitalWrite(ampelR, LOW); digitalWrite(ampelY, LOW); digitalWrite(ampelG, HIGH);
  }

  
  // // 9. DISPLAY LOGIC
  // We update the Terminal every 200ms
  if (currentMillis - lastTerminalUpdate >= TerminalInterval) {
    lastTerminalUpdate = currentMillis;
    
    Serial.print("Distance:       "); // Spaces clear old digits
    Serial.print(averageDistance);
    Serial.println(" cm");
    
    
    if(averageDistance <= DANGER_ZONE) Serial.println("Status: STOP!   ");
    else if(averageDistance <= WARNING_ZONE) Serial.println("Status: Caution ");
    else Serial.println("Status: Clear   ");
  }

}
