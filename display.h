#ifndef DISPLAY_H
#define DISPLAY_H

#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeMono24pt7b.h>
const GFXfont *title_font  = &FreeSerif12pt7b;
const GFXfont *number_font = &FreeMono24pt7b;
const GFXfont *mini_font   = NULL;

byte backlight_level;

void center_print_string(String s, int16_t x, int16_t y, const GFXfont *font) {
  // center string s on point {x, y}
  int16_t width = 0;
  int16_t height = font->yAdvance / 2;
  
  for (int i = 0; i < s.length(); i++) {
    width += (font->glyph + s[i] - 0x20)->xAdvance;
  }
  
  arcada.display->setFont(font);
  arcada.display->setCursor(x - width / 2, y - height / 2);
  arcada.display->print(s);
}

// neopixel enabling/disabling
// helpful because neopixel updates interfere w/ microphone

bool pixels_on = true;

void update_pixels() {
  if (pixels_on) {
    arcada.pixels.show();
  }
}

void enable_pixels() {
  pixels_on = true;
  update_pixels();
}

void disable_pixels() {
  for (byte i = 0; i < 5; i++) {
    arcada.pixels.setPixelColor(i, 0, 0, 0);
  }
  update_pixels();
  pixels_on = false;
}
  
#endif
