// // 1. PIN DEFINITIONS & LIBRARIES
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

///OLED  define
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT  64 //OLED display height, in pixels
//SSD1306 display connected  to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,  &Wire, -1);

#define  T_C 262
#define T_D 294
#define T_E 330
#define T_F 349
#define T_G  392
#define T_A 440
#define T_B 493

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 8;

// Traffic light LED pins
const int ampelR = 9;  // Red LED
const int ampelY = 10; // Yellow LED
const int ampelG = 11; // Green LED

// Buzzer pin
const int buz = 5;

// // 2. VARIABLES FOR DISTANCE & MOVING AVERAGE
long duration = 0;
int distance = 0;

const int numReadings = 5;
int readings[numReadings]; 
int readIndex = 0;         
long total = 0;            
int averageDistance = 0;   

// // 3. TIME CONTROL VARIABLES (Global)
unsigned long previousMillis = 0;   // For buzzer timing
unsigned long lastLCDUpdate = 0;    // For LCD refresh timing
const int lcdInterval = 200;        // Update LCD every 200ms
bool buzzerState = LOW;           

// // 4. SETUP
void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display(); //Displays the starting logo
  delay(1000); 
  display.clearDisplay();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ampelR, OUTPUT);
  pinMode(ampelY, OUTPUT);
  pinMode(ampelG, OUTPUT);
  pinMode(buz, OUTPUT);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

// // 5. MAIN LOOP
void loop() {
  unsigned long currentMillis = millis();
  int status = 2;

  // // 6. SENSOR MEASUREMENT
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000); 
  distance = (duration * 0.017);

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
  if (averageDistance <= 20 && averageDistance > 0) {
    digitalWrite(ampelR, HIGH); digitalWrite(ampelY, LOW); digitalWrite(ampelG, LOW);
    status = 2;
  } 
  else if (averageDistance > 20 && averageDistance <= 50) {
    digitalWrite(ampelR, LOW); digitalWrite(ampelY, HIGH); digitalWrite(ampelG, LOW);
    status = 1;
  } 
  else {
    digitalWrite(ampelR, LOW); digitalWrite(ampelY, LOW); digitalWrite(ampelG, HIGH);
    status = 0;
  }

  // // 9. BUZZER LOGIC
  int pauseInterval = 0;
  const int beepDuration = 10; 

   if (averageDistance <= 5 && averageDistance > 0) pauseInterval = 10; 
  else if (averageDistance <= 10) pauseInterval = 100;
  else if (averageDistance <= 15) pauseInterval = 300;
  else if (averageDistance <= 30) pauseInterval = 700;
  else if (averageDistance <= 50) pauseInterval = 1200;
  else pauseInterval = 0;

  if (pauseInterval == 1) {
    digitalWrite(buz, HIGH);
  } 
  else if (pauseInterval == 0) {
    digitalWrite(buz, LOW);
    buzzerState = LOW;
  } 
  else {
    if (buzzerState == HIGH) {
      if (currentMillis - previousMillis >= (unsigned long)beepDuration) {
        digitalWrite(buz, LOW);
        buzzerState = LOW;
        previousMillis = currentMillis;
      }
    } 
    else {
      if (currentMillis - previousMillis >= (unsigned long)pauseInterval) {
        digitalWrite(buz, HIGH);
        buzzerState = HIGH;
        previousMillis = currentMillis;
      }
    }
  }


  // // 10. LCD DISPLAY LOGIC
  // We update the LCD only every 200ms to avoid flickering
  if (currentMillis - lastLCDUpdate >= lcdInterval) {
    lastLCDUpdate = currentMillis;
   
    display.clearDisplay();
    oledDisplayHeader();
  //configures the position of the readings. (charactersize, x axis value, y axis value, reading, symbol)
    oledDisplay(2,80,28, status,"%");
    oledDisplay(2,0,44, status,"%%");
    oledDisplay(2,-12,16, averageDistance,"cm");

    display.display(); 
 
  }

  // // 11. OUTPUT FOR MATLAB
  Serial.println(averageDistance);

  delay(30);
}

void  oledDisplayHeader(){
  //configures position and qualities of headers
 display.setTextSize(1);
 display.setTextColor(WHITE);
  display.setCursor(0, 0);
 display.print("Distance");
 display.setCursor(80,  0);
 display.print("Status");
}
void oledDisplay(int size, int x,int  y, float value, String unit){
 int charLen=18;
 //int xo=x+charLen*3.2;
  int xunit=x+charLen*3.2;
 int xval = x; 
 display.setTextSize(size);
 display.setTextColor(WHITE);
  
 if (unit=="%%"){
   display.setCursor(x, y);
   if (value >=2) {
    display.print("STOP");
   }
   else if (value == 1) {
    display.print("CAUTION");
   }
   else {display.print("CLEAR");

   }
   //display.print(value,0);
   //display.print(unit);
 } else if (unit=="%") {
  display.setCursor(x, y);
   if (value >=2) {
    display.print(")))");
   }
   else if (value == 1) {
    display.print(")) ");
   }
   else {display.print(")  ");
   }

 } else {
   if (value>999){
    xval=x;
   }  else {
    xval=x+charLen;
   }
   display.setCursor(xval, y);
   display.print(value,0);
   //display.drawCircle(xo, y+2, 2, WHITE);  // print degree symbols
   display.setCursor(xunit,  y);
   display.print(unit);
 }
}
 