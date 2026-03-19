#include <Adafruit_NeoPixel.h>
#include <random>

#define LED_PIN 12

#define LED_COUNT 46

#define input 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);  // Initialize the light strip

bool firstTime = true;
int mode = 0;
int offset = 0;
int j = 0;

// Timing stuff
int timePassedMS;
int modeEndtime;
int buttonEndtime;

//colors
uint32_t gold = strip.Color(212, 80, 0),
         red = strip.Color(255, 0, 0),
         orange = strip.Color(255, 125, 0),
         yellow = strip.Color(255, 255, 0),
         green = strip.Color(0, 255, 0),
         blue = strip.Color(0, 0, 255),
         purple = strip.Color(125, 0, 255),
         chartreuse = strip.Color(127, 255, 0),
         skobeloff = strip.Color(0, 116, 116),
         periwinkle = strip.Color(204, 204, 255),
         ballKnowledge = strip.Color(135, 49, 182),
         currentColor = red;

int breathingMultiplier = 1;

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

void loop() {  // innefficient and could probably be optimized or made in a much better way
  // Change light mode based on button press
  if (!digitalRead(input) && timePassedMS >= buttonEndtime) {
    mode++;
    firstTime = true;
    if (mode >= 10) {
      mode = 0;
    }
    buttonEndtime = timePassedMS + 300;
  }
  // Set the lights to the correct color/pattern
  switch (mode) {
    case 0:  // Breathe between Gold and Red
      if (firstTime) {
        Serial.println("Starting Gold and Red Breathing");
        currentColor = red;
        strip.setBrightness(0);
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
        Serial.println("Starting scrolling Gold and Red");
        strip.setBrightness(80);
        currentColor = red;
        offset = 0;
        modeEndtime = timePassedMS + 70;
      }

      if (timePassedMS >= modeEndtime) {
        for (int i = 0; i < LED_COUNT; i += 3) {
          strip.fill(currentColor, (i + offset) % LED_COUNT, 3);
          currentColor = currentColor == red ? currentColor = gold : currentColor = red;
        }
        modeEndtime = timePassedMS + 70;
      }

      offset++;

      break;
    case 2:  // all gold
      strip.setBrightness(80);
      strip.fill(gold, 0, LED_COUNT);
      break;
    case 3:  // all red
      strip.setBrightness(80);
      strip.fill(red, 0, LED_COUNT);
      break;
    case 4:  // Fading Raibow
      if (firstTime) {
        Serial.println("Starting fading rainbow");
        strip.setBrightness(80);
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
      break;
    case 5:  // scrolling rainbow (Might need work)
      if (firstTime) {
        strip.setBrightness(80);
        Serial.println("Starting scrolling Rainbow");
        offset = 0;
        modeEndtime = timePassedMS + 10;
      }

      offset += 100;
      strip.rainbow(offset);

      break;
    case 6:  // static rainbow
      strip.setBrightness(80);
      strip.rainbow();
      break;
    case 7:  // Alternate gold and red
      strip.setBrightness(80);
      if (firstTime) {
        currentColor = red;
        modeEndtime = timePassedMS + 500;
      }

      strip.fill(currentColor, 0, LED_COUNT);
      if (timePassedMS >= modeEndtime) {
        if (currentColor == red) currentColor = gold;
        else currentColor = red;
        modeEndtime = timePassedMS + 500;
      }
      break;
    case 8:  // GRADLE DEMON COLORS (BALL KNOWLEDGE)
      if (firstTime) {
        strip.setBrightness(80);
        currentColor = skobeloff;
        offset = 0;
        modeEndtime = timePassedMS + 70;
      }

      if (firstTime) {
        Serial.println("Starting scrolling Gold and Red");
        strip.setBrightness(80);
        currentColor = skobeloff;
        offset = 0;
        modeEndtime = timePassedMS + 200;
      }

      if (timePassedMS >= modeEndtime) {
        for (int i = 0; i < LED_COUNT; i += 3) {
          strip.fill(currentColor, (i + offset) % LED_COUNT, 3);
          if (currentColor == skobeloff) {
            currentColor = chartreuse;
          } else if (currentColor == chartreuse) {
            currentColor = periwinkle;
          } else if (currentColor == periwinkle) {
            currentColor = ballKnowledge;
          } else if (currentColor == ballKnowledge) {
            currentColor = skobeloff;
          }
        }
        modeEndtime = timePassedMS + 200;
      }

      offset++;

    break;
    case 9:  // off
      strip.setBrightness(0);
      strip.clear();
      break;
  }

  firstTime = false;

  timePassedMS += 10;

  strip.show();
  Serial.println(mode);
  delay(10);
}