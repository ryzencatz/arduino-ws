#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// distance sensor
const int TRIG = 9; // trigger pulse input
const int ECHO = 10; // echo pulse output
int duration;
int distance;

// LCD display

void setup() {
  pinMode(TRIG, OUTPUT); // input to sensor is output from us
  pinMode(ECHO, INPUT); // output from sensor is input to us

  digitalWrite(TRIG, LOW);

  Serial.begin(115200);

  // initialize LCD
  lcd.begin(16,2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Distance Sensor");
  delay(1000);
}

void loop() {
  // send ultrasonic pulse
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // measure echo time
  duration = pulseIn(ECHO, HIGH);

  // convert time to cm
  distance = duration / 58;

  lcd.clear();

  if (duration >= 38000 || distance > 400) {
    lcd.setCursor(0, 0);
    lcd.print("Out of range");
    
    Serial.println("Out of range");

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Distance:");

    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print(" cm");

    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(300);
}