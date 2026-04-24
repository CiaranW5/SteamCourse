#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9

#define RED_LED 7
#define GREEN_LED 5

#define SERVO_PIN 3

MFRC522 rfid(SS_PIN, RST_PIN);

// ---- STORED RFID CARDS (4-byte UIDs) ----
byte Tag1[4] = {0x02, 0xC9, 0x8B, 0x41};
byte Tag2[4] = {0x09, 0xD6, 0x8B, 0x5A};
byte Tag3[4] = {0x49, 0x42, 0x9E, 0xb8};

// ---- BALANCES ----
int balance1 = 500;
int balance2 = 300;
int balance3 = 0;

int tollAmount = 20;

Servo myServo;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Closed

  Serial.println("RFID System Initialized...");
}

bool uidEquals(byte *a, byte *b) {
  for (byte i = 0; i < 4; i++)
    if (a[i] != b[i]) return false;
  return true;
}

void printUID(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] < 0x10) Serial.print("0");
    Serial.print(uid[i], HEX);
    if (i < 3) Serial.print(" ");
  }
}

void waitForCardRemoval() {
  while (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    delay(50);
  }
}

void processCard(bool Recognised) {
  if (Recognised) {
    Serial.println("Recognised Card — Access Granted");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    myServo.write(90);  // Open
  }
  else {
    Serial.println("Unknown Card — Access Denied");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    myServo.write(0);  // Closed
  }
  delay(1000);

}

void loop() {
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  myServo.write(0);  // Closed

  Serial.println("\nWaiting for RFID...\n");

  while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
      delay(100);
  }
  
  Serial.print("RFID Tag Detected: ");
  printUID(rfid.uid.uidByte);
  Serial.println();

  bool is1 = uidEquals(rfid.uid.uidByte, Tag1);
  bool is2 = uidEquals(rfid.uid.uidByte, Tag2);
  bool is3 = uidEquals(rfid.uid.uidByte, Tag3);

      // Process each card
  if (is1) {
      processCard(1);
    } else if (is2) {
      processCard(1);
    } else if (is3) {
      processCard(1);
    } else {
      processCard(0);
    }

    delay (1000);

    myServo.write(0);  // Closed
    
    // Cleanup and wait for removal
    waitForCardRemoval();
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(200);
}
