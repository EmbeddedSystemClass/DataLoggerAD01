#define TCAADDR 0x70  //  
//#define CHANNEL_VOC 2 //  
#define CHANNEL_TEMPHUM 7  // 0x40 default address 
#define CHANNEL_LIGHT 6  // 0x29 default address sio71 
#define CHANNEL_BAROMETRIC 5  // 0x77 default address bmp388 
//                            0x68 RTC sdefault address
// https://cdn-learn.adafruit.com/downloads/pdf/adafruit-tsl2591.pdf
// https://cdn-shop.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf
//https://cdn-learn.adafruit.com/downloads/pdf/adafruit-bmp388.pdf?timestamp=1556108471
// https://cdn-learn.adafruit.com/downloads/pdf/adafruit-si7021-temperature-plus-humidity-sensor.pdf


Adafruit_Si7021 THsensor = Adafruit_Si7021();
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp; // I2C



// Arduino BH1750FVI Light sensor
//https://github.com/mysensors/MySensorsArduinoExamples/blob/master/examples/LightLuxSensor/LightLuxSensor.ino


void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
    //dafruit Industries
    //  https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexerbreakout
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(20);
}

void SensorInit_Si072(){
  // Temperature & Humidity Sensor
  tcaselect(CHANNEL_TEMPHUM);
  Serial.println("Si7021 test!");
  
  if (!THsensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    delay(250);
  //  while (true)      
  }
    Serial.print(" Rev(");
    Serial.print(THsensor.getRevision());
    Serial.print(")");
    Serial.print(" Serial #"); Serial.print(THsensor.sernum_a, HEX); Serial.println(THsensor.sernum_b, HEX);
}
void SensorRead_Si072(){
    tcaselect(CHANNEL_TEMPHUM);
  Serial.print("Humidity:    ");
  Values.Humidity = THsensor.readHumidity();
//  Serial.print(THsensor.readHumidity(), 2);
  Serial.print(Values.Humidity, 2); 
   
  Serial.print("\tTemperature: ");
  //Serial.println(THsensor.readTemperature(), 2);
  Values.TemperatureSi072 = THsensor.readTemperature();
   Serial.println(Values.TemperatureSi072, 2); 
}
void SensorAlt_Init() {
  tcaselect(CHANNEL_BAROMETRIC);
  Serial.println("BMP388 test");
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    //while (1);
  }
  else 
      Serial.println(" BMP3 sensor Valid!");
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  //bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void SensorAlt_Read(){
      tcaselect(CHANNEL_BAROMETRIC);
      delay(1);
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Values.TemperatureBMP = bmp.temperature;
  Serial.print(Values.TemperatureBMP);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Values.Pressure = bmp.pressure / 100.0;
  Serial.print(Values.Pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Values.Altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print(Values.Altitude);
  Serial.println(" m");
}
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}



/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}
void SensorLight_Init(void) {
  tcaselect(CHANNEL_LIGHT);
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  if (tsl.begin()) {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else {
    Serial.println(F("No sensor found ... check your wiring?"));
   // while (1);
  }    
  /* Display some basic information on this sensor */
  displaySensorDetails();  
  /* Configure the sensor */
  configureSensor();
  // Now we're ready to get readings ... move on to loop()!
}
void SensorLight_Read(void) {
    tcaselect(CHANNEL_LIGHT);
    // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
//  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
  Values.Luminosity = tsl.getLuminosity(TSL2591_VISIBLE);
  
  //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Luminosity: "));
  Serial.println(Values.Luminosity, DEC);
}



void WindSpeed_Calculation(){
    tempReading = (double)Values.TemperatureSi072;
    // calculate wind speed 
    #define analogPinForRV 0 // Analog pin0 for mega 
    RV_ADunits = analogRead(analogPinForRV);      // read analog input of wind sensor RV output
    for (int i = 0; i < 7; i++) {                 // read another 7 times to improve reading quality and reduce noise
      RV_ADunits += analogRead(analogPinForRV);
    }
    RV_ADunits /= 8;                              // devide by 8, to get average
    RV_ADunits_dev1000 = RV_ADunits / 1000;       // devide by 1000, to match calibration factors and calculation
    velReading = (2.285 * pow(RV_ADunits_dev1000, 3) - 12.417 * pow(RV_ADunits_dev1000, 2) 
      + 22.831 * RV_ADunits_dev1000 - 14.19) / (T_rev_wind / tempReading) * v_calc_F4 
      - ((T_rev_wind - tempReading) * v_calc_F1 * pow(RV_ADunits_dev1000, v_calc_F3)) 
      - (T_rev_wind - tempReading) * v_calc_F2 ; // wind speed calculation based on own calibration - see reference on authors' website
    velReading = velReading - zeroWindAdjustment; // adjust zero windspeed
    if (velReading < 0) {
      velReading = 0; // error handling in case of false zeroWindAdjustment setting
    }
}
  




 //  This Part is based on the Sensors Manufacturer Data sheet
  //  Values.WindMPH
  
  // #define WND_OUT_Pin  0;// A0;   // wind sensor analog pin  hooked up to Wind P sensor "OUT" pin
  //  Values.WindTemp  
  // #define WND_TEMP_Pin 1;// A1;   // temp sesnsor analog pin hooked up to Wind P sensor "TMP" pin

  
/*
void WinSensor(){
        int windADunits = analogRead(WND_OUT_Pin);
    // Serial.print("RW ");   // print raw A/D for debug
    // Serial.print(windADunits);
    // Serial.print("\t");
    if (windADunits < 350 ) windADunits = 350; // min 350 for 
  
    Serial.print("windADunits");Serial.println(windADunits);
    Values.WindMPH =  pow((((float)windADunits - 264.0) / 85.6814), 3.36814);
  //  float windMPH =  pow((((float)windADunits - 264.0) / 85.6814), 3.36814);
   // Serial.print(windMPH);
  //  Serial.print(" MPH\t");    

 


    // temp routine and print raw and temp C
    int tempRawAD = analogRead(WND_TEMP_Pin);  
 //   Serial.print("tempRawAD");Serial.println(tempRawAD); 
    // Serial.print("RT ");    // print raw A/D for debug
    // Serial.print(tempRawAD);
    // Serial.print("\t");
    
    // convert to volts then use formula from datatsheet 
    // Vout = ( TempC * .0195 ) + .400
    // tempC = (Vout - V0c) / TC   see the MCP9701 datasheet for V0c and TC
    
   // float tempC = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195; 
    Values.WindTemp = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195; 
 //   Serial.print(tempC);
 //   Serial.println(" C");
    //delay(750);
}

*/



    
