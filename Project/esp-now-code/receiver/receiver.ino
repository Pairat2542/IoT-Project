#include <esp_now.h>
#include <WiFi.h>
typedef struct data_receive {
  String str;
  int timer;
} control_data;
control_data receiving;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(2,OUTPUT);
}

void loop() {
  if(receiving.str == "on"){
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(2,LOW);
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receiving, incomingData, sizeof(receiving));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("str: ");
  Serial.println(receiving.str);
  Serial.print("timer: ");
  Serial.println(receiving.timer);
  Serial.println();
}
