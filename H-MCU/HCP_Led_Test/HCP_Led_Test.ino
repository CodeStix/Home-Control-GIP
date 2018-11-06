#define LED_GREEN 12
#define LED_RED 13
#define BUTTON 16


//#define CHANNEL 002
//#define BAUD_RATE 1200

bool state = false;
bool state2 = false;

void setup()
{
  Serial.begin(1200);

  // HC12.println("AT+C" + CHANNEL);
  //HC12.println("AT+B" + BAUD_RATE);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop()
{
  if (digitalRead(BUTTON))
  {
      Serial.write("r");

      digitalWrite(LED_RED,state);
      state = !state;
      digitalWrite(LED_GREEN,state2);
      state2 = !state2;
      
      while (digitalRead(BUTTON));
  }

  if (Serial.available())
  {
    char e = Serial.read();
    
    if (e == 'r')
    {
      digitalWrite(LED_RED,state);
      state = !state;
    }
    else if (e == 'g')
    {
      digitalWrite(LED_GREEN,state2);
      state2 = !state2;
    }
    
    delay(20);
  }

  delay(1);
}
