/*
 * Filename: alan1.h
 * Author: Christopher Yin
 * Description: Header file containing methods and data fields for alan1.
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
 *            void init_lcd()
 *            void updateLCD(int val)
 *            void updateFPS()
 *            void updateLED()
 *            void updateLCD(int val)
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
#define TRIGGER_MODE 1
#define CYCLE_MODE 2

#include <Arduino.h>
#include <Button.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*
 * Begin data field declarations
 */
 

LiquidCrystal_I2C lcd(0x3F,20,4);

//led output pins
//int ledWritePins[] = {10,9,11};
int ledWritePins[] = {10,11,9};
int potPins[] = {A1,A0,A2,A3};
//int potPins[] = {A1,A2,A0,A3};
Button startButton = Button(3,PULLUP);
Button modeButton = Button(4,PULLUP);
int cameraPin = 7;

//state variables
int intensity[] = {-1,-1,-1,-1};
int on[] = {LOW,LOW,LOW};
boolean mode = CONSTANT_MODE;
boolean start = false;
unsigned long temp;
int cycle_led = 0;

//technical parameters
//old potmin = 700, potMax = 852
int minFPS = 5, maxFPS = 40, maxIntensity = 100, potMin = 830, potMax = 315;

//wave parameters
int t_exposure;      //CALCULATED AS 1/FPS - t_dead
int t_dead = 1;     
unsigned long stop_time;

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
void camera_write_trig();
void camera_write_const();
void camera_write_cycle();

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
    //intensity[led] = abs(map(temp, potMin, potMax, 0, maxIntensity)-maxIntensity);
    if(oldLed != intensity[led]){
      //update LCD
      updateLCD(led);
    }
  }
}

//check if in TRIGGER or CONSTANT mode
void modeCheck(){
  if(modeButton.uniquePress()){
      mode = (mode+1)%3;
      lcd.setCursor(16,0);
      switch(mode){
        case TRIGGER_MODE:
          lcd.print("TRGR");
          break;
        case CONSTANT_MODE:
          lcd.print("CNST");
          break;
        case CYCLE_MODE:
          lcd.print("CYCL");
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

void shutdown_LED(){
  for(int led=0;led<3;led++){
    on[led] = LOW;
    digitalWrite(ledWritePins[led],on[led]);
  }
}

//write instruction to camera
void camera_write_trig(){
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

void camera_write_cycle(){
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

#endif
