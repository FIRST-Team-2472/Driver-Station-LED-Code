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

//colors
uint32_t gold = strip.Color(212, 175, 55),
         reddish = strip.Color(255, 0, 0),
         red = strip.Color(255, 0, 0),
         orange = strip.Color(255, 125, 0),
         yellow = strip.Color(255, 255, 0),
         green = strip.Color(0, 255, 0),
         blue = strip.Color(0, 0, 255),
         purple = strip.Color(125, 0, 255),
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
        Serial.println("Starting Gold and Red Breathing");
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
        Serial.println("Starting scrolling Gold and Red");
        currentColor = red;
      }

      for (int i = 0; i < LED_COUNT / 2; i += 2) {
        strip.fill(currentColor, i, i + 1);
        strip.fill();
        currentColor = currentColor == red ? currentColor = gold : currentColor = red;
      }

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
        currentColor = red;
        state = 0;
        offset = 0;
        j = 0;
      } else {
        offset++;  // offset the red pixel to move the rainbow
      }

      r = 255;
      g = 0;
      b = 0;

      for (int i = 0; i < LED_COUNT; i++) {
        if (i + offset >= 46) {
          offset = -46;  // go back to the first pixel if we are on the last one
        }
        currentColor = strip.Color(r, g, b);
        strip.setPixelColor(i + offset, currentColor);
        switch (state) {
          case 0:  // Red to yellow (increase green)
            g += 34;
            if (g >= 255) {
              state = 1;
              g = 255;
            }
            break;
          case 1:  // Yellow to green (decrease red)
            r -= 34;
            if (r <= 0) {
              state = 2;
              r = 0;
            }
            break;
          case 2:  // Green to cyan (increase blue)
            b += 34;
            if (b >= 255) {
              state = 3;
              b = 255;
            }

            break;
          case 3:  // Cyan to blue (decrease green)
            g -= 34;
            if (g <= 0) {
              state = 4;
              g = 0;
            }
            break;
          case 4:  // Blue to purple (increase red)
            r += 34;
            if (r >= 255) {
              state = 5;
              r = 255;
            }

            break;
          case 5:  // Purple to red (decrease blue)
            b -= 34;
            if (b <= 0) {
              state = 0;
              b = 0;
            }
            break;
        }
      }

      break;
    case 6:  // static rainbow
      if (firstTime) {
        currentColor = red;
        strip.setBrightness(80);
      }
      for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, currentColor);
        switch (state) {
          case 0:  // Red to yellow (increase green)
            g += 34;
            if (g >= 255) {
              state = 1;
              g = 255;
            }
            break;
          case 1:  // Yellow to green (decrease red)
            r -= 34;
            if (r <= 0) {
              state = 2;
              r = 0;
            }
            break;
          case 2:  // Green to cyan (increase blue)
            b += 34;
            if (b >= 255) {
              state = 3;
              b = 255;
            }

            break;
          case 3:  // Cyan to blue (decrease green)
            g -= 34;
            if (g <= 0) {
              state = 4;
              g = 0;
            }
            break;
          case 4:  // Blue to purple (increase red)
            r += 34;
            if (r >= 255) {
              state = 5;
              r = 255;
            }

            break;
          case 5:  // Purple to red (decrease blue)
            b -= 34;
            if (b <= 0) {
              state = 0;
              b = 0;
            }
            break;
        }
      }
      break;
    case 7:  // Alternate gold and red
      strip.setBrightness(80);
      if (firstTime) {
        currentColor = red;
      }
      strip.fill(currentColor, 0, LED_COUNT);

      if (currentColor == red) currentColor = gold;
      else currentColor = red;
      delay(750);
      break;

    case 8:  // EXTRA: 20 Random pixels with random color (red or gold)
      // Get random number generator
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distr(0, 45);

      // get a random pixel then set a random color
      for (int i = 0; i < 20; i++) {
        int pixelNumber = distr(gen);

        std::uniform_int_distribution<> colorDist(1, 2);

        strip.setPixelColor(pixelNumber, colorDist(gen) == 1 ? red : gold);
      }

      strip.setBrightness(80);

      delay(100);

      for (int i = 500; i > 0; i++) {
        strip.setBrightness(strip.getBrightness() - 0.16);
        delay(1);
      }

      break;
  }

  firstTime = false;


  strip.show();
  Serial.println(mode);
  delay(10);
}