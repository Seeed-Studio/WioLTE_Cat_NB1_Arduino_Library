/*
 * Seeed_ws2812.cpp
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

#include "Seeed_ws2812.h"

#define STM32F40xx (1)

#define nop() __asm__ __volatile__ ("nop")
#define _DELAY_NOP(x)  for(uint32_t i = 0; i < x; i++){nop();}

#if(STM32F40xx == 1)
#define PARTEN_0_CODE(pin)  do{ \
								digitalWrite(pin, HIGH); \
								_DELAY_NOP(21) \
								digitalWrite(pin, LOW); \
								_DELAY_NOP(20) \
							} while(0)

#define PARTEN_1_CODE(pin) do{ \
								digitalWrite(pin, HIGH); \
								_DELAY_NOP(8) \
								digitalWrite(pin, LOW); \
								_DELAY_NOP(25) \
						   } while(0)
#endif

WS2812::WS2812(uint32_t ledn, uint8_t pin) {
	ledNum = ledn;
	sigPin = pin;
	WS2812Buffer = (uint8_t *)malloc(ledNum * 3);
	
}

void WS2812::begin(void) {
	pinMode(sigPin, OUTPUT);
	WS2812Clear();
	WS2812Send();
	
}
uint32_t WS2812::getLedNum(void) {
	return ledNum;
}

void WS2812::setLedNum(uint32_t lednum) {
	ledNum = lednum;
}

void WS2812::WS2812Clear(void) {
	uint32_t i;
	for(i = 0; i < ledNum*3; i++) WS2812Buffer[i] = 0;
	WS2812Send();
}

void WS2812::pureColor(uint8_t mode) {
	switch ( mode ) {
    case 0:
		for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 0, 0, 255, brightness);
		}
		break;
    case 1:
		for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 0, 255, 0, brightness);
		}
		break;
    case 2:
		for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 255, 0, 0, brightness);
		}
		break;   
    default: break;  
  } 
	
}

void WS2812::WS2812SetHSV(uint32_t led, uint32_t hue, 
							uint32_t saturation, uint32_t value) {	
	
	if(hue < 1536 && saturation < 256 && value < 256)
	{
		uint8_t red, green, blue;
		uint8_t min, max, inc, dec, hquot, hrem;
		
		if(saturation == 0)
		{
			WS2812SetRGB(led, value, value, value, brightness);
			return;
		}
		
		hquot = hue / 256;
		hrem = hue % 256;
		
		max = value;
		min = (value * (255 - saturation)) / 255;
		inc = (value * ((saturation * hrem) / 255)) / 255;
		dec = (value * ((saturation * (255-hrem)) / 255)) / 255;
		
		
		switch (hquot)
		{
		case 0:
			red = max;
			green = inc;
			blue = min;
			break;
		case 1:
			red = dec;
			green = max;
			blue = min;
			break;
		case 2:
			red = min;
			green = max;
			blue = inc;
			break;
		case 3:
			red = min;
			green = dec;
			blue = max;
			break;
		case 4:
			red = inc;
			green = min;
			blue = max;
			break;
		case 5:
			red = max;
			green = min;
			blue = dec;
			break;
		}
		WS2812SetRGB(led, red, green, blue);
	}
}

void WS2812::WS2812SetRGB(uint32_t led, uint8_t red, 
							uint8_t green, uint8_t blue) {
	WS2812Buffer[led*3] = green;
	WS2812Buffer[1+led*3] = red;
	WS2812Buffer[2+led*3] = blue;
}

void WS2812::WS2812SetRGB(uint32_t led, uint8_t red, 
							uint8_t green, uint8_t blue, uint8_t brightness) {
	WS2812Buffer[led*3] = green * brightness / 256;
	WS2812Buffer[1+led*3] = red * brightness / 256;
	WS2812Buffer[2+led*3] = blue * brightness / 256;
}


void WS2812::WS2812Send(void) {	
	uint32_t c;
	static uint32_t endTime = 0;  
	while(micros() - endTime < 50L);
	
	noInterrupts();
	
	for(c = 0; c < (ledNum * 3); c++)
	{	
		for( int b = 7; b >= 0; b--)		
		{					
			// 1 code
			if(WS2812Buffer[c] & (0x01<<b))
			{
				PARTEN_0_CODE(sigPin);
			} 
			// 0 code
			else
			{
				PARTEN_1_CODE(sigPin);
			}		
		}		
	}
	interrupts();
	endTime	= micros();
}	

void WS2812::rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) { 
    for(i=0; i< getLedNum(); i++) {
      colorWheel((i+j) & 0xff, i);
    }
    WS2812Send();
    delay(wait);
  }
}

void WS2812::colorWheel(byte WheelPos, uint8_t n) {
  if(WheelPos < 85) {
     WS2812SetRGB(n, 0, WheelPos * 3, 255 - WheelPos * 3, brightness);     
  } else if(WheelPos < 170) {
     WheelPos -= 85;
	 WS2812SetRGB(n, WheelPos * 3, 255 - WheelPos * 3, 0, brightness);     
  } else {
     WheelPos -= 170;
     WS2812SetRGB(n, 255 - WheelPos * 3, 0, WheelPos * 3, brightness);
  }  
}	

void WS2812::RGBCycle(uint16_t wait_ms) {
	for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 0, 0, 255, brightness);
	}
	WS2812Send();
	delay(wait_ms);
	for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 0, 255, 0, brightness);
	}
	WS2812Send();
	delay(wait_ms);
	for ( uint32_t i=0; i<ledNum; i++ ) {
			WS2812SetRGB(i, 255, 0, 0, brightness);
	}
	WS2812Send();
	delay(wait_ms);
}
