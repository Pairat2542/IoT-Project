#include <esp_now.h>
#include <WiFi.h>
esp_now_peer_info_t peerInfo;
uint8_t stationMac[6];
bool check_coming = false, check_win = false;
unsigned int start_time = 0;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_setup();
  pinMode(2, OUTPUT);
}
void loop() {
  if (check_coming) {
    start_time = millis();
    add_peer(stationMac);
    delay(200);
    send_data(stationMac, "ready");
    delay(300);
    ESP.restart();
  }
  if (check_win) {
    digitalWrite(2, HIGH);
    delay(2000);
    digitalWrite(2, LOW);
    ESP.restart();
  }
}
void esp_now_setup() {
  while (esp_now_init() != ESP_OK) {
    Serial.println("Error to init esp_now");
  }
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(displaySendData);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
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
  esp_err_t result = esp_now_send(macAdd, (uint8_t *) &sending, sizeof(sending));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  String check_word = "";
  memcpy(&check_word, incomingData, sizeof(check_word));
  Serial.println(check_word);
  if (check_word == "Start") {
    memcpy(&stationMac, mac, sizeof(stationMac));
    check_coming = true;
  } else if (check_word == "Win") {
    memcpy(&stationMac, mac, sizeof(stationMac));
    check_win = true;
  } else {
    ESP.restart();
  }
}
