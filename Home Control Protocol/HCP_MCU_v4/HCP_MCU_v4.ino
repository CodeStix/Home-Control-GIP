#define DEBUG_PIN LED_BUILTIN

void setup()
{
  pinMode(DEBUG_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(DEBUG_PIN, true);
  delay(500);
  digitalWrite(DEBUG_PIN, false);
  delay(500);
}
