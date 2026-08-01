#include <Arduino.h>

// put function declarations here:
int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

const int LEDPIN = 13;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (millis() < 5000) {
    sensorValue = analogRead(A0);
    if (sensorValue < sensorLow) {sensorLow = sensorValue;}
    if (sensorValue > sensorHigh) {sensorHigh = sensorValue;}
  }

  Serial.print("sensor low:");
  Serial.print(sensorLow);
  Serial.print("sensor high:");
  Serial.print(sensorHigh);

  digitalWrite(LEDPIN, LOW);
}

void loop() {
  sensorValue = analogRead(A0);

  int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 4000);
  tone(8, pitch);

  delay(10); // so we're telling the piezo to play for 20 ms earlier but we're only giving it 10 ms here? 
}

// put function definitions here:
