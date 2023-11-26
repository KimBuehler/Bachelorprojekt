#include <Arduino.h>
#include <Ultraschall_Sensor>
#include <Schrittmotor>
#include <Ventil>

// UltraschallSensor
#define PIN_TRIGGER 12
#define PIN_ECHO    13
unsigned long Geschwindigkeit; // in m/s

// Schrittmotor
#define STEPS 2000 // Aus Datenblatt entnehmen, Schrittanzahl für 360°
#define PIN_ENABLE 6
#define PIN_STEP 5
#define PIN_DIRECTION 4
int Winkel = 30; // in °, Nullebene ist parallel zum Boden

// Ventil
#define PIN_RELAIS 9

// Schalter
#define PIN_BUTTON 7
#define PIN_KEY 8


void setup() {
  Serial.begin(9600);

  // Einrichten Ultraschallsensor-Pins
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // Einrichten Motor-Pins
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_STEP,OUTPUT); 
  pinMode(PIN_DIRECTION, OUTPUT); 

  digitalWrite(PIN_ENABLE, LOW);

  // Einrichten Ventil-Pin
  pinMode(PIN_RELAIS, OUTPUT);

  // Einrichten der Schalter
  pinMode(PIN_KEY, INPUT_PULLUP);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

}

void loop() { 
  // Programm wird nur ausgefuehrt, wenn der Schluessel auf on gedreht wurde
  if (digitalRead(PIN_KEY)==HIGH){
  Winkeleinstellung(Winkel, STEPS, PIN_DIRECTION, PIN_STEP);
  Serial.println("Eingestellter Winkel: " + Winkel);
  Winkel = Winkelueberpruefung();
  Serial.println("Vorhandener Winkel: " + Winkel); 
  if (digitalRead(PIN_BUTTON)== HIGH){
  //VentilBetaetigen (PIN_RELAIS);
  Serial.println("Ventil wird betaetigt");

  // Messung der Geschwindigkeit
  Geschwindigkeitsmessung (PIN_TRIGGER, PIN_ECHO);
}}}

