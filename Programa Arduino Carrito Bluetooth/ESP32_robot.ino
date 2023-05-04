/*
 * Carrito con ESP32 con manejo por Bluetooth
 * por: Miguel Angel Peralta Martinez
 * Fecha: 20 de octubre de 2022
 * 
 * Este programa nos ayudará a poder controlar el carrito a través de Bluetooth
 * con ayuda del microcontrolador ESP32 ya que por defecto este microcontrolador
 * cuenta con Wifi y Bluetooth incluidos. */
#include "BluetoothSerial.h" // Libreria para utilizar el Bluetooth del ESP32 

 /* Definimos las salidas del ESP32 que controlan los motores y el LED para notar la 
  regulacion de la señal de la Modulación por Ancho de Pulso (PWM)*/
#define salida1 16
#define salida2 17
#define salida3 19
#define salida4 23
#define salida5 32

//Confirmación de que podemos utilizar el bluetooth del ESP32 

#if !defined (CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig´  to and enable it
#endif

BluetoothSerial SerialBT;  //Declaración de Bluetooth
String completo;
String orientacion;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("Robot ESP32"); // Este será el nombre del bluetooth
  pinMode(salida1, OUTPUT);
  pinMode(salida2, OUTPUT);
  pinMode(salida3, OUTPUT);
  pinMode(salida4, OUTPUT);

  //Salida PWM
  ledcSetup(0,5000,8);
  ledcAttachPin(salida5,0);

}

void loop() {
 if(SerialBT.available()){    //Con el if limpiaremos la variable que utilizaremos para interpterar la infromación que proviene de la aplicación
  completo = "";
 }
while(SerialBT.available()){  // Con el while leemos la información proveniente de la aplicación
  char dato = SerialBT.read();
  if (dato == '.'){
    if (completo.length()<5){
      orientacion = completo.substring(0,1);
      completo = completo.substring(1, completo.length());
      if(orientacion == "e"){
        Serial.println("Adelante: " + completo);  // Al mover el joystick en la aplicacion móvil hacia adelante, el carrito comenzará a moverse hacia adelante.
        digitalWrite(salida1, 1);
        digitalWrite(salida2, 0);
        digitalWrite(salida3, 1);
        digitalWrite(salida4, 0);
      }
      if (orientacion == "a"){
        Serial.println("Atrás: " + completo);  // Al mover el joystick en la aplicación móvil hacia atrás, el carrito comenzará a moverse hacia atrás.
        digitalWrite(salida1,0);
        digitalWrite(salida2,1);
        digitalWrite(salida3,0);
        digitalWrite(salida4,1);
      }
      if (orientacion ==  "d"){
        Serial.println("Derecha: " + completo); // Al mover el joystick en la aplicación móvil hacia la derecha, el carrito comenzará a moverse hacia la derecha.
        digitalWrite(salida1,1);
        digitalWrite(salida2,0);
        digitalWrite(salida3,0);
        digitalWrite(salida4,1);
      }
      if (orientacion == "i"){
        Serial.println("Izquierda: " + completo);  // Al mover el joystick en la aplicación móvil hacia la izquierda, el carrito comenzará a moverse hacia izquierda.
        digitalWrite(salida1,0);
        digitalWrite(salida2,1);
        digitalWrite(salida3,1);
        digitalWrite(salida4,0);
      }
      if (completo.toInt() <= 0){  // toInt Convierte una cadena válida en un entero. La cadena de entrada debe comenzar con un número entero, si la cadena contiene números no enteros, la función dejará de realizar la conversión.
        digitalWrite(salida1,0);
        digitalWrite(salida2,0);
        digitalWrite(salida3,0);
        digitalWrite(salida4,0); 
      }
      ledcWrite(0, completo.toInt());
    }
    delay(50);
    break;
  }else{
    completo +=dato;
  } 
}
}
