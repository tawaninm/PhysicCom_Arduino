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

// ตามโจทย์ รับจาก topic นี้
const char SUBSCRIBE_TOPIC[] = "67070069/venus";

// อันนี้จะส่งไว้เช็กว่าเรายังหายใจอยู่ก็ได้
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

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // ปิดก่อน
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

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

  // ส่งแค่ heartbeat ไปเรื่อยๆ ว่าเรายังอยู่
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

  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Arduino UNO R4 - Subscribed to the topic: ");
  else
    Serial.print("Arduino UNO R4 - Failed to subscribe to the topic: ");
  Serial.println(SUBSCRIBE_TOPIC);

  Serial.println("Arduino UNO R4 - MQTT broker Connected!");
}

void sendToMQTT() {
  String val_str = String(millis() / 1000);
  char messageBuffer[16];
  val_str.toCharArray(messageBuffer, 16);
  mqtt.publish(PUBLISH_TOPIC, messageBuffer);
}

// ถ้าเป็น common anode ให้กลับ HIGH/LOW เอง
void messageReceived(String &topic, String &payload) {
  Serial.println("Arduino UNO R4 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  int temp = payload.toInt();   // "37" -> 37
  Serial.print("temp parsed = ");
  Serial.println(temp);

  // 36 - 50 → RED
  if (temp >= 36 && temp <= 50) {
    digitalWrite(RED_PIN, 1);
    Serial.println("LED = RED");
  }
  // 26 - 35 → BLUE
  else if (temp >= 26 && temp <= 35) {
    digitalWrite(BLUE_PIN, 1);
    Serial.println("LED = BLUE");
  }
  // 10 - 25 → GREEN
  else if (temp >= 10 && temp <= 25) {
    digitalWrite(GREEN_PIN, 1);
    
    Serial.println("LED = GREEN");
  }
  // นอกช่วง ปิด
  else {
    digitalWrite(RED_PIN, 0);
    digitalWrite(GREEN_PIN, 0);
    digitalWrite(BLUE_PIN, 0);
    Serial.println("LED = OFF");
  }
}
