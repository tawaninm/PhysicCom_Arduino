/*
 * UNO R4: subscribe temp → show on RGB
 */

#include <WiFiS3.h>
#include <MQTTClient.h>

const char WIFI_SSID[] = "HotsHots";
const char WIFI_PASSWORD[] = "HotsHots";

const char MQTT_BROKER_ADRRESS[] = "phycom.it.kmitl.ac.th";
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "uno_r4_67070069";
const char MQTT_USERNAME[] = "";
const char MQTT_PASSWORD[] = "";

// ตามโจทย์
const char SUBSCRIBE_TOPIC[] = "67070069/venus";
const char PUBLISH_TOPIC[] = "67070069/venus";

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

const int RED_PIN   = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN  = 9;

unsigned long lastPublishTime = 0;
const int PUBLISH_INTERVAL = 5000;

void setup() {
  Serial.begin(9600);

  // RGB ต่อที่ 11 10 9
  pinMode(11, OUTPUT);   // RED
  pinMode(10, OUTPUT);   // GREEN
  pinMode(9,  OUTPUT);   // BLUE

  // ต่อ Wi-Fi ตามเดิม
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Arduino UNO R4 - Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(10000);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  connectToMQTT();
}

void loop() {
  mqtt.loop();

  // ไม่ได้บอกให้ publish อะไรเป็นพิเศษ ก็ส่งนิดหน่อยไว้เช็กได้
  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();
  }
}

void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
  mqtt.onMessage(messageReceived);

  Serial.print("Arduino UNO R4 - Connecting to MQTT broker");
  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("Arduino UNO R4 - MQTT broker Timeout!");
    return;
  }

  // subscribe ตามโจทย์
  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Arduino UNO R4 - Subscribed to the topic: ");
  else
    Serial.print("Arduino UNO R4 - Failed to subscribe to the topic: ");
  Serial.println(SUBSCRIBE_TOPIC);

  Serial.println("Arduino UNO R4 - MQTT broker Connected!");
}

void sendToMQTT() {
  // แค่ส่ง timestamp ไปเฉยๆ เผื่อดูว่าออนไลน์
  String val_str = String(millis() / 1000);
  char messageBuffer[16];
  val_str.toCharArray(messageBuffer, 16);
  mqtt.publish(PUBLISH_TOPIC, messageBuffer);
}

// ฟังก์ชันช่วย ตั้งสีแบบ common cathode (HIGH = ติด, LOW = ดับ)
// ถ้าเธอใช้ common anode ก็กลับค่าซะ
void setColor(int r, int g, int b) {
  digitalWrite(RED_PIN,   r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN,  b ? HIGH : LOW);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Arduino UNO R4 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  int temp = payload.toInt();   // "37" -> 37

  // ถ้าค่าอยู่ 36 - 50 ให้ LED เป็นสีแดง
  if (temp >= 36 && temp <= 50) {
    digitalWrite(11, HIGH);   // RED ON
    digitalWrite(10, LOW);    // GREEN OFF
    digitalWrite(9, LOW);     // BLUE OFF
  }
  // ถ้าค่าอยู่ 26 - 35 ให้ LED เป็นสีฟ้า
  else if (temp >= 26 && temp <= 35) {
    digitalWrite(11, LOW);    // RED OFF
    digitalWrite(10, LOW);    // GREEN OFF
    digitalWrite(9, HIGH);    // BLUE ON
  }
  // ถ้าค่าอยู่ 10 - 25 ให้ LED เป็นสีเขียว
  else if (temp >= 10 && temp <= 25) {
    digitalWrite(11, LOW);    // RED OFF
    digitalWrite(10, HIGH);   // GREEN ON
    digitalWrite(9, LOW);     // BLUE OFF
  }
  // นอกเหนือจากนี้ ปิดหมด
  else {
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
}
