#include <SPI.h>

/*\
 * CLK -> 13
 * SI ->  11
 * CS ->  10
 */

int ledWritePins[] = {7,8,9};
//pot read pins
int potPins[] = {A2,A1,A0,A3};
//digipot select pin
int selectPin = 10;
//digitpot pot address bytes
int potChannel[] = {0,2,1};
int potval, temp;
int intensity[] = {-1,-1,-1,-1};
int val = 70 ;



void updateLED();
void dPotWrite(int channel, int potval);



void setup() {
  pinMode(selectPin,OUTPUT); digitalWrite(selectPin,HIGH);
  pinMode(7,OUTPUT); digitalWrite(7,HIGH);
  pinMode(8,OUTPUT); digitalWrite(8,HIGH);
  pinMode(9,OUTPUT); digitalWrite(9,HIGH);
  pinMode(A0,INPUT);
  SPI.begin();
  Serial.begin(9600);
}

void loop() {
//  unsigned long t = 5000 + millis();
//  while(millis() < t){
//    updateLED();
//  }
//  t = 2000 + millis();
//  while(millis() < t){digitalWrite(7,LOW);}
//  digitalWrite(7,HIGH);
}

//change led intensity + update LCD
void updateLED(){
  for(int led=0;led<3;led++){
    int oldLed = intensity[led];
    
    //update stored led intensity
    temp = analogRead(potPins[led]);
    intensity[led] = map(temp,0,1023,0,100);
    potval = map( temp,0,1023,6,90);
    dPotWrite(potChannel[led],potval);
  }
}

//write new val to dpot
void dPotWrite(int channel, int potval){
  digitalWrite(selectPin,LOW);
  delay(10);
  SPI.transfer(channel);
  SPI.transfer(potval);
  delay(10);
  digitalWrite(selectPin,HIGH);
}


