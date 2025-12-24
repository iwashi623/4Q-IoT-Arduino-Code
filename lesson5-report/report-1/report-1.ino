#include "IRremote.h"       // IR(赤外線)受信ライブラリを読み込む
#include "LedControl.h"     // LEDマトリックス制御ライブラリを読み込む

int receiver = 7;           // IR受信機のシグナルピンをピン7に設定
IRrecv irrecv(receiver);    // IR受信オブジェクトを作成
decode_results results;     // 受信結果を格納する変数

LedControl lc = LedControl(12, 10, 11, 1);  // マトリックス制御オブジェクト（データ=12, クロック=10, CS=11, デバイス数=1）

unsigned long displayTime = 0;  // 数字を表示した時刻を記録する変数
bool isDisplayOn = false;       // マトリックスが表示中かどうかのフラグ

byte digits[10][8] = {          // 0〜9の表示パターンを2次元配列で定義（各数字は8バイト）
  {B00000000, B00111110, B01111111, B01001001, B01001101, B01111111, B00111110, B00000000}, // 0のパターン
  {B00000000, B00000000, B01000010, B01111111, B01111111, B01000000, B01000000, B00000000}, // 1のパターン
  {B00000000, B00110010, B01110011, B01011001, B01001001, B01001111, B01000110, B00000000}, // 2のパターン
  {B00000000, B00100010, B01100011, B01001001, B01001001, B01111111, B00110110, B00000000}, // 3のパターン
  {B00000000, B00011000, B00010100, B00010010, B01111111, B01111111, B00010000, B00000000}, // 4のパターン
  {B00000000, B00100111, B01100111, B01000101, B01000101, B01111101, B00111001, B00000000}, // 5のパターン
  {B00000000, B00111110, B01111111, B01001001, B01001001, B01111011, B00110010, B00000000}, // 6のパターン
  {B00000000, B00000001, B01110001, B01111001, B00001101, B00000111, B00000011, B00000000}, // 7のパターン
  {B00000000, B00110110, B01111111, B01001001, B01001001, B01111111, B00110110, B00000000}, // 8のパターン
  {B00000000, B00100110, B01101111, B01001001, B01001001, B01111111, B00111110, B00000000}  // 9のパターン
};

void displayDigit(int num) {    // 数字を表示する関数（引数は表示したい数字0-9）
  if (num < 0 || num > 9) return;  // 0〜9以外なら何もせず終了
  for (int row = 0; row < 8; row++) {  // 8行分ループ、左下から上に向かって埋まっていく。1バイトで1列。
    lc.setRow(0, row, digits[num][row]);  // マトリックスの各行にパターンを送信。表示上は行だけど、ライブラリがrowとよんでいるので、こちら側の変数名もrow
  }
  displayTime = millis();       // 現在時刻を記録（自動消灯用）
  isDisplayOn = true;           // 表示中フラグをONに
}

void setupMatrix() {
  Serial.begin(9600);           // シリアル通信を9600bpsで開始
  Serial.println("IR Matrix Display Ready"); // 起動メッセージを表示
}

void setupIR() {
  irrecv.enableIRIn();          // IR受信を有効化
  
  lc.shutdown(0, false);        // マトリックスのパワーセーブモードを解除
  lc.setIntensity(0, 2);        // 明るさを設定（0〜15、2は暗め）
  lc.clearDisplay(0);           // 画面をクリア
}

void setup() {                  // 初期設定（起動時に1回だけ実行）
  setupMatrix();
  setupIR();
}

void loop() {                   // メインループ（繰り返し実行）
  // 1秒経過したら消灯
  if (isDisplayOn && millis() - displayTime >= 1000) {  // 表示中かつ1秒経過したら
    lc.clearDisplay(0);         // マトリックスを消灯
    isDisplayOn = false;        // 表示中フラグをOFFに
  }
  
  if (irrecv.decode(&results)) {  // IR信号を受信したら
    Serial.print("Code: ");       // 「Code: 」と表示
    Serial.println(results.value, HEX);  // 受信したコードを16進数で表示
    
    switch (results.value) {      // 受信コードで分岐
      case 0xFF6897: displayDigit(0); break;  // 0ボタン → 0を表示
      case 0xFF30CF: displayDigit(1); break;  // 1ボタン → 1を表示
      case 0xFF18E7: displayDigit(2); break;  // 2ボタン → 2を表示
      case 0xFF7A85: displayDigit(3); break;  // 3ボタン → 3を表示
      case 0xFF10EF: displayDigit(4); break;  // 4ボタン → 4を表示
      case 0xFF38C7: displayDigit(5); break;  // 5ボタン → 5を表示
      case 0xFF5AA5: displayDigit(6); break;  // 6ボタン → 6を表示
      case 0xFF42BD: displayDigit(7); break;  // 7ボタン → 7を表示
      case 0xFF4AB5: displayDigit(8); break;  // 8ボタン → 8を表示
      case 0xFF52AD: displayDigit(9); break;  // 9ボタン → 9を表示
      case 0xFFFFFFFF: break;     // リピート信号 → 何もしない
      default:                    // その他のボタン
        Serial.println("-> Other button");  // シリアルに表示
        lc.clearDisplay(0);       // マトリックスを消灯
        isDisplayOn = false;      // 表示中フラグをOFFに
        break;
    }
    
    irrecv.resume();              // 次のIR信号を受信可能にする
  }
}