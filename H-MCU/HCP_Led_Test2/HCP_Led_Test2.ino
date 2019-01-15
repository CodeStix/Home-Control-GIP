#define LED_GREEN 13
#define LED_RED 15
#define BUTTON 16

//#define CHANNEL 002
//#define BAUD_RATE 1200
 
bool state = true;
bool state2 = true;

void setup()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  
  Serial.begin(1200);

  // HC12.println("AT+C" + CHANNEL);
  // HC12.println("AT+B" + BAUD_RATE);
  
  digitalWrite(LED_GREEN,true);
  digitalWrite(LED_RED,true);
  delay(500);
  digitalWrite(LED_GREEN,false);
  digitalWrite(LED_RED,false);
  delay(500);
}

void loop()
{
  if (!digitalRead(BUTTON))
  {
      Serial.write("r");

      digitalWrite(LED_RED,state);
      state = !state;
      
      while (!digitalRead(BUTTON));
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
