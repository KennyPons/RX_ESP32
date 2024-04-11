#ifndef TOOLS_RX_H
#define TOOLS_RX_H

class ToolsRX {
private:
  int diferenciaMinima;
  static const int volALength = 6;
  static const int volBLength = 6;
  int gpVoltageA[volALength] = { 0, 6, 12, 18, 24, 30 }; //0  a 5 index
  int gpVoltageB[volBLength] = { 36, 90, 130, 170, 210, 250 }; // 0 a 5 = index + 6

public:
  ToolsRX() { // Corregido el nombre del constructor a ToolsRX
    diferenciaMinima = 99999;
  }
  

  int* IndexAuswahlenKv(int valorDeseado) {
    static int selectedRelays[2];  // Array para almacenar los índices de los relés seleccionados

    int indiceA = -1;
    int indiceB = -1;
    if(valorDeseado == 0){
      Serial.print("Aqui hay un error el valor esta en 0 o = "+String(valorDeseado));
    }
    // Calcular las restas y encontrar el resultado más cercano al valor deseado
    for (int i = 0; i < volALength; i++) {
      for (int j = 0; j < volBLength; j++) {
        int resultado = gpVoltageB[j] - gpVoltageA[i];
        if (resultado <= valorDeseado) {
          int diferencia = valorDeseado - resultado;
          if (diferencia < diferenciaMinima) {
            diferenciaMinima = diferencia;
            indiceA = i;
            indiceB = j;
          }
        }
      }
    }

    selectedRelays[0] = indiceA;
    selectedRelays[1] = indiceB;

    return selectedRelays;
  }

  int ZeitErhalten(String zeit) {//enviar mAs para el tiempo de toma
    int duration = 0;
    if (zeit.startsWith("t")) {
      duration = zeit.substring(1).toInt();//toma la segunda letra y lo convierte en entero
      if (duration >= 0 && duration <= 3999) {
        return duration;
      }
    }
    return duration;
  }

  int SpannungErhalten(String Spannung) {//enviar voltaje de entrada para Kv
    int voltage = 0;
    if (Spannung.startsWith("v")) {
      voltage = Spannung.substring(1).toInt();
      if (voltage >= 0 && voltage <= 250) {
        return voltage;
      }
    }

    return voltage;
  }

}; //end class

#endif
