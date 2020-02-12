
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



#include  "C:\Projects\Arduino\DataLogAD\DataLog_Variables.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_RTC.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_SDCard.h"
#include  "C:\Projects\Arduino\DataLogAD\DataLog_Display.h"



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  delay(10);
 /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */
  DisplayInit();
  RTC_Init();
 // SD_Card_Info();
 //  SD_Card_Init(); 
 // SD_Create_File();
//  SD_Write_File();

}


void loop(void) {
    RTC_SerialAdj();
    Timer++;
    RTC_TimeClock();
    
    Serial.print("Str_Time:");
    Serial.println(Str_Time);
    Serial.print("Str_Date:");
    Serial.println(Str_Date);

    SD_Log_File();
    displayValues();
    delay(1000);
}
