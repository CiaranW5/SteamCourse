#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPR121.h>

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

char keyDecode[12] = {'1', '4', '7', '*', '2', '5', '8', '0', '3', '6', '9', '#'};

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

// Pin assignments
#define BUZZER_PIN 5
#define PIR_PIN 7

// PIN code (fixed for now)
String storedPIN = "1234";
String inputCode = "";

// System states
bool isArmed = false;
bool disarmCountdown = false;
bool exitDelayActive = false;

unsigned long entryStartTime;
unsigned long exitStartTime;
const int entryDelay = 15000; // 15 seconds
const int exitDelay = 10000;  // 10 seconds

// Timing for buzzer
unsigned long lastBeepTime = 0;

//OLED  define
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT  32 //OLED display height, in pixels
//SSD1306 display connected  to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,  &Wire, -1);

void beep(int duration = 100) {
  tone(BUZZER_PIN, 1000);
  delay(duration);
  noTone(BUZZER_PIN);
}

char getKey() {
  // Get the currently touched pads
  currtouched = cap.touched();
  char keyTouched = 0;

  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(currtouched); Serial.println(i); Serial.print(keyDecode[i]); Serial.println(" touched");
      
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
      keyTouched = keyDecode[i];
    }
  }

  // reset our state
  lasttouched = currtouched;
  return keyTouched;
}

void setup() {
  
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);

  delay(100);

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  // This is generally recommended since it seems to work well for most setups.
  // Can remove if wanting to manually configure touch channels (CDC and CDT).
  Serial.println("Running auto configuration.");
  cap.setAutoconfig(true);

  Serial.println("Initialization complete.");

 
  display.display(); //Displays the starting logo
  delay(1000); 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Enter PIN:");
 
  display.display(); 
  
}

void loop() {

  char key = getKey();
  //Serial.println(key);

  if (key) {
    //beep();
    handleKeyInput(key);
  }

  // Exit delay logic
  if (exitDelayActive) {
    unsigned long elapsed = millis() - exitStartTime;
    if (elapsed >= exitDelay) {
      isArmed = true;
      exitDelayActive = false;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("System ARMED");
            
    } else {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 16);
      display.print("Arming: ");
      display.setCursor(48, 16);
      display.print("      ");
      display.setCursor(48, 16);
      display.print(((exitDelay - elapsed) / 1000));
      display.setCursor(56, 16);
      display.print("s");
     
      if (millis() - lastBeepTime >= 1000) {
        lastBeepTime = millis();
        beep(100);
      }
    }
  }
  display.display(); 

  // Entry delay logic
  if (disarmCountdown) {
    unsigned long elapsed = millis() - entryStartTime;
    int remaining = (entryDelay - elapsed) / 1000;
   
    if (elapsed >= entryDelay) {
      disarmCountdown = false;
      soundAlarm();
    } else {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("Motion Detected!");
      display.setCursor(0,  16);
      display.print("Enter PIN");
      
      display.setCursor(0,  24);
      display.print(remaining);
      display.setCursor(28, 24);
      display.print("s");
      
      if (millis() - lastBeepTime >= 1000) {
        lastBeepTime = millis();
        beep(100);
      }
    }
    display.display(); 
  }

  // PIR detection when armed
  if (isArmed && digitalRead(PIR_PIN) == HIGH && !disarmCountdown) {
    disarmCountdown = true;
    entryStartTime = millis();
    lastBeepTime = 0;
    display.clearDisplay();
  }
}

void soundAlarm() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("!! ALARM !!");
    
  for (int i = 0; i < 10; i++) {
    tone(BUZZER_PIN, 2000);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);
  }
}

void handleKeyInput(char key) {
  int y_val = 0;
  int x_val = 0;

  if (disarmCountdown) {
    y_val = 16;
    x_val = 40;
  }
  else {
    y_val = 0;
    x_val = 0;
  }
  if (key == '#') {
    checkPIN();
  } else if (key == '*') {
    inputCode = "";
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("PIN cleared");
    
  } else if (inputCode.length() < 4 && isDigit(key)) {
    inputCode += key;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    if (!disarmCountdown) {
      display.setCursor(x_val, y_val);
      display.print("Code: ");
    }
    
    display.setCursor((x_val+40), y_val);
    display.print(inputCode);
    
  }
  Serial.print("input Code:");
  Serial.println(inputCode);
}

void checkPIN() {
  if (inputCode == storedPIN) {
    // Disarm system in any state
    if (disarmCountdown || exitDelayActive || isArmed) {
      disarmCountdown = false;
      exitDelayActive = false;
      isArmed = false;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("Disarmed");
      
    } else {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("Arming in 10s..");
     
      exitDelayActive = true;
      exitStartTime = millis();
      lastBeepTime = 0;
    }
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Wrong PIN!");
    
  }
  display.display();

  inputCode = "";
}
