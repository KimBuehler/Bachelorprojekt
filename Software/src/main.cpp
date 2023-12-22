#include <Arduino.h>
#include <InfrarotSensor>
#include <Schrittmotor>

// InfrarotSensor
#define PIN_TRIGGER 13 // ein-/ausschalten des Infrarot-Sensors
#define PIN_SIGNAL  A0 // Input des Infrarot-Sensor-Signals (digital)
const double ballDurchmesser = 0.052; // Tischtennisball: 0.04; Tennisball: 0.067


// Schrittmotor
#define STEPS 800 // Aus Datenblatt entnehmen, Schrittanzahl für 360°
#define PIN_ENABLE 6 
#define PIN_STEP 5 // Anzahl der zu verfahrenden Schritte
#define PIN_DIRECTION 4 // Einstellung der Drehrichtung (1 = Uhrzeigersinn, 0 = gegen Uhrzeigersinn)
#define PIN_M1 10 // mögliche Einstellung der Schrittweite
#define PIN_M2 11 // mögliche Einstellung der Schrittweite
#define PIN_M3 12 // mögliche Einstellung der Schrittweite

// Relais
#define PIN_RELAIS_Ventil 2 // Ansteuerung des Ventils ueber ein Relais
#define PIN_RELAIS_LAMPE 3 // Ansteuerung der Lampe im Knopf ueber ein Relais

// Schalter
#define PIN_BUTTON 7 // Input, ob Knopf betaetigt
#define PIN_KEY 8 // Input, ueber Schluesselposition
bool Schluessel_wurde_gerade_erst_on = false; // gibt an, ob eine Winkeleinstellung folgt oder nicht
int Knopf_per_bluetooth = 0; // 0 = nicht betaetigt, 1 = betaetigt


void setup() {
  Serial.begin(9600);

  // Einrichten Infrarot-Pins
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_SIGNAL, INPUT);
  digitalWrite(PIN_TRIGGER, HIGH);
  
  // Einrichten Motor-Pins
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_STEP,OUTPUT); 
  pinMode(PIN_DIRECTION, OUTPUT); 

  digitalWrite(PIN_ENABLE, LOW);

  // Einrichten Ventil-Pin
  pinMode(PIN_RELAIS_Ventil, OUTPUT);
  digitalWrite(PIN_RELAIS_Ventil,HIGH);

  // EInrichten Lampen-Pin
  pinMode(PIN_RELAIS_LAMPE, OUTPUT);
  digitalWrite(PIN_RELAIS_LAMPE,HIGH);

  // Einrichten der Schalter
  pinMode(PIN_KEY, INPUT_PULLUP);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

}

void loop() { 

  if(digitalRead(PIN_KEY)==HIGH){
    
    Schluessel_wurde_gerade_erst_on = true; // Zuruecksetzen, sodass bei Einschalten wieder Winkelabfrage kommt
    digitalWrite(PIN_RELAIS_LAMPE,HIGH); // Abschalten der Lampe
    //digitalWrite(PIN_TRIGGER, LOW); // Abschalten des Infrarotsensors
  }

  // Programm wird nur ausgefuehrt, wenn der Schluessel gerade erst auf on gedreht wurde
  // keine Funktion, sollte der Schluessel beim Einschalten schon auf on gedreht sein
  if (digitalRead(PIN_KEY)==LOW && Schluessel_wurde_gerade_erst_on == true ){
    
    Schluessel_wurde_gerade_erst_on = false;

    digitalWrite(PIN_TRIGGER, HIGH); // Einschalten des Infrarot-Senders
    digitalWrite(PIN_RELAIS_LAMPE,LOW); // Einschalten der Lampe

    //Einstellung des gewuenschten Abschusswinkels
    Winkeleinstellung(STEPS, PIN_DIRECTION, PIN_STEP);
  }

  // Es wird nur abgeschossen, wenn der Schluessel auf on gedreht ist und der Knopf betaetigt wurde
  if(digitalRead(PIN_KEY) == LOW){
  // Abfrage, ob per Bluetooth Signal fuer Abschuss kommt    
    Knopf_per_bluetooth = bluetooth("Knopf");

    if (digitalRead(PIN_BUTTON) == LOW || Knopf_per_bluetooth== 1){
      
      digitalWrite(PIN_RELAIS_Ventil,LOW); //Ventil betaetigen, oeffnen

      // Messung der Geschwindigkeit
      Geschwindigkeitsmessung ( PIN_SIGNAL, ballDurchmesser);

      digitalWrite(PIN_RELAIS_Ventil,HIGH); //Ventil betaetigen, schliessen
    }
  }
}