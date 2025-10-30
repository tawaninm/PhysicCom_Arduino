/*
 *
 * This Arduino UNO R4 code is made available for public use without any restriction
 *
 */

#include <WiFiS3.h>
#include <MQTTClient.h>

const char WIFI_SSID[] = "HotsHots";     // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "HotsHots";  // CHANGE TO YOUR WIFI PASSWORD

const char MQTT_BROKER_ADRRESS[] = "phycom.it.kmitl.ac.th";  // CHANGE TO MQTT BROKER'S ADDRESS
//const char MQTT_BROKER_ADRRESS[] = "192.168.0.11";  // CHANGE TO MQTT BROKER'S IP ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "uno_r4_67070069";  // CHANGE IT AS YOU DESIRE
const char MQTT_USERNAME[] = "";              // CHANGE IT IF REQUIRED, empty if not required
const char MQTT_PASSWORD[] = "";              // CHANGE IT IF REQUIRED, empty if not required

// The MQTT topics that Arduino should publish/subscribe
const char PUBLISH_TOPIC[] = "67070069/venus";       // CHANGE IT AS YOU DESIRE
const char SUBSCRIBE_TOPIC[] = "67070069/venus";  // CHANGE IT AS YOU DESIRE

const int PUBLISH_INTERVAL = 5000;  // 5 seconds

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

unsigned long lastPublishTime = 0;

void setup() {
  Serial.begin(9600);

  // ตั้งขา ultrasonic ให้เรียบร้อยก่อน
  pinMode(9, OUTPUT);  // Trig
  pinMode(10, INPUT);  // Echo

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

  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    sendToMQTT();
    lastPublishTime = millis();
  }
}

void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
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

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Arduino UNO R4 - Subscribed to the topic: ");
  else
    Serial.print("Arduino UNO R4 - Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("Arduino UNO R4 - MQTT broker Connected!");
}

void sendToMQTT() {
  // trigger ให้เป็นพิธี
  digitalWrite(9, LOW);
  delayMicroseconds(2);
  digitalWrite(9, HIGH);
  delayMicroseconds(10);
  digitalWrite(9, LOW);

  // อ่าน echo (ใส่ timeout กันค้าง)
  long pulseWidth = pulseIn(10, HIGH, 30000);  // 30 ms

  // ถ้าไม่เจอ echo เลย ก็ส่ง off ไปเลย
  if (pulseWidth == 0) {
    Serial.println("no echo -> publish off");
    mqtt.publish(PUBLISH_TOPIC, "off");
    return;
  }

  // แปลงเป็น cm
  long distance = pulseWidth / 29 / 2;
  Serial.print("distance: ");
  Serial.println(distance);

  // ตรงนี้แหละที่คุณเขียนผิด ต้องเช็ก "distance" ไม่ใช่ "pulseWidth"
  if (distance > 20) {
    mqtt.publish(PUBLISH_TOPIC, "off");
    Serial.println("distance > 20 -> publish off");
  } else {
    char msg[16];
    snprintf(msg, sizeof(msg), "%ld", distance);
    mqtt.publish(PUBLISH_TOPIC, msg);
    Serial.print("publish: ");
    Serial.println(msg);
  }
}



void messageReceived(String &topic, String &payload) {
  Serial.println("Arduino UNO R4 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
}