/**
 * Ping Pong Game - 2 Players
 * 可変抵抗×2でパドル操作
 * ボタンでスタート/リトライ
 * 得点時にLED点灯
 * カウントダウン付き加速
 */

import processing.serial.*;

// ===== 定数 =====
final int STATE_START = 0;
final int STATE_PLAYING = 1;
final int STATE_GAMEOVER = 2;

final float BASE_SPEED = 4.0;
final float SPEED_INCREMENT = 0.3;
final int SPEED_UP_INTERVAL = 10000;
final int WINNING_SCORE = 5;

// 色設定
final color COLOR_BACKGROUND = #2d3436;
final color COLOR_BALL = #FFFF00;
final color COLOR_PADDLE1 = #FF6B6B;
final color COLOR_PADDLE2 = #74B9FF;
final color COLOR_TEXT = #FFFFFF;
final color COLOR_CENTER_LINE = #636e72;
final color COLOR_COUNTDOWN = #FF9F43;

// ===== グローバル変数 =====
Serial myPort;
int pot1Value = 512;
int pot2Value = 512;
int buttonPressed = 0;

int gameState = STATE_START;

Ball ball;
Paddle paddle1;
Paddle paddle2;

int score1 = 0;
int score2 = 0;
float speedMultiplier = 1.0;
int gameStartTime = 0;
int lastSpeedUpTime = 0;

int ledOnTime = 0;
final int LED_DURATION = 500;

// カウントダウン用
int lastCountdown = 0;

// ===== クラス定義 =====

class Ball {
  PVector location;
  PVector velocity;
  float size;
  
  Ball(float size) {
    this.size = size;
    this.location = new PVector();
    this.velocity = new PVector();
  }
  
  void reset(float x, float y) {
    location.set(x, y);
    
    float angle = random(PI / 6, PI / 3);
    
    if (random(1) > 0.5) {
      angle *= -1;
    }
    
    if (random(1) > 0.5) {
      angle += PI;
    }
    
    velocity.set(BASE_SPEED * cos(angle), BASE_SPEED * sin(angle));
  }
  
  void update(float multiplier) {
    PVector currentVelocity = velocity.copy();
    currentVelocity.mult(multiplier);
    location.add(currentVelocity);
  }
  
  void bounceX() {
    velocity.x *= -1;
  }
  
  void bounceY() {
    velocity.y *= -1;
  }
  
  void draw() {
    noStroke();
    fill(COLOR_BALL);
    ellipse(location.x, location.y, size, size);
  }
  
  float left()   { return location.x - size / 2; }
  float right()  { return location.x + size / 2; }
  float top()    { return location.y - size / 2; }
  float bottom() { return location.y + size / 2; }
}

class Paddle {
  float x, y;
  float w, h;
  color c;
  
  Paddle(float x, float w, float h, color c) {
    this.x = x;
    this.y = height / 2;
    this.w = w;
    this.h = h;
    this.c = c;
  }
  
  void updateY(int sensorValue) {
    y = map(sensorValue, 0, 1023, h / 2, height - h / 2);
  }
  
  void draw() {
    noStroke();
    rectMode(CENTER);
    fill(c);
    rect(x, y, w, h, 5);
  }
  
  boolean collidesWith(Ball ball) {
    return ball.right() > x - w / 2 &&
           ball.left() < x + w / 2 &&
           ball.location.y > y - h / 2 &&
           ball.location.y < y + h / 2;
  }
  
  float left()  { return x - w / 2; }
  float right() { return x + w / 2; }
}

// ===== メイン処理 =====

void setup() {
  size(1280, 720);
  initSerial();
  initGameObjects();
}

