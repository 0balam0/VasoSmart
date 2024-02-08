#ifndef LOW_POWER_OBJ_H
#define LOW_POWER_OBJ_H

#include "LowPower.h"
#include "Arduino.h"
#include <Servo.h>

/*--------------------------------------------------CLASSE WAKEUP----------------------------------------*/
class My_WakeUp{
  private:
  public:
    uint8_t PIN_wakeUp;
    static unsigned long primoTempo;
    static uint16_t sleepInSeconds;
    static uint8_t PinWakeUPint;
    uint16_t sleepInSeconds_old;
    long int sleepAfter;
    My_WakeUp(uint8_t pin, uint16_t sleepInSec, long int sleepAfter){
      this -> PIN_wakeUp = pin;
      this -> sleepAfter = sleepAfter;
      sleepInSeconds_old = sleepInSec;
      My_WakeUp :: sleepInSeconds = sleepInSec;
      My_WakeUp :: PinWakeUPint = pin;
    }
    void begin(){
      this -> primoTempo = millis();
      detachInterrupt(digitalPinToInterrupt(PIN_wakeUp));
      attachInterrupt(digitalPinToInterrupt(PIN_wakeUp),My_WakeUp::resetTimer, RISING);
    }
    static void resetTimer(){
      primoTempo = millis();  
    }
    static void wakeupCallback() {
      sleepInSeconds = 0;
      while(digitalRead(PinWakeUPint)==HIGH){}
    }
    void longSleep(){
      detachInterrupt(digitalPinToInterrupt(PIN_wakeUp));
      sleepInSeconds = sleepInSeconds_old;
      if ( sleepInSeconds & 0x01 ) LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
      if ( sleepInSeconds & 0x02 ) LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
      if ( sleepInSeconds & 0x04 ) LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
      attachInterrupt(digitalPinToInterrupt(PIN_wakeUp),My_WakeUp::wakeupCallback, RISING);
      while (sleepInSeconds & 0xFFFFFFF8) {
        sleepInSeconds = sleepInSeconds - 8;
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      } 
      detachInterrupt(digitalPinToInterrupt(PIN_wakeUp));
      attachInterrupt(digitalPinToInterrupt(PIN_wakeUp),My_WakeUp::resetTimer, RISING);
      this -> primoTempo = millis();
    }
    bool isTime2sleep(){
      return (millis() - primoTempo) > sleepAfter;
    }
  };
uint16_t My_WakeUp::sleepInSeconds = 0; // Define it outside the class
uint8_t My_WakeUp::PinWakeUPint = 3; // Define it outside the class
static unsigned long My_WakeUp :: primoTempo;

/*--------------------------------------------------CLASSE INDICATORE SERBATOIO----------------------------------------*/
class myServo{
  public:
    uint8_t pin;
    int ultimoStato; //HIGH //LOW
    uint8_t emptyDeg;
    uint8_t fullDeg;
    Servo myServo;

    myServo(int pin, uint8_t emptyDeg, uint8_t fullDeg){
      this -> pin = pin;
      this -> ultimoStato = HIGH;
      this -> emptyDeg = emptyDeg;
      this -> fullDeg = fullDeg;
    }

    void goTo(uint8_t deg){
       this->myServo.write(deg);
    }

    void begin(){
      this -> myServo.attach(pin);
    }

    void ceckValueChanged(bool stato){
      if(stato != ultimoStato){
        if(stato == HIGH){
          this -> goTo(fullDeg);
        }
        else{
          this -> goTo(emptyDeg);
        }
        this -> ultimoStato = stato;
      }
    }
};
/*--------------------------------------------------CLASSE RELE----------------------------------------*/

class MultiRele{
  private:
  public:
  uint8_t rele_aux_PIN;
  uint8_t rele_pw_PIN;
  uint8_t rele_pompa_PIN;
  MultiRele(uint8_t _rele_pw_PIN, uint8_t _rele_aux_PIN, uint8_t _rele_pompa_PIN){
    rele_aux_PIN = _rele_aux_PIN;
    rele_pw_PIN = _rele_pw_PIN;
    rele_pompa_PIN = _rele_pompa_PIN;
  }
  void begin() {
    pinMode(rele_aux_PIN, OUTPUT);
    pinMode(rele_pompa_PIN, OUTPUT);
    pinMode(rele_pw_PIN, OUTPUT);
    digitalWrite(rele_pw_PIN, HIGH);
    digitalWrite(rele_pompa_PIN, HIGH);
    digitalWrite(rele_aux_PIN, HIGH);
  }
  void auxStart(){
    digitalWrite(rele_pw_PIN, HIGH);
    digitalWrite(rele_aux_PIN, LOW);  
  }
  void auxStop(){
    digitalWrite(rele_pw_PIN, HIGH);
    digitalWrite(rele_aux_PIN, HIGH);  
  }
  void pompaStart(){
    digitalWrite(rele_pompa_PIN, LOW);  
  }
  void pompaStop(){
    digitalWrite(rele_pompa_PIN, HIGH);  
  }
  void rele_start(){
    digitalWrite(rele_pompa_PIN, HIGH);
    digitalWrite(rele_aux_PIN, LOW); 
    digitalWrite(rele_pw_PIN, HIGH); 
  }
  void rele_stop(){
    digitalWrite(rele_pw_PIN, LOW);
    digitalWrite(rele_pompa_PIN, LOW);
    digitalWrite(rele_aux_PIN, LOW);  
  }  
};


/*--------------------------------------------------CLASSE SERBATOIO----------------------------------------*/
class SensoreSerbatorio{
  private:
  public:
    uint8_t pin;
    int stato;
    SensoreSerbatorio(uint8_t pin){
      this -> pin = pin;
    }
    void begin(){
      pinMode(pin, INPUT);
      this -> stato = digitalRead(pin);
    }
};
#endif