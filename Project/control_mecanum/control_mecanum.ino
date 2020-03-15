#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <analogWrite.h>
char auth[] = "fLHvvZW8zNnY3YnR90VqgTg4VuHf58G1";
char ssid[] = "konrhor";
char pass[] = "12345678";
const byte enALeftPin = 14, enBLeftPin = 32, in1LeftPin = 27, in2LeftPin = 26, in3LeftPin = 25, in4LeftPin = 33;
const byte enARightPin = 13, enBRightPin = 23, in1RightPin = 12, in2RightPin = 19, in3RightPin = 21, in4RightPin = 22;
const byte orangeLedPin = 18, blueLedPin = 5, greenLedPin = 4, yellowLedPin = 15;
byte frontState = 0, backState = 0, leftState = 0, rightState = 0;
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  innitial();
}
void loop() {
  Blynk.run();
  controlCar();
}
void innitial() {
  pinMode(enALeftPin, OUTPUT);
  pinMode(enARightPin, OUTPUT);
  pinMode(enBLeftPin, OUTPUT);
  pinMode(enBRightPin, OUTPUT);
  pinMode(in1LeftPin, OUTPUT);
  pinMode(in2LeftPin, OUTPUT);
  pinMode(in3LeftPin, OUTPUT);
  pinMode(in4LeftPin, OUTPUT);
  pinMode(in1RightPin, OUTPUT);
  pinMode(in2RightPin, OUTPUT);
  pinMode(in3RightPin, OUTPUT);
  pinMode(in4RightPin, OUTPUT);
}
void controlCar() {
  bool l1, l2, l3, l4, r1, r2, r3, r4;
  l1 = r3 = (!frontState && leftState && !rightState) || (!frontState && backState && !rightState);
  l2 = r4 = (frontState && !backState && !leftState) || (!backState && !leftState && rightState);
  l3 = r1 = (!frontState && backState && !leftState) || (!frontState && !leftState && rightState);
  l4 = r2 = (frontState && !backState && !rightState) || (!backState && leftState && !rightState);
  controlLeftWheel(255, l1, l2, 255, l3, l4);
  controlRightWheel(255, r1, r2, 255, r3, r4);
}
