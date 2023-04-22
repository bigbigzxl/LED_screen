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

#include "Matrix.h"
#include <Arduino.h>

NS_DT_BEGIN

bool Matrix::init()
{
    MatrixLayer *rootLayer = MatrixLayer::create();
    rootLayer->setContentSize(Size(40, 7));
    this->addChild(rootLayer);
    rootLayer->initLayer();
    return true;
}

MatrixLayer::~MatrixLayer()
{
    DT_SAFE_FREE(animateList);
}

bool MatrixLayer::initLayer()
{
    Size size = this->getContentSize();
    showInterval = size.height / 12.0; // 12
    animateCount = size.width;
    animateList = (Animate *)malloc(sizeof(Animate) * animateCount);
    for (int16_t i = 0; i < animateCount; i++)
    {
        Animate &animate = animateList[i];
        animate.offset = INT16_MAX;
    }
    canvasSprite = CanvasSprite::create(size.width, size.height);
    this->addChild(canvasSprite);
    this->scheduleUpdate();

    // canvas0 = CanvasSprite::create(size.width, size.height);
    // this->addChild(canvas0, -1);
    return true;
}

void MatrixLayer::update(float dt)
{
    Size size = this->getContentSize();
    SpriteCanvas *canvas = canvasSprite->getSpriteCanvas();
    canvas->canvasReset();
    // unsigned char m_c = 88;
    // for (int8_t i = 0; i < 32; i++)
    // {
    //     // canvas->canvasReset();
    //     canvas->drawChar(i, 0, m_c, DTRGB(100, 0, 0), DTRGB(100, 0, 0), 1);
    // }

    // SpriteCanvas *canvas1 = canvas0->getSpriteCanvas();

    // canvas->drawChar(0, 0, 'V', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(5, 0, 'C', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(10, 0, 'C', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(15, 0, '=', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);

    // canvas->drawChar(20, 0, '3', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(25, 0, '.', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(30, 0, '3', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);
    // canvas->drawChar(35, 0, 'V', DTRGB(0, 100, 0), DTRGB(0, 0, 0), 1);

    // TextSprite *name = TextSprite::create("VCC=3.3V", TextSprite::TextAlign::TextAlignScroll, NULL, 1);
    // this->addChild(name);
    // name->setAutoScroll(TextSprite::ScrollType::None, 3, 0.1);

    // return;

    for (int16_t i = 0; i < animateCount; i++)
    {
        Animate &animate = animateList[i];
        if (animate.offset > size.height * 2)
        {
            animate.offset = -random(0, size.height);
            animate.interval = random(showInterval * 1, showInterval * 2) / (float)(size.height * 2);
            animate.dInterval = animate.interval;
        }
        animate.dInterval -= dt;
        if (animate.dInterval <= 0)
        {
            animate.offset++;
            animate.dInterval = animate.interval;
        }

        // Serial.print("\n");
        // Serial.print(animate.offset);
        canvas->writeGradientLine(i, animate.offset, i, animate.offset - (size.height - 1), DTHSV(100, 50, 200), DTHSV(100, 255, 0));
        canvas->writeGradientLine(i, animate.offset - size.height, i, animate.offset - (size.height * 2 - 1), DTHSV(100, 50, 200), DTHSV(100, 255, 0));
    }
}

NS_DT_END