


void AdcRead(){
     #ifdef ARDUINO_MEGA
    ADCSRA |= (1 << ADEN); // enable adc
         #endif
    delay(1);
  //  Values.WindRaw = analogRead(0);delay(1);
    Values.WindMPH = analogRead(1);delay(1);
    Values.WindTemp = analogRead(2);
    WindSpeed_Calculation(); 
   #ifdef ARDUINO_MEGA
   ADCSRA &= ~ (1 << ADEN);            // turn off ADC
   #endif
}


void KeyTimeOutCheck(void){
        if(KeyBoardTimeOutEnb == ON){
          if(KeyBoardTimeOut)KeyBoardTimeOut--;
          else {
            KeyBoardTimeOutEnb = OFF;
             Menu =  MENU_NULL;
          }       
        }
}    


void KeyTimeOutStart(void){
    KeyBoardTimeOutEnb = ON;
     KeyBoardTimeOut  = 24;// 24 sec
}



void DisplayWakeUp(void){
      if((OLED_Timer==0) && (OLED_Init == OFF)){
        OLED_Init = ON;
      }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}

void Key_Functions(void){
       if(!KeyLeft_Rel && !KeyMid_Rel && !KeyRight_Rel){
          KeyReleased = ON;      
       }           
          if(KeyReleased && !digitalRead(KEY_LEFT)){
            KeyLeft_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 1;
              #endif 
            return;
          }
          if(KeyLeft_Rel  &&  digitalRead(KEY_LEFT)){
            KeyLeft_Rel = OFF;
                #ifdef DEBUG_KEY
                KeyLogger = 2;
                #endif  
             DisplayWakeUp();
             EnterMenuKey(); 
             KeyTimeOutStart();        
            return;                   
          }
          if(KeyReleased && !digitalRead(KEY_MID)){
            KeyMid_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 3;
              #endif            
            return;
          }
          if(KeyMid_Rel  &&  digitalRead(KEY_MID)){
             KeyMid_Rel = OFF;
              #ifdef DEBUG_KEY
              KeyLogger = 4;
              #endif
            DisplayWakeUp();  
            DownMenuKey();
            KeyTimeOutStart();                               
            return;                            
          }
          if(KeyReleased && !digitalRead(KEY_RIGHT)){
            KeyRight_Rel = ON;
              #ifdef DEBUG_KEY
              KeyLogger = 5;
              #endif            
            return;
          }
          if(KeyRight_Rel  &&  digitalRead(KEY_RIGHT)){
            KeyRight_Rel = OFF;
              #ifdef DEBUG_KEY
              KeyLogger = 6;
              #endif
            DisplayWakeUp(); 
           EscMenuKey();
           KeyTimeOutStart();                           
            return;
            
        }
}


void DispEnable(bool Enable){
  if(Enable==ON) {
    DisplaySleepEnable = ON; //go sleep
    OLED_Timer = 20;
  }
  else   DisplaySleepEnable = OFF;    // no sleep
}
void LogEnable(bool Enable){
  if(Enable == ON)LogPause = ON;
  else{
    LogPause = OFF;SD_KartStop = OFF;// once to get sd kart info only
  }
}


 void  DispExtTimeout(void){
    if(OLED_Timer <= KEYDISP_TIMER) OLED_Timer = KEYDISP_TIMER;
 }

void DownMenuKey(void){
  if(OLED_Timer==0) return;
  DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU3; //
      break;
    case MENU2 : Menu = MENU1;
      break;
    case MENU3 : Menu = MENU2;
      break;


    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB1;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB6;//
      break;
 //   case MENU2_SUB8 :  Menu =  MENU2_SUB7;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;


    default: Menu = MENU_NULL;

  }
}

void UpMenuKey(void){
  if(OLED_Timer==0) return;
    DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU2; //
      break;
    case MENU2 : Menu = MENU3;
      break;
    case MENU3 : Menu = MENU1;
      break;


    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB6;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB1;//
      break;
  //  case MENU2_SUB8 :  Menu =  MENU2_SUB1;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;


    default: Menu = MENU_NULL;

  }
}


void EscMenuKey(void){
  if(OLED_Timer==0) return;
   DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU_NULL; //
      break;
    case MENU2 : Menu = MENU_NULL;
      break;
    case MENU3 : Menu = MENU_NULL;
      break;


    case MENU1_SUB1 :  Menu =  MENU1;//
      break;
    case MENU1_SUB2 : Menu =  MENU1;//
      break;

    case MENU2_SUB1 :  Menu =  MENU2;//
      break;
    case MENU2_SUB2 : Menu =  MENU2;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2;//
      break;
    case MENU2_SUB4 : Menu =  MENU2;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2;//
      break;
   // case MENU2_SUB8 :  Menu =  MENU2;//
      break;

    case MENU3_SUB1 :  Menu =  MENU3;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3;//
      break;


    default: Menu = MENU_NULL;

  }
}



void EnterMenuKey(void){
  if(OLED_Timer==0) return;
    DispExtTimeout();
  switch(Menu){
    case MENU_NULL : Menu = MENU1;
  
      break;
    case MENU1 : //Menu = MENU1MIN; // go to sub menu  // sd kart log on
        if(LogPause == ON) Menu = MENU1_SUB2; //already logging
        else  Menu =MENU1_SUB1;
      break;
    case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one 
      switch(SampleTime){
       case TASK_500MSEC: Menu = MENU2_SUB1;
          break;        
        case TASK_1SEC : Menu = MENU2_SUB2;
          break; 
        case TASK_2SEC : Menu = MENU2_SUB3;
          break;        
        case TASK_5SEC : Menu = MENU2_SUB4;
          break;  
        case TASK_10SEC :Menu = MENU2_SUB5;
          break; 
        case TASK_20SEC :Menu = MENU2_SUB6;
          break;            
        case TASK_60SEC :Menu = MENU2_SUB7;
          break;     
      }
      break;
    case MENU3 : // Menu = MENU3MIN;

        if(DisplaySleepEnable == ON) Menu = MENU3_SUB2; //already logging
        else  Menu = MENU3_SUB1;
      break;


    case MENU1_SUB1 :  LogEnable(ON);
          Menu =  MENU_NULL;//MENU1
      break;
    case MENU1_SUB2 : LogEnable(OFF); // default
          Menu =  MENU_NULL;//MENU1
      break;
    case MENU2_SUB1 :  SampleTime = TASK_500MSEC ;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB2 : SampleTime = TASK_1SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB3 :  SampleTime = TASK_2SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB4 : SampleTime = TASK_5SEC; // default
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB5 :  SampleTime = TASK_10SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB6 :  SampleTime = TASK_20SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU2_SUB7 :  SampleTime = TASK_60SEC;
          Menu =  MENU_NULL;//MENU2;//
      break;
    case MENU3_SUB1 :  DispEnable(ON);
          Menu =  MENU_NULL;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF);
          Menu =  MENU_NULL;//MENU3
      break;


    default: Menu = MENU_NULL;

  }
}
