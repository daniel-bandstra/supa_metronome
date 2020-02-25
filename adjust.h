#ifndef ADJUST_H
#define ADJUST_H

uint8_t action_button (uint8_t pressed_buttons) {
  // debounce button press, limit to one click per second
  static nframes_t next_time = 0;
  static nframes_t debounce_time = 0;
  nframes_t now = frames;
  static uint8_t last_pressed = 0;
  static uint8_t maybe_pressed = 0;

  if (pressed_buttons)
    keep_screen_on();

  if (pressed_buttons != maybe_pressed) {
    maybe_pressed = pressed_buttons;
    debounce_time = now + SAMPLE_RATE / 60;
  }
  
  if ((now > debounce_time) &&
      (pressed_buttons != last_pressed || now > next_time)) {
    last_pressed = pressed_buttons;
    next_time = now + SAMPLE_RATE;
    return pressed_buttons;
  }

  return 0;
}

uint8_t adjust_int(const char *title, nframes_t *val,
		   const nframes_t max_val, const nframes_t min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  static nframes_t down_time, next_time = frames;
  static bool button_down;
  uint8_t pressed_buttons;
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  pressed_buttons = arcada.readButtons();
    
  if (!button_down && pressed_buttons) {
    button_down = true;
    down_time = frames;
  } else if (!pressed_buttons) {
    button_down = false;
    next_time = frames;
  }
    
  if (button_down && frames > next_time) {
    if (pressed_buttons & ARCADA_BUTTONMASK_UP && *val < max_val) {
      *val = *val + 1;
      *update = true;
    } else if (pressed_buttons & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
      *val = *val - 1;
      *update = true;
    }
    if (*update) {
      arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
      center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    }
    next_time = frames + ((SAMPLE_RATE * 4) / sqrt(frames - down_time));
  }
  
  return action_button(pressed_buttons);
}

uint8_t adjust_int(const char *title, uint16_t *val,
		   const uint16_t max_val, const uint16_t min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  static nframes_t down_time, next_time = frames;
  static bool button_down;
  uint8_t pressed_buttons;
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  pressed_buttons = arcada.readButtons();
    
  if (!button_down && pressed_buttons) {
    button_down = true;
    down_time = frames;
  } else if (!pressed_buttons) {
    button_down = false;
    next_time = frames;
  }
    
  if (button_down && frames > next_time) {
    if (pressed_buttons & ARCADA_BUTTONMASK_UP && *val < max_val) {
      *val = *val + 1;
      *update = true;
    } else if (pressed_buttons & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
      *val = *val - 1;
      *update = true;
    }
    if (*update) {
      arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
      center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    }
    next_time = frames + ((SAMPLE_RATE * 4) / sqrt(frames - down_time));
  }
  
  return action_button(pressed_buttons);
}

uint8_t adjust_int(const char *title, byte *val,
		   const byte max_val, const byte min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  static nframes_t down_time, next_time = frames;
  static bool button_down;
  uint8_t pressed_buttons;
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String(*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  pressed_buttons = arcada.readButtons();
    
  if (!button_down && pressed_buttons) {
    button_down = true;
    down_time = frames;
  } else if (!pressed_buttons) {
    button_down = false;
    next_time = frames;
  }
    
  if (button_down && frames > next_time) {
    if (pressed_buttons & ARCADA_BUTTONMASK_UP && *val < max_val) {
      *val = *val + 1;
      *update = true;
    } else if (pressed_buttons & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
      *val = *val - 1;
      *update = true;
    }
    if (*update) {
      arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
      center_print_string(String(*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    }
    next_time = frames + ((SAMPLE_RATE * 4) / sqrt(frames - down_time));
  }
  
  return action_button(pressed_buttons);
}

uint8_t toggle_precount(const uint16_t fg, const uint16_t bg,
			bool *first_run, bool *update) {
  uint8_t pressed_buttons;
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string("count in", ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String(precount_enabled ? "ON" : "OFF"), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  pressed_buttons = action_button(arcada.readButtons());
    
  if (pressed_buttons & (ARCADA_BUTTONMASK_UP | ARCADA_BUTTONMASK_DOWN)) {
    precount_enabled = !precount_enabled;
    *update = true;
  }
    
  if (*update) {
    arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
    center_print_string(String(precount_enabled ? "ON" : "OFF"), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
  }
  
  return pressed_buttons;
}

#endif
