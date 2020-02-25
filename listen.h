#ifndef LISTEN_H
#define LISTEN_H

nframes_t acoustic_delay = SAMPLE_RATE / 470;

void show_lag(nframes_t lag) {
  uint16_t width;
  uint16_t height = ARCADA_TFT_HEIGHT / 2;
  
  uint16_t x = ARCADA_TFT_WIDTH / 2;
  uint16_t y = (ARCADA_TFT_HEIGHT - height) / 2;
  
  arcada.display->fillScreen(ARCADA_YELLOW);
  arcada.display->drawFastVLine(x, y, ARCADA_TFT_HEIGHT / 2, ARCADA_BLACK);

  if (lag > 0) {
    width = (x * lag) / half_interval;
    arcada.display->fillRect(x + 1, y, width, height, ARCADA_RED);
  } else if (lag < 0) {
    width = (x * (lag * -1)) / half_interval;
    arcada.display->fillRect(x - width, y, width, height, ARCADA_RED);
  }
}

void listen(bool *first_run) {
  static nframes_t last_onset = note_onset;
  
  static bool light_on = false;
  static nframes_t light_off_time = frames;

  static bool metronome_was_playing = metronome_playing;

  if (!metronome_playing && metronome_was_playing)
    *first_run = true;
  metronome_was_playing = metronome_playing;
  
  if (light_on && frames > light_off_time) {
    analogWrite(13, 0);
    light_on = false;
  }
  
  if (note_onset != last_onset) {
    last_onset = note_onset;
    analogWrite(13, 128);
    light_on = true;
    light_off_time = frames + SAMPLE_RATE / 10;

    if (metronome_playing) {
      keep_screen_on();

      nframes_t offset = (last_onset - current_tick) - acoustic_delay;
      
      if (offset > half_interval)
	offset -= interval;
      
      show_lag(offset);
    }
  }
}

bool calibrating = false;

void calibrate_menu() {
  stop_metronome();
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setFont(mini_font);
  const char *menu_items[] = {"Cancel", "Calibrate"};
  switch (arcada.menu(menu_items, 2, ARCADA_BLUE, ARCADA_WHITE, true)) {
  case 1:
    calibrating = true;
    start_metronome();
    break;
  default:
    break;
  }
}

void calibrate(bool *first_run, bool *update) {
  static nframes_t last_onset = note_onset;
  
  static bool light_on = false;
  static nframes_t light_off_time = frames;
  
  static bool was_calibrating = calibrating;
  static nframes_t sum;
  static nframes_t count;
  
  static bool metronome_was_playing = metronome_playing;

  if (!metronome_playing && metronome_was_playing)
    *first_run = true;
  metronome_was_playing = metronome_playing;
  
  if (calibrating && !was_calibrating) {
    sum = 0;
    count = 0;
  }
  was_calibrating = calibrating;

  if (light_on && frames > light_off_time) {
    analogWrite(13, 0);
    light_on = false;
  }
  
  if (note_onset != last_onset) {
    last_onset = note_onset;
    analogWrite(13, 128);
    light_on = true;
    light_off_time = frames + SAMPLE_RATE / 10;

    if (calibrating) {
      if (metronome_playing) {
	keep_screen_on();

	nframes_t offset = last_onset - current_tick;
      
	if (offset > half_interval)
	  offset -= interval;

	if (offset > -500 && offset < 500) { // discard outliers
	  sum += offset;
	  count++;
	  acoustic_delay = sum / count;
	  show_lag((offset - acoustic_delay) * 4);
	  *update = true;
	}
      } else {
	calibrating = false;
      }
    } else if (metronome_playing) {
      keep_screen_on();

      nframes_t offset = (last_onset - current_tick) - acoustic_delay;
      
      if (offset > half_interval)
	offset -= interval;
      
      show_lag(offset);
    }
  }
}

#endif
