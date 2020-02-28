//https://www.arduino.cc/en/reference/SPI
//https://learn.adafruit.com/adafruit-shield-compatibility/data-logging-shield-assembled

// https://learn.adafruit.com/adafruit-data-logger-shield/overview

//https://cdn-learn.adafruit.com/assets/assets/000/066/497/original/adafruit_products_schem.png?1543358466'
/*
  SD card test

  This example shows how use the utility libraries on which the'
  SD library is based in order to get info about your SD card.
  Very useful for testing a card when you're not sure whether its working or not.

  The circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
     Pin 4 used here for consistency with other Arduino examples

  created  28 Mar 2011
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe
*/
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN

const int chipSelect = 53; // mega
// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

//File logfile;

#define SD_NOT_Present 0
#define SD1_TYPE 1
#define SD2_TYPE 2
#define SDHC_TYPE 3
#define UNKNOWN_TYPE 4

void SD_Card_Info(void){
  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    SDCardStatus = SD_NOT_Present;

    //while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
    
  
    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        SDCardStatus = SD1_TYPE;
      break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        SDCardStatus = SD2_TYPE;
      break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        SDCardStatus = SDHC_TYPE;
      break;
      default:
        SDCardStatus = UNKNOWN_TYPE;
        Serial.println("Unknown");
    }

      // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
      if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      // while (1);
    }
  else {

    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    SD_Volume = (float)volumesize / 1024.0;
    Serial.println(SD_Volume);    
    //Serial.println((float)volumesize / 1024.0);

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);

    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
    }
  }
}
void SD_Create_File(void){

  if (SD.exists("AD_Logfile.txt")) {
    Serial.println("AD_Logfile.txt exists.");
  } else {
    Serial.println("AD_Logfile.txt doesn't exist. Creating");
    logfile = SD.open("AD_Logfile.txt", FILE_WRITE);
    logfile.close();
     if (SD.exists("AD_Logfile.txt")) 
     Serial.println("AD_Logfile.txt exists.");   
     else
     Serial.println("Cannot Create File!!"); 
  }
}

void SD_Card_Init(){
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    //while (1);
      SDCardStatus = SD_NOT_Present;
  }
  else
     Serial.println("card Ready For Logging."); 
      switch(SDCardStatus){
      case 1 : //SD1
      break;
      case 2 ://SD2
      break;
      case 3 ://SDHC
      break; 
      case 0 ://no card   
      default: SD_Card_Reset = OFF;//unknown  
      break;     
    }
     
}
void SD_CardLogTask(void){
     if (LogPause)SD_Log_File();
     else SD_Info_Only();        
    /*
     if(SD_Card_Reset){
          SD_Card_Reset = 0;
          SD_Card_Info();
          SD_Card_Init();      
     }
     */
}
void SD_Info_Only(){
      if(SD_KartStop == OFF){
        SD_KartStop = ON;
        SD_Card_Info();
        SD_Card_Init();
      } 
}

void SD_Log_File(){  
    if(SD_Card_Reset){
      SD_Card_Reset = 0;
      SD_Card_Info();
      SD_Card_Init(); 
   //   dataString = "Year,Month,Date,Hour,Min,Sec,Data1,Data2,Data3";    
      dataString = "Year,Month,Date,Hour,Min,Sec,WindRaw,velReading,WindMPH,WindTemp,TemperatureSi072,Humidity,Pressure(hPa),";
      dataString += "TemperatureBMP,Altitude(m),Luminosity,Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z)";       
    }
    else{
      SD_Card_Init();     
      dataString = Str_DispTime;          
      dataString += String(RV_ADunits) + ',' + String(velReading)+ ',' + String(Values.WindTemp) + ',' +String(Values.WindMPH)+ ','       
      + String(Values.TemperatureSi072)+ ',' + String(Values.Humidity)+ ','+ String(Values.Pressure)+ ',' 
      + String(Values.TemperatureBMP) + ',' + String(Values.Altitude)+ ','+ String(Values.Luminosity) +','
      + String(Accelometer.x) + ',' + String(Accelometer.y)+ ','+ String(Accelometer.z) + ','      
      + String(Gyro.x) + ',' + String(Gyro.y)+ ','+ String(Gyro.z) ;       
   }     
/*
    if (! SD.exists(LOG_FILE)) {
        // file doesnt exist
      // only open a new file if it doesn't exist
      //logfile = SD.open(filename, FILE_WRITE); 
     // break;  // leave the loop!
      Serial.println("file doesnt exist"); 
    }
    else{
       Serial.println("file exist   ");    
    }
 */ 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(LOG_FILE, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
        Serial.print("dataString:");
      Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
  //  Serial.println("error opening datalog.txt");
    Serial.print("error opening : "); 
    Serial.println(LOG_FILE);    
  }
  
}

/*
void SD_Write_File() {
  // make a string for assembling the data to log:
 // String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
*/

/*
void SD_Create_File(void){
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
    if (! logfile) {
   // error("couldnt create file");
    Serial.print("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);

   logfile.println("millis,stamp,datetime,light,temp,vcc");  
 
}
*/
