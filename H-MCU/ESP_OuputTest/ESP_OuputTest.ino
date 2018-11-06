void setup() 
{
  Serial.begin(1200);
  Serial.println("Testing outputs...");
  
  byte d[7] = {16,15,14,13,12,02,00};
  
  for(int i = 0; i < 7; i++)
  {
    Serial.println(d[i]);
    pinMode(d[i], OUTPUT);
    digitalWrite(d[i], true);
  }
}

void loop() 
{

}
