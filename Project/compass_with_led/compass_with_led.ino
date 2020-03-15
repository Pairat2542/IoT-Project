#include <analogWrite.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
const byte enALeftPin = 14, enBLeftPin = 32, in1LeftPin = 27, in2LeftPin = 26, in3LeftPin = 25, in4LeftPin = 33;
const byte enARightPin = 13, enBRightPin = 23, in1RightPin = 12, in2RightPin = 19, in3RightPin = 21, in4RightPin = 22;
const byte orangeLedPin = 18, blueLedPin = 5, greenLedPin = 4, yellowLedPin = 15;
void setup() {
  Serial.begin(9600);
  pinMode(orangeLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  checkBno();
  bno.setExtCrystalUse(true);
}

void loop() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  float x = euler.x();
  Serial.print("X: ");
  Serial.println(x);
  analogWrite(orangeLedPin, (x > 300 || x < 60) ? (x >= 0 && x < 60 ? map(x, 0, 60, 0, 255) : map(x, 300, 360, 250, 5)) : 255);
  analogWrite(blueLedPin, (x > 30 && x < 150) ? map(abs(x - 90), 0, 60, 0, 255) : 255);
  analogWrite(greenLedPin, (x > 120 && x < 240) ? map(abs(x - 180), 0, 60, 0, 255) : 255);
  analogWrite(yellowLedPin, (x > 210 && x < 330) ? map(abs(x - 270), 0, 60, 0, 255) : 255);
  delay(100);
}
void checkBno() {
  while (!bno.begin()) {
    Serial.println("Please connect adafruit BNO055");
    showAllLed(false, false, false, false);
    delay(1000);
  }
}
void showAllLed(bool a, bool b, bool c, bool d) {
  digitalWrite(orangeLedPin, a);
  digitalWrite(blueLedPin, b);
  digitalWrite(greenLedPin, c);
  digitalWrite(yellowLedPin, d);
}
