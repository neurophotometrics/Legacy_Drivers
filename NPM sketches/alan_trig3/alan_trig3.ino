#include "alan_trig3.h"

/*
 * Filename: alan_trig3
 * Author: Christopher Yin
 * Description:
 * User interface for Neurophotometrics
 * Date: 10.24.17
 *
 */

void setup() {

  //initialize all I/O pins
  for(int pin = 0;pin++;pin<3){
    //pinMode(ledWritePins[pin],OUTPUT);
    pinMode(potPins[pin],INPUT);
  }

  //individually initialize output pins- error when in loop (FIXME)
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  
  pinMode(potPins[FPS],INPUT);
  pinMode(cameraPin,OUTPUT);
  
  init_lcd();
  shutdown_LED();
  Serial.begin(9600);
}

void loop() {
  //check inputs
  //updateLED();
  updateFPS();    
  modeCheck();
  startCheck();

  //write camera high (triggered by falling edge)
  digitalWrite(cameraPin,HIGH);

  //start capturing data if startButton is pressed
  if(start){
    lcd.setCursor(17,3);
    lcd.print("ON ");
    switch(mode){

      case TRIGGER1_MODE:
      
        //initialize LED states
        init_LED(LOW,HIGH,HIGH);
  
        //capture data until start button pressed
        while(start){
          camera_write_trig1();
          startCheck();
        }
        break;

      case TRIGGER2_MODE:
      
        //initialize LED states
        init_LED(LOW,HIGH,LOW);
  
        //capture data until start button pressed
        while(start){
          camera_write_trig2();
          startCheck();
        }
        break;

      case TRIGGER3_MODE:
      
        //initialize LED states
        init_LED(LOW,LOW,LOW);
        
        //capture data until start button pressed
        while(start){
          camera_write_trig3();
          startCheck();
        }
        break;
        
      case CONSTANT_MODE:

        //initialize LED states
        init_LED(HIGH,HIGH,HIGH);
        
        //capture data until start button pressed
        while(start){
          camera_write_const();
          startCheck();
        }
        break;
      
    }

    //turn off LEDs
    lcd.setCursor(17,3);
    lcd.print("OFF");
  }
  //turn off all LEDs
  shutdown_LED();
}

