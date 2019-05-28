/*





void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  

}



void loop(){
  controller="A";

    Serial.println("INCOMING");
}*/



















#include <VirtualWire.h>
#include <NewPing.h>
#include <MedianFilter.h>
#include <Wire.h>
#include <MedianFilter.h>

int TRIGGER_PIN = 6;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
int ECHO_PIN = 9;  // Arduino pin tied to echo pin on the ultrasonic sensor.
int MAX_DISTANCE = 250; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
char *controller;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
MedianFilter filter(31,0);

void sendSignal(char* msg)
{
  controller = msg;
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // Wait until the whole message is gone  
}

int measure()
{
  unsigned int o;
  int uS = sonar.ping();
  filter.in(uS);
  o = filter.out();
  return o / US_ROUNDTRIP_CM;
}

void intToStr(char* result, int val, int key)
{
  int idx = key;
  while(val > 0)
  {
    result[idx] = 48 + val%10;
    val = val/10;
    idx--;
  }
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.

  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);
  vw_setup(2000);// speed of data transfer Kbps

  
}


void loop() {

  char msg[6] = {'0', '0', '0', '0', '0'};
  
  measure();
  measure();
  measure();
  measure();
  measure();
  measure();
  measure();
  measure();
  measure();
  measure();
  
  int measuredLvl = MAX_DISTANCE - measure() + 20;
  bool waterState = false;
  bool fullState = false;

  if(analogRead(A0) > 500) waterState = true;
  if(analogRead(A1) > 500) fullState = true;
  
  
  /*int measuredLevel = measure();
  delay(100);           
  Serial.print("Ping: ");
  Serial.print(measuredLevel);
  Serial.println("cm");

  sendSignal(getLevel(measuredLevel));*/

  intToStr(msg, waterState, 0);
  intToStr(msg, measuredLvl, 3);
  intToStr(msg, fullState, 4);
  
  sendSignal(msg);
  Serial.println(msg);

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);

  
}
