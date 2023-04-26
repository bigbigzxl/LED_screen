// #include "FastLED.h"            // 此示例程序需要使用FastLED库

// #define NUM_LEDS 1              // LED灯珠数量
// #define DATA_PIN 5              // Arduino输出控制信号引脚
// #define LED_TYPE WS2812         // LED灯带型号
// #define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序

// uint8_t max_bright = 128;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高

// CRGB leds[NUM_LEDS];            // 建立光带leds

// void setup() { 
//   // LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带 
//   FastLED.addLeds<WS2812Controller800Khz, NUM_LEDS, GRB>((CRGB *)leds, 1 * 1);
//   FastLED.setBrightness(max_bright);                            // 设置光带亮度

// //   pinMode(34, OUTPUT);   // 开启引脚输出模式（有个别引脚不可以设置输出模式，在文章后面会提到）
// //   digitalWrite(34,HIGH);  //输出高电平，LOW就是低电平

//    Serial.begin(115200);
// }

// void loop() { 
// //   leds[0] = CRGB::Red;          // 设置光带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
// //   FastLED.setBrightness(max_bright++);
// //   FastLED.show();                // 更新LED色彩

//   delay(500);                     // 等待500毫秒
//   max_bright++;
//   if (max_bright % 2 == 0)
//   {
//     digitalWrite(5,HIGH);  //输出高电平，LOW就是低电平
//     Serial.printf("-----HIGHT-----\n");

//   }
//   else
//   {
//      digitalWrite(5,LOW);  //输出高电平，LOW就是低电平
//      Serial.printf("-----LOW-----\n");
//   }
  
// }


#include <Arduino.h>
#include "OneButton.h"
#include <ESP32Encoder.h>
//按键配置
#define EC11_A_PIN 25
#define EC11_B_PIN 26
#define EC11_K_PIN 4//按键引脚

ESP32Encoder encoder;
OneButton SW(EC11_K_PIN, false, false);

int lastEncoderValue = 0;
int now_count = 0;
int mode = 0;   //0,1两种mode,可自行添加
bool activate = true;

//按键单击回调函数
void click() {
    if (mode == 0) {
        Serial.println("Click: MUTE");
       
    }
    if (mode == 1) {
        Serial.println("Enter");
       
    }
}

//按键长按回调函数
void longclick() {
    if (activate) {   //如果旋钮转动，则不切换状态
        Serial.println("Longclick: Mode Change");
        Serial.print("current mode: ");
        if (mode == 0) {
            mode = 1;
            Serial.println("Arrow");
            return;
        }
        if (mode == 1) {
            mode = 0;
            Serial.println("Media");
            return;
        }
    }
    activate = true;
}

//按键双击回调函数
void doubleclick() {
    if (mode == 0) {
         Serial.println("Doubleclick: Input test");
   		 
    }
    if (mode == 1) {
         Serial.println("Doubleclick: Input test");
    	
    }
}

void setup() {
    Serial.begin(115200);
    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoder.attachSingleEdge(EC11_A_PIN, EC11_B_PIN);

    // pinMode(EC11_K_PIN, INPUT_PULLUP);

    //初始化按键事件检测
    SW.attachClick(click);
    // SW.attachDoubleClick(doubleclick);
    // SW.attachLongPressStop(longclick);
    // SW.setDebounceTicks(20);//滤波(ms)
    // SW.setClickTicks(200);
    // SW.setPressTicks(500);

    Serial.println("Starting BLE work");

}

void loop() {
    // vTaskDelay(30 / portTICK_PERIOD_MS);
    ets_delay_us(30*1000); 
    
    SW.tick();

    if (1) {
        if (lastEncoderValue != encoder.getCount()) {
            now_count = encoder.getCount();
            if (now_count != lastEncoderValue) {
                if (!SW.isIdle()) {         //检测按键是否空闲
                    activate = false;
                    Serial.print("(Long_pressed)Encoder value: ");
                    Serial.println(now_count);
                } else {
                    Serial.print("Encoder value: ");
                    Serial.println(now_count);
                }
            }

            if (now_count > lastEncoderValue) {
                if (!SW.isIdle()) {         //检测按键是否空闲
                    if (mode == 0) {}     //模式0按钮按下顺时针功能
                    if (mode == 1) {		//模式1按钮按下顺时针功能
                        // bleKeyboard.write(KEY_DOWN_ARROW);
                        Serial.println("DOWN_ARROW");
                    }
                } else {
                    if (mode == 0) {	//模式0顺时针功能
                        // bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
                        Serial.println("MEDIA_VOLUME_UP");
                    }
                    if (mode == 1) {	//模式1顺时针功能
                        // bleKeyboard.write(KEY_RIGHT_ARROW);
                        Serial.println("RIGHT_ARROW");
                    }
                }
            }
            if (now_count < lastEncoderValue) {
                if (!SW.isIdle()) {         //检测按键是否空闲
                    if (mode == 0) {}    //模式0按钮按下逆时针功能
                    if (mode == 1) {	//模式1按钮按下逆时针功能
                        // bleKeyboard.write(KEY_UP_ARROW);
                        Serial.println("UP_ARROW");
                    }
                } else {
                    if (mode == 0) {	//模式0逆时针功能
                        // bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
                        Serial.println("MEDIA_VOLUME_DOWN");
                    }
                    if (mode == 1) {	//模式1逆时针功能
                        // bleKeyboard.write(KEY_LEFT_ARROW);
                        Serial.println("LEFT_ARROW");
                    }
                }
            }
            lastEncoderValue = now_count;
        }
    }
}
