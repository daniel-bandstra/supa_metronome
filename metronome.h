#ifndef METRONOME_H
#define METRONOME_H

typedef uint16_t index_t;

#include "audio.h"

#define SPEAKER_RAMP_UP (SAMPLE_RATE / 10)

#define CLICK_LEN 2476
static const int16_t click_pcm[2476] = { 84, 198, -195, -430, 52, -154, -686, 393, 417, 997, 786, 23, -389, -1577, -1269, -356, -136, 774, 645, 1084, 1630, 641, -210, -1076, -1430, -1046, -47, -426, 199, -226, 53, 1354, 1021, 1022, 320, -246, -200, -684, -1055, -1153, -1198, -403, 465, 1290, 1542, 1089, 612, 468, -252, -923, -1391, -1639, -778, 51, 545, 1037, 711, 738, 810, 380, 148, -711, -1113, -851, -484, -237, -11, 106, 420, 851, 879, 697, 146, -388, -437, -568, -656, -685, -664, 17, 615, 954, 970, 418, 73, -94, -357, -421, -671, -670, -376, 32, 470, 554, 457, 317, 350, 206, 27, -383, -621, -530, -404, -37, 44, 18, 264, 502, 654, 602, 143, -232, -462, -543, -510, -494, -391, -111, 396, 782, 861, 633, 240, -74, -418, -597, -719, -723, -385, 50, 533, 643, 551, 450, 324, 172, -115, -400, -668, -721, -530, -142, 176, 365, 505, 565, 582, 391, 42, -318, -534, -616, -522, -367, -207, 82, 368, 636, 699, 532, 221, -96, -290, -490, -611, -644, -436, -73, 305, 579, 614, 532, 339, 162, -68, -342, -561, -608, -463, -238, 13, 216, 416, 513, 530, 424, 159, -145, -403, -477, -453, -385, -277, -91, 181, 423, 540, 496, 352, 127, -91, -277, -437, -548, -517, -269, 22, 285, 450, 454, 452, 349, 152, -100, -370, -501, -505, -350, -153, 46, 191, 357, 453, 412, 286, 30, -158, -303, -356, -370, -316, -173, 16, 245, 376, 396, 302, 185, 43, -98, -248, -355, -387, -294, -107, 89, 243, 302, 323, 276, 199, 32, -162, -303, -349, -281, -187, -59, 77, 195, 314, 348, 271, 101, -110, -221, -267, -254, -224, -154, 1, 148, 288, 303, 231, 89, -43, -107, -188, -225, -231, -143, 0, 132, 201, 190, 147, 87, 40, -15, -128, -188, -183, -131, -30, 33, 87, 109, 151, 168, 106, 38, -71, -156, -159, -151, -103, -72, 16, 128, 165, 194, 146, 72, -27, -115, -146, -164, -153, -118, -10, 93, 158, 183, 142, 89, 35, -25, -75, -169, -182, -169, -108, 63, 110, 174, 173, 150, 105, -24, -103, -164, -154, -147, -73, -10, 52, 126, 143, 144, 75, 27, -43, -73, -77, -143, -126, -59, -4, 86, 93, 88, 90, 42, 65, 32, -49, -77, -126, -91, -69, -57, -1, 26, 115, 152, 161, 94, 18, -82, -128, -140, -163, -111, -58, 55, 145, 184, 152, 86, 6, 0, -85, -137, -159, -170, -71, -1, 118, 147, 141, 143, 113, 29, -63, -151, -191, -145, -77, -4, 40, 84, 104, 141, 124, 57, -21, -72, -79, -114, -111, -78, -42, 25, 78, 134, 121, 72, 30, -17, -41, -99, -103, -91, -41, 0, 22, 70, 68, 87, 74, 43, 0, -69, -100, -94, -62, -16, 37, 47, 69, 79, 52, 16, -25, -31, -53, -52, -44, -27, -3, 31, 76, 75, 49, -1, -37, -61, -48, -35, -10, 12, 19, 40, 14, 15, 0, 1, 9, -4, -26, -51, -44, -26, 15, 44, 52, 57, 46, 33, -2, -56, -79, -91, -50, -10, 43, 63, 59, 59, 49, 36, -4, -42, -79, -78, -59, -38, -8, 25, 49, 77, 68, 53, 30, -15, -29, -55, -73, -69, -61, -14, 27, 68, 82, 74, 53, 34, 0, -36, -66, -101, -85, -62, -10, 35, 80, 103, 99, 67, 18, -47, -88, -86, -80, -37, -15, 6, 30, 63, 87, 79, 51, -4, -46, -64, -58, -50, -47, -31, 7, 49, 65, 59, 42, 20, 2, -2, -45, -53, -58, -45, -1, 18, 42, 37, 53, 34, 17, 0, -30, -42, -45, -36, -25, 0, 15, 29, 39, 48, 25, 4, -15, -29, -30, -43, -28, -27, -4, 29, 62, 73, 41, 19, -20, -40, -55, -65, -41, -22, 23, 46, 61, 55, 25, 10, -25, -42, -55, -40, -8, 3, 20, 23, 20, 16, 16, 7, 0, -3, -8, -6, -9, -15, -21, -15, 0, 17, 27, 29, 24, 21, 5, -13, -32, -49, -38, -25, 4, 38, 50, 40, 28, 15, 2, -21, -51, -39, -43, -17, 7, 21, 48, 34, 39, 38, 12, -19, -42, -54, -48, -35, -14, 18, 36, 41, 43, 35, 20, 3, -17, -36, -41, -32, -24, -9, 6, 20, 25, 41, 43, 27, 17, -11, -37, -50, -47, -35, -18, 16, 32, 45, 36, 19, 1, -7, -7, -19, -7, -14, -14, -21, -23, -1, 13, 32, 45, 38, 33, 11, -11, -31, -48, -49, -50, -32, 1, 33, 59, 64, 59, 43, 7, -21, -60, -73, -79, -56, -12, 19, 58, 59, 72, 63, 35, 2, -39, -61, -78, -71, -46, -10, 17, 51, 78, 75, 71, 24, -17, -40, -70, -81, -65, -45, -2, 39, 66, 84, 64, 37, 5, -9, -39, -68, -78, -61, -22, 18, 45, 59, 63, 54, 39, 12, -24, -68, -80, -69, -39, 1, 31, 50, 66, 56, 45, 15, -18, -38, -63, -51, -43, -26, -3, 27, 56, 73, 68, 32, -3, -42, -54, -59, -50, -30, -8, 35, 64, 72, 48, 9, -25, -38, -41, -39, -32, -24, 0, 19, 40, 46, 32, 11, 2, -6, -19, -39, -53, -39, -18, 17, 45, 53, 46, 34, 19, -6, -42, -63, -56, -34, 6, 31, 43, 34, 17, 13, 0, -11, -13, -16, -10, 1, 0, -8, -12, -15, -1, 16, 22, 26, 19, 10, 3, -13, -34, -42, -34, -9, 12, 37, 39, 32, 27, 7, -4, -26, -40, -44, -34, -11, 15, 32, 33, 37, 31, 10, -6, -32, -36, -29, -17, 7, -1, 0, 10, 24, 34, 18, 1, -16, -21, -15, -7, -7, -14, -3, 8, 22, 23, 9, 9, 0, -1, -5, -26, -32, -23, -3, 21, 19, 17, 14, 9, 11, 0, -15, -29, -21, -1, 14, 13, -3, -5, -9, -1, 11, 1, 7, 4, 7, 9, -8, -8, -18, -10, 2, 4, 7, -5, 0, 7, 13, 7, -3, 0, -4, -1, -6, -11, -11, -10, 6, 23, 18, 15, 2, -4, -4, -22, -20, -14, 3, 9, 19, 13, 9, 5, -9, -1, -14, -17, -13, -4, 9, 13, 21, 16, 6, 4, -5, -17, -20, -15, -9, 8, 13, 19, 21, 9, 7, -1, -10, -22, -26, -16, 0, 6, 14, 18, 17, 17, -5, -12, -14, -25, -13, -6, 4, 7, 11, 22, 25, 17, -5, -18, -28, -25, -20, -7, 8, 21, 29, 33, 27, 6, -22, -35, -33, -21, -4, 3, 18, 20, 22, 23, -3, -11, -20, -19, -5, -3, 2, 1, 4, 8, 10, 9, 0, 0, 0, 3, 0, -13, -12, -7, 9, 9, 6, -3, -10, 5, 12, 15, 5, -1, -11, -8, -8, -7, -11, -3, 9, 15, 21, 3, -7, -14, 1, 3, -3, -4, -14, -4, 0, 6, -2, 1, 11, 18, 22, 0, -13, -25, -22, -13, -3, 3, 10, 23, 29, 22, 4, -7, -17, -19, -32, -25, -13, 4, 29, 33, 34, 14, -2, -20, -29, -29, -26, -12, 7, 18, 28, 28, 16, 6, -5, -17, -24, -24, -16, -1, 8, 21, 21, 17, 11, 0, -8, -22, -27, -23, -8, 9, 20, 22, 16, 9, 4, -5, -13, -22, -25, -12, -3, 11, 16, 20, 22, 18, 6, -13, -26, -31, -21, -6, 14, 23, 18, 13, 6, -2, -11, -13, -11, -3, -5, 1, 2, 1, 9, 9, 15, 11, -1, -15, -24, -23, -8, 3, 16, 21, 12, 9, 1, -3, -11, -19, -19, -11, -4, 0, 12, 17, 22, 20, 11, -2, -25, -30, -27, -14, 0, 16, 25, 26, 15, 2, -1, -15, -16, -16, -14, -5, -2, 5, 12, 15, 10, 10, 5, 0, -6, -11, -9, -11, -8, -2, 0, 6, 3, 5, 11, 11, 8, 1, -10, -14, -15, -13, -7, -1, 6, 17, 20, 16, 12, -2, -13, -21, -23, -20, -15, -3, 12, 23, 26, 25, 12, 1, -11, -22, -26, -27, -18, -4, 10, 25, 24, 22, 13, -2, -12, -15, -21, -17, -9, -3, 10, 8, 8, 7, 6, 12, 6, -2, -10, -17, -14, -8, 2, 12, 7, 10, 4, 0, -1, -5, -5, -1, 0, 3, 3, 6, 0, -6, -12, -18, -9, -5, 11, 23, 25, 21, 5, -16, -27, -33, -25, -9, 3, 19, 25, 28, 23, 11, -2, -18, -25, -27, -24, -11, 0, 17, 26, 31, 24, 8, -9, -20, -23, -27, -19, -9, 3, 18, 21, 22, 16, 4, -4, -14, -17, -20, -16, -9, -1, 11, 18, 25, 20, 14, 3, -13, -21, -24, -16, -5, -1, 10, 11, 16, 17, 11, 10, -4, -10, -16, -20, -14, -10, -4, 6, 13, 20, 23, 17, 9, -5, -13, -20, -22, -19, -14, -1, 10, 22, 22, 20, 14, 4, -1, -15, -21, -26, -21, -8, 4, 17, 18, 22, 15, 11, 0, -11, -16, -22, -11, -9, -3, 0, 5, 15, 17, 21, 13, 0, -11, -17, -18, -20, -13, -6, 5, 20, 22, 17, 9, -1, -3, -6, -14, -15, -17, -7, 2, 8, 10, 8, 7, 7, 7, 2, -6, -13, -13, -12, -6, 0, 1, 5, 12, 12, 12, 4, -5, -10, -11, -8, -7, -8, -2, 7, 10, 14, 5, -1, -6, -5, -3, -8, -9, -10, -2, 6, 12, 7, 2, 3, 2, 1, -6, -13, -14, -6, 3, 6, 6, 3, 2, 6, 6, 3, -3, -8, -7, -6, -6, -8, -6, 0, 10, 15, 14, 7, 0, -4, -8, -11, -14, -12, -8, 1, 12, 14, 12, 9, 4, 0, -5, -11, -12, -11, -7, -4, 1, 4, 9, 15, 14, 10, -1, -7, -10, -11, -10, -12, -4, 0, 8, 15, 14, 11, 5, 0, -8, -12, -16, -14, -7, 2, 8, 12, 14, 11, 8, 0, -8, -11, -11, -7, -5, 0, 2, 4, 9, 9, 10, 5, 0, -3, -9, -13, -14, -9, -1, 6, 11, 14, 11, 8, 3, -3, -10, -12, -15, -10, -2, 3, 12, 10, 12, 10, 3, -2, -9, -9, -12, -9, -3, -2, 1, 6, 10, 10, 9, 4, -1, -8, -12, -11, -10, -7, 0, 7, 13, 16, 13, 9, 0, -9, -12, -17, -12, -10, -1, 8, 12, 17, 13, 9, 0, -6, -13, -15, -16, -11, 0, 7, 16, 16, 13, 10, 1, -7, -14, -17, -13, -7, -1, 6, 11, 14, 15, 10, 1, -8, -14, -11, -10, -5, -2, 2, 8, 11, 11, 8, 1, -3, -5, -6, -6, -6, -5, 0, 3, 4, 5, 6, 4, 3, 3, 0, -3, -8, -8, -6, -5, -4, 0, 5, 9, 9, 9, 5, -1, -2, -6, -7, -10, -12, -7, -1, 6, 12, 15, 15, 12, 3, -6, -16, -22, -19, -12, -1, 7, 16, 18, 19, 14, 3, -6, -14, -18, -18, -13, -6, 2, 9, 15, 18, 16, 9, 0, -11, -15, -18, -15, -9, -2, 5, 9, 14, 13, 10, 4, -1, -6, -10, -12, -12, -7, -3, 3, 8, 11, 13, 11, 5, -2, -10, -13, -14, -11, -5, 0, 7, 11, 11, 10, 4, -2, -7, -10, -12, -9, -5, 2, 7, 9, 10, 7, 3, -3, -7, -10, -10, -9, -4, 2, 6, 9, 7, 6, 3, 1, -2, -7, -9, -8, -5, -2, 1, 2, 5, 7, 9, 6, 0, -4, -7, -8, -9, -6, -5, 0, 5, 9, 11, 8, 5, 2, -2, -8, -12, -11, -6, -1, 3, 8, 8, 8, 9, 5, 0, -8, -12, -11, -7, -2, 0, 5, 5, 7, 7, 6, 1, -1, -4, -6, -8, -8, -5, -2, 3, 8, 12, 10, 6, 1, -3, -8, -12, -12, -9, -1, 5, 10, 12, 9, 5, 0, -5, -8, -10, -7, -2, 0, 4, 6, 3, 2, 0, 0, 0, -1, 0, 0, -1, -3, -4, -4, -1, 0, 4, 5, 5, 4, 0, -1, -3, -5, -6, -4, -3, 0, 1, 3, 4, 4, 4, 3, 0, -2, -5, -7, -5, -5, -2, 0, 3, 6, 9, 5, 3, 0, -3, -4, -8, -7, -6, -3, 1, 5, 6, 6, 5, 2, 0, -4, -5, -6, -5, -2, -1, 2, 3, 4, 6, 5, 3, 1, -2, -4, -4, -4, -1, 0, 2, 5, 5, 3, 1, -1, -1, -2, 0, 0, 0, 1, 2, 2, 1, 0, -1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, -1, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, -1, 0, -2, -1, 0, 2, 3, 2, 2, 1, 0, -1, -2, -1, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, -1, -1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, -1, -2, -1, 0, 0, 0, 1, 2, 1, 0, 0, -1, -1, -1, -2, -2, -2, -1, 0, 2, 3, 3, 3, 2, 0, -2, -5, -6, -5, -3, 0, 3, 5, 6, 4, 2, 0, -4, -5, -5, -3, -1, 1, 2, 4, 4, 3, 2, 0, -2, -4, -3, -2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 1, 1, 0, 0, 0, -1, -1, -1, 0, 0, 1, 2, 1, 0, 0, 0, -1, -1, -2, -2, 0, 1, 2, 1, 2, 1, 0, -1, -2, -3, -3, -1, 0, 1, 2, 3, 2, 2, 1, 0, -2, -3, -3, -3, 0, 0, 3, 4, 3, 3, 1, 0, -1, -3, -4, -3, -2, 0, 2, 4, 5, 5, 3, 0, -1, -4, -6, -5, -4, -1, 1, 4, 5, 5, 4, 1, -2, -4, -5, -5, -3, -1, 0, 2, 3, 4, 3, 2, 0, 0, -2, -3, -4, -3, -2, 0, 1, 4, 5, 5, 3, 2, -1, -4, -6, -5, -3, 0, 2, 4, 5, 4, 3, 0, -2, -4, -5, -3, -1, 0, 1, 2, 3, 3, 3, 1, 0, -2, -2, -2, -2, -1, 0, 1, 1, 2, 2, 1, 1, 0, 0, -1, -2, -2, -1, -1, 0, 1, 2, 3, 3, 2, 1, -1, -1, -2, -2, -2, -1, 0, 1, 3, 3, 3, 2, 0, -1, -3, -3, -3, -2, 0, 2, 3, 4, 3, 2, 0, -1, -3, -4, -3, -2, 0, 1, 3, 3, 3, 3, 1, 0, -2, -3, -3, -2, -2, 0, 1, 2, 3, 3, 2, 0, -1, -2, -3, -3, -2, 0, 0, 2, 2, 3, 2, 1, 0, -1, -2, -2, -3, -1, 0, 1, 2, 3, 3, 2, 0, 0, -1, -2, -2, -1, -1, 0, 0, 1, 2, 3, 3, 1, 0, -1, -3, -3, -2, -1, 0, 2, 3, 4, 3, 2, 0, -1, -2, -2, -2, -1, 0, 1, 2, 3, 3, 2, 1, 0, -1, -2, -2, -2, -1, 0, 1, 3, 3, 3, 2, 1, -1, -2, -3, -3, -2, 0, 1, 2, 4, 4, 3, 1, -1, -2, -3, -3, -1, 0, 1, 2, 3, 3, 2, 1, 0, -1, -2, -2, -2, -1, 0, 1, 2, 2, 2, 1, 0, 0, -1, -2, -1, -1, 0, 0, 0, 2, };

