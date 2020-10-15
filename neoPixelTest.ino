#define SKETCH "neoPixelTest.ino"
//#include <SoftwareSerial.h>

/*
 * Thiis was someone else's code on the forum. I saved it because it has
 * a non-blocking function to wait for the operation to finish.
 * Modified for use with a Wemos D1 Mini.
 */

#include <Adafruit_NeoPixel.h>
//SoftwareSerial BT(10, 11);
#define PIXEL_PIN      D1    // Data Pin of Led strip
#define PIXEL_COUNT    275   // Number of LEDs in the strip
#define BRIGHTNESS     100    // use 96 for medium brightness
#define SPEED          0    // Speed of each Color Transition (in ms)
#define IMMEDIATELY    0    // Transition happen instantly
#define RAINBOW_SPEED  100    // Rainbow Transition speed 
#define WIPE_SPEED 5     // Sweep animation delay.


// number of pixel to change
int pixNum;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

enum MODE {
  mBLUE,
  mRED,
  mGREEN,
  mORANGE,
  mORANGERED
};
const int mode_count = mORANGERED - mBLUE;

uint32_t ORANGERED1 = strip.Color(255, 40, 0);
uint32_t AMBER = strip.Color(255, 130, 0);
uint32_t RED1   = strip.Color(255, 0, 0);
uint32_t WHITE5 = strip.Color(255, 255, 255);
uint32_t BLUE1  = strip.Color(135, 206, 250);
uint32_t GREEN1  = strip.Color(173, 255, 47);
uint32_t OFF1   = strip.Color(0, 0, 0);

// initial mode
MODE mode = mBLUE;


void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
  colorWipeNonBlock(strip.Color(255, 255, 255), 30);
  //colorWipeNonBlock(strip.Color(0, 0, 255), 30);

  Serial.begin(115200);
  Serial.println();
  Serial.println(F("--------------------"));
  Serial.println(F(SKETCH));
  Serial.print(F("modes count= "));
  Serial.println(mode_count);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
}


void loop() {

  switch (mode)
  {

    case mBLUE:
      if (colorWipeNonBlock(strip.Color(0, 0, 255), 30) == false)
      {
        // done with colour wipe
        Serial.println("colorWipeNonBlock finished");
        delay(5000);

        // reset pixNum so other functions start from 0; only if needed
        pixNum = 0;
        mode = mRED;
      }
      break;


    case mRED:
      if (colorWipeNonBlock(strip.Color(255, 0, 0), 30) == false)
      {
        // done with colour wipe
        Serial.println("colorWipeNonBlock finished");
        delay(5000);

        // reset pixNum so other functions start from 0; only if needed
        pixNum = 0;
        mode = mGREEN;
      }
      break;

    case mGREEN:
      if (colorWipeNonBlock(strip.Color(0, 255, 0), 30) == false)
      {
        // done with colour wipe
        Serial.println("colorWipeNonBlock finished");
        delay(5000);

        // reset pixNum so other functions start from 0; only if needed
        pixNum = 0;
        mode = mORANGERED;
      }
      break;



    case mORANGERED:
      if (colorWipeNonBlock(strip.Color(255, 69, 0), 30) == false)
      {
        // done with colour wipe
        Serial.println("colorWipeNonBlock finished");
        delay(5000);

        // reset pixNum so other functions start from 0; only if needed
        pixNum = 0;
        mode = mBLUE;
      }
      break;



      // other cases here
  }
}



/**
   Helper function to make all LEDs color (c)
*/
void all(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}



void WhiteLight() {
  strip.setBrightness(BRIGHTNESS);
  all(WHITE5);
  strip.show();
}


/*
  non-blocking colorWipe
  In:
  \\\\\\\\\\\
    colour
    delay time
  Returns:
    true if wipe is in progress, else false
*/
bool colorWipeNonBlock(uint32_t c, uint8_t wait) {
  static unsigned long startTime;
  static bool inProgress = false;

  // if colorWipe is not yet in progress
  if (inProgress == false)
  {
    // indicate that it's in progress
    inProgress = true;
    // set the start time (for delay)
    startTime = millis();
  }

  // if a 'delay' period has lapsed
  if (millis() - startTime >= wait)
  {
    // set a pixelcolour
    strip.setPixelColor(pixNum, c);
    strip.show();
    // 'reset' the start time for the delay
    startTime = millis();
    // next time that 'delay' has lapsed, use next pixel
    pixNum++;
  }

  // if all pixels done
  if (pixNum == strip.numPixels())
  {
    // color wipe finished
    inProgress = false;
  }

  // indicate the status to caller
  return inProgress;
}
