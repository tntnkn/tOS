#ifndef WIDGET_H_SENTRY
#define WIDGET_H_SENTRY


#include "entity.h"
#include "utils.h"

class Widget : protected Drawable
             : protected HasParent {

  private:
    void redrawScreen() final 
    { //nothing! widget's not supposed to redraw the area outside of it. };
    
    void redrawBox(Utils::Coord top_left, Utils::Coord bot_right) final 
    {
      //TODO: claculate botright for parent somehow
      m_parent->redrawBox(top_left, bot_right);
    }
    
  public:
    virtual void show() = 0;
    void hide() final { 
      //asks parent to redraw it`s bounding box
      //calculate its size;
      //add it to size already gotten;
      //call m_parent->hide();
      m_parent->redrawBox(m_top_left, m_bot_right);
    }
    void redraw() final {
      hide(); show();
    };
    
    void getSize();
    void placeAt(Utils::Coord top_left);
    void magnify();
    void minify();
    void setGamma();
    void setFont();
};

#endif