#define CLICKS_BUF_LEN 32
volatile nframes_t clicks[CLICKS_BUF_LEN];
volatile index_t indices[CLICKS_BUF_LEN];
index_t clicks_ahead;

uint16_t bpm; // range 1 - 300
#define BPM_MIN 1
#define BPM_MAX 300

byte subdiv; // beat subdivisions
#define SUBDIV_MIN 1 // generally a quarter note
#define SUBDIV_MAX 16

byte meas_len; // beats per measure
#define MEAS_LEN_MIN 1
#define MEAS_LEN_MAX 16

byte measures; // measures per pattern
#define MEASURES_MIN 1
#define MEASURES_MAX 16

#define PATTERN_MAX (SUBDIV_MAX * MEAS_LEN_MAX * MEASURES_MAX)
byte pattern[PATTERN_MAX];
index_t pattern_length;

bool precount_enabled = false;
byte precount = 0;

bool metronome_playing = false;

// CLICK

void erase_click_by_index(index_t index) {
  for (index_t i = 1; i < clicks_ahead; i++)
    if (indices[i] == index)
      clicks[i] = 0;
}

void get_prev_click(nframes_t *prev_click, index_t *prev_i) {
  for (byte i = clicks_ahead - 1; i >= 0; i--) {
    if (clicks[i]) {
      *prev_click = clicks[i];
      *prev_i = indices[i];
      return;
    }
  }
  *prev_click = 0;
  *prev_i = 0;
}

