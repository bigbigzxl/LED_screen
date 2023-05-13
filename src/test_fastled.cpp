// #include <Arduino.h>
// #include <FastLED.h>       //头文件

// // How many leds in your strip?
// #define NUM_LEDS 40*7        //灯珠数目

// // For led chips like Neopixels, which have a data line, ground, and power, you just
// // need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// // ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// #define DATA_PIN 18             //Neopixels 灯珠所在信号线
// //#define CLOCK_PIN 13        

// // Define the array of leds
// CRGB leds[NUM_LEDS];          //LED参数数组
// //HSV方法定义颜色  
// CHSV myHSVcolor(80,255,200);    //HSV方法定义颜色  myHSVcolor（色调，饱和度，明亮度）

// void setup() 
// { 
    //  Serial.begin(115200);
//       FastLED.addLeds<WS2812Controller800Khz, DATA_PIN, GRB>((CRGB *)leds, 40*7);
//       FastLED.setBrightness(10);
//       Serial.println("start \n");
//     //   FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  //WS2812B灯珠选这个
// }

// void loop() 
// { 
//   //  演示如何使用.h方法改变CHSV颜色的某一个数值
// //   myHSVcolor.h++;  // 修改HSV定义颜色的单一数值
//                    // myHSVcolor.h为myHSVcolor的色调数值
//            HeatColor(100);        
// //    fill_solid(leds, NUM_LEDS, myHSVcolor);  
// //    FastLED.show();    
// //  delay(1000); 
// //   fadeToBlackBy(leds, NUM_LEDS, 200);
//   FastLED.show();    
//    delay(1000); 
// // fill_rainbow(leds, NUM_LEDS,0,5);
// //    int i = 0;
// //    while (i < 255)
// //    {
// //     fadeLightBy(leds, NUM_LEDS, i);   
// //     unsigned long start = millis(); // current (relative) time in msecs.

// //     FastLED.show();

// //     unsigned long end = millis(); // current (relative) time in msecs.
// //     Serial.printf("%d \n", end - start);
// //     delay(100);  
// //     i++;
// //    }
//   delay(10);     
// }
