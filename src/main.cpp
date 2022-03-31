#include <Arduino.h>
#include <blink.h>
#include <motor.h>
#include <Encoder.h>
#include <SoftwareSerial.h>
#include "config.h"

// Motor funcionando a lazo cerrado.


// En esta región se selecciona el puerto, asignando a cada una de las opciones el mismo nombre.
#if COM_SOFTWARESERIAL
  SoftwareSerial  PuertoCom(10, 11);		// Puerto Bluetooth.
#else
  HardwareSerial & PuertoCom = Serial;						// Puerto nativo
#endif
//static float ppr=200;

blink parpadeo(LED_BUILTIN);
motor motordc(PIN_ENABLE,PIN_IN1,PIN_IN2);
Encoder miEncoder(PIN_ENCODER_A,PIN_ENCODER_B);

void Telemetria(unsigned long interval);


void setup() {
  delay(2000);
  PuertoCom.begin(BAUDRATE);
  parpadeo.init();
  motordc.init();
  PuertoCom.flush();
  
}

void loop() {
  float Kp = 0.1;

  signals.r = 1000.0;  // pulsos
  signals.y = (float)miEncoder.read(); // pulsos
  signals.e = signals.r - signals.y;   // pulsos 
  signals.u = Kp * signals.e;
  int potencia = constrain((int) 255 / 9 * signals.u,-255,255);
  motordc.run(potencia);
  
  Telemetria(100);
  

  parpadeo.update(250);
    
}

void Telemetria(unsigned long interval)
{
  static bool primeraVez=true;

	static unsigned long previousMillis = 0;        // will store last time LED was updated
	//const long interval = 1000;           // interval at which to blink (milliseconds)
	unsigned long currentMillis = millis();
	static float yk_1=10;
  
	if (primeraVez)
  {
    PuertoCom.print("y e u");
    primeraVez=false;
  }

	if(currentMillis - previousMillis > interval) 
	{
		previousMillis = currentMillis;
    // cálculo de la velocidad.
    signals.v=(signals.y-yk_1)/(float)interval;
    yk_1=signals.y;

		PuertoCom.print(signals.y);
    PuertoCom.print("\t");
    //PuertoCom.print(signals.e);
    //PuertoCom.print("\t");
    //PuertoCom.print(signals.v);
    //PuertoCom.print("\t");
    //PuertoCom.print(signals.u);
    //PuertoCom.print("\t");
    PuertoCom.println();
	}
    
}