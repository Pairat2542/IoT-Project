esp_now_peer_info_t peerInfo;
void esp_now_setup() {
  while (esp_now_init() != ESP_OK) {
    Serial.println("Error to init esp_now");
  }
  esp_now_register_send_cb(displaySendData);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  add_peer(macAddressCar1);
  add_peer(macAddressCar2);
  add_peer(macAddressCar3);
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
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  check_online = status == ESP_NOW_SEND_SUCCESS;
}
void send_data(uint8_t macAdd[]) {
  esp_err_t result = esp_now_send(macAdd, (uint8_t *) &sending, sizeof(sending));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
void send_check_online(uint8_t macAdd[]){
  esp_err_t result = esp_now_send(macAdd, (uint8_t *) &sending, sizeof(sending));
}
