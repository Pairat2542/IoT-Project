WidgetLED board1(V6);
WidgetLED board2(V7);
WidgetLED board3(V8);
BLYNK_WRITE(V3) {
  switch1 = param.asInt() == 1 ? "on" : "off";
}
BLYNK_WRITE(V4) {
  switch2 = param.asInt() == 1 ? "on" : "off";
}
BLYNK_WRITE(V5) {
  switch3 = param.asInt() == 1 ? "on" : "off";
}
