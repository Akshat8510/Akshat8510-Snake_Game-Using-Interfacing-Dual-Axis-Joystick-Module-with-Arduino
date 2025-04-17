#include <MD_MAX72xx.h>
#include <SPI.h>
#include <EEPROM.h>

#define MAX_DEVICES 4
#define CLK_PIN 10
#define DATA_PIN 12
#define CS_PIN 11
MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define JOY_X A3
#define JOY_Y A4
#define JOY_SW 2

#define WIDTH 32
#define HEIGHT 8

int snakeX[WIDTH * HEIGHT];
int snakeY[WIDTH * HEIGHT];
int snakeLength = 3;
int foodX, foodY;
int direction = 1;
int prevDirection = 1;
bool gameOver = false;
bool gameOverShown = false;
int gameSpeed = 250;
int foodCount = 0;
int highScore = 0;
const bool wallMode = false; // Always off

void setup() {
  Serial.begin(9600);
  matrix.begin();
  pinMode(JOY_SW, INPUT_PULLUP);

  EEPROM.get(0, highScore);
  if (highScore > WIDTH * HEIGHT - 1 || highScore < 0) highScore = 0;

  initSnake();
  spawnFood();

  Serial.println("🔥 Welcome to the Snake Game: cYBER PHYSICAL SYSTEM 🔥");
  Serial.print("📈 High Score: "); Serial.println(highScore);
  Serial.println("🎮 Use joystick. Don’t be a noob.");
  Serial.println("🧱 Wall Mode: OFF (You lucky slacker)");
}

void loop() {
  if (!gameOver) {
    handleJoystick();
    moveSnake();
    checkCollisions();
    render();
    delay(gameSpeed);
  } else if (!gameOverShown) {
    Serial.println("💀 You lost. Again. Classic.");
    Serial.print("🍖 Final Length: "); Serial.println(snakeLength);
    Serial.print("📈 High Score: "); Serial.println(highScore);
    Serial.println("🔄 Press Joystick to redeem yourself...");
    gameOverShown = true;
  } else {
    if (digitalRead(JOY_SW) == LOW) {
      delay(300);
      resetGame();
    }
  }
}

void handleJoystick() {
  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);
  if (xVal > 700 && prevDirection != 2) direction = 1;
  if (xVal < 300 && prevDirection != 1) direction = 2;
  if (yVal > 700 && prevDirection != 4) direction = 3;
  if (yVal < 300 && prevDirection != 3) direction = 4;
  prevDirection = direction;
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  switch (direction) {
    case 1: snakeX[0]++; break;
    case 2: snakeX[0]--; break;
    case 3: snakeY[0]--; break;
    case 4: snakeY[0]++; break;
  }
}

void checkCollisions() {
  // Wraparound logic (wallMode is false)
  if (snakeX[0] >= WIDTH) snakeX[0] = 0;
  if (snakeX[0] < 0) snakeX[0] = WIDTH - 1;
  if (snakeY[0] >= HEIGHT) snakeY[0] = 0;
  if (snakeY[0] < 0) snakeY[0] = HEIGHT - 1;

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      Serial.println("🧠 Cannibalism? Snake ate itself.");
      gameOver = true;
      return;
    }
  }

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    if (snakeLength < WIDTH * HEIGHT - 1) {
      snakeLength++;
      foodCount++;

      if (snakeLength > highScore) {
        highScore = snakeLength;
        EEPROM.put(0, highScore);
        Serial.println("🏆 New High Score!");
      }

      spawnFood();

      if (foodCount % 4 == 0 && gameSpeed > 100) {
        gameSpeed -= 10;
        Serial.print("⚡ Speeding up. Delay: ");
        Serial.println(gameSpeed);
      }
    }
  }
}

void spawnFood() {
  bool valid;
  int attempts = 0;
  do {
    foodX = random(WIDTH);
    foodY = random(HEIGHT);
    valid = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        valid = false;
        break;
      }
    }
    attempts++;
  } while (!valid && attempts < 100);

  Serial.print("🍎 Food spawned at: [");
  Serial.print(foodX); Serial.print(", "); Serial.print(foodY); Serial.println("]");
}

void render() {
  matrix.clear();
  for (int i = 0; i < snakeLength; i++) {
    int display = snakeX[i] / 8;
    int col = snakeX[i] % 8;
    int row = snakeY[i];
    matrix.setPoint(row, (display * 8) + col, true);
  }
  int fd = foodX / 8;
  int fc = foodX % 8;
  matrix.setPoint(foodY, (fd * 8) + fc, true);
}

void resetGame() {
  initSnake();
  direction = 1;
  prevDirection = 1;
  gameOver = false;
  gameOverShown = false;
  gameSpeed = 250;
  foodCount = 0;
  spawnFood();
  Serial.println("🔁 Game Reset. You get a second chance. Don’t waste it.");
}

void initSnake() {
  snakeLength = 3;
  snakeX[0] = 16;
  snakeY[0] = 4;
  for (int i = 1; i < snakeLength; i++) {
    snakeX[i] = snakeX[0] - i;
    snakeY[i] = snakeY[0];
  }
}
