#include <SPI.h>

/*\
 * CLK -> 13
 * SI ->  11
 * CS ->  10
 */

int select = 10;
byte command = 0x11;
int potval = 0;
int potPin = A0;

void setup() {
  pinMode(select,OUTPUT);
  SPI.begin();
  Serial.begin(9600);
}

void loop() {
  potval = map( analogRead(potPin),0,1023,0,255);
  dPotWrite(potval);
  Serial.println(potval);
}

// command bits
//00010001


void dPotWrite(int potval){

  digitalWrite(select,LOW);
  delay(100);
  SPI.transfer(command);
  SPI.transfer(potval);
  delay(100);
  digitalWrite(select,HIGH);
}