volatile nframes_t current_tick = 0; // calling the time at each passing of a subdivision a "tick"
volatile nframes_t next_tick = 0;
volatile index_t current_i = 0;
volatile index_t next_i = 0;

void play_click() {
  static bool speaker_on = false;
  static bool light_on = false;
  int16_t output = 2048;
  bool playing = false;
  bool flashing = false;
  nframes_t now = frames;
  nframes_t current_tick_internal = current_tick;
  nframes_t next_tick_internal = next_tick;
  index_t current_i_internal = current_i;
  index_t next_i_internal = next_i;
  for (byte i = 1; i < clicks_ahead; i++) {
    if(!clicks[i] && i + 1 < clicks_ahead) { // move clicks
      clicks[i] = clicks[i + 1];
      indices[i] = indices[i + 1];
      clicks[i + 1] = 0;
    }
    if(clicks[i]) { // play clicks
      if (now > clicks[i] - SPEAKER_RAMP_UP)
	playing = true;
      if (now >= clicks[i]) {
	flashing = true;
	current_tick_internal = clicks[i];
	next_tick_internal = current_tick_internal + interval;
	current_i_internal = indices[i];
	next_i_internal = (current_i_internal + 1) % pattern_length;
	nframes_t elapsed = now - clicks[i];
	if (elapsed < CLICK_LEN) {
	  output = (int16_t)(click_pcm[elapsed] * output_gain) + 2048;
	} else {
	  clicks[i - 1] = clicks[i];
	  indices[i - 1] = indices[i];
	  clicks[i] = 0;
	}
      }
    }
  }
  next_tick = next_tick_internal;
  next_i = next_i_internal;
  if (frames >= next_tick_internal) {
    current_tick = next_tick;
    next_tick = current_tick + interval;
    current_i = next_i;
    next_i = (current_i + 1) % pattern_length;
  } else {
    current_tick = current_tick_internal;
    current_i = current_i_internal;
  }
  analogWrite(ARCADA_AUDIO_OUT, output);
  if (light_on != flashing) {
    light_on = flashing;
    if (light_on)
      arcada.pixels.setPixelColor(0, 16, 0, 0);
    else
      arcada.pixels.setPixelColor(0, 0, 0, 0);
    update_pixels();
  }
  if (speaker_on != playing) {
    arcada.enableSpeaker(speaker_on = playing);
  }
}

