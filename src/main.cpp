#include <Arduino.h>
#include <math.h>
#include <ESP32Encoder.h>
#include "OneButton.h"
#include "FastLED.h"
#include "fsm/menuFSM.h"
#include "user/cfg.h"

/*********EC11 config*************************************/
#define EC11_A_PIN 25
#define EC11_B_PIN 265
#define EC11_K_PIN 4
#define POWER_KEY_PIN 0 // to add.

/*********WS2812 LOGO*************************************/
#define LOGO_LED_PIN 5

/*********WS2812 screen parameters config*****************/
#define MATRIX_WIDTH 40
#define MATRIX_HEIGHT 7
#define MATRIX_LED_PIN 18
#define MATRIX_MAX_BRIGHTNESS 10

/**************global value*******************************/
int32_t g_lastEncoderValue = 0;
int32_t g_delta = 0;
MenuFsm* g_menuFSM = MenuFsm::getInstance();
ESP32Encoder g_spin(false); // true interrupt.
OneButton g_spinKey(EC11_K_PIN, false, false);
OneButton g_powerKey(POWER_KEY_PIN);

/***********key callback functions************************/
void click(void);
void doubleclick(void);
void longclick(void);

// callback function for power key;
void poweron()
{
    if (!g_menuFSM->get_power_state())
    {
        g_menuFSM->set_power_state(true); // g_powerKey on
    }
}

void poweroff()
{
    g_menuFSM->set_power_state(false); // g_powerKey off if long click the button any time.
}

void click()
{
    g_menuFSM->set_button_state(Menu::ButtonType::CLICK);
}

void doubleclick()
{
    g_menuFSM->set_button_state(Menu::ButtonType::D_CLICK);
}

// 按键长按回调函数
void longclick()
{
    g_menuFSM->set_button_state(Menu::ButtonType::L_CLICK);
}

void setup()
{
    Serial.begin(115200);
    // Serial.printf("ESP32 chip revision : %d\r\n", (int16_t)ESP.getChipRevision());
    // Serial.printf("ESP32 SDK Version : %s\r\n", ESP.getSdkVersion());
    // Serial.printf("ESP32 Speed : %u MHz\r\n", ESP.getCpuFreqMHz());
    // Serial.print("Task render function running on core: ");
    // Serial.println(xPortGetCoreID());

    // init ec11-g_spin.
    ESP32Encoder::useInternalWeakPullResistors = UP;
    g_spin.attachSingleEdge(EC11_A_PIN, EC11_B_PIN);

    // init ec11-button callback function.
    g_spinKey.attachClick(click);
    g_spinKey.attachDoubleClick(doubleclick);
    g_spinKey.attachLongPressStop(longclick);
    // g_spinKey.setDebounceTicks(20);//滤波(ms)
    // g_spinKey.setClickTicks(200);
    // g_spinKey.setPressTicks(500);
    
//     // init power key callback function.
    g_powerKey.attachClick(poweron);
    g_powerKey.attachLongPressStop(poweroff);

    Serial.println("#############setup done.##################\n");
}

void loop()
{
    // button input for g_powerKey select or menu select;
    g_spinKey.tick();
    
    // EC11 spin data update;
    if (g_lastEncoderValue != g_spin.getCount())
    {
        int now_count = g_spin.getCount();
        g_delta = now_count - g_lastEncoderValue; // global value
        g_lastEncoderValue = now_count;

        g_menuFSM->update_g_delta(g_delta);
        // Serial.printf("g_delta = %d \n", g_delta);
    }

    // menu FSM tick.
    g_menuFSM->FSM_tick();
    g_menuFSM->FSM_executor();

    // delay(10);
    // unsigned long start = millis(); // current (relative) time in msecs.

    // unsigned long end = millis(); // current (relative) time in msecs.
    // Serial.printf("%d \n", end - start);
}
