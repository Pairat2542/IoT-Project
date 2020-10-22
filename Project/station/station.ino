#include <ArduinoJson.h>
#include "EEPROM.h"
#include <esp_now.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiAP.h>



int station_number = 2;
WebServer server(8080);
int state = 1;
bool Status = false;
bool restart_esp = false;
bool checkSend = true;
bool checkAdd = true;
bool check_send_fail = true;

esp_now_peer_info_t peerInfo;
uint8_t broadcastMac[] = {0xFF, 0xFF , 0xFF , 0xFF , 0xFF , 0xFF };
uint8_t registerMac[6];
unsigned int start_time = 0;
typedef struct Data {
  String msg;
  float latency;
} MyData;
MyData receive_data, winner_data;
char ssid[] = "Station#";
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);
  ssid[7] = station_number + 48;
  Serial.println(ssid);
  WiFi.softAP(ssid);
  Serial.println(WiFi.softAPIP());
  esp_now_setup();
  pinMode(0, INPUT);
  EEPROM.begin(64);
  Status = EEPROM.read(0);

  server.on("/", handleRoot);
  server.on("/setRestart", setRestart);
  server.on("/setStatusTrue", setTrue);
  server.on("/refresh", reFresh);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  server.handleClient();
  if (Status && checkSend) {
    checkSend = false;
    start_send();
    EEPROM.write(0, Status);
    EEPROM.commit();
  }
  if (start_time != 0 && millis() - start_time >= 10000) {
    if (winner_data.latency == 0) start_send();
    else if (checkAdd) {
      checkAdd = false;
      add_peer(registerMac);
      delay(100);
      int i = 0;
      for (i = 0 ; i < 3; i++) {
        send_data(registerMac, "Win");
        delay(1000);
        Serial.println(check_send_fail);
        if (check_send_fail) {
          state = 2;
          EEPROM.write(0, false);
          EEPROM.commit();
          break;
        }
      }
      if (i >= 3) {
        ESP.restart();
      }
    }
  }
  if (!check_send_fail) {
    checkSend = false;
    start_send();
    check_send_fail = true;
  }
}


void handleRoot() {
  const size_t capacity = JSON_OBJECT_SIZE(2);
  String replyMsg;
  DynamicJsonDocument doc(capacity);

  doc["state"] = state;
  doc["status"] = Status;


  serializeJson(doc, replyMsg);
  server.send(200, "application/json", replyMsg);

}

void setTrue() {
  Status = true;
  //   StatusCar = true;
  server.send(200, "text/plain", "set false success");
}

void setFalse() {
  Status = false;
  server.send(200, "text/plain", "set true success");
}

void reFresh() {
  Status = false;
  state = 1;

  //  StatusCar = false;
  //StatusCarEnd = false;

  ESP.restart();


  server.send(200, "text/plain", "reset success");
}

void setRestart() {
  state = 4;
  //  StatusCar = false;
  //StatusCarEnd = false;
  server.send(200, "text/plain", "reset success");

}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void start_send() {
  start_time = millis();
  winner_data.latency = 0;
  send_data(broadcastMac, ssid);
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
  check_send_fail = status == ESP_NOW_SEND_SUCCESS;
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
  memcpy(&receive_data, incomingData, sizeof(receive_data));
  Serial.println(receive_data.msg);
  if (receive_data.msg == "ready") {
    //StatusCar = true;
    if (winner_data.latency == 0) {
      winner_data = receive_data;
      memcpy(&registerMac, mac, sizeof(registerMac));
    }
    else {
      if (receive_data.latency < winner_data.latency) {
        winner_data = receive_data;
        memcpy(&registerMac, mac, sizeof(registerMac));
      }
    }
    display_macaddress(mac);
    Serial.print("Latency: ");
    Serial.println(receive_data.latency);
  }
  else if (receive_data.msg == "end") {
    // StatusCarEnd = true;
    state = 3;
    //if(state == 4){
    //restart_esp = false;
    //ESP.restart();
    //}

  }
}
void display_macaddress(const uint8_t* mac) {
  Serial.print("Mac Address: ");
  for (int i = 0 ; i < 6; i++) {
    Serial.print(mac[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
