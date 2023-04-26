#include "config.h"
#include "mylib.h"
#include "app.h"

bool irq = false;


void setup() {
    MyLib::initWatch();
    //MyLib::testText();
    //MyLib::testAwakeMain();
    //MyLib::showBattery();
    
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    MyLib::power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    MyLib::power->clearIRQ();

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);


    App<std::vector> app{};
    app.setBG(TFT_DARKGREY);
    app.drawBG();
    
    MyLib::syncTime();
}

void loop() {
  if(irq) {
    MyLib::power->clearIRQ();
    irq = false;
    switch(MyLib::watch_state) {
      case MyLib::WOKE:
        MyLib::watch->displaySleep();
        MyLib::watch->closeBL();   
        MyLib::watch->powerOff();
        MyLib::watch_state = MyLib::SLEEP;
        esp_light_sleep_start();
        break;
      case MyLib::SLEEP:
        MyLib::watch->displayWakeup();
        MyLib::watch->openBL();     
        MyLib::watch_state = MyLib::WOKE;
        MyLib::testAwakeLoop();
        break;
    }
  }
  
  //MyLib::showSecAwaken();
  MyLib::showBattery();
  MyLib::showClock();
}
