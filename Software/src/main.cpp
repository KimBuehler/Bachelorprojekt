#include <Arduino.h>
#include <UltraschallSensor>

#define PIN_TRIGGER 12
#define PIN_ECHO    13
double Geschwindigkeit;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop() {  
  bool Ball_erkannt = Ballerkennung(PIN_TRIGGER, PIN_ECHO);
  Geschwindigkeit = Geschwindigkeitsmessung(Ball_erkannt);
}

