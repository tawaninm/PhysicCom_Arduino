#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiS3.h>
#include <MQTTClient.h>

// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char STUDENT_ID[] = "67070069";

byte HEART[8] = {
  B00000, B01010, B11111, B11111,
  B11111, B01110, B00100, B00000
};

// ---------- WiFi / MQTT ----------
const char WIFI_SSID[] = "HotsHots";
const char WIFI_PASSWORD[] = "HotsHots";

const char MQTT_BROKER_ADRRESS[] = "phycom.it.kmitl.ac.th";
const int  MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "uno_r4_67070069";
const char MQTT_USERNAME[] = "";
const char MQTT_PASSWORD[] = "";

// ตรงนี้คือหัวข้อที่เราจะ "รับ" ข้อความจากเว็บ
const char SUBSCRIBE_TOPIC[] = "67070069/text";

// จะส่ง heartbeat ก็ส่ง topic อื่นไป
const char PUBLISH_TOPIC[] = "67070069/status";

WiFiClient network;
MQTTClient mqtt(256);

unsigned long lastPublishTime = 0;
const int PUBLISH_INTERVAL = 5000;

void setup() {
  // LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, HEART);

  // Serial
  Serial.begin(9600);

  // แสดงข้อความเริ่มต้น
  lcd.setCursor(0, 0);
  lcd.print("Waiting MQTT...");
  lcd.setCursor(0, 1);
  lcd.print(STUDENT_ID);

  // WiFi
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("WiFi: connecting to ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(10000);
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // MQTT
  connectToMQTT();
}

void loop() {
  mqtt.loop();

  // ส่ง heartbeat ไปให้รู้ว่ายังอยู่
  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();
  }
}

void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
  mqtt.onMessage(messageReceived);

  Serial.print("MQTT: connecting");
  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("MQTT: connected");

  if (mqtt.subscribe(SUBSCRIBE_TOPIC)) {
    Serial.print("subscribed to: ");
    Serial.println(SUBSCRIBE_TOPIC);
  } else {
    Serial.println("subscribe failed");
  }

  // บอกบนจอด้วย
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sub to /text OK");
  lcd.setCursor(0, 1);
  lcd.print(STUDENT_ID);
}

void sendToMQTT() {
  // ส่งเวลาไปเฉยๆ
  char buf[16];
  snprintf(buf, sizeof(buf), "%lu", millis() / 1000);
  mqtt.publish(PUBLISH_TOPIC, buf);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("MQTT msg:");
  Serial.print("topic: ");   Serial.println(topic);
  Serial.print("payload: "); Serial.println(payload);

  // เอาข้อความที่ได้มาแสดงบน LCD
  lcd.clear();

  // บรรทัดบน: I ♥ INN (ตามสไตล์เดิมคุณ)
  lcd.setCursor(5, 0);
  lcd.print("I ");
  lcd.write(byte(0));
  lcd.print(" INN");

  // บรรทัดล่าง: ข้อความที่มาจาก MQTT (ตัดให้พอดี 16 ตัว)
  lcd.setCursor(0, 1);
  // ถ้าข้อความยาวเกิน 16 ก็แสดงแค่ 16
  for (int i = 0; i < 16 && i < payload.length(); i++) {
    lcd.write(payload[i]);
  }
}
