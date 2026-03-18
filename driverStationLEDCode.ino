#include <Adafruit_NeoPixel.h>

#define LED_PIN 12

#define LED_COUNT 46

#define input 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

uint32_t colors[] = {
  strip.Color(0, 0, 255),
  strip.Color(0, 255, 0),
  strip.Color(255, 0, 0),
  strip.Color(127, 255, 0)
};

int mode = 0;

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(input, INPUT);
  strip.begin();
  strip.setBrightness(75);
  strip.show();
}

void loop() {
  if (!digitalRead(input)) {
      mode++;
      if (mode >= sizeof(colors) / sizeof(colors[0])) {
        mode = 0;
      }
      delay(170);
  }

  strip.fill(colors[mode], 0, LED_COUNT);
  strip.show();

  Serial.println(mode);
}