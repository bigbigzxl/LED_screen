// #include  "display/display.h"

// Display screen = Display(18,40,7);
// CRGB text_clolr = CRGB(250,0,0);
// CRGB back_clolr = CRGB(0,30,0);
// int count = 25;
// void setup() { 
// 	Serial.begin(115200);
//     screen.setBright(10);
// 	Serial.println("starting...");
//     screen.drawstring((unsigned char*)" HIBIKI ");
// }



// void loop() { 

//     // count = 100;
   
//     // screen.render();
//     unsigned long start = millis(); // current (relative) time in msecs.
//     unsigned long end = start;
//     uint16_t pix_pos[4] = {273,274, 279,278};
//     // uint16_t pix_pos[16] = {246,247,248,254,259,
//     //                         264,269,274,278,277,
//     //                         276,270,265,260,255,
//     //                         250};
//     int i = 0;
//     while(1)//(end - start < 100000)
//     {   
//         screen.fadeOutChar(7, 180);
//         i++;
//         i %= 4;
//         screen.get_buffer()[pix_pos[i]] = CRGB::Red;
//         screen.render();
//         delay(300);
//         // end = millis(); // current (relative) time in msecs.
//     }

//     // 

//     // while(count--)
//     // {
//     //     if (count > 75)
//     //     {
//     //         // screen.fadeOutChar(5, 1);
//     //         screen.fadeOutAll(1);
//     //     }
//     //     else
//     //     {
//     //         // screen.fadeOutChar(5, 5);
//     //         screen.fadeOutAll(10);
//     //     }
//     //     //  screen.fadeOutChar(5, 10);
        
        
//         // screen.render();
//     //     //  delay(10);
//     // }

       
//     // unsigned long end = millis(); // current (relative) time in msecs.
//     // Serial.printf("%d ", end - start);
//     // delay(2000);
 

//     // for(int i = 0; i < 280; i++) {
// 	// // screen.screenReset();
//     // // screen.drawChar(0,0,48 + 1,text_clolr,back_clolr);
//     // screen.fadeall();
//     // // screen.drawstring((unsigned char*)"zxlZXL11");
   
//     // screen.render();
//     // // delay(1000);
    
//     // // screen.render();
//     // // delay(100);
//     // }


// //     int pos = random8(250);
// //   EVERY_N_MILLISECONDS( 100 ) { 
// //     if( random8() < 80) {
// //       screen.get_buffer()[pos] = ColorFromPalette( PartyColors_p, random8(255), 128, LINEARBLEND);
// //     }   
// //   }
// //   screen.fadeall();
// // //   EVERY_N_MILLISECONDS( 10 ) { 
// // //     screen.fadeall();
// // //   }    
// //   FastLED.show(); 
// // //   delay(50);       


// }