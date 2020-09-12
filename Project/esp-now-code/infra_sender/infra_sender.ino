#define BLYNK_PRINT Serial
#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = "eXz9GnKzmMA7HRODZaQneSwo6UlmZegc";
char ssid[] = "prj_test";
char pass[] = "12345678";
char ip[] = "192.168.1.2";
uint8_t macAddressCar1[] = {0x30, 0xAE , 0xA4 , 0x16 , 0x6D , 0x88 };
uint8_t macAddressCar2[] = {0x24, 0x0A , 0xC4 , 0xAF , 0xC1 , 0x8C };
uint8_t macAddressCar3[] = {0x24, 0x0A , 0xC4 , 0xAF , 0x75 , 0xBC };
typedef struct data_send {
  String str;
  int timer;
} control_data;
control_data sending;
unsigned long timer = 0;
int state = 0;
String switch1 = "off", switch2 = "off", switch3 = "off", check1, check2, check3;
bool check_online = false;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192, 168, 1, 3), IPAddress(0, 0, 0, 0), IPAddress(255, 255, 255, 0));
  Blynk.begin(auth, ssid, pass, ip, 8080);
  esp_now_setup();
}
void loop() {
  Blynk.run();
  control();
}
