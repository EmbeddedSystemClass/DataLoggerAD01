
//https://github.com/adafruit/Adafruit_SSD1306

#define OLED_CS    48// 13
#define OLED_RESET 46
#define OLED_DC    44// 12  // common 50
#define OLED_CLK   42 //13 // common  52
#define OLED_MOSI  40// 11 //common 51

    //pinMode(8, OUTPUT);
    //pinMode(9, OUTPUT);

/*
#define OLED_CS    34// 13
#define OLED_RESET 36
#define OLED_DC    38// 12  // common 50
#define OLED_CLK   40 //13 // common  52
#define OLED_MOSI  42// 11 //common 51
*/


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

  /*
 * In Adafruit_SSD1306.h
 * 1. uncomment #define SSD1306_128_64
2. comment #define SSD1306_128_32
3. comment #define SSD1306_96_16
In the example ssd1306_128x64_i2c
4. add #define SSD1306_LCDHEIGHT 64
 *
 */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SSD1306_LCDHEIGHT 64
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//Adafruit_SSD1306 display(OLED_RESET);
//DISPLAY INITIALIZER
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void DisplayInit(void){
    //-- DISPLAY INIT --//
    Serial.println("Display Initing");
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println("SSD1306 allocation failed");
  
      for(;;); // Don’t proceed, loop forever
      }

   // display.begin(SSD1306_SWITCHCAPVCC);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println();
    display.println("DATA"); 
      display.display();
      delay(500);
}

void displayValues(void)
{
  display.clearDisplay();
  display.setTextSize(1);
        display.setCursor(0, 1);
    //display.print("->");

    display.print(Str_Time); 
    display.print(' ');    
    display.println(Str_Date);       
    //Str_Date + Str_Time + ' ';  
/*
      DateTime now = rtc.now();

       //       display.setCursor(0, 1);
       //       display.setCursor(1, 1);
    if(now.hour()<10)display.print(' ');           
    display.print(now.hour(), DEC);
    display.print(':');
    if(now.minute()<10)display.print('0');      
    display.print(now.minute(), DEC);
    display.print(':');
    if(now.second()<10)display.print('0');        
    display.print(now.second(), DEC);
    display.print(" ");

   display.print(daysOfTheWeek[now.dayOfTheWeek()]);

   // display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  
     display.print(" ");   
  //  display.print(now.year(), DEC);
 //   display.print('/');
    if(now.month()<10)display.print('0');   
    display.print(now.month(), DEC);
    display.print('/');
    if(now.day()<10)display.print('0');   
    display.println(now.day(), DEC);
*/

   // display.print("Card:");//5
    switch(SDCardStatus){
      case 0 :display.print("No Card  ");
      break;
      case 1 :display.print("SD1 Card ");display.print(SD_Volume);display.print(" Gb ");
      break;
      case 2 :display.print("SD2 Card ");display.print(SD_Volume);display.print(" Gb ");
      break;
      case 3 :display.print("SDHC Card ");display.print(SD_Volume);display.print(" Gb");
      break; 
      default:display.print("Unknown Problem !");        
    }
    display.println();
    
    display.print("Log: ");
    if(SDCardStatus != SD_NOT_Present){
      display.println(LOG_FILE);
    }
    else{
      Serial.println("Card Problem");
        display.println("Problem");    
    }

    display.print("Timer: ");
    display.println(Timer);
    //   display.print("SDCardStatus: ");
    // display.println(SDCardStatus);  
    display.print("RTC_Test: ");
    display.println(RTC_Test);
    
    
   display.display();
}
