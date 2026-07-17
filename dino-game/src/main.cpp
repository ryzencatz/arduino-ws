#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// pushbutton
const int BUTTON = 8;

// for piezo
int notes[] = {262, 294, 330, 349};

// Game variables
int obstacleX = 15;
bool jumping = false;
int jumpTimer = 0;
bool gameOver = false;
long score = 0;

// Dinosaur character
byte dino[8] = {
  B00111,
  B00101,
  B00111,
  B00110,
  B01111,
  B10110,
  B00110,
  B01010
};

// Obstacle (cactus)
byte cactus[8] = {
  B00100,
  B10101,
  B10101,
  B10111,
  B00100,
  B00100,
  B00100,
  B00100
};

void resetGame()
{
  obstacleX = 15;
  jumping = false;
  jumpTimer = 0;
  score = 0;
  gameOver = false;
}

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("setup done");

  lcd.begin(16, 2);

  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);

  lcd.clear();
  lcd.print("Dino Game!");
  delay(2000);

  lcd.clear();
  lcd.print("press btn to start");
  while (digitalRead(BUTTON) == HIGH) {
    delay(100);
  } 

  resetGame();
}

void loop()
{
  // Restart after game over
  if (gameOver)
  {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("GAME OVER");

    lcd.setCursor(0,1);
    lcd.print("Btn=Restart");

    while (digitalRead(BUTTON) == HIGH);

    delay(200);

    resetGame();
    return;
  }

  // Jump
  if (!jumping && digitalRead(BUTTON) == LOW)
  {
    jumping = true;
    jumpTimer = 5;      // Jump duration
  }

  // Jump timer
  if (jumping)
  {
    jumpTimer--;

    if (jumpTimer <= 0)
      jumping = false;
  }

  // Move obstacle
  obstacleX--;

  if (obstacleX < 0)
  {
    obstacleX = 15;
    score++;
  }

  // Collision
  if (obstacleX == 0 && !jumping)
  {
    gameOver = true;
    return;
  }

  // Draw screen
  lcd.clear();

  // Draw dinosaur
  if (jumping)
  {
    lcd.setCursor(0,0);
    lcd.write(byte(0));
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.write(byte(0));
  }

  // Draw obstacle
  if (obstacleX >= 0 && obstacleX < 16)
  {
    lcd.setCursor(obstacleX,1);
    lcd.write(byte(1));
  }

  // Score
  lcd.setCursor(10,0);
  lcd.print(score);

  delay(120);
}