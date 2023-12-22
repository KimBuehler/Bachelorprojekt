#include <Arduino.h>
#include <InfrarotSensor>
#include <Schrittmotor>

// InfrarotSensor
#define PIN_TRIGGER 13 // Ein-/Ausschalten der Infrarot-LED
#define PIN_SIGNAL  A0 // Input des Infrarot-Sensor-Signals (analog, bei großer Differenz kann auch auf digital umgestellt werden)
const double ballDurchmesser = 0.064; // Tischtennisball: 0.04; Tennisball: 0.067


// Schrittmotor
#define STEPS 800 // Aus Datenblatt entnehmen, Schrittanzahl für 360°, in diesem Fall Schrittanzahl an Abschusswelle, bzw. nach Uebersetzung
#define PIN_ENABLE 6 
#define PIN_STEP 5 // Anzahl der zu verfahrenden Schritte
#define PIN_DIRECTION 4 // Einstellung der Drehrichtung des Motors (1 = Uhrzeigersinn, 0 = gegen Uhrzeigersinn), somit ist nach der Uebersetzung die Drehrichtung entgegengesetzt
#define PIN_M1 10 // mögliche Einstellung der Schrittweite, aktuell ungenutzt, da fullsteps genutzt werden
#define PIN_M2 11 // mögliche Einstellung der Schrittweite, aktuell ungenutzt, da fullsteps genutzt werden
#define PIN_M3 12 // mögliche Einstellung der Schrittweite, aktuell ungenutzt, da fullsteps genutzt werden

// Relais
#define PIN_RELAIS_Ventil 2 // Ansteuerung des Ventils ueber ein Relais (ini 1)
#define PIN_RELAIS_LAMPE 3 // Ansteuerung der Lampe im Knopf ueber ein Relais (ini 2)

// Schalter
#define PIN_BUTTON 7 // Input, ob Knopf betaetigt
#define PIN_KEY 8 // Input, ueber Schluesselposition
bool WinkeleinstellungVornehmen = false; // gibt an, ob eine Winkeleinstellung folgt oder nicht
int Knopf_per_bluetooth_betaetigt = 0; // 0 = nicht betaetigt, 1 = betaetigt


void setup() {
  Serial.begin(9600);

  // Einrichten Infrarot-Pins
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_SIGNAL, INPUT);
  
  // Einrichten Motor-Pins
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_STEP,OUTPUT); 
  pinMode(PIN_DIRECTION, OUTPUT); 
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_M2, OUTPUT);
  pinMode(PIN_M3, OUTPUT);

  digitalWrite(PIN_ENABLE, LOW);

  // Einrichten Ventil-Pin
  pinMode(PIN_RELAIS_Ventil, OUTPUT);
  digitalWrite(PIN_RELAIS_Ventil,HIGH); //auf HIGH ist das Ventil geschlossen

  // Einrichten Lampen-Pin
  pinMode(PIN_RELAIS_LAMPE, OUTPUT);
  digitalWrite(PIN_RELAIS_LAMPE,HIGH); // auf HIGH ist die Lampe aus

  // Einrichten der Schalter
  pinMode(PIN_KEY, INPUT_PULLUP);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

}

void loop() { 
  //Wenn der Schluessel auf off gedreht ist
  if(digitalRead(PIN_KEY)==HIGH){ 
    
    WinkeleinstellungVornehmen = true; // Zuruecksetzen, sodass bei Einschalten wieder Winkelabfrage kommt 
    digitalWrite(PIN_RELAIS_LAMPE,HIGH); // Abschalten der Lampe
    digitalWrite(PIN_TRIGGER, LOW); // Abschalten des Infrarotsensors
  }

  // Programm wird nur ausgefuehrt, wenn der Schluessel gerade erst auf on gedreht wurde
  // keine Funktion, sollte der Schluessel beim Einschalten schon auf on gedreht sein
  if (digitalRead(PIN_KEY)==LOW && WinkeleinstellungVornehmen == true ){
    
    WinkeleinstellungVornehmen = false;

    //Einstellung des gewuenschten Abschusswinkels
    Winkeleinstellung(STEPS, PIN_DIRECTION, PIN_STEP);
    
    digitalWrite(PIN_TRIGGER, HIGH); // Einschalten des Infrarot-Senders    
    digitalWrite(PIN_RELAIS_LAMPE,LOW); // Einschalten der Lampe
    delay (1000);
  }

  // Es wird nur abgeschossen, wenn der Schluessel auf on gedreht ist und der Knopf (haendisch oder per bluetooth) betaetigt wurde
  if(digitalRead(PIN_KEY) == LOW){
  // Abfrage, ob per Bluetooth Signal fuer Abschuss kommt    
    Knopf_per_bluetooth_betaetigt = bluetooth("Knopf");

    if (digitalRead(PIN_BUTTON) == LOW || Knopf_per_bluetooth_betaetigt == 1){
      Knopf_per_bluetooth_betaetigt == 0;
      digitalWrite(PIN_RELAIS_Ventil,LOW); //Ventil oeffnen

      // Messung der Geschwindigkeit
      Geschwindigkeitsmessung ( PIN_SIGNAL, ballDurchmesser);

      digitalWrite(PIN_RELAIS_Ventil,HIGH); //Ventil schliessen
    }
  }
}