#include <Arduino.h>
#include <UltraschallSensor>
#include <Schrittmotor>
#include <Stepper.h> 

// UltraschallSensor
#define PIN_TRIGGER 12
#define PIN_ECHO    13
unsigned long Geschwindigkeit; // in m/s

// Schrittmotor
#define STEPS 200 // Aus Datenblatt entnehmen, Schrittanzahl für 360°
int Winkel = 30; // in °, Nullebene ist der Boden

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  //Einrichten Motor-Pins
  pinMode(6, OUTPUT); //Enable
  pinMode(5,OUTPUT); //Step
  pinMode(4, OUTPUT); //direction
  digitalWrite(6, LOW);

}

void loop() { 
  digitalWrite(4, LOW);
    for(int stepCounter = 0; stepCounter < 2000; stepCounter++) {
      digitalWrite(5,HIGH);
    delayMicroseconds(5000);
    digitalWrite(5,LOW);
    delayMicroseconds(5000);
    }
delay(1000);
  //Winkeleinstellung(Winkel);
  //Serial.println("Eingestellter Winkel: " + Winkel);
  Winkel = Winkelueberpruefung();
  //Serial.println("Vorhandener Winkl: " + Winkel); 
  do{
  bool Ball_erkannt = Ballerkennung(PIN_TRIGGER, PIN_ECHO);
  //Serial.println(Ball_erkannt);
  Geschwindigkeit = Geschwindigkeitsmessung(Ball_erkannt); // m/s
  } while (Ball_erkannt==true);
  //Serial.println(Geschwindigkeit);
}

