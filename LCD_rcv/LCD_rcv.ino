#include <LiquidCrystal.h>
#include <VirtualWire.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//-------------------------------------------------------
byte firstChar[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};
byte secondChar[8] = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte thirdChar[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100
};
byte fourthChar[8] = {
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110
};
byte fullChar[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
//-------------------------------------------------------

void setup() {
  pinMode(13, OUTPUT);
  lcd.createChar(0, firstChar);
  lcd.createChar(1, secondChar);
  lcd.createChar(2, thirdChar);
  lcd.createChar(3, fourthChar);
  lcd.createChar(4, fullChar);
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("ARIF Tech");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print("Pvt Limited");
  delay(3000);
  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(10);
  vw_set_ptt_pin(6);
  vw_setup(2000);
  vw_rx_start();
  lcd.clear();
}

int LVLstrToVal(char* msg)
{
  int val = (msg[1] - 48)*100;
  val += (msg[2] - 48)*10;
  val += (msg[3] - 48);
  return val;
}

void writeProgressBar(int Val, int Min, int Max)
{
  if(Val > Max)
  {
    Serial.println("Error writeProgressBar");
    return;
  }
  int val = map(Val, Min, Max, 0, 80);
  int val1 = val/5;

  lcd.setCursor(0, 0);
  for(int i = 0; i < val1; i++)
  {
    lcd.setCursor(i, 0);
    lcd.write((uint8_t)4);
  }
  if(val - val1*5 > 0){lcd.write((uint8_t)(val - val1*5));}
  
}

void runProgressBar(int Val, int Min, int Max)
{
  if(Val > Max)
  {
    Serial.println("Error runProgressBar");
    return;
  }
  int val = map(Val, Min, Max, 0, 80);
  for(int i = 0; i < val; i++)
  {
    lcd.setCursor(0, 0);
    writeProgressBar(i, 0, 80);
    delay(10);
  }
}

void getPercent(int Val, char* result, int Min, int Max)
{
  if(Val > Max)
  {
    Serial.println("Error getPercent");
    return;
  }
  int val = map(Val, Min, Max, 0, 100);
  int idx = 2;
  while(val > 0)
  {
    result[idx] = 48 + val%10;
    val = val/10;
    idx--;
  }
}

void loop() {
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;

   if (vw_get_message(buf, &buflen))
    {
      int Min = 135;
      int Max = 250;
      char msg[6] = {'0', '0', '0', '0', '0'};
      char percent[5] = {'0', '0', '0', '%'};

      lcd.clear();

      msg[0] = (char)buf[0];
      msg[1] = (char)buf[1];
      msg[2] = (char)buf[2];
      msg[3] = (char)buf[3];
      msg[4] = (char)buf[4];

      getPercent(LVLstrToVal(msg), percent, Min, Max);
      lcd.setCursor(0, 1);
      lcd.write(percent);

      if(msg[4] == '1')
      {
        lcd.setCursor(5, 1);
        lcd.write("Full");
      }

      if(msg[0] == '1')
      {
        runProgressBar(LVLstrToVal(msg), Min, Max);  
      }
      else
      {
        writeProgressBar(LVLstrToVal(msg), Min, Max);
      }
      
      Serial.println(msg);

      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
    }
}
