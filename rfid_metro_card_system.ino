#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 22  // Adjust as needed
#define SS_PIN  21  // Adjust as needed

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Simulated metro card balance database
struct Card {
  String uid;
  int balance;
};

Card cards[] = {
  {"12345678", 100},
  {"A1B2C3D4", 50}
};
const int NUM_CARDS = sizeof(cards) / sizeof(cards[0]);

void setup() {
  Serial.begin(115200);
  SPI.begin();          
  mfrc522.PCD_Init();   
  Serial.println("Metro RFID System Initialized");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  Serial.print("Card detected: ");
  Serial.println(uid);

  processCard(uid);
  mfrc522.PICC_HaltA();
}

void processCard(String uid) {
  for (int i = 0; i < NUM_CARDS; i++) {
    if (cards[i].uid.equalsIgnoreCase(uid)) {
      if (cards[i].balance >= 10) {
        cards[i].balance -= 10;
        Serial.print("Access Granted. Remaining Balance: ₹");
        Serial.println(cards[i].balance);
      } else {
        Serial.println("Access Denied. Insufficient Balance.");
      }
      return;
    }
  }
  Serial.println("Access Denied. Card not registered.");
}
