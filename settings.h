#ifndef SETTINGS_H
#define SETTINGS_H

char *settings_file = "/metronome_settings";

byte screen_select;

void save_settings() {
  File file = arcada.open(settings_file, FILE_WRITE);
  file.seek(0);

  file.write(screen_select);
  file.write(gain_to_volume(output_gain));
  file.write(backlight_level);
  file.write(&bpm, sizeof(uint16_t));
  file.write(subdiv);
  file.write(meas_len);
  file.write(measures);
  file.write(precount_enabled);
  file.write(chance);
  file.write(mic_volume);
  file.write(pct_on_threshold);
  file.write(&acoustic_delay, sizeof(nframes_t));
  file.write(pattern, PATTERN_MAX);
  
  file.close();
}

void default_settings() {
  screen_select = 0;
  output_gain = volume_to_gain(94);
  backlight_level = 127;
  arcada.setBacklight(backlight_level);
  bpm = 120;
  subdiv = 1;
  update_interval();
  meas_len = 4;
  measures = 1;
  set_pattern_length();
  reset_pattern();
  precount_enabled = true;
  chance = 0;
  mic_volume = 100;
  pct_on_threshold = 25;
  acoustic_delay = SAMPLE_RATE / 372;
  save_settings();
}

void load_settings() {
  if (arcada.exists(settings_file)) {
    File file = arcada.open(settings_file, FILE_READ);
    file.seek(0);

    screen_select = file.read();
    output_gain = volume_to_gain(file.read());
    backlight_level = file.read();
    file.read(&bpm, sizeof(uint16_t));
    subdiv = file.read();
    update_interval();
    meas_len = file.read();
    measures = file.read();
    set_pattern_length();
    precount_enabled = file.read();
    chance = file.read();
    mic_volume = file.read();
    pct_on_threshold = file.read();
    file.read(&acoustic_delay, sizeof(nframes_t));
    file.read(pattern, PATTERN_MAX);
    
    file.close();
  } else { // set to defaults
    default_settings();
  }
  arcada.setBacklight(backlight_level);
}

void reset_menu() {
  stop_metronome();
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setFont(mini_font);
  const char *menu_items[] = {"Cancel", "Reset"};
  switch (arcada.menu(menu_items, 2, ARCADA_BLUE, ARCADA_WHITE, true)) {
  case 1:
    default_settings();
    break;
  default:
    break;
  }
}

#define SAVE_DELAY SAMPLE_RATE / 100
#define SAVE_RATE SAMPLE_RATE

bool save_settings_maybe() {
  // save settings every so often if it won't interfere with clicks
  static nframes_t next_time = frames;
  nframes_t now = frames;

  if (now > next_time && !arcada.readButtons()) {
    for (byte i = 0; i < TICKS_BUF_LEN; i++)
      if (clicks[i] && now + SAVE_DELAY + SPEAKER_RAMP_UP >= ticks[i])
	return false;
  
    save_settings();
    next_time = now + SAVE_RATE;
    return true;
  }
  
  return false;
}

#endif
