#include <Arduino.h>
#include <math.h>
#include <ESP32Encoder.h>
#include "OneButton.h"
#include "FastLED.h"

#include "Dot2D/dot2d.h"
#include "Examples/Matrix/Matrix.h"
#include "Examples/TripleClick/TripleClick.h"
#include "Examples/Snake/Snake.h"
#include "Examples/Text/Text.h"

#include "menu/menu.h"

// EC11 config
#define EC11_A_PIN 25
#define EC11_B_PIN 26
#define EC11_K_PIN 4
#define POWER_KEY_PIN 0 // to add.

// WS2812 LOGO
#define LOGO_LED_PIN 5

// WS2812 config
#define MATRIX_WIDTH 40
#define MATRIX_HEIGHT 7
#define MATRIX_LED_PIN 18
#define MATRIX_MAX_BRIGHTNESS 10


int lastEncoderValue = 0;
int delta = 0;
extern float Voltage;
extern int16_t cmd_index;

ESP32Encoder spin(true); // true interrupt.
OneButton spin_key(EC11_K_PIN, false, false);
OneButton power(POWER_KEY_PIN);
Menu menu;

dot2d::Director *director = nullptr;

void click(void);
void doubleclick(void);
void longclick(void);


void poweron()
{
  if (!menu.get_power_state())
  {
    menu.set_power_state(true); // power on
  }  
}

void poweroff()
{
  menu.set_power_state(false); // power off
}


void click()
{
  //   Serial.println("double clicked.");
  // director->runWithScene(dot2d::Text::create());
  menu.set_button_state(Menu::ButtonType::CLICK);
}

void doubleclick()
{
  // director->runWithScene(dot2d::TripleClick::create());
  menu.set_power_state(!menu.get_power_state()); // for test.
  Serial.println("double clicked. %d", menu.get_power_state());

  menu.set_button_state(Menu::ButtonType::D_CLICK);
}

// 按键长按回调函数
void longclick()
{
  //  director->runWithScene(dot2d::Snake::create());
  menu.set_button_state(Menu::ButtonType::L_CLICK);
}

void callback_boot()
{
  director->runWithScene(dot2d::Text::create()); // power on LOGO render.
}

void callback_home()
{
  director->runWithScene(dot2d::Matrix::create()); // power on LOGO render.
}

void callback_spin_menu()
{
  director->runWithScene(dot2d::Matrix::create()); // power on LOGO render.
}

void callback_spin_cmd()
{
  director->runWithScene(dot2d::Matrix::create()); // power on LOGO render.
}


// 必须要实现的dot2d导演对象代理方法
class MainDelegate : public dot2d::DirectorDelegate
{

  void render()
  {
    FastLED.show();
    // 输出ESP32内存占用情况
    // Serial.printf("-----Free Heap Mem : %d [%.2f%%]-----\n",
    //               ESP.getFreeHeap(),
    //               ESP.getFreeHeap() / (double)ESP.getHeapSize() * 100);
    // Serial.printf("-----Free PSRAM Mem: %d [%.2f%%]-----\n",
    //               ESP.getFreePsram(),
    //               ESP.getFreePsram() / (double)ESP.getPsramSize() * 100);
  }

  size_t write(uint8_t c)
  {
    return Serial.write(c);
  }

  void initMatrix(dot2d::DTRGB *data)
  {
    FastLED.addLeds<WS2812Controller800Khz, MATRIX_LED_PIN, GRB>((CRGB *)data, MATRIX_WIDTH * MATRIX_HEIGHT);
  }

  uint32_t dotOrder(uint16_t x, uint16_t y)
  {
    uint16_t factor = floor(x / 5);
    uint32_t order = y * 5 + x % 5 + factor * 35;
    return order;
  }
};

void setup()
{
  Serial.begin(115200);

  FastLED.setBrightness(MATRIX_MAX_BRIGHTNESS);

  ESP32Encoder::useInternalWeakPullResistors = UP;
  spin.attachSingleEdge(EC11_A_PIN, EC11_B_PIN);

  // init ec11-button.
  spin_key.attachClick(click);
  spin_key.attachDoubleClick(doubleclick);
  spin_key.attachLongPressStop(longclick);
  // spin_key.setDebounceTicks(20);//滤波(ms)
  // spin_key.setClickTicks(200);
  // spin_key.setPressTicks(500);

  power.attachClick(poweron);
  power.attachLongPressStop(poweroff);

  // init engine and init  canvas.
  director = dot2d::Director::getInstance();            // get director object.
  director->setDelegate(new MainDelegate());            // set director delegate.
  director->setFramesPerSecond(30);                     // set FPS.
  director->initDotCanvas(MATRIX_WIDTH, MATRIX_HEIGHT); // init director canvas.
  
  menu.attachBoot(callback_boot);
  menu.attachHome(callback_home);
  menu.attachSpinMenu(callback_spin_menu);
  menu.attachSpinCmd(callback_spin_cmd);
    
  Serial.println("Starting work");
}

void loop()
{
  // vTaskDelay(10);

  spin_key.tick();


  if (lastEncoderValue != spin.getCount())
  {
    int now_count = spin.getCount();
    delta = now_count - lastEncoderValue;
    lastEncoderValue = now_count;
    menu.update_delta(delta);

    // text_ptr->rootLayer->next_cmd(delta);

    // Voltage += delta * 0.1;
    // if (Voltage >= 3.3)
    // {
    //   Voltage = 3.3;
    // }
    // else if (Voltage <= 0)
    // {
    //   Voltage = 0.0;
    // }

    // cmd_index += delta;
    // cmd_index = cmd_index % 2;

    // if (!spin_key.isIdle()) {         //检测按键是否空闲
    //     // Serial.print("(Long_pressed)Encoder value: ");
    //     // Serial.println(now_count);
    // } else {
    //     Serial.print("Encoder value: ");
    //     Serial.println(now_count);
    // }

    // lastEncoderValue = now_count;
  }

  menu.tick();

  director->mainLoop();
}
