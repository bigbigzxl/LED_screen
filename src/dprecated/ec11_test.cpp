// /****************************************************************************
// Copyright (c) 2021      Yinbaiyuan

// https://www.yinbaiyuan.com

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// ****************************************************************************/

// #include <Arduino.h>
// #include "Dot2D/dot2d.h"
// #include "FastLED.h"
// #include "Button.h"
// #include "EC11.h"
// #include <math.h>
// #include "Examples/Matrix/Matrix.h"
// #include "Examples/TripleClick/TripleClick.h"
// #include "Examples/Snake/Snake.h"
// #include "Examples/Text/Text.h"

// /*
//  This is a sample sketch to show how to use the OneButtonLibrary
//  to detect double-click events on a button.
//  The library internals are explained at
//  http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

//  Setup a test circuit:
//  * Connect a pushbutton to pin A1 (ButtonPin) and ground.
//  * The pin 13 (StatusPin) is used for output attach a led and resistor to ground
//    or see the built-in led on the standard arduino board.

//  The sketch shows how to setup the library and bind the functions (singleClick, doubleClick) to the events.
//  In the loop function the button.tick function must be called as often as you like.
// */

// // 03.03.2011 created by Matthias Hertel
// // 01.12.2011 extension changed to work with the Arduino 1.0 environment

// #include "OneButton.h"
// void doubleClick();
// void Click();
// // Example pin assignments for a ESP32 board
// // Some boards have a BOOT switch using GPIO 0.
// #define PIN_INPUT 14
// #define PIN_ENCODER_A 15
// #define PIN_ENCODER_B 32
// // Attach a LED using GPIO 25 and VCC. The LED is on when output level is LOW.
// #define PIN_LED 27

// // Setup a new OneButton on pin PIN_INPUT
// // The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
// // OneButton button(PIN_INPUT, true);

// // In case the momentary button puts the input to HIGH when pressed:
// // The 2. parameter activeLOW is false when the external wiring sets the button to HIGH when pressed.
// // The 3. parameter can be used to disable the PullUp .
// OneButton button(PIN_INPUT, false, false);
// ZXL::EC11 encoder;
// // current LED state, staring with LOW (0)
// int ledState = LOW;

// // setup code here, to run once:
// void setup()
// {
//   Serial.begin(115200);
//   Serial.println("One Button Example with polling.");

//   // enable the standard led on pin 13.
//   pinMode(PIN_LED, OUTPUT); // sets the digital pin as output
//   pinMode(PIN_ENCODER_A, INPUT_PULLUP);
//   pinMode(PIN_ENCODER_B, INPUT_PULLUP);

//   // enable the standard led on pin 13.
//   digitalWrite(PIN_LED, ledState);

//   // link the doubleclick function to be called on a doubleclick event.
//   button.attachDoubleClick(doubleClick);
//   button.attachClick(Click);
// } // setup

// // main code here, to run repeatedly:
// void loop()
// {
//   // keep watching the push button:
//   button.tick();

//   ZXL::EC11Event e;
//   if (encoder.read(&e))
//   {
//     if (e.type == ZXL::EC11Event::Type::StepCW)
//     {
//       Serial.println("+1");
//     }
//     else
//     {
//       Serial.println("-1");
//     }
//   }

//   for (int i = 0; i < 10; i++)
//   {
//     encoder.checkPins(digitalRead(PIN_ENCODER_A), digitalRead(PIN_ENCODER_B));
//     delay(1); 
//   }
//   // You can implement other code in here or just wait a while

//   delay(40);
// } // loop

// // this function will be called when the button was pressed 2 times in a short timeframe.
// void doubleClick()
// {
//   Serial.println("x2");

//   // ledState = !ledState; // reverse the LED
//   // digitalWrite(PIN_LED, ledState);
// } // doubleClick

// void Click()
// {
//   Serial.println("x1");

//   ledState = !ledState; // reverse the LED
//   digitalWrite(PIN_LED, ledState);
// } // doubleClick