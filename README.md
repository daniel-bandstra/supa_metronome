# supa_metronome
Supa Metronome: A Responsive Metronome for the Adafruit EdgeBadge

I wanted a metronome as nice and basic as my trusty Seiko SQ50-V, but with some extra magical properties. These properties included: BPM settings lower than 40, the ability to play patterns other than steady quarter notes, a one-measure quarter-note count before repeating the pattern indefinitely, and the ability to click randomly but in-time with the beat. My slightly less basic Teenage Engineering PO-12 could do some of these things, but not enough of them. I figured the best way was to program a gadget in Arduino. The best way to make an interface to that gadget was to buy an Adafruit EdgeBadge. I went for the EdgeBadge because I thought the built-in microphone could come in useful and I was right.

## Setup
Supa Metronome uses the Adafruit Arcada library. It expects to find the QSPI Flash Filesystem where it can create a file to remember your settings. I didn't create this filesystem, I think it was a byproduct of running one of the Adafruit speech recognition demos on the EdgeBadge.

## Usage
The Start Button starts and stops the metronome. The Left and Right buttons navigate between different settings. Unless there's a menu that the Select button by itself would pop up, pressing Select and Start together gives the option of resetting all the settings to some reasonable defaults. Here are the various settings among which you can navigate:

### bpm
Use the Up/Down keys to adjust this from 1 to 300 bpm. Use the Select key for Tap tempo. This is a literal tap-tempo feature. It uses the built-in accelerometer to sense physical taps to the EdgeBadge and sets the BPM accordingly.

### edit pattern
Press the Down button to start editing, navigate with the Left/Right buttons, press A to toggle a beat. Press Up when done.

### count in
Toggles whether a measure of quarter notes is played before your pattern.

### chance
This is a randomness factor from 0 (only plays your pattern) to 128 (every subdivision has a 50% chance of being silent when the pattern says to click or vice versa).

### beat tracking
This is a feature that I had not anticipated programming. When you're on the following three screens, the microphone listens for musical or percussive events. It's not very sophisticated. It works by detecting sudden increases in the volume level. The red LED on the back of the EdgeBadge will flash for each detected event. If the metronome is playing, then the display will show a black vertical line and a red block. The more the block goes to the left of the line, the more you're rushing, relative to the nearest subdivision of the beat. The more to the right, the more you're dragging.

#### mic gain
This turns the microphone up or down. 100 is unity gain.

#### gain thresh
This is a percentage. A noise has to be this much louder than the average noise level in order to register as a musical event.

#### acoustic delay
This is how long it takes a sound to go from the metronome's speaker, out into the world, and back into the microphone. If you're in a quiet room, you can press the Select button for the metronome to calibrate this automatically by listening to itself while it plays.

### backlight
This is the screen brightness, from 0 (off) to 255 (too bright).

### volume
Pretty self explanatory.

### subdivisions
This is the number of subdivisions per beat, e.g. 4 would generally indicate 16th notes. This is useful for pattern writing, randomization, and beat-tracking.

### measure length
This is the number of beats per measure.

### measures
This is the number of measures in the pattern. Useful for those who want their metronome to click only once every other measure, or every four measures, and so on.
