// あらかじめピンを定数管理して、配列に入れておく
const int LED_PIN_1 = 12;
const int LED_PIN_2 = 11;
const int LED_PIN_3 = 10;
const int LED_PIN_4 = 9;
const int LED_PIN_5 = 8;
int led_array[5] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, LED_PIN_5}; 

void setup() {
  // それぞれのPINを出力モードで設定する
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(LED_PIN_5, OUTPUT);
}

void loop() {
  // 出力モードで設定したピンの配列をloopの中でforで回す。
  // すると、無限にそれぞれにピンに対して繰り返し処理ができる
  for (int i = 0; i < 5; i++) {
    digitalWrite(led_array[i], HIGH); // LED点灯
    delay(500);  // 0.5秒
    digitalWrite(led_array[i], LOW); // LED消灯
  }
}