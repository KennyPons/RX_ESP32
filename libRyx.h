#include "esp32-hal-gpio.h"
// Definir 
#define NUM_PIN_GROUP 12
//define pines
#define PIN_ENERGIA 22
#define PIN_ANODO 23
#define PIN_FOCO 15
#define LED_ERROR 13

const int relayPins[NUM_PIN_GROUP] = { 2, 4, 5, 14, 18, 19, 21, 25, 26, 27, 32, 33 }; //recordar que esto va de posicio 0 a 11

//Konstruktor
void InitialElectricalComponents(){
  pinMode(PIN_ENERGIA, OUTPUT);//salida
  pinMode(PIN_ANODO, OUTPUT);
  pinMode(PIN_FOCO, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  // Configurar pines de relé como salidas
  for (int i = 0; i < NUM_PIN_GROUP; i++) {
    pinMode(relayPins[i], OUTPUT);
  }
}

// Declarar métodos
void AnodeEinschalten(){//Turn on Rotating Anode
  digitalWrite(PIN_ANODO,1);
  delay(3000);
  digitalWrite(PIN_ANODO,0);
}

void alleRelaisDeaktivieren() {  //Disable Relays
  for (int r = 0; r < NUM_PIN_GROUP; r++) {
    digitalWrite(relayPins[r], 0);
  }
}

void encenderRAY() {
  digitalWrite(PIN_ENERGIA, HIGH);
}

void apagarRAY() {
  digitalWrite(PIN_ENERGIA, LOW);
}

void inSMALL() {
  digitalWrite(PIN_FOCO, LOW);
}

void inLARGE() {
  digitalWrite(PIN_FOCO, HIGH);
}

void EnableError(){
  digitalWrite(LED_ERROR, 1);
  delay(1000);
  digitalWrite(LED_ERROR, 0);
}

void NotAllowError(){
  digitalWrite(LED_ERROR, 0);
}

