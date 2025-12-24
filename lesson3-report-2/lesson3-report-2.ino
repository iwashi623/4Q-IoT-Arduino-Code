// ライブラリの読み込み
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // 16文字×2行のLCDとして初期化
  lcd.begin(16, 2);
}

void loop() {
  lcd.clear(); // まず画面の初期化
  lcd.setCursor(0, 0); // 1行目の戦闘にカーソルを合わせる
  byte line1[] = {0xCA, 0xBC, 0xDE, 0xD2, 0xC3, 0xC9}; // "ハジメテノ"の文字列をLCD対応文字コードのバイトスライスで保持
  for (int i = 0; i < 6; i++) lcd.write(line1[i]); // 1文字ずつ表示
  
  lcd.setCursor(0, 1); // 2行目の先頭にカーソルを合わせる
  lcd.print("IoT"); // "IoT"という文字列を表示、ローマ字はバイトスライスにしなくても直接表示できる
  byte line2[] = {0xC2, 0xB8, 0xD8}; // "ツクリ" の文字列をLCD対応文字コードのバイトスライスで保持
  for (int i = 0; i < 3; i++) lcd.write(line2[i]); // 1文字ずつ表示
  delay(2000); // 2秒待機

  lcd.clear(); // 画面を初期化する
  lcd.setCursor(0, 0); // 1行目の戦闘にカーソルを合わせる
  lcd.print("IoT"); // "IoT"という文字列を表示、ローマ字はバイトスライスにしなくても直接表示できる
  byte line2b[] = {0xC2, 0xB8, 0xD8}; // "ツクリ" の文字列をLCD対応文字コードのバイトスライスで保持
  for (int i = 0; i < 3; i++) lcd.write(line2b[i]); // 1文字ずつ表示
  
  lcd.setCursor(0, 1); // 2行目の先頭にカーソルを合わせる
  byte line3[] = {0xB6, 0xDE, 0xDD, 0xCA, 0xDE, 0xD9, 0xBF, 0xDE}; // "ガンバルゾ" の文字列をLCD対応文字コードのバイトスライスで保持
  for (int i = 0; i < 8; i++) lcd.write(line3[i]); // 1文字ずつ表示
  delay(2000); // 2秒待機
}