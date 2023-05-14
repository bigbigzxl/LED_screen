// #include <FastLED.h>

// FASTLED_USING_NAMESPACE

// // FastLED "100-lines-of-code" demo reel, showing just a few 
// // of the kinds of animation patterns you can quickly and easily 
// // compose using FastLED.  
// //
// // This example also shows one easy way to define multiple 
// // animations patterns and have them automatically rotate.
// //
// // -Mark Kriegsman, December 2014

// #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
// #warning "Requires FastLED 3.1 or later; check github for latest code."
// #endif

// #define DATA_PIN    18
// //#define CLK_PIN   4
// #define LED_TYPE    WS2811
// #define COLOR_ORDER GRB
// #define NUM_LEDS    280
// CRGB leds[NUM_LEDS];

// #define BRIGHTNESS          10
// #define FRAMES_PER_SECOND  60


// #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
// uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  

// void rainbow() 
// {
//   // FastLED's built-in rainbow generator
// //   fill_rainbow( leds, NUM_LEDS, gHue, 7);
//     CHSV hsv;
//     hsv.hue = gHue;
//     hsv.val = 255;
//     hsv.sat = 240;
//     for( int i = 0; i < NUM_LEDS; i++) {

//         uint16_t x = i % 40;
//         uint16_t y = i / 40;
//         uint16_t factor = floor(x / 5);
//         uint16_t order = y * 5 + x % 5 + factor * 35;
//         leds[order] = hsv;
//         hsv.hue += 7;
//     }
// }
// void addGlitter( fract8 chanceOfGlitter) 
// {
//   if( random8() < chanceOfGlitter) {
//     leds[ random16(NUM_LEDS) ] += CRGB::White;
//   }
// }
// void rainbowWithGlitter() 
// {
//   // built-in FastLED rainbow, plus some random sparkly glitter
//   rainbow();
//   addGlitter(80);
// }



// void confetti() 
// {
//   // random colored speckles that blink in and fade smoothly
//   fadeToBlackBy( leds, NUM_LEDS, 10);
//   int pos = random16(NUM_LEDS);
//   leds[pos] += CHSV( gHue + random8(64), 200, 255);
// }

// void sinelon()
// {
//   // a colored dot sweeping back and forth, with fading trails
//   fadeToBlackBy( leds, NUM_LEDS, 20);
//   int pos = beatsin16( 13, 0, NUM_LEDS-1 );
//   leds[pos] += CHSV( gHue, 255, 192);
// }

// bool f = false;
// void bpm()
// {
//   // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//   uint8_t BeatsPerMinute = 62;
//   CRGBPalette16 palette = PartyColors_p;
//   uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
//   for( int i = 0; i < NUM_LEDS; i++) { //9948
//     leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
//   }
// }

// void juggle() {
//   // eight colored dots, weaving in and out of sync with each other
//   fadeToBlackBy( leds, NUM_LEDS, 20);
//   byte dothue = 0;
//   for( int i = 0; i < 8; i++) {
//     leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
//     dothue += 32;
//   }
// }

// // List of patterns to cycle through.  Each is defined as a separate function below.
// typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = {sinelon};//confetti, juggle,  bpm, rainbow, rainbowWithGlitter, 

// void nextPattern()
// {
//   // add one to the current pattern number, and wrap around at the end
//   gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
// }

// void setup() {
//   delay(30); // 3 second delay for recovery
  
//   // tell FastLED about the LED strip configuration
// //   FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<WS2812Controller800Khz, 18, GRB>((CRGB *)leds, 280);
//   //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

//   // set master brightness control
//   FastLED.setBrightness(BRIGHTNESS);
// }


// void loop()
// {
//   // Call the current pattern function once, updating the 'leds' array
// //   gPatterns[gCurrentPatternNumber]();
//   // a colored dot sweeping back and forth, with fading trails
  
//   fadeToBlackBy(leds, NUM_LEDS, 200);
//   int pos = beatsin16( 13, 0, NUM_LEDS-1 );
//     uint16_t x = pos % 40;
//     uint16_t y = pos / 40;
//     uint16_t factor = floor(x / 5);
//     uint16_t order = y * 5 + x % 5 + factor * 35;
//     leds[order] += CHSV( gHue, 255, 192);
// // f = !f;
// // if (f)
// // {leds[0] += CHSV( gHue, 255, 192);}
// // else
// // {leds[1] += CHSV( gHue, 255, 192);}
  

//   // send the 'leds' array out to the actual LED strip
//   FastLED.show();  
//   // insert a delay to keep the framerate modest
// //   FastLED.delay(1000); 

//   // do some periodic updates
// //   EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
// //   EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
// }