/*
 * Seeed_ws2812.h
 * Arduino library for RGB led WS2812
 *  
 * Copyright (c) 2017 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : lambor
 * Create Time: April 2017
 * Change Log :
 *
 * The MIT License (MIT)
  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef SEEED_WS2812_h
#define SEEED_WS2812_h

#include <Arduino.h>
#include <stdlib.h>

class WS2812 {
	
	public:
		WS2812(uint32_t ledn, uint8_t sigPin);
		void begin(void);
		uint32_t getLedNum(void);
		void setLedNum(uint32_t);
		void WS2812Clear(void);
		void pureColor(uint8_t);
		void WS2812SetRGB(uint32_t led, uint8_t red,
		uint8_t green, uint8_t blue);
		void WS2812SetRGB(uint32_t led, uint8_t red, 
		uint8_t green, uint8_t blue, uint8_t light);
		void WS2812Send(void);
		void WS2812SetHSV(uint32_t led, uint32_t hue, 
		uint32_t saturation, uint32_t value);
		void rainbowCycle(uint8_t wait);
		void colorWheel(byte WheelPos, uint8_t n);
		void RGBCycle(uint16_t);
		
		uint8_t sigPin;
		uint32_t ledNum;
		uint8_t brightness = 255;
		uint8_t *WS2812Buffer;

};

#endif
