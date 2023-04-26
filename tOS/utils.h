#ifndef UTILS_H_SENTRY
#define UTILS_H_SENTRY



class Utils {
  private:
    using xy_t = uint8_t;

  public:

    struct coord_t {
        xy_t x, y;
    };

    //typedefs to the types used by TFT_eSPI libary by LillyGO
    //https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/src/libraries/TFT_eSPI/TFT_eSPI.h
    using color_t       = uint32_t;
    using wallpaper_t   = uint8_t;
};


#endif
