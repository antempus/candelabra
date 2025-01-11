#include <Adafruit_NeoPixel.h>
#include <EncoderButton.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define RGB_PIN     2
#define NUMPIXELS   4
#define ROTARY_MIN  0
#define ROTARY_MAX 255
#define ENCODER_PIN_A 7
#define ENCODER_PIN_B 8
#define SW_PIN 9
#define LAST_KNOWN_SIGNAL LOW
#define BRIGHTNESS_MODIFIER 5
int mrotateLast;
int mrotate;
int positionval;
bool switchval;
unsigned int LIGHT_CONFIG = 0;
volatile unsigned int BRIGHTNESS = 0;
volatile unsigned int RED = 0;
volatile unsigned int BLUE = 100;
volatile unsigned int GREEN = 225;

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
EncoderButton eb1(SW_PIN);

#define DELAYVAL 500
void setup() {
  pixels.setBrightness(50);
  pixels.begin();
  // encoder setup
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  digitalWrite(ENCODER_PIN_A, RISING);       // turn on pullup resistor
  digitalWrite(ENCODER_PIN_B, RISING);       // turn on pullup resistor
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), readEncoder, HIGH);
  int mrotateLast = digitalRead(ENCODER_PIN_A);
  eb1.setClickHandler(incrementLightConfig);
  Serial.begin (115200);
  delay(10);
}
void readEncoder(){
  cli();
  mrotate = digitalRead(ENCODER_PIN_A);
  if (mrotate != mrotateLast) { //knob is rotating
      if (digitalRead(ENCODER_PIN_B) != mrotate) {  //switch A changed first -> rotating clockwise
      Serial.println ("rotated clockwise");
      changeComponent(LIGHT_CONFIG, 1);
    }
    else {// switch B changed first -> rotating counterclockwise
      Serial.println ("rotated counterclockwise");
      changeComponent(LIGHT_CONFIG, -1);
    }
  }
  mrotateLast = mrotate;
  sei();
}

void changeComponent(int component, int changeValue){
  switch (component) {
    case 1:
      Serial.print("changing RED");
      RED = boundaryCheck(RED += changeValue, ROTARY_MIN, ROTARY_MAX);
      break;
     case 2:
      Serial.print("changing BLUE");
      BLUE = boundaryCheck(BLUE += changeValue, ROTARY_MIN, ROTARY_MAX);
      break;
    case 3:
      Serial.print("changing GREEN");
      GREEN = boundaryCheck(GREEN += changeValue, ROTARY_MIN, ROTARY_MAX);
      break;
    case 4:
      Serial.print("changing BRIGHTNESS");
      BRIGHTNESS = boundaryCheck(BRIGHTNESS += changeValue, ROTARY_MIN, ROTARY_MAX);
      break;
   default:
      Serial.print("should not reach, error");
      break;
  }
//
}

int boundaryCheck(int valueToCheck, int _min, int _max){
  if (valueToCheck > _max) return _max;
  if (valueToCheck < _min) return _min;
  return valueToCheck;
}

void incrementLightConfig(){
  Serial.print("eb1 clickCount: ");
  Serial.println(eb1.clickCount());
  if (LIGHT_CONFIG >= 3){
    Serial.println("restting LIGHT_CONFIG");
    LIGHT_CONFIG = 0;
  }
  else {
    LIGHT_CONFIG++;
  }
  Serial.print("LIGHT_CONFIG IS position is: ");
  Serial.println(LIGHT_CONFIG);
}



void loop() {
  eb1.update();
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(RED, GREEN, BLUE));
    pixels.show();
  }
  pixels.setBrightness(BRIGHTNESS);
}