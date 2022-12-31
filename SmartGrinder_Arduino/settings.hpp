#ifndef __SETTINGS_HPP
#define __SETTINGS_HPP

#include <Arduino.h>

// Load eeprom parameters as far as available with settings here as fallback
// ATTENTION: On a new board, this must at least be true ONCE to initialize the EEPROM to valid values!
#define LOAD_INITIAL_PARAMETERS_FROM_EEPROM 1  // Default true;

// Turn on/off debug information for each module
#define DEBUG_EEPROM_MEMORY 0
#define DEBUG_BUTTON 0

#pragma region debug helper definitions

// Always overwrite eeprom values on setting new parameters in this script
#if LOAD_INITIAL_PARAMETERS_FROM_EEPROM
#define INITIALIZE_EEPROM 0
#endif

#if DEBUG_EEPROM_MEMORY == 0
#define LOG_EEPROM_MEMORY(message)
#else
#define LOG_EEPROM_MEMORY(message) Serial.println(message);
#endif

#if DEBUG_COMM == 0
#define LOG_COMM(message)
#else
#define LOG_COMM(message) Serial.println(message);
#endif

#if DEBUG_BUTTON == 0
#define LOG_BUTTON(message)
#else
#define LOG_BUTTON(message) Serial.println(message);
#endif

#pragma endregion debug helper definitions

#pragma region I / O pin settings

constexpr const int SSR_PIN = 3;
constexpr const int BUTTON_PIN = 7;    // connects to PIN and GND, for brew lever switch

#pragma endregion I / O pin settings

#pragma region User settings (app settable)

extern unsigned long DOUBLE_DOSE_TIME;
extern unsigned long SINGLE_DOSE_TIME;

#pragma endregion User settings (app settable)

#pragma region global program stuff

const uint16_t BUTTON_PRESS_RECOVERY = 300;  // time between button presses which will count as multi click rather than individual clicks in milliseconds

#pragma endregion global program stuff

#endif
