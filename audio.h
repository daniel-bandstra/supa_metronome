#ifndef AUDIO_H
#define AUDIO_H

#include <Adafruit_ZeroPDMSPI.h>
void (*audio_function)();

#define PDM_SPI SPI2
#define PDM_SERCOM_HANDLER SERCOM3_0_Handler
#define SAMPLE_RATE 16000

typedef int64_t nframes_t;

Adafruit_ZeroPDMSPI pdmspi(&PDM_SPI);

volatile float output_gain;
volatile nframes_t frames = 0;

byte mic_volume;

byte gain_to_volume(float gain) {
  return 100 + round(20.0 * log10(gain));
}

float volume_to_gain(byte volume) {
  return pow(10.0, (float)(volume - 100) / 20.0);
}

#define LONG_SIZE 2048
volatile uint16_t long_vals[LONG_SIZE] = { };
volatile uint16_t long_i = 0;
volatile uint32_t long_sum = 0;

#define SHORT_SIZE 256
volatile uint16_t short_vals[SHORT_SIZE] = { };
volatile uint16_t short_i = 0;
volatile uint32_t short_sum = 0;

byte pct_on_threshold = 255; // percent threshold to mark a note onset
byte pct_off_threshold = 10; // percent threshold to mark a note not onsetting anymore
volatile nframes_t note_onset = 0;
volatile nframes_t interval = 0;
volatile nframes_t half_interval = 0;

void onset_detect(uint16_t sample) {
  // note sample for long- and short-term means
  long_sum -= long_vals[long_i];
  long_vals[long_i] = sample < 32769 ? 32768 - sample : sample - 32768;
  long_sum += long_vals[long_i];
  long_i = (long_i + 1) % LONG_SIZE;
  
  short_sum -= short_vals[short_i];
  short_vals[short_i] = sample < 32769 ? 32768 - sample : sample - 32768;
  short_sum += short_vals[short_i];
  short_i = (short_i + 1) % SHORT_SIZE;

  // note a threshold, or not
  static bool note_on = false;
  uint16_t long_mean = long_sum / LONG_SIZE;
  uint16_t short_mean = short_sum / SHORT_SIZE;
  
  if (note_on) {
    if (short_mean < (long_mean * (100 + pct_off_threshold)) / 100) {
      note_on = false;
    }
  } else {
    if ((frames > note_onset + half_interval) &&
	(short_mean > (long_mean * (100 + pct_on_threshold)) / 100)) {
      note_on = true;
      note_onset = frames;
    }
  }
}

void PDM_SERCOM_HANDLER(void) {
  uint16_t v = 0;
  
  if (pdmspi.decimateFilterWord(&v)) {
    frames++;
    onset_detect(v);
    audio_function();
  }
}

#endif
