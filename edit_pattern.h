#ifndef EDIT_PATTERN_H
#define EDIT_PATTERN_H

#define BOX_SIDE 15 // prettier if this is odd
#define DOT_OFFSET (BOX_SIDE / 2)
#define DOT_RADIUS (BOX_SIDE / 2 - 2)
#define BOX_FILL ARCADA_WHITE
#define BOX_SELECT ARCADA_YELLOW
#define BOX_PLAY ARCADA_GREEN
#define BOX_LINE ARCADA_BLACK
#define PATTERN_BG ARCADA_BLUE
#define GLYPH_WIDTH 5
#define GLYPH_HEIGHT 7
#define BORDER ((GLYPH_WIDTH / 2) + 1)
#define SCREEN_WIDTH (ARCADA_TFT_WIDTH - BORDER * 2)
#define SCREEN_HEIGHT (ARCADA_TFT_HEIGHT - BORDER * 2)
#define LEFT_EDGE BORDER
#define RIGHT_EDGE (ARCADA_TFT_WIDTH - BORDER)

bool pattern_changed = false;

inline int16_t pattern_width() {
  return pattern_length * (BOX_SIDE - 1) + meas_len * measures;
}

inline int16_t pattern_height() {
  return BOX_SIDE + GLYPH_HEIGHT * 2 + 5;
}

void redraw_pattern(int16_t pos_x, int16_t pos_y,
		    int16_t select, bool first_run,
		    int16_t *left, int16_t *right) {
  // keep from redrawing unless necessary
  static int16_t last_pos_x = pos_x;
  static int16_t last_pos_y = pos_y;
  static int16_t last_select = select;
  static index_t last_current_i = current_i;
  static bool last_metronome_playing = metronome_playing;
  
  if (!first_run &&
      last_pos_x == pos_x &&
      last_pos_y == pos_y &&
      last_select == select &&
      ((last_current_i == current_i) || !metronome_playing) &&
      last_metronome_playing == metronome_playing &&
      !pattern_changed)
    return;

  keep_screen_on();
  
  last_pos_x = pos_x;
  last_pos_y = pos_y;
  last_select = select;
  last_current_i = current_i;
  last_metronome_playing = metronome_playing;
  pattern_changed = false;
  
  // x, y is upper left corner of pattern, including superscripts
  int16_t x = pos_x;
  int16_t y = pos_y - pattern_height() / 2;

  int16_t box_top = y + GLYPH_HEIGHT + 2;
  int16_t width = pattern_width();
  
  // color fills
  arcada.display->fillRect(x, box_top, width, BOX_SIDE, BOX_FILL);
  // playing cursor fill
  if (metronome_playing) {
    uint16_t play_offset = 1 + current_i * (BOX_SIDE - 1) + current_i / subdiv;
    arcada.display->fillRect(x + play_offset, box_top, BOX_SIDE, BOX_SIDE, BOX_PLAY);
    *left = x + play_offset;
    *right = *left + BOX_SIDE;
  }
  // editing select fill
  if (select >= 0) {
    uint16_t select_offset = 1 + select * (BOX_SIDE - 1) + select / subdiv;
    arcada.display->fillRect(x + select_offset, box_top, BOX_SIDE, BOX_SIDE, BOX_SELECT);
    *left = x + select_offset;
    *right = *left + BOX_SIDE;
  } else if (!metronome_playing) {
    *left = x;
    *right = *left + BOX_SIDE;
  }

  // line work
  arcada.display->drawFastHLine(x, box_top, width, BOX_LINE);
  arcada.display->drawFastHLine(x, box_top + BOX_SIDE - 1, width, BOX_LINE);

  for (int i = 0; i < pattern_length; i++) {
    if (!(i % subdiv)) { // new beat
      // fat line
      arcada.display->drawFastVLine(x, box_top, BOX_SIDE, BOX_LINE);
      x++;
    }
    arcada.display->drawFastVLine(x, box_top, BOX_SIDE, BOX_LINE);
    if (pattern[i])
      arcada.display->fillCircle(x + DOT_OFFSET, box_top + DOT_OFFSET, DOT_RADIUS, BOX_LINE);
    x += BOX_SIDE - 1;
  }
  arcada.display->drawFastVLine(x, box_top, BOX_SIDE, BOX_LINE);
}

