#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#define TIMEOUT (SAMPLE_RATE * 5)

nframes_t screen_timeout = frames + TIMEOUT;

void screensaver() {
  static bool screen_on = true;
  
  if (screen_on && frames > screen_timeout) {
    arcada.setBacklight(0);
    screen_on = false;
    return;
  }

  if (!screen_on && screen_timeout > frames) {
    arcada.setBacklight(backlight_level);
    screen_on = true;
    return;
  }
}

void keep_screen_on() {
  screen_timeout = frames + TIMEOUT;
}

#endif
