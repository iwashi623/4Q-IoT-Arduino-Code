char data;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();

    if (data == 'b') {
      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
    } else if (data == '0') {
      digitalWrite(11, HIGH);
      digitalWrite(13, LOW);
    }
  }

  delay(1000);
}