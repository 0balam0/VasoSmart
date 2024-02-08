#include "LowPowerObj.h"
#include "SchermoLib.h"
#include <DHT22.h>

/* Define PIN */
#define wakeUpPIN 3

#define tankSensorPIN 8 // il galleggiante

#define servoPin 6 // è il servo il tank indicator

#define rele_aux_PIN 4    //Ausiliari sensori e pulsanti
#define rele_pompa_PIN 5  //Rele pompa
#define rele_pw_PIN 9     //Pin di alimentazione dei rele 

#define plantHum_PIN A1 //sensore di umidità della pianta

#define umdSensPin 2 //sensore di umidità/temperatura ambiente

My_WakeUp wakeUPobj(wakeUpPIN, 40, 6000); // inizializza il pulsante di Wakeup nel pin wakeUpPIN, va in sleep per 40s ogni 6000ms
myServo tankIndicator(servoPin, 100, 10); // pin a cui è collegato il Servo, gradi quando il serbatoio è vuoto, gradi quando il serbatoio è pieno.
My_Screen myLCD;
MultiRele myRele(rele_pw_PIN, rele_aux_PIN, rele_pompa_PIN);
DHT22 dht22(umdSensPin);

void setup() {
  Serial.begin(9600);
  Serial.println("Ciao!");

  myRele.begin(); // inizializza la scheda rele
  myRele.auxStart(); // accende gli ausiliari

  wakeUPobj.begin(); // inizializzo wake up controllerà quando è tempo di andare a nanna
  tankIndicator.begin(); // inizializzo ogetto tankindicator è il servo
  myLCD.begin("III B"); // inizializzo lo schermo e gli salvo il nome della classe
  pinMode(tankSensorPIN, INPUT); // tankSensorPIN è un interruttore
  pinMode(plantHum_PIN, INPUT);
  pinMode(7, OUTPUT);
  myLCD.InitDisplay(); // accende lo schermo
  delay(200);
  myLCD.printBaseScreen(); // stampa la struttura base
  digitalWrite(7, HIGH);
}

void loop() {
  float plantHum = analogRead(plantHum_PIN)/1024.0 * 100.0; // calcolo l'umidità della pianta
  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  tankIndicator.ceckValueChanged(digitalRead(tankSensorPIN)); //controlla se la il tankindicator è posizionato correttamente se non ci sono stati cambiamenti allora si accende
  myLCD.printTmp(t); // stampo a schermo la temperatura
  myLCD.printHum(h); // stampo a schermo la umidità
  Serial.println(t);
  myLCD.printPlant(plantHum); // stampo a schermo la umidità pianta
  myLCD.printBattery(3.75); // stampo la tensione batteria

  if (wakeUPobj.isTime2sleep()){ //Controllo se è tempo di andare a nanna /*se sono passati 6000ms da quando è andato a dormire l'ultima volta va dormire per 6 secondi*/
    Serial.println("Vado a nanna!");
    Serial.flush(); //Svuoto la serial se necessario

    myRele.rele_stop(); // disabilito i carichi ausiliari
    // qua conviene spegnere tutti i pin di output che sono stati abilitati//

    myLCD.u8g2.setPowerSave(true);

    wakeUPobj.longSleep(); //vado a nanna
    myLCD.u8g2.setPowerSave(false);
    // riattivazione
    myRele.auxStart(); // accendo gli ausiliari
    Serial.println("Buondì");
    myLCD.InitDisplay(); // inizializzo id display
    delay(200);
    myLCD.printBaseScreen(); //stampo le informazioni di base
  }
  delay(500);
}
