#include <Arduino.h>

/**
 * HardwareTimer.h create 4 timer instances  - Timer1, Timer2, Timer3, Timer4
 * Use the below methods to initialize a timer interrupt:
 * - HardwareTimer::setMode(int channel, timer_mode mode) 
 * - HardwareTimer::setPeriod(uint32 microseconds)
 * - HardwareTimer::refresh(void)
 * - HardwareTimer::attachInterrupt(int channel, voidFuncPtr handler)
 * 
 * To detachInterrupt:
 * - HardwareTimer::detachInterrupt(int channel)
*/

uint32_t timeBegin = 0;
bool time1Exit = false;

void callback(void)
{
  SerialUSB.print("Timer arrived - ");
  SerialUSB.println(millis() - timeBegin, DEC);
}

void Time1Init()
{  
  Timer1.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer1.setPeriod(1000000);  // microseconds
  Timer1.refresh(); 
  Timer1.attachInterrupt(TIMER_CH1, callback);
}

void setup() {   
  delay(2000);
  SerialUSB.begin();
  if(SerialUSB)
  {
    SerialUSB.println("SerialUSB does not begin");
  }
  timeBegin = millis();  
  Time1Init();
}

void loop() {
  if((millis() - timeBegin > 5000) && !time1Exit)
  {
    SerialUSB.print("Timer runs elapse - ");
    SerialUSB.println(millis() - timeBegin, DEC);
    time1Exit = true;
    Timer1.detachInterrupt(TIMER_CH1);
  }
}
