#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define RGB_PIN     2
#define NUMPIXELS   4
#define ROTARY_MIN  0
#define ROTARY_MAX 255
#define ENCODER_PIN_A 7
#define ENCODER_PIN_B 8
#define LAST_KNOWN_SIGNAL LOW
volatile unsigned int ROTARY_POS = 0;
int mrotateLast;
int mrotate;
int positionval;
bool switchval;

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
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

  Serial.begin (115200);
  delay(50);
}

void handle_encoder() {
    cli();
    Serial.println(ROTARY_POS);
    if (ROTARY_POS < ROTARY_MAX){
      Serial.println("inc");
      ROTARY_POS++;
    }
    sei();
}
void readencoder(){
  cli();
  mrotate = digitalRead(ENCODER_PIN_A);
  if (mrotate != mrotateLast) { //knob is rotating
      if (digitalRead(ENCODER_PIN_B) != mrotate) {  //switch A changed first -> rotating clockwise
      Serial.println ("rotated clockwise");
      ROTARY_POS += 3;
      if (ROTARY_POS > ROTARY_MAX) ROTARY_POS = 255;
    }
    else {// switch B changed first -> rotating counterclockwise
      ROTARY_POS -= 3;
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


void loop() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(127, 127, 127));
    pixels.show();
  }
  pixels.setBrightness(ROTARY_POS);
}

