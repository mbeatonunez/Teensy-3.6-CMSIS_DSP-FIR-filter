# Teensy-3.6-CMSIS_DSP-FIR-filter
This project combines the use of MATLAB, a [Teensy 3.6] MCU, the [CMSIS-DSP Library], and a Micro SD card to create an FIR Filter.

[Teensy 3.6]:https://www.pjrc.com/store/teensy36.html
[CMSIS-DSP Library]:http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html

## How it works:
Using MATLAB, a wave file is exported as a binary file, placed in a SD card, then read and filtered by the Teensy 3.6. The output
file is, in turn, imported into MATLAB and played back to confirm that the data has been filtered.

## What is included
1. Instructions
2. Teensy 3.6 code
3. Required CMSIS_DSP library files, modified to work with the Arduino IDE
4. MATLAB scripts to export and import data.
5. Sample wave file.

Enjoy!
