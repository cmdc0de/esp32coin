/*
 * app.cpp
 *
 * Author: cmdc0de
 */

#include "app.h"
#include <esp_log.h>
#include <system.h>
#include <spibus.h>

#include <driver/uart.h>
#include <driver/gpio.h>
#include "device/leds/apa102.h"
#include "spibus.h"
#include "freertos.h"
#include "fatfsvfs.h"
#include "pinconfig.h"

using libesp::ErrorType;
using libesp::System;
using libesp::FreeRTOS;
using libesp::RGBB;
using libesp::APA102c;
using libesp::SPIBus;

const char *MyApp::LOGTAG = "AppTask";
const char *MyApp::sYES = "Yes";
const char *MyApp::sNO = "No";


const char *MyErrorMap::toString(int32_t err) {
	return "TODO";
}

MyApp MyApp::mSelf;

MyApp &MyApp::get() {
	return mSelf;
}

MyApp::MyApp() : AppErrors(), CurrentMode(ONE), LastTime(0) {
	ErrorType::setAppDetail(&AppErrors);
}

MyApp::~MyApp() {

}
  
static RGBB leds[14];
static size_t NumLEDs = sizeof(leds)/sizeof(leds[0]);
libesp::APA102c LedControl;

libesp::ErrorType MyApp::onInit() {
	ErrorType et;
	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

  et = APA102c::initAPA102c(PIN_NUM_LEDS_MOSI, PIN_NUM_lEDS_CLK, VSPI_HOST, 1);
  if(!et.ok()) {
    return et;
  } else {
    ESP_LOGI(LOGTAG,"APA102c inited");
  }

	ESP_LOGI(LOGTAG,"OnInit: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());

	SPIBus *bus = SPIBus::get(VSPI_HOST);
  et = LedControl.initDevice(bus);

  if(et.ok()) {
    for(int i=0;i<NumLEDs;++i) {
      leds[i].setBlue(255);
      leds[i].setBrightness(100);
    }
    LedControl.init(NumLEDs, &leds[0]);
    LedControl.send();
    ESP_LOGI(LOGTAG,"OnInit:After Send: Free: %u, Min %u", System::get().getFreeHeapSize(),System::get().getMinimumFreeHeapSize());
  } else {
    ESP_LOGI(LOGTAG,"failed initDevice");
  }

	

	return et;
}

static bool Down = true;

ErrorType MyApp::onRun() {
#if 0
		  return ErrorType();
#else
  ErrorType et;
  uint32_t timeSinceLast = FreeRTOS::getTimeSinceStart()-LastTime;
  if(timeSinceLast>=TIME_BETWEEN_PULSES) {
    LastTime = FreeRTOS::getTimeSinceStart();
    switch(CurrentMode) {
    case ONE:
      {
        for(int i=0;i<NumLEDs;++i) {
#if 0
          int16_t n = leds[i].getBrightness();
          if(Down) n-=10;
          else n+=10;
          if(n>=100) Down = true;
          else if(0<=n) Down = false;

          leds[i].setBrightness(uint8_t(n&0xFF));
#else
          leds[i].setBlue(0);
          leds[i].setBrightness(50);
          leds[i].setGreen(255);
#endif
        }

        LedControl.init(NumLEDs, &leds[0]);
        LedControl.send();
        CurrentMode = TWO;
      }
      break;
    case TWO:
      {
        for(int i=0;i<NumLEDs;++i) {
          leds[i].setBrightness(10);
          leds[i].setGreen(0);
          leds[i].setRed(255);
        }
        LedControl.init(NumLEDs, &leds[0]);
        LedControl.send();
        CurrentMode = THREE;
      }
      break;
    case THREE:
      {
        for(int i=0;i<NumLEDs;++i) {
          leds[i].setBrightness(5);
          leds[i].setGreen(0);
          leds[i].setRed(0);
          leds[i].setBlue(255);
        }
        LedControl.init(NumLEDs, &leds[0]);
        LedControl.send();
      }
      break;
    }
  }
	return et;
#endif
}


