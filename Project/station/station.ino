#include <esp_now.h>
#include <WiFi.h>
#include <WiFiAP.h>
esp_now_peer_info_t peerInfo;
uint8_t broadcastMac[] = {0xFF, 0xFF , 0xFF , 0xFF , 0xFF , 0xFF };
uint8_t registerMac[6];
unsigned int start_time = 0;
typedef struct Data{
  String msg;
  float latency;
}MyData;
MyData receive_data,winner_data;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  String mac = WiFi.macAddress();
  char ssid[mac.length()-1];
  Serial.println(mac);
  for(int i=0;i<mac.length();i++) ssid[i] = mac[i];
  Serial.println(ssid);
  WiFi.softAP(ssid);
  Serial.println(WiFi.softAPIP());
  esp_now_setup();
  pinMode(0, INPUT);
}
void loop() {
  if (digitalRead(0) == 0) {
    start_send();
    while (digitalRead(0) == 0);
  }
  if (start_time != 0 && millis() - start_time >= 5000) {
    if (winner_data.latency == 0) start_send();
    else {
      add_peer(registerMac);
      send_data(registerMac, "Win");
      ESP.restart();
    }
  }
}
void start_send() {
  start_time = millis();
  winner_data.latency = 0;
  send_data(broadcastMac, "Start");
}
