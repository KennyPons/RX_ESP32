enum Estado {
  APAGADO,
  ENCENDIDO,
  PRE,
  D_RX,
  FILAMENTO,
  RESETEO
};


String getDataSerial() {//recibir o enviar los datos seriales
  while (!Serial.available()) {
  }

  String inputSerial = Serial.readStringUntil('\n');
  inputSerial.trim();

  return inputSerial;
}

enum Estado getSystemStatus(String command) {// enviar un dato de tipo enum Estado 

  Serial.readStringUntil('\n');
  command.trim();
  if (command.equalsIgnoreCase("PRE")) {
    Serial.println("Haciendo Pre...");
    return PRE;
  } else if (command.equalsIgnoreCase("D_RX")) {
    Serial.println("Haciendo Rayos X...");
    return D_RX;
  }else if (command.equalsIgnoreCase("FILAMENTO")) {
    Serial.println("Cambiar filamento...");
    return FILAMENTO;
  }else if (command.equalsIgnoreCase("RESETEO")) {
    Serial.println("Reseteo Realizado...");
    return RESETEO;
  }else{
    Serial.println("NIN:APAGANDO!...");
    return APAGADO;
  }
}