// Metronome

inline void update_interval() {
  interval = (pdmspi.sampleRate * 60.0) / (bpm * subdiv);
  half_interval = interval / 2;
  clicks_ahead = min(max(3, 2 + SPEAKER_RAMP_UP / interval), CLICKS_BUF_LEN);
}

void stop_metronome() {
  for (index_t i = 0; i < CLICKS_BUF_LEN; i++) // clear all clicks
    clicks[i] = 0;
  metronome_playing = false;
  arcada.pixels.setPixelColor(0, 0, 0, 0);
  arcada.pixels.setPixelColor(4, 16, 16, 0);
  update_pixels();
}

void start_metronome() {
  metronome_playing = true;
  if (precount_enabled) {
    precount = meas_len + 1;
    clicks[0] = (frames - interval * subdiv) + SPEAKER_RAMP_UP;
    indices[0] = pattern_length - subdiv;
  } else {
    clicks[0] = (frames - interval) + SPEAKER_RAMP_UP;
    indices[0] = pattern_length - 1;
  }
  arcada.pixels.setPixelColor(4, 0, 16, 0);
  update_pixels();
}

void metronome_stop_start() {
  if (metronome_playing)
    stop_metronome();
  else
    start_metronome();
}

bool chance_play(index_t i) {
  if (chance)
    return randomly_correct() ? pattern[i] : !pattern[i];
  else
    return pattern[i];
}

