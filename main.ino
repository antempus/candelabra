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
volatile int ROTARY_POS = 0;
int mrotateLast;
int mrotate;
int positionval;
bool switchval;
unsigned int LIGHT_CONFIG = 0;
unsigned int RED = 0;
unsigned int BLUE = 100;
unsigned int GREEN = 225;

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
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), readencoder, HIGH);
  int mrotateLast = digitalRead(ENCODER_PIN_A);
  eb1.setClickHandler(incrementLightConfig);
  Serial.begin (115200);
  delay(10);
}
void readencoder(){
  // TODO: Replace this with EncoderButton library
  cli();
  mrotate = digitalRead(ENCODER_PIN_A);
  if (mrotate != mrotateLast) { //knob is rotating
      if (digitalRead(ENCODER_PIN_B) != mrotate) {  //switch A changed first -> rotating clockwise
      Serial.println ("rotated clockwise");
      ROTARY_POS += BRIGHTNESS_MODIFIER;
      if (ROTARY_POS > ROTARY_MAX) ROTARY_POS = 255;
    }
    else {// switch B changed first -> rotating counterclockwise
      ROTARY_POS -= BRIGHTNESS_MODIFIER;
      if (ROTARY_POS < ROTARY_MIN) ROTARY_POS = 0;
      Serial.println ("rotated counterclockwise");
    }
    Serial.print("Encoder Position: ");
    Serial.println(ROTARY_POS);
    Serial.println("");
  }
  mrotateLast = mrotate;
  sei();
 }

void incrementLightConfig(){
  Serial.print("eb1 clickCount: ");
  Serial.println(eb1.clickCount());
  // TODO: Update this to use a switch statement for configuring which component to change:
    // 0: RED
    // 1: GREEN
    // 2: BLUE
    // 3: BRIGHTNESS
  // Also need to add in support for double clicks to move to effects and long click to reset color to default
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
  pixels.setBrightness(ROTARY_POS);
}