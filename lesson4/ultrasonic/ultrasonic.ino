#include <SR04.h>

SR04 sr = SR04(11, 12);

void setup() {
  Serial.begin(9600);
}

void loop() {
  long d = sr.Distance();
  Serial.print(d);
  Serial.println("cm");
  delay(1000);
}