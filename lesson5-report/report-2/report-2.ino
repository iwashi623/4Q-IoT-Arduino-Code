#include <Wire.h>                       // I2C通信ライブラリを読み込む
#include <Servo.h>                      // サーボモーター制御ライブラリを読み込む

const int MPU_addr = 0x68;              // MPU-6050のI2Cアドレス
const int MPU_ACCEL_XOUT_H = 0x3B;      // 加速度X軸データのレジスタアドレス
const int MPU_PWR_MGMT_1 = 0x6B;        // 電源管理レジスタのアドレス
const int16_t GRAVITY = 16000;          // 1G（重力加速度）に対応する値
const int SERVO_PIN = 9;                // サーボを接続するピン番号

Servo myservo;                          // サーボを制御するオブジェクトを作成

void setupGyro() {                      // ジャイロセンサ初期化関数
  Wire.begin();                         // I2C通信を開始
  Wire.beginTransmission(MPU_addr);     // MPU-6050との通信を開始
  Wire.write(MPU_PWR_MGMT_1);           // 電源管理レジスタのアドレスを指定
  Wire.write(0);                        // 0を書き込んでスリープモードを解除
  Wire.endTransmission(true);           // 通信を終了
}

void setupServo() {                     // サーボ初期化関数
  myservo.attach(SERVO_PIN);            // サーボをピン9に接続
}

void setup() {                          // 初期設定（起動時に1回だけ実行）
  setupGyro();                          // ジャイロセンサを初期化
  setupServo();                         // サーボを初期化
  Serial.begin(9600);                   // シリアル通信を9600bpsで開始
}

int16_t getX() {                        // X軸の加速度を取得する関数
  Wire.beginTransmission(MPU_addr);     // MPU-6050との通信を開始
  Wire.write(MPU_ACCEL_XOUT_H);         // 加速度X軸のレジスタアドレスを指定
  Wire.endTransmission(false);          // 通信を維持したまま送信完了
  Wire.requestFrom(MPU_addr, 2, true);  // 2バイトのデータを要求
  
  return Wire.read() << 8 | Wire.read(); // 上位・下位バイトを結合して16ビット値を返す
}

void loop() {                           // メインループ（繰り返し実行）
  int angle = map(getX(), -GRAVITY, GRAVITY, 0, 180);  // 傾き（-16000〜16000）を角度（0〜180）に変換
  angle = constrain(angle, 0, 180);     // 範囲外なら0か180に制限（サーボ保護）
  
  myservo.write(angle);                 // サーボを指定角度に動かす
  
  delay(50);                            // 50ミリ秒待機
}