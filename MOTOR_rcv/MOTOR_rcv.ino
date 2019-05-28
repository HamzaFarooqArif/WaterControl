#include <VirtualWire.h>

int relayPin = 11;

char *controller;

int OnButtonState = 0;
int OffButtonState = 0;
bool relayState   = 0;

int LVLstrToVal(char* msg)
{
  int val = (msg[1] - 48)*100;
  val += (msg[2] - 48)*10;
  val += (msg[3] - 48);
  return val;
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(10);
  vw_set_ptt_pin(6);
  vw_setup(2000);
  vw_rx_start();
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  OnButtonState = analogRead(A0);
  OffButtonState = analogRead(A1);

  if (vw_get_message(buf, &buflen)) 
    {
      char msg[6] = {'0', '0', '0', '0', '0'};

      msg[0] = (char)buf[0];
      msg[1] = (char)buf[1];
      msg[2] = (char)buf[2];
      msg[3] = (char)buf[3];
      msg[4] = (char)buf[4];
      
      if(msg[4]=='1') relayState = 0;

      Serial.println(msg);
      
      digitalWrite(13,1);
      delay(100);
      digitalWrite(13,0);
    }

  if (OnButtonState >= 500) relayState = 1;
  
  if (OffButtonState >= 500) relayState = 0;

  if(relayState == 1) digitalWrite(relayPin, HIGH);
  else digitalWrite(relayPin, LOW);
}
/*#include <VirtualWire.h>
const int button1Pin = 5;
const int button2Pin = 6;
const int relayPin =  7;

char *controller;
int button1State = 0;
int button2State = 0;
int relayState   = 0;
int counter   = 0;
void setup() {
  pinMode(13, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(8);
  vw_set_ptt_pin(9);
  vw_setup(2000);
  vw_rx_start();
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  
  if (button1State == HIGH) {
      relayState = 1;
  }
  
  if (button2State == HIGH) {
      relayState = 0;
  }
  if (vw_get_message(buf, &buflen)) 
    {
     if(buf[0]=='C'){
     relayState = 0;
     }
     if(buf[0]=='1'){
     counter = counter + 1;
     }
     
     digitalWrite(13,0);
     delay(500);
     digitalWrite(13,1);
    }
  if (counter == 10){
     relayState = 0;
     counter = 0;
   }
    
  
  if (relayState == 1){
     digitalWrite(relayPin, HIGH);
  }
  if (relayState == 0){
     digitalWrite(relayPin, LOW);
  }

}*/
