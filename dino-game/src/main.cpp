#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// pushbutton
const int BUTTON = 8;

// piezo speaker
const int BUZZER = 7;

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

// ---------- Sound effects ----------
// These use tone()/noTone() which is non-blocking-ish but we add small
// delay()s only where the game loop was already delaying, so gameplay
// timing isn't affected.

void sfxStart()
{
  // Cheerful ascending arcade "ready" jingle
  int notes[]     = {523, 659, 784, 1047}; // C5 E5 G5 C6
  int durations[] = {100, 100, 100, 200};
  for (int i = 0; i < 4; i++)
  {
    tone(BUZZER, notes[i], durations[i]);
    delay(durations[i] + 20);
  }
  noTone(BUZZER);
}

void sfxJump()
{
  // Quick upward blip, classic "jump" sound
  tone(BUZZER, 1200, 60);
}

void sfxLand()
{
  // Short low blip on landing
  tone(BUZZER, 400, 40);
}

void sfxGameOver()
{
  // Descending "game over" arcade sting
  int notes[]     = {392, 349, 294, 196}; // G4 F4 D4 G3
  int durations[] = {150, 150, 150, 300};
  for (int i = 0; i < 4; i++)
  {
    tone(BUZZER, notes[i], durations[i]);
    delay(durations[i] + 20);
  }
  noTone(BUZZER);
}

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
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
  Serial.println("setup done");

  lcd.begin(16, 2);

  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);

  lcd.clear();
  lcd.print("Dino Game!");
  delay(2000);

  lcd.clear();
  lcd.print("btn to start");
  while (digitalRead(BUTTON) == HIGH);

  sfxStart();

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

    sfxStart();

    resetGame();
    return;
  }

  // Jump
  if (!jumping && digitalRead(BUTTON) == LOW)
  {
    jumping = true;
    jumpTimer = 5;      // Jump duration
    sfxJump();
  }

  // Jump timer
  if (jumping)
  {
    jumpTimer--;

    if (jumpTimer <= 0)
    {
      jumping = false;
      sfxLand();
    }
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
    sfxGameOver();
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