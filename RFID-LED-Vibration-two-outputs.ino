#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Outputs
const int LED1_PIN = 4;   // Tag 1 LED
const int LED2_PIN = 5;   // Tag 2 LED
const int VIB_PIN  = 7;   // Vibration motor driver/module input

// UIDs (yours)
byte TAG1_UID[] = {0x33, 0x0E, 0xDE, 0x0B}; // keyfob
byte TAG2_UID[] = {0x86, 0x8E, 0x5A, 0x30}; // white card
const byte UID_LEN = 4;

// Timing
const unsigned long ON_TIME   = 1500; // how long outputs stay on after a valid scan
const unsigned long COOLDOWN  = 1200; // prevents repeated triggers while holding tag

unsigned long lastTriggerAt = 0;

void allOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(VIB_PIN, LOW);
}

bool uidEquals(const byte *a, const byte *b, byte len) {
  for (byte i = 0; i < len; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void printUID() {
  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
}

void triggerTag1() { // LED1 + vibration
  allOff();
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(VIB_PIN, HIGH);
  delay(ON_TIME);
  allOff();
}

void triggerTag2() { // LED2 only
  allOff();
  digitalWrite(LED2_PIN, HIGH);
  delay(ON_TIME);
  allOff();
}

void unknownFeedback() { // quick blink both LEDs
  allOff();
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  delay(200);
  allOff();
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(VIB_PIN, OUTPUT);

  allOff();
  Serial.println("Ready. Tap keyfob or card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  printUID();

  // cooldown to avoid rapid retrigger when tag stays on reader
  if (millis() - lastTriggerAt < COOLDOWN) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  byte *scanned = mfrc522.uid.uidByte;

  if (mfrc522.uid.size >= UID_LEN && uidEquals(scanned, TAG1_UID, UID_LEN)) {
    Serial.println("Keyfob → LED1 + VIB ✅");
    lastTriggerAt = millis();
    triggerTag1();
  } else if (mfrc522.uid.size >= UID_LEN && uidEquals(scanned, TAG2_UID, UID_LEN)) {
    Serial.println("Card → LED2 ✅");
    lastTriggerAt = millis();
    triggerTag2();
  } else {
    Serial.println("Unknown tag ⛔");
    lastTriggerAt = millis();
    unknownFeedback();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
