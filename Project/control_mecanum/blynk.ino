BLYNK_WRITE(V0) {
  frontState = param.asInt();
}
BLYNK_WRITE(V1) {
  backState = param.asInt();
}
BLYNK_WRITE(V2) {
  leftState = param.asInt();
}
BLYNK_WRITE(V3) {
  rightState = param.asInt();
}
void controlLeftWheel(int enA, bool in1, bool in2, int enB, bool in3, bool in4) {
  analogWrite(enALeftPin, enA);
  analogWrite(enBLeftPin, enB);
  digitalWrite(in1LeftPin, in1);
  digitalWrite(in2LeftPin, in2);
  digitalWrite(in3LeftPin, in3);
  digitalWrite(in4LeftPin, in4);
}
void controlRightWheel(int enA, bool in1, bool in2, int enB, bool in3, bool in4) {
  analogWrite(enARightPin, enA);
  analogWrite(enBRightPin, enB);
  digitalWrite(in1RightPin, in1);
  digitalWrite(in2RightPin, in2);
  digitalWrite(in3RightPin, in3);
  digitalWrite(in4RightPin, in4);
}
