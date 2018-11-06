#define LED_GREEN 12
#define LED_RED 13
#define BUTTON 16

//#define CHANNEL 002
//#define BAUD_RATE 1200

bool state = true;
bool state2 = true;

void setup()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON, INPUT);
  
  Serial.begin(1200);

  // HC12.println("AT+C" + CHANNEL);
  //HC12.println("AT+B" + BAUD_RATE);
  
  byte d[7] = {16,15,14,13,14,02,00};

  for(int i = 0; i < 7; i++)
  {
    digitalWrite(d[i], true);
  }

  while(true);

  digitalWrite(LED_GREEN,true);
  digitalWrite(LED_RED,true);
  delay(500);
  digitalWrite(LED_GREEN,false);
  digitalWrite(LED_RED,false);
  delay(500);
}

void loop()
{
  if (digitalRead(BUTTON))
  {
      Serial.write("r");

      digitalWrite(LED_RED,state);
      state = !state;
      
      while (digitalRead(BUTTON));
  }

  if (Serial.available())
  {
    Serial.read();
    
    digitalWrite(LED_GREEN,state2);
    state2 = !state2;
    
    delay(20);
  }

  delay(1);
}