void initSerial() {
  printArray(Serial.list());
  String portName = "/dev/cu.usbmodem14401";
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void initGameObjects() {
  ball = new Ball(25);
  ball.reset(width / 2, height / 2);
  
  paddle1 = new Paddle(40, 20, 120, COLOR_PADDLE1);
  paddle2 = new Paddle(width - 40, 20, 120, COLOR_PADDLE2);
}

void draw() {
  background(COLOR_BACKGROUND);
  
  if (buttonPressed == 1) {
    if (gameState == STATE_PLAYING) {
      gameState = STATE_START;
      score1 = 0;
      score2 = 0;
    } else {
      startGame();
    }
    buttonPressed = 0;
  }
  
  if (ledOnTime > 0 && millis() - ledOnTime > LED_DURATION) {
    myPort.write('0');
    ledOnTime = 0;
  }
  
  switch (gameState) {
    case STATE_START:
      drawStartScreen();
      break;
    case STATE_PLAYING:
      updateGame();
      drawGame();
      break;
    case STATE_GAMEOVER:
      drawGameOverScreen();
      break;
  }
}

// ===== 画面描画 =====

void drawStartScreen() {
  fill(COLOR_TEXT);
  textAlign(CENTER, CENTER);
  textSize(72);
  text("PING PONG", width / 2, height / 3);
  
  textSize(24);
  fill(200);
  text("Player1: Left Potentiometer  |  Player2: Right Potentiometer", width / 2, height / 2 - 30);
  text("First to " + WINNING_SCORE + " wins!", width / 2, height / 2 + 10);
  
  textSize(32);
  fill(COLOR_TEXT);
  text("Press Button to Start", width / 2, height / 2 + 80);
  
  paddle1.updateY(pot1Value);
  paddle2.updateY(pot2Value);
  paddle1.draw();
  paddle2.draw();
}

void drawGameOverScreen() {
  String winner;
  color winnerColor;
  if (score1 >= WINNING_SCORE) {
    winner = "Player 1 Wins!";
    winnerColor = COLOR_PADDLE1;
  } else {
    winner = "Player 2 Wins!";
    winnerColor = COLOR_PADDLE2;
  }
  
  fill(winnerColor);
  textAlign(CENTER, CENTER);
  textSize(72);
  text(winner, width / 2, height / 3);
  
  fill(COLOR_TEXT);
  textSize(48);
  text(score1 + " - " + score2, width / 2, height / 2);
  
  textSize(32);
  text("Press Button to Retry", width / 2, height / 2 + 80);
}

void drawGame() {
  // センターライン
  stroke(COLOR_CENTER_LINE);
  strokeWeight(3);
  for (int y = 0; y < height; y += 30) {
    line(width / 2, y, width / 2, y + 15);
  }
  
  ball.draw();
  paddle1.draw();
  paddle2.draw();
  drawScore();
  drawTime();
  drawCountdown();
}

void drawScore() {
  fill(COLOR_TEXT);
  textSize(64);
  textAlign(CENTER, TOP);
  
  text(score1, width / 4, 20);
  text(score2, width * 3 / 4, 20);
}

void drawTime() {
  int gameTime = (millis() - gameStartTime) / 1000;
  int minutes = gameTime / 60;
  int seconds = gameTime % 60;
  
  fill(COLOR_TEXT);
  textSize(24);
  textAlign(CENTER, TOP);
  text(nf(minutes, 2) + ":" + nf(seconds, 2), width / 2, 20);
}

void drawCountdown() {
  int timeSinceSpeedUp = millis() - lastSpeedUpTime;
  int timeToNextSpeedUp = SPEED_UP_INTERVAL - timeSinceSpeedUp;
  int secondsLeft = timeToNextSpeedUp / 1000;
  
  // 残り3秒以下の時にカウントダウン表示
  if (secondsLeft <= 3 && secondsLeft >= 0) {
    fill(COLOR_COUNTDOWN);
    textSize(48);
    textAlign(CENTER, CENTER);
    text("SPEED UP in " + (secondsLeft + 1), width / 2, height - 50);
  }
}

// ===== ゲームロジック =====

void updateGame() {
  updateSpeed();
  paddle1.updateY(pot1Value);
  paddle2.updateY(pot2Value);
  ball.update(speedMultiplier);
  checkCollisions();
  checkScore();
}

void updateSpeed() {
  int timeSinceSpeedUp = millis() - lastSpeedUpTime;
  int timeToNextSpeedUp = SPEED_UP_INTERVAL - timeSinceSpeedUp;
  int secondsLeft = timeToNextSpeedUp / 1000;
  
  // カウントダウン音
  if (secondsLeft <= 3 && secondsLeft >= 0 && secondsLeft != lastCountdown) {
    lastCountdown = secondsLeft;
    if (secondsLeft == 2) {
      myPort.write('3');  // 3秒前
    } else if (secondsLeft == 1) {
      myPort.write('2');  // 2秒前
    } else if (secondsLeft == 0) {
      myPort.write('A');  // 1秒前
    }
  }
  
  // 加速
  if (timeSinceSpeedUp > SPEED_UP_INTERVAL) {
    speedMultiplier += SPEED_INCREMENT;
    lastSpeedUpTime = millis();
    lastCountdown = 10;  // リセット
    myPort.write('G');   // 加速音
  }
}

void checkCollisions() {
  if (ball.top() < 0 || ball.bottom() > height) {
    ball.bounceY();
    ball.location.y = constrain(ball.location.y, ball.size / 2, height - ball.size / 2);
  }
  
  if (ball.left() < paddle1.right() &&
      ball.location.x > paddle1.x &&
      ball.location.y > paddle1.y - paddle1.h / 2 &&
      ball.location.y < paddle1.y + paddle1.h / 2) {
    ball.bounceX();
    ball.location.x = paddle1.right() + ball.size / 2;
  }
  
  if (ball.right() > paddle2.left() &&
      ball.location.x < paddle2.x &&
      ball.location.y > paddle2.y - paddle2.h / 2 &&
      ball.location.y < paddle2.y + paddle2.h / 2) {
    ball.bounceX();
    ball.location.x = paddle2.left() - ball.size / 2;
  }
}

void checkScore() {
  if (ball.location.x < -ball.size) {
    score2++;
    onScore();
  }
  
  if (ball.location.x > width + ball.size) {
    score1++;
    onScore();
  }
}

void onScore() {
  myPort.write('1');
  ledOnTime = millis();
  
  myPort.write('S');
  
  if (score1 >= WINNING_SCORE || score2 >= WINNING_SCORE) {
    gameState = STATE_GAMEOVER;
    myPort.write('W');
  } else {
    ball.reset(width / 2, height / 2);
    speedMultiplier = 1.0;
    lastSpeedUpTime = millis();
    lastCountdown = 10;
  }
}

void startGame() {
  gameState = STATE_PLAYING;
  ball.reset(width / 2, height / 2);
  speedMultiplier = 1.0;
  gameStartTime = millis();
  lastSpeedUpTime = millis();
  lastCountdown = 10;
  score1 = 0;
  score2 = 0;
}

// ===== シリアル通信 =====

void serialEvent(Serial p) {
  String input = p.readStringUntil('\n');
  if (input != null) {
    input = trim(input);
    String[] values = split(input, ',');
    if (values.length == 3) {
      pot1Value = int(values[0]);
      pot2Value = int(values[1]);
      buttonPressed = int(values[2]);
    }
  }
}
