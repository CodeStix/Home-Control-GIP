bool prepareReceive(String received, float* f1, float* f2, float* f3, int* i1, int* i2, String* c)
{
  if (received.length() < 14)
    return false;

  String data = received.substring(2, received.length());
  String cs = received.substring(0, 2);
  String newCs = byteToString(generateChecksum(data));

  if (cs != newCs)
  {
    Serial.println("Faulty integrity!!");

    return false;
  }

  const int DATA_COUNT = 6;

  String splitted[DATA_COUNT];

  for (int i = 0, j = 0; i < data.length() && j < DATA_COUNT; i++)
  {
    char current = data[i];

    if (current == '<' || current == '>' || current == ' ')
      continue;

    if (isAlphaNumeric(current) || current == '.')
    {
      splitted[j].concat(current);
    }
    else
    {
      Serial.print("Data #" + String(j) + ": ");
      Serial.println(splitted[j]);

      switch (j)
      {
        case 0:
          *f1 = splitted[j].toFloat();
          break;

        case 1:
          *f2 = splitted[j].toFloat();
          break;
          
        case 2:
          *f3 = splitted[j].toFloat();
          break;
          
        case 3:
          *i1 = splitted[j].toInt();
          break;
          
        case 4:
          *i2 = splitted[j].toInt();
          break;

        case 5:
          *c = splitted[j];
          break;
      }

      j++;
    }
  }

  Serial.print("Total data: ");
  Serial.println(data);
  Serial.print("Checksum: ");
  Serial.println(cs);

  return true;
}

String prepareSend(float f1, float f2, float f3, int i1, int i2, String c)
{
  String str = "<" + String(f1) + "," + String(f2) + "," + String(f3) + "," + String(i1) + "," + String(i2) + "," + c + ",>";

  String cs = byteToString(generateChecksum(str));

  return cs + str;
}

byte generateChecksum(String str)
{
  byte sum = 0;

  for (int i = 0; i < str.length(); i++)
    sum ^= str[i];

  return sum;
}

const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

String byteToString(byte b)
{
  return String(hex[b / 16]) + String(hex[b % 16]);
}

byte stringToByte(String str)
{
  byte r = 0;

  for (int l = 0; l < 2; l++)
  {
    char c = str[l];
    byte v;

    for (byte i = 0; i < 16; i++)
      if (hex[i] == c)
      {
        v = i;
        break;
      }

    if (l == 0)
      r += 16 * v;
    else if (l == 1)
      r += v;
  }

  return r;
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting....");

  String toSend = prepareSend(2.222222222222f, 19.0f, 316.0f, 138, 696969, "Z");

  Serial.println(toSend);

  float f1, f2, f3;
  int i1, i2;
  String c;

  prepareReceive(toSend, &f1, &f2, &f3, &i1, &i2, &c);

  Serial.println("f1: " + String(f1));
  Serial.println("f2: " + String(f2));
  Serial.println("f3: " + String(f3));
  Serial.println("i1: " + String(i1));
  Serial.println("i2: " + String(i2));
  Serial.println("c: " + c);
}

void loop()
{
}
