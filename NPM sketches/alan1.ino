
#include "alan1.h"

/*
 * Filename: alan1
 * Author: Christopher Yin
 * Description:
 * User interface for Neurophotometrics
 * Date: 10.24.17
 *
 */

void setup() {

  //initialize all I/O pins
  for(int pin = 0;pin++;pin<3){
    pinMode(ledWritePins[pin],OUTPUT);
    pinMode(potPins[pin],INPUT);
  }

  pinMode(potPins[FPS],INPUT);
  pinMode(cameraPin,OUTPUT);
  
  init_lcd();

  Serial.begin(9600);
}

void loop() {

  //check inputs
  updateFPS();    
  updateLED();
  modeCheck();
  startCheck();

  //write camera high
  digitalWrite(cameraPin,HIGH);

  //start capturing data if startButton is pressed
  if(start){
    lcd.setCursor(17,3);
    lcd.print("ON ");
    //TRIGGER MODE
    if(mode==TRIGGER_MODE){
      
      //initialize LED states
      init_LED(LOW,HIGH,HIGH);  //CHANGE LED TRIGGERING: 410, 470, 560

      //capture data until start button pressed
      while(start){
        camera_write_trig();
        startCheck();
      }
    }

    //CONSTANT MODE
    else{
      
      //initialize LED states
      init_LED(HIGH,HIGH,HIGH);
      
      //capture data until start button pressed
      while(start){
        camera_write_const();
        startCheck();
      }
    }
    
    //turn off LEDs
    shutdown_LED();
    lcd.setCursor(17,3);
    lcd.print("OFF");
  }
  
}



