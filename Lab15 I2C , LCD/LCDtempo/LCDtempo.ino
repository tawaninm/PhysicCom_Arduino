#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int TMP_PIN = A0;
const char STUDENT_ID[] = "67070069";

byte HEART[8] = {
  B00000, B01010, B11111, B11111,
  B11111, B01110, B00100, B00000
};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, HEART);
  Serial.begin(9600);
}

float readTempC() {
  int sensorValue = analogRead(TMP_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) / 0.01;
  return temperatureC;
}

void loop() {
  lcd.setCursor(0, 0); for (int i=0;i<16;i++) lcd.write(' ');
  lcd.setCursor(5, 0);
  lcd.print("I ");
  lcd.write(byte(0));
  lcd.print(" INN");

  // แถวล่าง: <ID> <อุณหภูมิ>°C (ทศนิยม 2 ตำแหน่ง)
  float tC = readTempC();
  char buf[8];
  dtostrf(tC, 5, 2, buf);

  lcd.setCursor(0, 1);
  lcd.print(STUDENT_ID);
  lcd.print(' ');
  lcd.print(buf);
  lcd.write(byte(223));
  lcd.print('C');

  Serial.print("Temperature: ");
  Serial.print(tC);
  Serial.println(" C");

  delay(500);
}
