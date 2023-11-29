#include <Arduino.h>
#include <InfrarotSensor>
#include <Schrittmotor>

// InfrarotSensor
#define PIN_TRIGGER 12
#define PIN_ECHO    A0
const double ballDurchmesser = 0.04; // Tischtennisball: 0.04; Tennisball: 0.067


// Schrittmotor
#define STEPS 200 // Aus Datenblatt entnehmen, Schrittanzahl für 360°
#define PIN_ENABLE 6
#define PIN_STEP 5
#define PIN_DIRECTION 4

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
  if (digitalRead(PIN_KEY)==LOW){
    Winkeleinstellung(STEPS, PIN_DIRECTION, PIN_STEP);
  }

  // Es wird nur abgeschossen, wenn der Schluessel auf on gedreht ist und der Knopf betaetigt wurde
  if (digitalRead(PIN_BUTTON) == LOW && digitalRead(PIN_KEY) == LOW){
    //digitalWrite(PIN_RELAIS,HIGH); //VentilBetaetigen oeffnen
    Serial.println("Ventil wird geoeffnet");

    // Messung der Geschwindigkeit
    Geschwindigkeitsmessung (PIN_TRIGGER, PIN_ECHO, ballDurchmesser);

    //digitalWrite(PIN_RELAIS,LOW); //VentilBetaetigen schliessen
    Serial.println("Ventil wird geschlossen");
  }
}