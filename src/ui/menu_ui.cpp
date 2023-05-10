/****************************************************************************
Copyright (c) 2021      Yinbaiyuan

https://www.yinbaiyuan.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include <Arduino.h>

#include "../Dot2D/dtSprite.h"
#include "../Dot2D/third/gfxfont.h"
#include "menu_ui.h"


#include "menu/cmd.h"
float Voltage = 0.0;
int16_t cmd_index = 0;

NS_DT_BEGIN

int32_t MenuUiLayer::screen_H = 7;
int32_t MenuUiLayer::screen_W = 40;
int32_t MenuUiLayer::screen_char_num = 40 / 5;

bool MenuUi::init()
{
    MenuUiLayer* rootLayer = MenuUiLayer::create();
    rootLayer->setContentSize(Size(MenuUiLayer::screen_W, MenuUiLayer::screen_H));
    this->addChild(rootLayer);
    rootLayer->initLayer();
    return true;
}

MenuUiLayer::~MenuUiLayer()
{
}

bool MenuUiLayer::initLayer()
{
    draw_cmd();

    auto fadeIn = FadeIn::create(1.0f);
    m_str_sprite->runAction(fadeIn);

    // timeMenuUi->setTransparent(true);
    // m_str_sprite->setPosition(-40, 0);
    // this->addChild(m_str_sprite);
    // MoveBy *move1 = MoveBy::create(0.2, Vec2(40, 0));
    // m_str_sprite->runAction(move1);
    // this->scheduleUpdate();
    return true;
}

void MenuUiLayer::draw_cmd(void)
{
    SpriteCanvas *canvas = m_str_sprite->getSpriteCanvas();
    canvas->canvasReset();

    char ten = 48 + (int8_t)Voltage;
    char frac = 48 + (int8_t)(Voltage * 10) % 10;
    for (uint8_t x = 0; x < screen_char_num; ++x)
    {
        canvas->drawChar(x*5, 0, boot[cmd_index*screen_char_num + x],  DTRGB(100, 100, 0), DTRGB(100, 100, 100), 1);
    }
    // canvas->drawChar(0, 0, 'V', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(5, 0, 'C', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(10, 0, 'C', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(15, 0, '=', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);

    // canvas->drawChar(20, 0, ten, DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(25, 0, '.', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(30, 0, frac, DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(35, 0, 'V', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
}
void MenuUiLayer::update(float dt)
{

    draw_cmd();

    // m_str_sprite =  MenuUiSprite::create(Size(40,7), "0123456789", MenuUiSprite::MenuUiAlign::MenuUiAlignLeft, nullptr, 1);

    // m_str_sprite->setPosition(-40, 0);
    // MoveBy *move1 = MoveTo::create(0.2, Vec2((int8_t)(Voltage * 10), 0));
    // m_str_sprite->runAction(move1);


    // SpriteCanvas *canvas = m_str_sprite->getSpriteCanvas();
    // canvas->canvasReset();

    // char ten = 48 + (int8_t)Voltage;
    // char frac = 48 + (int8_t)(Voltage * 10) % 10;
    // // for (uint8_t x = 0; x < screen_char_num; ++x)
    // // {
    // //     canvas->drawChar(x*5, 0, cmd[cmd_index*screen_char_num + x],  DTRGB(100, 100, 0), DTRGB(100, 100, 100), 1);
    // // }
    // canvas->drawChar(0, 0, 'V', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(5, 0, 'C', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(10, 0, 'C', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(15, 0, '=', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);

    // canvas->drawChar(20, 0, ten, DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(25, 0, '.', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(30, 0, frac, DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);
    // canvas->drawChar(35, 0, 'V', DTRGB(100, 100, 0), DTRGB(0, 100, 0), 1);

    // auto fadeIn = FadeIn::create(1.0f);
    // m_str_sprite->runAction(fadeIn);
    // canvas->setMenuUiWrap(false); // ???

    // Size size = this->getContentSize();
    // SpriteCanvas *canvas = canvasSprite->getSpriteCanvas();
    // canvas->canvasReset();
    // unsigned char m_c = 88;
    // for (int8_t i = 0; i < 32; i++)
    // {
    //     // canvas->canvasReset();
    //     canvas->drawChar(i, 0, m_c, DTRGB(100, 0, 0), DTRGB(100, 0, 0), 1);
    // }

    // canvas->drawChar(15, 0, 88, DTRGB(100, 0, 0), DTRGB(100, 0, 0), 1);

    // MenuUiSprite *name = MenuUiSprite::create("zxl", MenuUiSprite::MenuUiAlign::MenuUiAlignScroll, NULL, 1);
    // this->addChild(name);
    // name->setAutoScroll(MenuUiSprite::ScrollType::None, 3, 0.1);
    // name->runAction();
}

NS_DT_END