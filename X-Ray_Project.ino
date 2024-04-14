#include "libRyx.h"
#include "libToolsRx.h"
#include "libParametrica.h"

// Variables globales
Estado estadoActual = APAGADO;  // Estado inicial apagado


int tiempo = 0, Spannung = 0, indexReleNow[2];
bool preActivo = false;
String comando = "";
ToolsRX _toolRx;

void setup() {
  Serial.begin(115200);
  InitialElectricalComponents();
  inSMALL();
  NotAllowError();
}

void loop() {  //es un bucle


  switch (estadoActual) {
    case APAGADO:
      AufEinschaltungWarten();  
      break;
    case ENCENDIDO:
      Operationen_Durchfuhren(); // Choose which operation to perform
      break;
    case PRE:
      Operation_Vorbereiten();
      break;
    case D_RX:
      Rontgen_Aufnahme_Durchfuhren();
      break;
  }

}  //fin de loop



//===================================================================================================================///
void AufEinschaltungWarten() {  //Wait for Power On
  comando = getDataSerial();
  if (comando.equalsIgnoreCase("ON")) {
      estadoActual = ENCENDIDO;
      encenderRAY();
      Serial.println("Sistema encendido.");
    } else if (comando.equalsIgnoreCase("OFF")) {
      estadoActual = APAGADO;  //
      apagarRAY();
      Serial.println("Sistema apagado.");
      return;
    }
} 

void Operationen_Durchfuhren() {  // While in power-on mode, it requests which operation to perform - FIRST OPERATION
  comando = getDataSerial();
  estadoActual = getSystemStatus(comando);
}

void Operation_Vorbereiten() {  //PRE - SECOND OPERATION

  String getRayFactors = getDataSerial();// Monitor en serie obteniendo los factores en cadena de texto - t1000v100
   
  if(getRayFactors != NULL){
    int indexKv = getRayFactors.indexOf('v');//obtiene el index de la letra v

    String input_mAs = getRayFactors.substring( 0, indexKv); //Toma el String de intervalos 0 hasta - v

    tiempo = _toolRx.ZeitErhalten(input_mAs);
    Serial.println("tengo tiempo: " + String(tiempo));

    //==================================================================//
    String input_Kv = getRayFactors.substring(indexKv);

    Spannung = _toolRx.SpannungErhalten(input_Kv);

    Serial.println("tengo Voltaje: " + String(Spannung));
  }

  if(Spannung !=0 ){
    /*Turn On the First Rele*/
    _toolRx.IndexAuswahlenKv(Spannung, indexReleNow);//obtiene con el valor deseado el voltaje de entrada
    OnlyActivateRelais(indexReleNow[0],'a');
  }else{
    EnableError();
    Serial.println("Error en voltaje, no existe! "+String(Spannung));
  }
  
  

  comando =  getDataSerial();
  estadoActual = getSystemStatus(comando);//D_RX
}

void Rontgen_Aufnahme_Durchfuhren() {  //Performing X-ray Shot - Third Operation

  if (tiempo != 0) {
    /*Turn On the Second Rele*/
    OnlyActivateRelais(indexReleNow[1],'b');
    delay(tiempo);
    alleRelaisDeaktivieren();//desactiva todos los realys
  } else {
    Serial.println("No hay tiempo XD: "+String(tiempo));
    EnableError();
  }

  estadoActual = ENCENDIDO;
}

void RontgenZurucksetzen() {  //Restart X-Ray System - FOURTH OPERATION
  alleRelaisDeaktivieren();//desactiva todos los realys
  apagarRAY();
  delay(500);
  encenderRAY();

  estadoActual = ENCENDIDO;
}

//============================================================================================================//


void OnlyActivateRelais(int indexR,char e){ //Activating the first and second Rele
  if(indexR != -1){
    if(e == 'a'){
      Serial.print("Índice en Grupo A: ");
      Serial.println(String(indexR));
      Serial.println("Esta Prendiendo el primer Relay: "+String(relayPins[indexR]));
      digitalWrite(relayPins[indexR], HIGH);
    }else{
      int indexRB = indexR + 6; // se suma 6 para encajar con el numero de posicion que se encuentra el segundo grupo de relays
      Serial.print("Índice en Grupo B: ");
      Serial.println(String(indexRB));
      Serial.println("Esta Prendiendo el segundo Relay: "+String(relayPins[indexRB]));
      digitalWrite(relayPins[indexRB], HIGH);
    }
  }else{
    EnableError();
    String Error21 = "Es indice?: " + String(indexR) + " Or characteres: " + String(e);
    Serial.println("Aqui en prender Relays existe un Error cual es?: "+Error21);
  }
}

void selectOperation(Estado statusNow){
  switch (statusNow) {
    case PRE: AnodeEinschalten(); break;
    case FILAMENTO: FilamentWechseln();break;
    case RESETEO: RontgenZurucksetzen();break;
  }
}

void FilamentWechseln(){// Making 
  comando = getDataSerial();

  if(comando.equalsIgnoreCase("LARGE")){
    inLARGE();
  }else{
    inSMALL();
  }
  estadoActual = ENCENDIDO;
}
