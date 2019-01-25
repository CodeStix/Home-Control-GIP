int r;
int g;
int b;
int rd = 1;
int gd = 1;
int bd = 1;

void setup()
{
  pinMode(9, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  r += 6 * rd;
  g += gd;
  b += 5 * bd;

  if (r > 255 && rd > 0)
  {
    rd = -1;
    r = 255;
  }
  if (g > 255 && gd > 0)
  {
    gd = -1;
    g = 255;
  }
  if (b > 255 && bd > 0)
  {
    bd = -1;
    b = 255;
  }

  if (r < 0 && rd < 0)
  {
    rd = 1;
    r = 0;
  }
  if (g < 0 && gd < 0)
  {
    gd = 1;
    g = 0;
  }
  if (b < 0 && bd < 0)
  {
    bd = 1;
    b = 0;
  }

  analogWrite(9, r);
  analogWrite(5, g);
  analogWrite(6, b);

  delay(12);
}
