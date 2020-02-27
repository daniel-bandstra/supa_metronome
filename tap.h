#ifndef TAP_H
#define TAP_H

#define TAP_THRESH 80
#define MIN_TAP_WAIT ((SAMPLE_RATE * 60) / BPM_MAX)
#define TAP_SAMPLE_SIZE 4

uint8_t tap_tempo(const uint16_t fg, const uint16_t bg,
		  bool *first_run, bool *update) {
  // tap tempo menu
  stop_metronome();
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setFont(mini_font);
  const char *menu_items[] = {"Cancel", "Tap Tempo"};
  switch (arcada.menu(menu_items, 2, ARCADA_BLUE, ARCADA_WHITE, true)) {
  case 1:
    break;
  default:
    *first_run = true;
    return 0;
    break;
  }

  // initial display
  arcada.display->fillScreen(bg);
  arcada.display->setTextColor(fg);

  center_print_string("bpm (tap)", ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
  center_print_string(String(bpm), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);

  // tap setup
  arcada.accel->setRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  arcada.accel->setClick(1, TAP_THRESH);
  nframes_t tap_samples[TAP_SAMPLE_SIZE];
  nframes_t tap_total = 0;
  nframes_t soonest = frames;
  index_t i = 0;
  bool samples_full = false;
  nframes_t now, last_tap = 0;
  uint8_t buttons;
  bool new_tap = false;

  bool light_on = false;
  nframes_t light_off_time;
  
  while (true) {
    now = frames;

    if (light_on && now > light_off_time) {
      arcada.pixels.setPixelColor(0, 0, 0, 0);
      update_pixels();
      light_on = false;
    }
    
    if (arcada.accel->getClick() & 0x30 && now > soonest) {
      if (last_tap) {
	if (samples_full) {
	  tap_total -= tap_samples[i];
	  tap_samples[i] = now - last_tap;
	  tap_total += tap_samples[i];
	  i = (i + 1) % TAP_SAMPLE_SIZE;
	  bpm = (SAMPLE_RATE * 60) / (tap_total / TAP_SAMPLE_SIZE);
	} else {
	  tap_samples[i] = now - last_tap;
	  tap_total += tap_samples[i];
	  i++;
	  bpm = (SAMPLE_RATE * 60) / (tap_total / i);
	  if (i == TAP_SAMPLE_SIZE) {
	    i = 0;
	    samples_full = true;
	  }
	}
	bpm = bpm > BPM_MAX ? BPM_MAX : bpm;
	bpm = bpm < BPM_MIN ? BPM_MIN : bpm;
	new_tap = true;
	*update = true;
      }
      last_tap = now;
      soonest = now + MIN_TAP_WAIT;

      arcada.pixels.setPixelColor(0, 16, 0, 0);
      update_pixels();
      light_on = true;
      light_off_time = now + SAMPLE_RATE / 10;
    }

    if (new_tap) {
      arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
      center_print_string(String(bpm), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
      new_tap = false;
    }
    
    if (buttons = arcada.readButtons()) {
      arcada.accel->setClick(0, TAP_THRESH);
      *first_run = true;
      return action_button(buttons);
    }
  }
}

#endif
