/**
 * Ping Pong Controller - 2 Players
 * 可変抵抗×2、ボタン、LED、ブザー
 */

const int POT1_PIN = A0;
const int POT2_PIN = A1;
const int BUTTON_PIN = 2;
const int LED_PIN = 9;
const int BUZZER_PIN = 8;

int lastButtonState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);
  
  int buttonState = digitalRead(BUTTON_PIN);
  
  int buttonPressed = 0;
  if (buttonState == HIGH && lastButtonState == LOW) {
    buttonPressed = 1;
  }
  lastButtonState = buttonState;
  
  Serial.print(pot1);
  Serial.print(",");
  Serial.print(pot2);
  Serial.print(",");
  Serial.println(buttonPressed);
  
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    switch (cmd) {
      case '1':  // LED ON
        digitalWrite(LED_PIN, HIGH);
        break;
      case '0':  // LED OFF
        digitalWrite(LED_PIN, LOW);
        break;
      case 'S':  // 得点音
        tone(BUZZER_PIN, 1200, 150);
        break;
      case 'W':  // 勝利音
        playWinSound();
        break;
      case '3':  // カウントダウン 3
        tone(BUZZER_PIN, 440, 100);
        break;
      case '2':  // カウントダウン 2
        tone(BUZZER_PIN, 554, 100);
        break;
      case 'A':  // カウントダウン 1（'1'はLEDで使用中なので'A'）
        tone(BUZZER_PIN, 659, 100);
        break;
      case 'G':  // 加速音（Go!）
        tone(BUZZER_PIN, 880, 200);
        break;
    }
  }
  
  delay(20);
}

void playWinSound() {
  tone(BUZZER_PIN, 523, 150);
  delay(200);
  tone(BUZZER_PIN, 659, 150);
  delay(200);
  tone(BUZZER_PIN, 784, 150);
  delay(200);
  tone(BUZZER_PIN, 1047, 300);
}