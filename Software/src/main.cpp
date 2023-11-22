#include <Arduino.h>
#include <UltraschallSensor>
#include <Schrittmotor>

#define PIN_TRIGGER 12
#define PIN_ECHO    13

unsigned long Geschwindigkeit; // in m/s
int Winkel = 30; // in °, Nullebene ist der Boden

#include <Stepper.h> 
#define STEPS 200 // Aus Datenblatt entnehmen, Schrittanzahl für 360°
Stepper stepper(STEPS, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  stepper.setSpeed(30); // in Umdrehungen/min
}

void loop() {  
  do{
  bool Ball_erkannt = Ballerkennung(PIN_TRIGGER, PIN_ECHO);
  Geschwindigkeit = Geschwindigkeitsmessung(Ball_erkannt); // m/s
  } while (Ball_erkannt==true);
  Winkeleinstellung(Winkel, stepper);
}

