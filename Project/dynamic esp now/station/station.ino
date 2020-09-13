#include <esp_now.h>
#include <WiFi.h>
esp_now_peer_info_t peerInfo;
uint8_t broadcastMac[] = {0xFF, 0xFF , 0xFF , 0xFF , 0xFF , 0xFF };
uint8_t registerMac[6];
unsigned int start_time = 0, min_time = 0;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  esp_now_setup();
  pinMode(0, INPUT);
}
void loop() {
  if (digitalRead(0) == 0) {
    start_send();
    while (digitalRead(0) == 0);
  }
  if (start_time != 0 && millis() - start_time >= 5000) {
    if (min_time == 0) start_send();
    else {
      add_peer(registerMac);
      send_data(registerMac, "Win");
      ESP.restart();
    }
  }
}
void start_send() {
  start_time = millis();
  send_data(broadcastMac, "Start");
}
void esp_now_setup() {
  while (esp_now_init() != ESP_OK) {
    Serial.println("Error to init esp_now");
  }
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(displaySendData);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  add_peer(broadcastMac);
}
void add_peer(uint8_t macAdd[]) {
  memcpy(peerInfo.peer_addr, macAdd, 6);
  while (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
}
void displaySendData(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void send_data(uint8_t macAdd[], String sending) {
  esp_err_t result = esp_now_send(macAdd, (uint8_t *) &sending , sizeof(sending));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  unsigned int in_time = millis();
  String check_member = "";
  memcpy(&check_member, incomingData, sizeof(check_member));
  Serial.println(check_member);
  if (check_member == "ready") {
    if (min_time == 0) {
      min_time = in_time;
      memcpy(&registerMac, mac, sizeof(registerMac));
    }
    else {
      if (min_time - start_time > in_time - start_time) {
        min_time = in_time;
        memcpy(&registerMac, mac, sizeof(registerMac));
      }
    }
  }
}
