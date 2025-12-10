const int LED_PIN = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  Serial.println("HIGH");

  digitalWrite(LED_PIN, LOW);
  delay(2000);
  Serial.println("LOW");
}