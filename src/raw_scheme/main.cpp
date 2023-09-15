// #include <Arduino.h>
// #include <math.h>
// #include <ESP32Encoder.h>
// #include "OneButton.h"
// #include "FastLED.h"

// #include "menu/menuFSM.h"
// #include "cmd/cmd.h"

// // EC11 config
// #define EC11_A_PIN 25
// #define EC11_B_PIN 26
// #define EC11_K_PIN 4
// #define POWER_KEY_PIN 0 // to add.

// // WS2812 LOGO
// #define LOGO_LED_PIN 5

// // WS2812 config
// #define MATRIX_WIDTH 40
// #define MATRIX_HEIGHT 7
// #define MATRIX_LED_PIN 18
// #define MATRIX_MAX_BRIGHTNESS 10

// int32_t lastEncoderValue = 0;
// int32_t delta = 0;

// ESP32Encoder spin(false); // true interrupt.
// OneButton spin_key(EC11_K_PIN, false, false);
// OneButton power_key(POWER_KEY_PIN);

// Menu* menuFSM = Menu::getInstance();

// void click(void);
// void doubleclick(void);
// void longclick(void);


// // callback function for power key;
// void poweron()
// {
//     if (!menuFSM->get_power_state())
//     {
//         menuFSM->set_power_state(true); // power_key on
//     }
// }

// void poweroff()
// {
//     menuFSM->set_power_state(false); // power_key off if long click the button any time.
// }

// void click()
// {
//     menuFSM->set_button_state(Menu::ButtonType::CLICK);
// }

// void doubleclick()
// {
//     menuFSM->set_button_state(Menu::ButtonType::D_CLICK);
// }

// // 按键长按回调函数
// void longclick()
// {
//     menuFSM->set_button_state(Menu::ButtonType::L_CLICK);
// }

// void setup()
// {
//     Serial.begin(115200);
//     // Serial.printf("ESP32 chip revision : %d\r\n", (int16_t)ESP.getChipRevision());
//     // Serial.printf("ESP32 SDK Version : %s\r\n", ESP.getSdkVersion());
//     // Serial.printf("ESP32 Speed : %u MHz\r\n", ESP.getCpuFreqMHz());
//     // Serial.print("Task render function running on core: ");
//     // Serial.println(xPortGetCoreID());

//     // init ec11-spin.
//     ESP32Encoder::useInternalWeakPullResistors = UP;
//     spin.attachSingleEdge(EC11_A_PIN, EC11_B_PIN);

//     // init ec11-button callback function.
//     spin_key.attachClick(click);
//     spin_key.attachDoubleClick(doubleclick);
//     spin_key.attachLongPressStop(longclick);
//     // spin_key.setDebounceTicks(20);//滤波(ms)
//     // spin_key.setClickTicks(200);
//     // spin_key.setPressTicks(500);
    
//     // init power key callback function.
//     power_key.attachClick(poweron);
//     power_key.attachLongPressStop(poweroff);

//     Serial.println("#############setup done.##################\n");
// }

// void loop()
// {
//     // button input for power_key select or menu select;
//     spin_key.tick();
    
//     // EC11 spin data update;
//     if (lastEncoderValue != spin.getCount())
//     {
//         int now_count = spin.getCount();
//         delta = now_count - lastEncoderValue; // global value
//         lastEncoderValue = now_count;

//         menuFSM->update_delta(delta);
//         // Serial.printf("delta = %d \n", delta);
//     }

//     // menu FSM tick.
//     menuFSM->tick();

//     // delay(10);
//     // unsigned long start = millis(); // current (relative) time in msecs.

//     // unsigned long end = millis(); // current (relative) time in msecs.
//     // Serial.printf("%d \n", end - start);
// }
