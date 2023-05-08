#include <SPI.h>
#include <Servo.h> 
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

const int SS_PIN = 10;
const int RST_PIN = 9;
const int LOCK_POS = 90;
const int UNLOCK_POS = 180;

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo lockServo;

void displayLCDMessage(int row, int col, const char* message) {
  lcd.setCursor(col, row);
  lcd.print(message);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init(); 
  lcd.backlight();
  lockServo.attach(7);
  lockServo.write(LOCK_POS);
  displayLCDMessage(0, 0, "RFID door");
  displayLCDMessage(1, 1, "place your card");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
       content.concat(" ");
    }
    content.toUpperCase();
    if (content == "F3 DE 97 A9 ") {
      displayLCDMessage(0, 0, "Access Granted,");
      displayLCDMessage(1, 0, "Welcome Mr. xyz");
      lockServo.write(UNLOCK_POS);
      delay(2000);
      displayLCDMessage(1, 0, "Have fun");
      delay(2000);
      lockServo.write(LOCK_POS);
    } else {
      displayLCDMessage(0, 0, "Access denied");
      delay(2000);
      displayLCDMessage(1, 0, "Please try again");
      delay(2000);
    }
  }
}
