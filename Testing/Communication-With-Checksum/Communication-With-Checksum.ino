#define MAX_DATA_COUNT 7

byte getCheckSum(String str)
{
  int XOR = 0;
  for (int i = 0; str[i] != '\0'; i += 2) // <-- increment by 2
  {
    //Serial::print(i);
    //Serial::print(": ");
    //Serial::println(str[i]);

    // make single byte value out of 2 characters from the string...
    unsigned char b1, b2, b;

    // First byte: hex to bin
    b1 = str[i];
    if (b1 >= 'a')
      b1 = b1 - 'a' + 10;
    else if (b1 >= 'A')
      b1 = b1 - 'A' + 10;
    else
      b1 -= '0';

    // Second byte: hex to bin
    b2 = str[i + 1];
    if (b2 >= 'a')
      b2 = b2 - 'a' + 10;
    else if (b2 >= 'A')
      b2 = b2 - 'A' + 10;
    else
      b2 -= '0';

    // Combine the two
    b = 0x10 * b1 + b2;

    XOR = XOR ^ b;
  }

  if (XOR == '<' || XOR == '>' || XOR == ' ' || XOR == ',')
    XOR = 's';

  return XOR;
}

int count(String input, char toCount)
{
  int j = 0;
  for (int i = 0; i < input.length(); i++)
  {
    if (input[i] == toCount)
      j++;
  }
  return j;
}

char grabChecksum(String input)
{
  if (input.length() < 2)
    return '\0';
  
  char c = input[1];

  if (c == '<' || c == '>' || c == ' ' || c == ',')
    return '\0';

  return c;
}

String* split(String input)
{
  int j = 0;
  String data[MAX_DATA_COUNT];

  for (int i = 0; i < input.length(); i++)
  {
    char c = input[i];

    if (c == '>')
      break;

    if (c == '<' || c == ' ')
      continue;

    if (c == ',')
    {
      j++;

      continue;
    }

    data[j] += c;
  }

  return data;
}

String convertToString(String data[MAX_DATA_COUNT])
{
 // String dataString = ;

  return "";

}

String* convertFromString(String input)
{
  return new String[MAX_DATA_COUNT];
}

public

void setup()
{
  Serial.begin(9600);
  
  Serial.println("Starting...");

  char* input = "<[,8,1,454,64,546,46>";

  Serial.print("Checksum for input: ");
  Serial.println(getCheckSum(input));

  Serial.print("Grabbed checksum for input: ");
  Serial.println(grabChecksum(input));

  Serial.println("Splitting...");

  String* out = split(input);
  
  Serial.print(sizeof(out));


  //Serial::print("count:");
  //Serial::println(to_string(count(input, ',')));

  

    return 0;
}

void loop()
{
  
}

