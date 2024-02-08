#ifndef LOW_POWER_OBJ_H
#define LOW_POWER_OBJ_H

#include "LowPower.h"
#include "Arduino.h"

class My_WakeUp{
  private:
  public:
    int PIN_wakeUp;
    unsigned long primoTempo;
    static uint16_t sleepInSeconds;

    void begin(int pin, uint16_t sleepInSec){
      this -> PIN_wakeUp = pin;
      this -> primoTempo = millis();
      this -> sleepInSeconds = sleepInSec;
      detachInterrupt(digitalPinToInterrupt(PIN_wakeUp));
    }
    static void wakeupCallback() {
      sleepInSeconds = 0;
    }
    void longSleep(){
      if ( sleepInSeconds & 0x01 ) LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
      if ( sleepInSeconds & 0x02 ) LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
      if ( sleepInSeconds & 0x04 ) LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
      attachInterrupt(digitalPinToInterrupt(PIN_wakeUp),My_WakeUp::wakeupCallback, RISING);
      delay(100);
      while (sleepInSeconds & 0xFFFFFFF8) {
        sleepInSeconds = sleepInSeconds - 8;
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      }
    }

  };
uint16_t My_WakeUp::sleepInSeconds = 0; // Define it outside the class

/*My_WakeUp::My_WakeUp(){}
void My_WakeUp::begin(int PIN_wakeUp_Int, uint16_t sleepInSeconds){
  PIN_wakeUp = PIN_wakeUp_Int;
  primoTempo = millis();
  sleepInSeconds = sleepInSeconds;
};*/
#endif