void draw_whole_pattern(int16_t pos_x, int16_t pos_y,
			int16_t select, bool first_run,
			int16_t *left, int16_t *right) {
  arcada.display->setFont(mini_font);
    
  // x, y is upper left corner of pattern, including superscripts
  int16_t x = pos_x;
  int16_t y = pos_y - pattern_height() / 2;

  
  int16_t box_top = y + GLYPH_HEIGHT + 2;
  int16_t width = pattern_width();
  
  arcada.display->fillRect(0, y, ARCADA_TFT_WIDTH, pattern_height(), PATTERN_BG);
  redraw_pattern(pos_x, pos_y, select, first_run, left, right);
  
  //beat & measure numbers
  for (int i = 0; i < pattern_length; i++) {
    if (!(i % subdiv)) { // new beat
      x++;
      // superscript
      int n = ((i / subdiv) % meas_len) + 1;
      if (n < 10)
	arcada.display->setCursor(x - GLYPH_WIDTH / 2, y);
      else
	arcada.display->setCursor(x - GLYPH_WIDTH, y);
      arcada.display->print(n);
    }
    if (!(i % (subdiv * meas_len))) { // new measure
      // subscript
      arcada.display->setCursor(x - GLYPH_WIDTH / 2, box_top + BOX_SIDE + 3);
      arcada.display->print((i / (subdiv * meas_len)) + 1);
    }
    x += BOX_SIDE - 1;
  }
}

void pattern_menu(bool *first_run, bool *update) {
  stop_metronome();
  const char *menu_items[] = {"Cancel", "Reset Pattern", "Clear Pattern"};
  arcada.display->fillScreen(ARCADA_BLACK);
  arcada.display->setFont(mini_font);
  switch (arcada.menu(menu_items, 3, ARCADA_BLUE, ARCADA_WHITE, true)) {
  case 1:
    reset_pattern();
    *first_run = true;
    *update = true;
    break;
  case 2:
    clear_pattern();
    *first_run = true;
    *update = true;
    break;
  default:
    *first_run = true;
    break;
  }
}

uint8_t edit_pattern(bool *first_run, bool *update) {
  static int16_t select = 0;
  static bool editing = false;
  static int16_t y = (ARCADA_TFT_HEIGHT / 4) * 3;
  bool redraw = false;
  static int16_t x;
  static int16_t left, right;
  
  if (*first_run) {
    arcada.display->fillScreen(PATTERN_BG);
    arcada.display->setTextColor(BOX_FILL);

    center_print_string("edit pattern", ARCADA_TFT_WIDTH / 2, (ARCADA_TFT_HEIGHT / 8) * 3, title_font);

    x = ARCADA_TFT_WIDTH / 2 - pattern_width() / 2;
    draw_whole_pattern(x, y, editing ? select : -1, *first_run, &left, &right);
    
    *first_run = false;
  }
  
  uint8_t pressed_buttons = action_button(arcada.readButtons());
  
  if (left < LEFT_EDGE) {
    x += LEFT_EDGE - left;
    redraw = true;
  } else if (right > RIGHT_EDGE) {
    x -= right - RIGHT_EDGE;
    redraw = true;
  }
  
  switch (pressed_buttons) {
  case ARCADA_BUTTONMASK_SELECT:
    pattern_menu(first_run, update);
    break;
  case ARCADA_BUTTONMASK_DOWN:
    if (!editing) {
      select = select >= pattern_length ? pattern_length - 1 : select;
      editing = true;
    }
    break;
  case ARCADA_BUTTONMASK_UP:
    if (editing) {
      editing = false;
    }
    break;
  case ARCADA_BUTTONMASK_LEFT:
    if (editing) {
      select = select ? select - 1 : select;
    }
    break;
  case ARCADA_BUTTONMASK_RIGHT:
    if (editing) {
      select = select == pattern_length - 1 ? select : select + 1;
    }
    break;
  case ARCADA_BUTTONMASK_A:
    if (editing) {
      if (pattern[select]) {
	pattern[select] = 0;
	erase_click_by_index(select);
      } else {
	pattern[select] = 1;
      }
      *update = true;
      pattern_changed = true;
    }
    break;
  default:
    break;
  }
  
  if (redraw) {
    draw_whole_pattern(x, y, editing ? select : -1, *first_run, &left, &right);
  } else {
    redraw_pattern(x, y, editing ? select : -1, *first_run, &left, &right);
  }
  
  if (editing)
    return pressed_buttons & (ARCADA_BUTTONMASK_START | ARCADA_BUTTONMASK_SELECT);
  else
    return pressed_buttons;
}

#endif
