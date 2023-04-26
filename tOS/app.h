#ifndef APP_H_SENTRY
#define APP_H_SENTRY


#include "entity.h"
#include "background.h"
#include "utils.h"

template<typename Children>
class App : protected Drawable,
            protected Executable, 
            protected HasChildren<Children> {
  private:
    using col_t = Utils::color_t;
    
    Background *m_bg = nullptr;
    col_t m_colors[4];
    //it should have some sort of config
     
  public:
    App() {} 
    App(Background *bg, col_t col1, col_t col2, col_t col3, col_t col4) : 
      m_bg(bg), m_colors{col1, col2, col3, col4} {}
      
    void redrawScreen() override final { 
        m_bg->redrawScreen();
    }
    
    void redrawBox(Utils::coord_t top_left, Utils::coord_t bot_right) override final 
    {
      m_bg->redrawBox(top_left, bot_right);
    } 

    void setBG(Utils::color_t color) { 
      if(m_bg) delete(m_bg); m_bg = new MonoBackground(color); drawBG();
    }
    //void setBG(Utils::wallpaper_t wp){}

    void drawBG() { m_bg->redrawScreen(); }
};

#endif
