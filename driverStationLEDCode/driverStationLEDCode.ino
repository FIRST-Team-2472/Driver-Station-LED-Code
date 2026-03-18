#include <Adafruit_NeoPixel.h>

#define LED_PIN 12

#define LED_COUNT 46

#define input 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);  // Initialize the light strip

bool firstTime = true;
int mode = 0;

//colors
uint32_t gold = strip.Color(212, 175, 55),
         reddish = strip.Color(255, 0, 0),
         red = strip.Color(255, 0, 0),
         orange = strip.Color(255, 125, 0),
         yellow = strip.Color(255, 255, 0),
         green = strip.Color(0, 255, 0),
         blue = strip.Color(0, 0, 255),
         purple = strip.Color(125, 0, 255);

//breathing stuff
int breathingMultiplier = 1;
uint32_t currentColor = red;

// rainbow stuff
uint32_t r = 255;
uint32_t g = 0;
uint32_t b = 0;
int state;


void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(input, INPUT);
  strip.begin();
  strip.setBrightness(0);
  strip.show();
}

void loop() { // innefficient and could probably be optimized or made in a much better way
  // Change light mode based on button press
  if (!digitalRead(input)) {
    mode++;
    firstTime = true;
    if (mode >= 7) {
      mode = 0;
    }
    delay(170);
  }
  // Set the lights to the correct color/pattern
  switch (mode) {
    case 0:  // Breathe between Gold and Red
      if (firstTime) {
        Serial.println("first Time Breath between Gold and Red");
        currentColor = red;
      }
      strip.fill(currentColor, 0, LED_COUNT);
      strip.setBrightness(strip.getBrightness() + (1 * breathingMultiplier));
      if (strip.getBrightness() == 100) {  // slowly increase/decrease brightness
        breathingMultiplier *= -1;
      }
      if (strip.getBrightness() == 0) {
        if (currentColor == red) {  // switch color when the lights dimmed out
          currentColor = gold;
          Serial.println("gold");
        } else {
          currentColor = red;
          Serial.println("red");
        }
        breathingMultiplier *= -1;
      }
      break;
    case 1:  // Scrolling Gold and Red
      if (firstTime) {
        Serial.println("first Time scrolling Gold and Red");
        currentColor = red;
      }

      // CODE FREEZES HERE (probably because I used a loop)
      
      
      break;
    case 2:  // all gold
      strip.setBrightness(80);
      strip.fill(gold, 0, LED_COUNT);
      Serial.println("gold only");
      break;
    case 3:  // all red
      strip.setBrightness(80);
      strip.fill(red, 0, LED_COUNT);
      Serial.println("red only");
      break;
    case 4:  // Fading Raibow
      if (firstTime) {
        Serial.println("first Time fading rainbow");
        currentColor = strip.Color(255, 0, 0);
        r = 255;
        g = 0;
        b = 0;
        state = 0;
      }

      switch (state) {
        case 0:  // Red to yellow (increase green)
          g += 1;
          if (g == 255) state = 1;
          break;
        case 1:  // Yellow to green (decrease red)
          r -= 1;
          if (r == 0) state = 2;
          break;
        case 2:  // Green to cyan (increase blue)
          b += 1;
          if (b == 255) state = 3;
          break;
        case 3:  // Cyan to blue (decrease green)
          g -= 1;
          if (g == 0) state = 4;
          break;
        case 4:  // Blue to purple (increase red)
          r += 1;
          if (r == 255) state = 5;
          break;
        case 5:  // Purple to red (decrease blue)
          b -= 1;
          if (b == 0) state = 0;
          break;
      }

      strip.setBrightness(80);
      strip.fill(strip.Color(r, g, b), 0, LED_COUNT);
      Serial.print(r);
      Serial.print(", ");
      Serial.print(g);
      Serial.print(", ");
      Serial.println(b);
      break;
  }

  firstTime = false;


  strip.show();
  delay(10);
  Serial.println(mode);
}