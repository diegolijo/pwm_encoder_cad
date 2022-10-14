#define LEDGND 0
#define LEDB 15
#define LEDG 2
#define LEDR 4

#define CADVDD 27
#define CADGND 32
#define CADIN 25

#define SW1 23
#define ENC_A 19
#define ENC_B 18

#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1
#define LEDC_CHANNEL_2 2
#define LEDC_TIMER_8_BIT 8
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

int encoder0Pos = 0;
int color = LEDR;

void setup()
{
  //  reset PINS
  pinMode(LEDGND, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  resetLeds();
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LEDR, LEDC_CHANNEL_0);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LEDG, LEDC_CHANNEL_1);
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LEDB, LEDC_CHANNEL_2);

  pinMode(CADVDD, OUTPUT);
  pinMode(CADGND, OUTPUT);
  pinMode(CADIN, INPUT);
  digitalWrite(CADVDD, HIGH);
  digitalWrite(CADGND, LOW);

  Serial.begin(9600);
}

void loop()
{
  static int ad_Value;
  read_switch();
  read_encoder();
  ad_Value = analogRead(CADIN);
  ledcAnalogWrite(encoder0Pos, ad_Value);
}

/******************************** FNS ********************************/
void resetLeds()
{
  encoder0Pos = 0;
  digitalWrite(LEDGND, HIGH);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

int switchColor()
{
  return (color == LEDR) ? LEDG : ((color == LEDG) ? LEDB : LEDR);
}

void ledcAnalogWrite(int channel, int value)
{
  int duty = 255 - (value / 16);
  ledcWrite(channel, duty);
}

int read_encoder()
{
  static int n;
  static int encoder0PinALast;
  n = digitalRead(ENC_A);
  if ((encoder0PinALast == LOW) && (n == HIGH))
  {
    if (digitalRead(ENC_B) == LOW)
    {
      encoder0Pos -= 1;
    }
    else
    {
      encoder0Pos += 1;
    }
    print(encoder0Pos, "\n encoder: ");
  }
  encoder0PinALast = n;
  return encoder0Pos;
}

void read_switch()
{
  static boolean cheched;
  if (LOW == digitalRead(SW1) && !cheched)
  {
    cheched = true;
    resetLeds();
    color = switchColor();
  }
  if (HIGH == digitalRead(SW1))
  {
    cheched = false;
  }
}

void print(int value, const char *label)
{
  Serial.print(label);
  Serial.print(value);
}
