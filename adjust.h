#ifndef ADJUST_H
#define ADJUST_H

uint8_t adjust_int(const char *title, nframes_t *val,
		   const nframes_t max_val, const nframes_t min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  uint8_t action = action_button();
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  if (action & ARCADA_BUTTONMASK_UP && *val < max_val) {
    *val = *val + 1;
    *update = true;
  } else if (action & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
    *val = *val - 1;
    *update = true;
  }
  if (*update) {
    arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
  }
  
  return action;
}

uint8_t adjust_int(const char *title, uint16_t *val,
		   const uint16_t max_val, const uint16_t min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  uint8_t action = action_button();
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  if (action & ARCADA_BUTTONMASK_UP && *val < max_val) {
    *val = *val + 1;
    *update = true;
  } else if (action & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
    *val = *val - 1;
    *update = true;
  }
  if (*update) {
    arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
  }
  
  return action;
}

 
uint8_t adjust_int(const char *title, byte *val,
		   const byte max_val, const byte min_val,
		   const uint16_t fg, const uint16_t bg,
		   bool *first_run, bool *update) {
  uint8_t action = action_button();
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string(title, ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  if (action & ARCADA_BUTTONMASK_UP && *val < max_val) {
    *val = *val + 1;
    *update = true;
  } else if (action & ARCADA_BUTTONMASK_DOWN && *val > min_val) {
    *val = *val - 1;
    *update = true;
  }
  if (*update) {
    arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
    center_print_string(String((int)*val), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
  }
  
  return action;
}

uint8_t toggle_precount(const uint16_t fg, const uint16_t bg,
			bool *first_run, bool *update) {
  uint8_t action = action_button();
  
  if (*first_run) {
    arcada.display->fillScreen(bg);
    arcada.display->setTextColor(fg);

    center_print_string("count in", ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);
    center_print_string(String(precount_enabled ? "on" : "off"), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
    *first_run = false;
  }
  
  if (action & (ARCADA_BUTTONMASK_UP | ARCADA_BUTTONMASK_DOWN)) {
    precount_enabled = !precount_enabled;
    *update = true;
  }
    
  if (*update) {
    arcada.display->fillRect(0, ARCADA_TFT_HEIGHT / 2, ARCADA_TFT_WIDTH, ARCADA_TFT_HEIGHT / 2, bg);
    center_print_string(String(precount_enabled ? "on" : "off"), ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 7, number_font);
  }
  
  return action;
}

#endif
