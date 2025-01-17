#include <Adafruit_NeoPixel.h>
#include <EncoderButton.h>

//#ifdef __AVR__
//#include <avr/power.h>
//#endif

// setup RAM FUNC
void ICACHE_RAM_ATTR readEncoder();
void ICACHE_RAM_ATTR changeComponent(int component, int changeValue);
void ICACHE_RAM_ATTR printStuff(String prefix, int suffix);
int ICACHE_RAM_ATTR boundaryCheck(int valueToCheck, int _min, int _max);

#define LAST_KNOWN_SIGNAL LOW
// setup pins
const uint8_t ENCODER_PIN_A = 5;
const uint8_t ENCODER_PIN_B = 4;
const uint8_t SW_PIN = 13;
const uint8_t RGB_PIN = 14;

// setup constants
const int INC_DEC_MOD = 5;
const int DELAYVAL = 500;
unsigned int LIGHT_CONFIG = 0;
const int NUM_PIXELS = 4;
const int ROTARY_MIN = 0;
const int ROTARY_MAX = 255;
// values accessed during interrupts
volatile unsigned int BRIGHTNESS = 0;
volatile unsigned int RED = 255;
volatile unsigned int BLUE = 255;
volatile unsigned int GREEN = 255;

int mrotateLast;
int mrotate;
int positionval;
bool switchval;

Adafruit_NeoPixel pixels(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
EncoderButton eb1(ENCODER_PIN_A, ENCODER_PIN_B, SW_PIN);

void handleEncoder(EncoderButton& eb){
  Serial.print("encoder rotation: ");
  int changeValue = eb.increment();
  Serial.println(changeValue);
  changeComponent(LIGHT_CONFIG, changeValue*INC_DEC_MOD);
}

void setup(){
  Serial.begin(115200);
  delay(10);
  Serial.println("setup...");
  pixels.setBrightness(100);
  pixels.begin();
  eb1.setClickHandler(incrementLightConfig);
  eb1.setEncoderHandler(handleEncoder);
  Serial.println("..complete");
}


void changeComponent(int component, int changeValue)
{
  switch (component)
  {
  case 0:
    BRIGHTNESS = boundaryCheck(BRIGHTNESS += changeValue, ROTARY_MIN, ROTARY_MAX);
    printStuff("changing BRIGHTNESS: ", BRIGHTNESS);
    break;
  case 1:
    printStuff("changing RED: ", BRIGHTNESS);
    RED = boundaryCheck(RED += changeValue, ROTARY_MIN, ROTARY_MAX);
    break;
  case 2:
    printStuff("changing GREEN: ", BRIGHTNESS);
    GREEN = boundaryCheck(GREEN += changeValue, ROTARY_MIN, ROTARY_MAX);
    break;
  case 3:
    printStuff("changing BLUE: ", BRIGHTNESS);
    BLUE = boundaryCheck(BLUE += changeValue, ROTARY_MIN, ROTARY_MAX);
    break;

  default:
    Serial.print("should not reach, error");
    break;
  }
  //
}

void printStuff(String prefix, int suffix){
  Serial.println(prefix);
  Serial.println(suffix);
}

int boundaryCheck(int valueToCheck, int _min, int _max)
{
  if (valueToCheck > _max)
    return _max;
  if (valueToCheck < _min)
    return _min;
  return valueToCheck;
}

void incrementLightConfig(EncoderButton& eb1)
{
  int clicks = eb1.clickCount();
  Serial.print("eb1 clickCount: ");
  Serial.println(eb1.clickCount());
  if (clicks == 1)
  {
    if (LIGHT_CONFIG >= 3)
    {
      Serial.println("restting LIGHT_CONFIG");
      LIGHT_CONFIG = 0;
    }
    else
    {
      LIGHT_CONFIG++;
    }
    Serial.print("LIGHT_CONFIG IS position is: ");
    Serial.println(LIGHT_CONFIG);
  }
  if (clicks == 3){
    Serial.println("setting BRIGHTNESS to 0");
    BRIGHTNESS = 0;
  }
}

void setColors(int _red, int _green, int _blue, int _len){
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(RED, GREEN, BLUE));
    pixels.show();
  }
}
void loop()
{
  eb1.update();
  pixels.clear();
  setColors(RED, GREEN, BLUE, NUM_PIXELS);
  pixels.setBrightness(BRIGHTNESS);
}