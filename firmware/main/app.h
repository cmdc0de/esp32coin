/*
 * app.h
 *
 * Author: cmdc0de
 */

#ifndef LIGHTBOX_APP_H
#define LIGHTBOX_APP_H

#include <app/app.h>

namespace libesp {
class GUI;
class DisplayDevice;
class DisplayMessageState;
class XPT2046;
};

enum ERRORS {
	APP_OK = libesp::ErrorType::APP_OK
	, OTA_INIT_FAIL = libesp::ErrorType::APP_BASE + 1
	, BT_INIT_FAIL
	, GAME_TASK_INIT_FAIL
	, EXPLOIT_TASK_INIT_FAIL
	, WIFI_TASK_INIT_FAIL
	, BUTTON_INIT_FAIL
	, TOP_BOARD_INIT_FAIL
};

class MyErrorMap : public libesp::IErrorDetail {
public:
	virtual const char *toString(int32_t err);
	virtual ~MyErrorMap() {}
};

class MyApp : public libesp::App {
public:
  enum MODE {
    ONE,
    TWO
    , THREE
  };
public:
	static const char *LOGTAG;
	//static const int QUEUE_SIZE = 10;
	//static const int ITEM_SIZE = sizeof(MyAppMsg);
	static const char *sYES;
	static const char *sNO;
  static const uint32_t TIME_BETWEEN_PULSES = 1000;
	static MyApp &get();
public:
	virtual ~MyApp();

protected:
	MyApp();
	virtual libesp::ErrorType onInit();
	virtual libesp::ErrorType onRun();
private:
	MyErrorMap AppErrors;
  MODE CurrentMode;
  uint32_t LastTime;
private:
	static MyApp mSelf;
};

#endif /* DC27_APP_H */
