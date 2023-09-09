
// // Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
// #define qsubd(x, b)  ((x>b)?b:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
// #define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

// #include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

// #if FASTLED_VERSION < 3001000
// #error "Requires FastLED 3.1 or later; check github for latest code."
// #endif

// // Fixed definitions cannot change on the fly.
// #define LED_DT 12                                             // Serial data pin for WS2812 or WS2801.
// #define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
// #define COLOR_ORDER BGR                                       // Are they GRB for WS2812 and GBR for APA102
// #define LED_TYPE APA102                                       // What kind of strip are you using? WS2812, APA102. . .
// #define NUM_LEDS 280                                           // Number of LED's.

// uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

// struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// // Initialize changeable global variables. Play around with these!!!
// int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
// uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
// int thisphase = 0;                                            // Phase change value gets calculated.
// uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
// int thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
// uint8_t bgclr = 0;                                            // A rotating background colour.
// uint8_t bgbright = 10;                                        // Brightness of background colour

// // Palette definitions
// CRGBPalette16 currentPalette;
// CRGBPalette16 targetPalette;
// TBlendType    currentBlending;

// uint16_t map(uint16_t index)
// {
//     uint8_t x;
//     uint8_t y;
//     x = index % 40;
//     y = index / 40;
//     uint16_t factor = floor(x / 5);
//     uint16_t order = y * 5 + x % 5 + factor * 35;
//     return order;
// }

// void one_sine_pal(uint8_t colorIndex) {                                       // This is the heart of this program. Sure is short.
  
//   thisphase += thisspeed;                                                     // You can change direction and speed individually.
  
//   for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
//     int pos = map(k);
//     int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
//     leds[pos] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
//     // leds[pos] += ColorFromPalette( currentPalette, colorIndex, thisbright, currentBlending);    // Let's now add the foreground colour.
//     colorIndex +=3;

//   }
  
//   bgclr++;
  
// } // one_sine_pal()

// void setup() {

//   Serial.begin(115200);                                        // Initialize serial port for debugging.
//   delay(1000);                                                // Soft startup to ease the flow of electrons.
 
// //   LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   //WS2801 and APA102
// //  LEDS.addLeds<LED_TYPE, LED_DT,COLOR_ORDER>(leds, NUM_LEDS);   // WS2812
// FastLED.addLeds<WS2812Controller800Khz, 18, GRB>((CRGB *)leds, 40*7).setCorrection(0xFFB0F0); 

//   FastLED.setBrightness(max_bright);

//   FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);               // FastLED Power management set at 5V, 500mA.

//   currentPalette = LavaColors_p;
//   currentBlending = LINEARBLEND;
  
// } // setup()



// void loop () {

// //  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
//   EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
//     one_sine_pal(millis()>>4);
//   }

//   EVERY_N_MILLISECONDS(100) {
//     uint8_t maxChanges = 24; 
//     nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
//   }

//   EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
//     static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
//     targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
//   }
  
//   FastLED.show();

// } // loop()


