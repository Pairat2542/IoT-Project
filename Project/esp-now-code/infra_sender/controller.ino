void control() {
  if (check1 != switch1) {
    sending.str = switch1;
    sending.timer = 1;
    send_data(macAddressCar1);
  }
  if (check2 != switch2) {
    sending.str = switch2;
    sending.timer = 1;
    send_data(macAddressCar2);
  }
  if (check3 != switch3) {
    sending.str = switch3;
    sending.timer = 1;
    send_data(macAddressCar3);
  }
  check1 = switch1;
  check2 = switch2;
  check3 = switch3;
  if (millis() - timer >= 1000) {
    timer = millis();
    checking_online_status();
  }
}
void checking_online_status() {
  check_online = false;
  sending.str = switch1;
  send_check_online(macAddressCar1);
  for (int i = 0; i < 50 ; i++) {
    if (check_online) break;
    delay(1);
  }
  if (check_online) {
    board1.on();
  } else {
    Serial.println("test1");
    board1.off();
  }
  check_online = false;
  sending.str = switch2;
  send_check_online(macAddressCar2);
  for (int i = 0; i < 50 ; i++) {
    if (check_online) break;
    delay(1);
  }
  if (check_online) {
    board2.on();
  } else {
    Serial.println("test2");
    board2.off();
  }
  check_online = false;
  sending.str = switch3;
  send_check_online(macAddressCar3);
  for (int i = 0; i < 50 ; i++) {
    if (check_online) break;
    delay(1);
  }
  if (check_online) {
    board3.on();
  } else {
    Serial.println("test3");
    board3.off();
  }
}
