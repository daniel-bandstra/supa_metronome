#ifndef ACTION_BUTTON_H
#define ACTION_BUTTON_H

#define MAX_REPEAT (SAMPLE_RATE / 2)
#define MIN_REPEAT (SAMPLE_RATE / 20)
#define DEBOUNCE (SAMPLE_RATE / 50)

uint8_t action_button () {
  // debounce button presses and repeat held buttons
  static nframes_t debounce_time = 0;
  static nframes_t repeat_time = 0;
  static nframes_t interval = MAX_REPEAT;

  nframes_t now = frames;
  
  static uint8_t last_reading = 0;
  static uint8_t maybe_reading = 0;
  uint8_t reading = arcada.readButtons();
  
  if (reading)
    keep_screen_on();

  if (reading != maybe_reading) {
    maybe_reading = reading;
    debounce_time = now + DEBOUNCE;
    return 0;
  }

  if (now > debounce_time) {
    if (reading == last_reading) { // button held down
      if (now > repeat_time) {
	interval -= interval >  MIN_REPEAT ? (interval - MIN_REPEAT) / 2 : 0;
	repeat_time = now + interval;
	return reading;
      } else {
	return 0;
      }
    } else { // new button press
      last_reading = reading;
      interval = MAX_REPEAT;
      repeat_time = now + interval;
      return reading;
    }
  } else { // waiting for debounce
    return 0;
  }
}

#endif
