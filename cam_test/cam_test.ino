/**
 * CHANGE THESE PARAMETERS
 */
int t_dead = 1;
int fps = 5;

int t_exposure = 1000/fps-t_dead;
int cam = 7;

void setup() {
  pinMode(cam,OUTPUT);
  digitalWrite(cam,HIGH);
}

void loop() {
  delay(t_dead);
  digitalWrite(cam,LOW);
  delay(t_exposure);
  digitalWrite(cam,HIGH);
}
