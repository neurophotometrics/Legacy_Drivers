/*
 * Filename: alan_trig3.h
 * Author: Christopher Yin
 * Description: Header file containing methods and data fields for alan)trig3.
 * Date: 10.24.17
 *
 * Data Fields:
 *         
 *            int ledWritePins[]
 *            int potPins[] 
 *            int ledPower[]
 *            int cameraPin
 *            
 *            int intensity[] 
 *            int on[]
 *            int mode
 *            
 *            int minFPS
 *            int maxFPS
 *            int maxIntensity
 *            
 *
 * Methods:
 *            void init_lcd();
 *            void updateLCD(int val);
 *            void updateFPS();
 *            void updateLED();
 *            void updateLCD(int val);
 *            void modeCheck();
 *            void startCheck();
 *            void init_LED(int led1,int led2, int led3);
 *            void shutdown_LED();
 *            void camera_write_trig1();
 *            void camera_write_trig2();
 *            void camera_write_trig3();
 *            void camera_write_const();
 *
 */

#ifndef ALAN1
#define ALAN1

#define LED410 0
#define LED470 1
#define LED560 2
#define FPS 3
#define VAL_CURSOR 10
#define CONSTANT_MODE 0
#define TRIGGER1_MODE 1
#define TRIGGER2_MODE 2
#define TRIGGER3_MODE 3

#include <Arduino.h>
#include <Button.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*
 * Begin data field declarations
 */
 

LiquidCrystal_I2C lcd(0x3F,20,4);

//led output pins
/*new board*/
//int ledWritePins[] = {10,9,11};
//int potPins[] = {A1,A2,A0,A3};
/*old board*/
int ledWritePins[] = {10,11,9};
int potPins[] = {A1,A0,A2,A3};
Button startButton = Button(3,PULLUP);
Button modeButton = Button(4,PULLUP);
int cameraPin = 7;

//state variables
int intensity[] = {-1,-1,-1,-1};
int on[] = {LOW,LOW,LOW};
int mode = CONSTANT_MODE;
boolean start = false;
unsigned long temp;
int cycle_led = 0;

//technical parameters
int minFPS = 5, maxFPS = 40, maxIntensity = 100, potMin = 830, potMax = 315;

//wave parameters
int t_exposure;      //CALCULATED AS 1/FPS - t_dead
int t_dead = 1;     

/*
 * Begin forward declaration of functions.
 */

void init_lcd();
void updateLCD(int val);
void updateFPS();
void updateLED();
void updateLCD(int val);
void modeCheck();
void startCheck();
void init_LED(int led1,int led2, int led3);
void shutdown_LED();
void camera_write_trig1();
void camera_write_trig2();
void camera_write_trig3();
void camera_write_const();

/*
 * Begin function definitions.
 */

void init_lcd(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,LED410);
  lcd.print("LED410: ");
  lcd.setCursor(0,LED470);
  lcd.print("LED470: ");
  lcd.setCursor(0,LED560);
  lcd.print("LED560: ");
  lcd.setCursor(0,FPS);
  lcd.print("FPS:    ");

  updateLED();
  updateFPS();

  //print capture status
  lcd.setCursor(17,3);
  lcd.print("OFF");

  //print mode
  lcd.setCursor(16,0);
  lcd.print("CNST");
  
}

//update a value on the LCD
void updateLCD(int val){
  // set cursor to appropriate line
  (lcd).setCursor(VAL_CURSOR,val);
  (lcd).print("     ");

  (lcd).setCursor(VAL_CURSOR,val);
  // print updated value
  (lcd).print(intensity[val]);
}

//change FPS + update LCD
void updateFPS(){

  int oldFPS = intensity[FPS];

  //update FPS value
  intensity[FPS] = abs(map(analogRead(potPins[FPS]),0,1023,minFPS,maxFPS)-(minFPS+maxFPS));
  //update LCD
  if(oldFPS != intensity[FPS]){
    updateLCD(FPS);
    //update exposure time
    t_exposure = 1000/intensity[FPS] - t_dead;
  }
}

//change led intensity + update LCD
void updateLED(){
  for(int led=0;led<3;led++){
    int oldLed = intensity[led];
    
    //update stored led intensity
    temp = analogRead(potPins[led]);
    temp = min(temp,potMin);
    temp = max(temp,potMax);
    intensity[led] = map(temp,potMin,potMax,0,maxIntensity);
    if(oldLed != intensity[led]){
      //update LCD
      updateLCD(led);
    }
  }
}

//check mode
void modeCheck(){
  if(modeButton.uniquePress()){
      mode = (mode+1)%4;
      lcd.setCursor(16,0);
      switch(mode){
        case CONSTANT_MODE:
          lcd.print("CNST");
          break;
        case TRIGGER1_MODE:
          lcd.print("TRG1");
          break;
        case TRIGGER2_MODE:
          lcd.print("TRG2");
          break;
        case TRIGGER3_MODE:
          lcd.print("TRG3");
          break;
      }
  } 
}

//check if experiment initiated
void startCheck(){
  start = !startButton.isPressed();
}


//initialize LEDs for trigger mode
void init_LED(int led1,int led2, int led3){
  on[LED410] = led1;
  on[LED470] = led2;
  on[LED560] = led3;

  for(int led=0;led<3;led++){
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//turn off all LEDs
void shutdown_LED(){
  for(int led=0;led<3;led++){
    on[led] = LOW;
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//trigger mode 1 - alternate 410 and 470/560
void camera_write_trig1(){
  //dead time
  delay(t_dead);
  //take picture
  digitalWrite(cameraPin,LOW); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(1);
  digitalWrite(cameraPin,HIGH); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(t_exposure - 1);

  //switch LED states
  for(int led=0;led<3;led++){
    on[led] = !on[led];
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//trigger mode 2 - alternate 470 and 560
void camera_write_trig2(){
  //dead time
  delay(t_dead);
  //take picture
  digitalWrite(cameraPin,LOW); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(1);
  digitalWrite(cameraPin,HIGH); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(t_exposure - 1);

  //switch LED states
  for(int led=1;led<=2;led++){
    on[led] = !on[led];
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//trigger mode 3 - cycle through all LEDs
void camera_write_trig3(){
  //dead time
  delay(t_dead);
  //take picture
  digitalWrite(cameraPin,LOW); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(1);
  digitalWrite(cameraPin,HIGH); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(t_exposure - 1);

  on[cycle_led] = LOW;
  cycle_led = (cycle_led + 1)%3;
  on[cycle_led] = HIGH;
  //switch LED states
  for(int led=0;led<3;led++){
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//constant mode
void camera_write_const(){
  updateLED();
  //dead time
  delay(t_dead);

  //take picture
  digitalWrite(cameraPin,LOW); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(1);
  digitalWrite(cameraPin,HIGH); //CHANGE POLARITY HERE -- LOW2HIGH or HIGH2LOW
  delay(t_exposure - 1);
}

#endif
