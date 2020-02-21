
/********************************************************************
 FileName:       Datalog.ino
 Processors:    Arduino Mega 2560
 Complier:    Arduino 
 Company: 
 Author :     ilker yagci
File Description:
    Data Logger for common purpose 
 Change History:
  Rev   Date         Description
  1.0   12.02.2020   Initial release

     //for setting date&time open arduino serila monitor and send the data stream 
         //   Year,Month,Date,Hour,Minute;Second
       //   2020,02,11,16,12,23
       // 57600 baud Both NL & CR
       // put leading zero for numbers less than 10

  *********************************************************************/


//
#include <SPI.h>
#include <SD.h>



#include "RTClib.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_Si7021.h"
#include "Adafruit_TSL2591.h"
#include "Adafruit_BMP3XX.h"

#include  "C:\Projects\Arduino\DataLogAD\DataLog_Def.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_Variables.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_RTC.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_SDCard.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_Display.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_Sensors.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_Functions.h"

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(10);
    pinMode(LED_GREEN, OUTPUT);           // set pin to input
    digitalWrite(LED_GREEN, LOW);       // turn on pullup resistors  
    pinMode(LED_RED, OUTPUT);           // set pin to input
    digitalWrite(LED_RED, LOW);       // turn on pullup resistors

    pinMode(KEY_LEFT, INPUT);           // set pin to input
    pinMode(KEY_LEFT,INPUT_PULLUP);

    pinMode(KEY_MID, INPUT);           // set pin to input
    pinMode(KEY_MID,INPUT_PULLUP);

    pinMode(KEY_RIGHT, INPUT);           // set pin to input
    pinMode(KEY_RIGHT,INPUT_PULLUP);
       ADCSRA &= ~ (1 << ADEN);            // turn off ADC to save power ,, enable when needed and turn off again
  
 /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */
  DisplayInit();
  RTC_Init();
  SensorInit_Si072();
  SensorAlt_Init();
  SensorLight_Init();
 // SD_Card_Info();
 //  SD_Card_Init(); 
 // SD_Create_File();
//  SD_Write_File();

             // initialize timer1 
      noInterrupts();           // disable all interrupts
      TCCR1A = 0;
      TCCR1B = 0;

   //    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz 500mS
       TCNT1 = 64286;            // preload timer 65536-16MHz/256/50Hz 20 ms       
       TCCR1B |= (1 << CS12);    // 256 prescaler 
       TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
       interrupts(); 
}
// interrupt vector
ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
 //   TCNT1 = 34286;            // preload timer for 500mSec
       TCNT1 = 64286;            // preload timer for 20mSec
       
    IntTimer250++;
    IntTimer500 ++;
    IntTimer1 ++;
    IntTimer2 ++;
    IntTimer5 ++;
    IntTimer10 ++;
    IntTimer20 ++;   
    IntTimer60 ++;   

    if(IntTimer250 >= 13){
      IntTimer250 = 0;
      LoopTask_250msec = ON;
    }
    if(IntTimer500 >= 25){ // 500 msec
      IntTimer500 = 0;
      LoopTask_500msec = ON;
    }
    if(IntTimer1 >= 50){  // 1 sec
      IntTimer1 = 0;
      LoopTask_1Sec = ON;
      digitalWrite(LED_GREEN, digitalRead(LED_GREEN) ^ 1);  
   
      if(DisplaySleepEnable == ON){
        if(OLED_Timer) OLED_Timer--;   // sleep active
      }
      else OLED_Timer = 32768; // no sleep    
      if(DisplayInitDelay == OFF)DisplayInitDelay = ON;           
    }
    if(IntTimer2 >= 100){ // 2 sec
      IntTimer2 = 0;
      LoopTask_2Sec = ON;
    }
    if(IntTimer5 >= 250){  // 5 sec
      IntTimer5 = 0;
      LoopTask_5Sec = ON;
    }
    if(IntTimer10 >= 500){  // 10 sec
      IntTimer10 = 0;
      LoopTask_10Sec = ON;
    }
    if(IntTimer20 >= 1000){  // 20 sec
      IntTimer20 = 0;
      LoopTask_20Sec = ON;
    }
    if(IntTimer60 >= 1000){  // 60 sec
      IntTimer60 = 0;
      LoopTask_60Sec = ON;
    }    
    

   
    
 /*
    if(IntTimer >= 50){
      IntTimer = 0;
      LoopTask_1sec = ON; 
      digitalWrite(LED_GREEN, digitalRead(LED_GREEN) ^ 1);  
      
      if(DisplaySleepEnable == ON){
        if(OLED_Timer) OLED_Timer--;   // sleep active
      }
      else OLED_Timer = 32768; // no sleep
      
      if(DisplayInitDelay == OFF)DisplayInitDelay = ON;
    }
  */  

       
    Key_Functions();
       //  digitalWrite(LED_RED, digitalRead(LED_RED) ^ 1);
     //   if(!digitalRead(KEY_LEFT) || !digitalRead(KEY_MID) || !digitalRead(KEY_RIGHT))
}


void loop(void) { 



  //Sample_LogTime

      if(LoopTask_250msec){
          LoopTask_250msec = OFF;

     
          // One time after wake up form sleep
          if(OLED_Init == ON){
            Display_ReInit(20); 
            OLED_Init = OFF;
          }
          if(OLED_Timer) {
              displayValues();
          }
          else {
            Display_SwitchOff();
          }
             
    }

       if(LoopTask_500msec){
            LoopTask_500msec = OFF;
            if(SampleTime == TASK_500MSEC) SD_CardLogTask();
        
        
        }
        
    if(LoopTask_1Sec){
        LoopTask_1Sec = OFF;
        
        RTC_SerialAdj();
        RTC_TimeClock();
    
        Serial.print("Str_Time:");
         Serial.println(Str_Time);
        Serial.print("Str_Date:");
        Serial.println(Str_Date);
        
        AdcRead();
        SensorRead_Si072();
        SensorAlt_Read();
        SensorLight_Read();

        KeyTimeOutCheck();

        if(SampleTime == TASK_1SEC) SD_CardLogTask();
             
    }
      if(LoopTask_2Sec){
        LoopTask_2Sec = OFF;
        if(SampleTime == TASK_2SEC) SD_CardLogTask();
      
    }
        if(LoopTask_5Sec){
          LoopTask_5Sec = OFF;
          if(SampleTime == TASK_5SEC) SD_CardLogTask();
      
    }
        if(LoopTask_10Sec){
          LoopTask_10Sec = OFF;
          if(SampleTime == TASK_10SEC) SD_CardLogTask();
      
    }
        if(LoopTask_20Sec){
          LoopTask_20Sec = OFF;
          if(SampleTime == TASK_20SEC) SD_CardLogTask();
      
    }
        if(LoopTask_60Sec){
          LoopTask_60Sec = OFF;
          if(SampleTime == TASK_60SEC) SD_CardLogTask();
      
    }   
}
