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
int potChannel[] = {1,3,2};
int potval, temp;
int intensity[] = {-1,-1,-1,-1};

byte ad1 = 001;
byte ad2 = 010;
byte ad3 = 011;

void updateLED();
void dPotWrite(int channel, int potval);


//int select = 10;
//byte command = 0x11;
//int potval = 0;
//int potPin = A0;

void setup() {
  pinMode(selectPin,OUTPUT); digitalWrite(selectPin,HIGH);
  pinMode(7,OUTPUT); digitalWrite(7,HIGH);
  pinMode(8,OUTPUT); digitalWrite(8,HIGH);
  pinMode(9,OUTPUT); digitalWrite(9,HIGH);
  
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
}

void loop() {
  dPotWrite(ad1,128);
  dPotWrite(ad2,128);
  dPotWrite(ad3,128);
  delay(1000);
}

//change led intensity + update LCD
void updateLED(){
  for(int led=0;led<3;led++){
    int oldLed = intensity[led];
    
    //update stored led intensity
    temp = analogRead(potPins[led]);
    intensity[led] = map(temp,0,1023,0,100);
    potval = map( temp,0,1023,0,127);
    dPotWrite(potChannel[led],potval);
    Serial.println("Channel: " + led);
    Serial.println(temp);
    Serial.println(potval);
  }
}

//write new val to dpot
void dPotWrite(int channel, int potval){
  digitalWrite(selectPin,LOW);
  SPI.transfer(channel);
  SPI.transfer(potval);
  digitalWrite(selectPin,HIGH);
}


