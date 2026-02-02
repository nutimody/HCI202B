#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define MOTOR_PIN 7

MFRC522 mfrc522(SS_PIN, RST_PIN);

// 🔁 CHANGE THESE TO MATCH YOUR TAGS
byte KEYFOB_UID[] = {0x33, 0x0E, 0xDE, 0x0B};
byte CARD_UID[]   = {0x86, 0x8E, 0x5A, 0x30};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  Serial.println("RFID ready. Scan a tag...");
}

void loop() {
  // Wait for a new card
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID detected: ");
  printUID(mfrc522.uid.uidByte, mfrc522.uid.size);

  if (compareUID(mfrc522.uid.uidByte, KEYFOB_UID)) {
    Serial.println("Keyfob detected → single vibration");
    vibrateOnce();
  }
  else if (compareUID(mfrc522.uid.uidByte, CARD_UID)) {
    Serial.println("White card detected → looping vibration");
    vibrateLoop();
  }
  else {
    Serial.println("Unknown card");
    digitalWrite(MOTOR_PIN, LOW);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// -------- FUNCTIONS --------

bool compareUID(byte *readUID, byte *knownUID) {
  for (byte i = 0; i < 4; i++) {
    if (readUID[i] != knownUID[i]) return false;
  }
  return true;
}

void vibrateOnce() {
  digitalWrite(MOTOR_PIN, HIGH);
  delay(300);   // gentle buzz
  digitalWrite(MOTOR_PIN, LOW);
}

void vibrateLoop() {
  for (int i = 0; i < 5; i++) {   // loop vibration
    digitalWrite(MOTOR_PIN, HIGH);
    delay(400);
    digitalWrite(MOTOR_PIN, LOW);
    delay(400);
  }
}

void printUID(byte *uid, byte size) {
  for (byte i = 0; i < size; i++) {
    Serial.print(uid[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}