void metronome() {
  static uint16_t old_bpm = bpm;
  static byte old_subdiv = subdiv;

  if (old_bpm != bpm || old_subdiv != subdiv) {
    update_interval();
    for (index_t i = 1; i < CLICKS_BUF_LEN; i++) // clear all future clicks
      clicks[i] = clicks[i] >= frames ? 0 : clicks[i];
    old_bpm = bpm;
    old_subdiv = subdiv;
  }
  
  if (!clicks[clicks_ahead - 1] && metronome_playing) {
    index_t next_i_maybe;
    nframes_t next_click_maybe;
    index_t check_i;
    nframes_t prev_click;
    index_t prev_i;
    get_prev_click(&prev_click, &prev_i);
 
    if (precount) { // count in
      if (precount == 1) {
	check_i = next_i_maybe = 0;
	next_click_maybe = prev_click + interval * subdiv;
	while(!pattern[next_i_maybe]) {
	  next_i_maybe = (next_i_maybe + 1) % pattern_length;
	  next_click_maybe += interval;
	  if (next_i_maybe == check_i) { // empty pattern
	    precount--;
	    return;
	  }
	}
	clicks[clicks_ahead - 1] = next_click_maybe;
	indices[clicks_ahead - 1] = next_i_maybe;
      } else {
	clicks[clicks_ahead - 1] = prev_click + interval * subdiv;
	indices[clicks_ahead - 1] = pattern_length - (precount - 1) * subdiv;
      }
      precount--;
    } else { // play the pattern
      check_i = next_i_maybe = (prev_i + 1) % pattern_length;
      next_click_maybe = prev_click + interval;
      while(!chance_play(next_i_maybe)) {
	next_click_maybe += interval;
	next_i_maybe = (next_i_maybe + 1) % pattern_length;
	if (check_i == next_i_maybe) { // empty pattern
	  return;
	}
      }
      clicks[clicks_ahead - 1] = next_click_maybe;
      indices[clicks_ahead - 1] = next_i_maybe;
    }
  }
}

void set_pattern_length() {
  pattern_length = subdiv * meas_len * measures;
}

void reset_pattern() {
  // set pattern to click once per beat
  for (int i = 0; i < PATTERN_MAX; i++) {
    pattern[i] = !(i % subdiv);
  }
}

void clear_pattern() {
  // remove all clicks from pattern
  for (int i = 0; i < PATTERN_MAX; i++)
    pattern[i] = 0;
}

#endif
