//OLED
#include  <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED  define
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT  64 //OLED display height, in pixels
//SSD1306 display connected  to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,  &Wire, -1);

#define BUZZER 5
int sensor=A3; 
float gas_value;

void setup() {

  pinMode (BUZZER, OUTPUT);
  pinMode(sensor,INPUT); 
  //Serial.begin(115200);
  
  //Necessary code for OLED I2C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display(); //Displays the starting logo
  delay(1000); 
  display.clearDisplay();
  delay(30000);
}

void loop(){ 
  gas_value=analogRead(sensor); 
  //Serial.print("Result from MQ5/MQ6 sensor: ");
  //Serial.println(gas_value); 

  display.clearDisplay();
  oledDisplayHeader();

  //configures the position of the readings. (charactersize, x axis value, y axis value, reading, symbol)
  oledDisplay(2, 20, 28, gas_value, " ");
  
  display.display(); 
  
  if (gas_value > 160) {
    tone (BUZZER, 300);
  }
  else {
    noTone (BUZZER);
  }
 
  delay(1000);
} 

void  oledDisplayHeader(){
  //configures position and qualities of headers
 display.setTextSize(2);
 display.setTextColor(WHITE);
  display.setCursor(0, 0);
 display.print("Gas Sensor");
 
}

void oledDisplay(int size, int x,int  y, float value, String unit){
 int charLen=18;
 int xo=x+charLen*3.2;
  int xunit=x+charLen*3.6;
 int xval = x; 
 display.setTextSize(size);
 display.setTextColor(WHITE);
  
 if (unit=="%"){
   display.setCursor(x, y);
   display.print(value,0);
   display.print(unit);
 } else {
   if (value>99){
    xval=x;
   }  else {
    xval=x+charLen;
   }
   display.setCursor(xval, y);
   display.print(value,0);
   display.drawCircle(xo, y+2, 2, WHITE);  // print degree symbols
   display.setCursor(xunit,  y);
   display.print(unit);
 }
 
}