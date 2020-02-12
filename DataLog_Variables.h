int Timer;
int SDCardStatus = 0;
// the logging file
File logfile;
#define LOG_FILE "AD_Log.txt"
    String dataString = "";
     String DataStr = "";   
   // String Date_Time = "";
    String Str_Time="";
    String Str_Date="";
    String Str_DispTime="";
    float SD_Volume;
    bool SD_Card_Reset = 1;
    byte RTC_Test = 2;
    String incomingStr ="";
    int incomingByte = 0;
  //  int Character_Length = 0;


const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;

 //(   int Analog 
  
