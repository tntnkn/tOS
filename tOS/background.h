#ifndef BACKGROUND_H_SENTRY
#define BACKGROUND_H_SENTRY


#include "utils.h" 

class Background : public Drawable { public: Background() {}; };

class MonoBackground final : public Background {
    Utils::color_t m_bg_color;
    
  public:
    MonoBackground() {};
    MonoBackground(Utils::color_t bg_color) : m_bg_color(bg_color) {};
    
    void redrawScreen() {
      TTGOClass::getWatch()->tft->fillScreen(m_bg_color);
    }
    
    void redrawBox(Utils::coord_t top_left, Utils::coord_t bot_right) {
      TTGOClass::getWatch()->tft->fillRect(top_left.x, top_left.y, 
                                           top_left.x - bot_right.x, 
                                           top_left.y - bot_right.y, m_bg_color);
    }    
};


#endif
