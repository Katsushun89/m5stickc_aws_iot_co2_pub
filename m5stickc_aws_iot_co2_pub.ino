#include <M5StickC.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "src/config.h"
#include "src/cert.h"

#define ALARM_WAIT_SEC 1000 * 30
#define WAIT_SEC 1000 * 1

WiFiClientSecure https_client;
PubSubClient mqtt_client(https_client);

void setupCO2Sensor()
{

}

void setup()
{
  Serial.begin(115200);

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);


  setupCO2Sensor();
  connectWifi();
  initMQTT();
  connectAWSIOT();
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  WiFi.disconnect(true);
  delay(1000);

  WiFi.begin(SSID, PASS);

  Serial.println("Connecting to WiFi");

  bool is_connected = true;
  Serial.print("Connecting...");
  uint32_t i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 40) {
      is_connected = false;
      break;
    }
    i++;
  }
  Serial.println("");
  if(is_connected){
    Serial.print("Connected to ");
    Serial.println(SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }else{
    Serial.println("Connection failed.");
  }
  return is_connected;
}

void reconnectWifi(){
  if(WiFi.status() != WL_CONNECTED){
    connectWifi();
  }
}

void connectAWSIOT() {
  reconnectWifi();

  while(!mqtt_client.connected()) {
    Serial.println("Try to MQTT connect..");
    if (mqtt_client.connect(THING_NAME)){
      Serial.println("Connected");
    }else{
      Serial.printf("Failed rc=%d ", mqtt_client.state());
      Serial.println("try again in 5 sec");
      delay(5000);
    }
  }
}

void initMQTT() {
  https_client.setCACert(root_ca);
  https_client.setCertificate(certificate);
  https_client.setPrivateKey(private_key);
  mqtt_client.setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);
}

void printStatus()
{
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println("CO2 Sensor");
}

void loop()
{
  M5.update();  // ボタン状態更新
  printStatus();
  delay(60 * 1000);
}
