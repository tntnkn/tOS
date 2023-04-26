#ifndef MYLIB_SENTRY_H
#define MYLIB_SENTRY_H

#include <WiFi.h>

class MyLib {
  public:

    inline static TTGOClass *watch = NULL;
    inline static TFT_eSPI *tft = NULL;
    inline static AXP20X_Class *power = NULL;

    enum WATCH_STATE { WOKE, SLEEP, UNINIT};
    inline static WATCH_STATE watch_state = UNINIT;

    static TTGOClass *
    initWatch(void) {
        watch = TTGOClass::getWatch();
        watch->begin();  
        watch->openBL();

        watch_state = WOKE;
        power = watch->power;
        tft   = watch->tft;

        power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | 
                          AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, 
                          true);

        watch->tft->fillScreen(TFT_BLACK); 
  
            
        return watch;
    }

    static void 
    syncTime() {
        // WiFi settings ******* Use your network values **********
        const char* ssid     = "Igor";
        const char* password = "qpzm1029";

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {}
        
        const char* ntpServer = "pool.ntp.org";
        const long  gmtOffset_sec = 3439;
        const int   daylightOffset_sec = 3600;

        configTime(3 * 3600, 0, "pool.ntp.org");
        delay(3000);
        
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
          tft->drawString("Failed",  5, 30, 1);
          //TODO: DO SMTH!!!
        }   
        watch->rtc->setDateTime(timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                          
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }

    static void
    showClock() {
        static RTC_Date prev_date;
        static byte hx = 0;
        static byte hy = 30;
        static byte mx = 0;
        static byte my = 70;
        static byte dx = 0;
        static byte dy = 160;
        static byte qx = 0;
        static byte qy = 120;
         
        static signed char pm = 0;
        
#include "appClockStr.h"

        RTC_Date cur_date = watch->rtc->getDateTime();

        if(prev_date.minute  != cur_date.minute) 
        {
          watch->tft->setTextColor(TFT_BLACK); 
          
          watch->tft->setTextSize(4); 
          tft->drawString(clockStr[prev_date.hour], hx, hy);  
          watch->tft->setTextSize(3);
          tft->drawString(clockStr[prev_date.minute], mx, my);  
          
//TODO: when it is 00:XX then hour is not shown!
          if        (cur_date.hour == 12 && cur_date.minute == 0) { 
            cur_date.hour = 60; pm = 0;
          } else if (cur_date.hour == 0  && cur_date.minute == 0) {
            cur_date.hour = 61; pm = 0;
          } else {
            pm = cur_date.hour - 12; cur_date.hour = cur_date.hour % 12;
          }
          
          if     (pm > 0) watch->tft->setTextColor(TFT_RED);
          else if(pm < 0) watch->tft->setTextColor(TFT_BLUE);
          else            watch->tft->setTextColor(TFT_WHITE);
           
          watch->tft->setTextSize(4); 
          tft->drawString(clockStr[cur_date.hour], hx, hy);  
          //watch->tft->setTextColor(TFT_WHITE); 
          watch->tft->setTextSize(3);
          tft->drawString(clockStr[cur_date.minute], mx, my); 
          //tft->drawString(clockDay[cur_date.day],    qx, qy); 
 /*
          watch->tft->setTextSize(2);
          tft->setCursor(dx, dy);
          tft->print(cur_date.day);
          tft->print("/");
          tft->print(cur_date.month);
          tft->print("/");
          tft->print(cur_date.year);
 */      
          prev_date.minute = cur_date.minute;
          prev_date.hour   = cur_date.hour;
        }
    }

    static void
    testText(void) {
        watch->tft->setTextColor(TFT_WHITE); 
        watch->tft->setTextFont(2); 
        watch->tft->setTextSize(2);   
        watch->tft->setCursor(0, 0); 
        watch->tft->println(F("Hello World!"));
    }

    static void
    testAwakeMain() {
      constexpr const char *str        = "main awakened: %i";
      constexpr int mes_l        = strlen(str) + 1 + 1;
      static char mes[mes_l];
      
      static int wakes = 0;
      
      snprintf(mes, mes_l, str, wakes);
      watch->tft->setTextFont(2); 
      watch->tft->setTextSize(2);   
      watch->tft->setCursor(0, 20); 
      watch->tft->println(F(mes)); 
      wakes++;
    }

    static void
    testAwakeLoop() {
      constexpr  char const *str = "%03i";
      constexpr int mes_l       = strlen(str) + 1 + 1;
      static char mes[mes_l];
      
      static int wakes = 0;
      
      watch->tft->setTextFont(2); 
      watch->tft->setTextSize(2);
       
      if(wakes) {
        watch->tft->setCursor(180, 0); 
        watch->tft->setTextColor(TFT_BLACK);   
        watch->tft->println(F(mes)); 
      }
      
      snprintf(mes, mes_l, str, wakes);
      
      watch->tft->setCursor(180, 0);
      watch->tft->setTextColor(TFT_WHITE); 
      watch->tft->println(F(mes)); 
      
      wakes++;
    }

  static void
  showBattery() {
      static int prev_per = 0;
      int per = power->getBattPercentage();
        
      if(per != prev_per) {
        watch->tft->setTextFont(2); 
        watch->tft->setTextSize(2);
        
        watch->tft->setCursor(0, 0); 
        watch->tft->setTextColor(TFT_BLACK); 
        tft->print(F("battery: ")); tft->print(prev_per);
          
        watch->tft->setCursor(0, 0); 
        watch->tft->setTextColor(TFT_WHITE); 
        tft->print(F("battery: ")); tft->print(per);

        prev_per = per;
      }
  }

  static void
  showSecAwaken() {
    static time_t el1 = 0;
    static time_t el2 = 0;
    el1 = el2;
    time(&el2);
    if(el1 != el2) {
      watch->tft->setTextFont(2); 
      watch->tft->setTextSize(2);
        
      watch->tft->setCursor(0, 80); 
      watch->tft->setTextColor(TFT_BLACK); 
      tft->print(F("secs awaken: ")); tft->print(el1);
          
      watch->tft->setCursor(0, 80); 
      watch->tft->setTextColor(TFT_WHITE); 
      tft->print(F("secs awaken: ")); tft->print(el2);
    }
  }


  
  private:
    MyLib();
    MyLib(MyLib &other);
    MyLib(MyLib &&other);
    MyLib &operator=(MyLib &other);
    MyLib &&operator=(MyLib &&other);
    ~MyLib();
};

#endif
