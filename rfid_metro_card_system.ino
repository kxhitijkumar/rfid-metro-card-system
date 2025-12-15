#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN  21
#define RST_PIN 17
MFRC522 rfid(SS_PIN, RST_PIN);

// Wi-Fi creds
const char* ssid = "kxhitij";
const char* password = "kxhitijkumar";

// Supabase
const char* supabase_url = "URL";
const char* supabase_apikey = "KEY";

// Google Sheets Webhook
const char* sheets_webhook_url = "SHEETS_URL";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);

  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() {

  Serial.println("Waiting for RFID...");
  delay(1000);
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) uid += String(rfid.uid.uidByte[i], HEX);
  uid.toUpperCase();

  Serial.println("UID: " + uid);

  if (handleCardTap(uid)) {
    Serial.println("Access Granted");
  } else {
    Serial.println("Access Denied or Insufficient Balance");
  }

  rfid.PICC_HaltA();
  delay(1000);
}

bool handleCardTap(String uid) {
  HTTPClient http;

  // Step 1: GET current balance
  String url = String(supabase_url) + "?uid=eq." + uid;
  http.begin(url);
  http.addHeader("apikey", supabase_apikey);
  http.addHeader("Authorization", supabase_apikey);

  int httpCode = http.GET();
  if (httpCode != 200) return false;

  String response = http.getString();
  StaticJsonDocument<200> doc;
  deserializeJson(doc, response);
  if (doc.size() == 0) return false;

  int balance = doc[0]["balance"];
  Serial.println("Current Balance: " + String(balance));

  if (balance < 10) {
    logToSheets(uid, "Denied", balance);
    return false;
  }

  // Step 2: PATCH to deduct fare
  int newBalance = balance - 10;
  String patchUrl = String(supabase_url) + "?uid=eq." + uid;

  http.begin(patchUrl);
  http.addHeader("apikey", supabase_apikey);
  http.addHeader("Authorization", supabase_apikey);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"balance\": " + String(newBalance) + "}";
  int patchCode = http.PATCH(payload);

  if (patchCode == 200) {
    logToSheets(uid, "Success", newBalance);
    return true;
  }

  logToSheets(uid, "Error", balance);
  return false;
}

void logToSheets(String uid, String status, int balance) {
  HTTPClient http;
  http.begin(sheets_webhook_url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["uid"] = uid;
  doc["status"] = status;
  doc["balance"] = balance;

  String output;
  serializeJson(doc, output);
  http.POST(output);
  http.end();
}
