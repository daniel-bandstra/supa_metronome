#include <Adafruit_Arcada.h>

Adafruit_Arcada arcada;

#include "audio.h"
#include "display.h"
#include "random.h"
#include "metronome.h"
#include "screensaver.h"
#include "adjust.h"
#include "edit_pattern.h"
#include "listen.h"
#include "settings.h"

void setup() {
  trngInit();
  arcada.arcadaBegin();
  audio_function = &play_click;
  pdmspi.begin(SAMPLE_RATE);

  arcada.displayBegin();
  arcada.display->setTextWrap(false);
  
  if (arcada.filesysBegin(ARCADA_FILESYS_QSPI) != ARCADA_FILESYS_QSPI) {
    // do some kind of error recovery
  }
  
  load_settings();
  
  arcada.pixels.setPixelColor(4, 16, 16, 0);
  update_pixels();
  
  analogWriteResolution(12);
}

void loop() {
  static bool first_run = true;
  bool update = false;
  static bool changed = false;
  static byte value = 0;
  uint8_t action = 0;
  switch (screen_select) {
  case 0: // bpm
    action = adjust_int("bpm", &bpm, 300, 1, ARCADA_WHITE, ARCADA_BLUE, &first_run, &update);
    break;
  case 1: // pattern
    action = edit_pattern(&first_run, &update);
    break;
  case 2: // precount
    action = toggle_precount(ARCADA_WHITE, ARCADA_PURPLE, &first_run, &update);
    break;
  case 3: // chance
    if (first_run)
      enable_pixels();
    action = adjust_int("chance", &chance, 128, 0, ARCADA_WHITE, ARCADA_RED, &first_run, &update);
    break;
  case 4: // listening -- mic gain
    if (first_run)
      disable_pixels();
    action = adjust_int("mic gain", &mic_volume, 200, 1, ARCADA_BLACK, ARCADA_YELLOW, &first_run, &update);
    listen(&first_run);
    if (update)
      pdmspi.setMicGain(volume_to_gain(mic_volume));
    break;
  case 5: // listening -- threshold
    if (first_run)
      disable_pixels();
    action = adjust_int("gain thresh", &pct_on_threshold, 255, 0, ARCADA_BLACK, ARCADA_YELLOW, &first_run, &update);
    listen(&first_run);
    break;
  case 6: // listening -- calibrate acoustic delay
    if (first_run)
      disable_pixels();
    action = adjust_int("acoustic delay", &acoustic_delay, 500, -500, ARCADA_BLACK, ARCADA_YELLOW, &first_run, &update);
    if (action & ARCADA_BUTTONMASK_SELECT) {
      calibrate_menu();
      first_run = true;
    }
    calibrate(&first_run, &update);
    break;
  case 7: // brightness
    if (first_run)
      enable_pixels();
    action = adjust_int("backlight", &backlight_level, 255, 1, ARCADA_BLACK, ARCADA_WHITE, &first_run, &update);
    if (update)
      arcada.setBacklight(backlight_level);
    break;
  case 8: // volume
    if (first_run)
      value = gain_to_volume(output_gain);
    action = adjust_int("volume", &value, 112, 0, ARCADA_BLACK, ARCADA_ORANGE, &first_run, &update);
    if (update)
      output_gain = volume_to_gain(value);
    break;
  case 9: // subdiv
    action = adjust_int("subdivisions", &subdiv, SUBDIV_MAX, SUBDIV_MIN, ARCADA_WHITE, ARCADA_DARKGREEN, &first_run, &update);
    if (update) set_pattern_length();
    break;
  case 10: // meas_len
    action = adjust_int("measure length", &meas_len, MEAS_LEN_MAX, MEAS_LEN_MIN, ARCADA_WHITE, ARCADA_DARKGREEN, &first_run, &update);
    if (update) set_pattern_length();
    break;
  case 11: // measures
    action = adjust_int("measures", &measures, MEASURES_MAX, MEASURES_MIN, ARCADA_WHITE, ARCADA_DARKGREEN, &first_run, &update);
    if (update) set_pattern_length();
    break;
  default:
    break;
  }
  
  switch (action) {
  case ARCADA_BUTTONMASK_LEFT:
    screen_select = screen_select ? screen_select - 1 : 11;
    update = true;
    first_run = true;
    break;
  case ARCADA_BUTTONMASK_RIGHT:
    screen_select = (screen_select + 1) % 12;
    update = true;
    first_run = true;
    break;
  case ARCADA_BUTTONMASK_START:
    metronome_stop_start();
    break;
  case (ARCADA_BUTTONMASK_START | ARCADA_BUTTONMASK_SELECT):
    reset_menu();
    first_run = true;
    break;
  default:
    break;
  }
  
  if (!changed && update) {
    changed = true;
  }
  
  if (changed) // autosave
    changed = !save_settings_maybe();
  
  metronome();
  screensaver();
}
