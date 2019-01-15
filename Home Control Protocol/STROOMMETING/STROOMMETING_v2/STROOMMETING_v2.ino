
int sensorValue = 0;

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  //analogReference(INTERNAL);
}

void loop()
{
  sensorValue = analogRead(A1);

  float u = (sensorValue / 1023.0f) * 5.0f;
  float i = u * 1000.0f / 363.63636363f / 1.03f;
  Serial.print("U=");
  Serial.print(u);
  Serial.print("\tI=");
  Serial.println(i);

  delay(200);
}
