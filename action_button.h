#ifndef ACTION_BUTTON_H
#define ACTION_BUTTON_H

#define DEBOUNCE (SAMPLE_RATE / 50)
#define MAX_REPEAT (SAMPLE_RATE / 2)
#define MIN_REPEAT (SAMPLE_RATE / 20)

uint8_t action_button () {
  // debounce button presses and repeat held buttons
  static nframes_t next_time = 0;
  static nframes_t debounce_time = 0;
  static nframes_t interval = MAX_REPEAT;
  nframes_t now = frames;
  static uint8_t last_pressed = 0;
  static uint8_t maybe_pressed = 0;

  uint8_t pressed_buttons = arcada.readButtons();
  if (pressed_buttons)
    keep_screen_on();

  if (pressed_buttons != maybe_pressed) {
    maybe_pressed = pressed_buttons;
    interval = MAX_REPEAT;
  }
  
  if ((now > debounce_time) &&
      (pressed_buttons != last_pressed || now > next_time)) {
    last_pressed = pressed_buttons;
    next_time = now + interval;
    interval -= interval > MIN_REPEAT ? (interval - MIN_REPEAT) / 2 : interval;
    return pressed_buttons;
  }

  return 0;
}

#endif
