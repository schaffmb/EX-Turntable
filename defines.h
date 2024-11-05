/*
 *  © 2023 Peter Cole
 *
 *  This file is part of EX-Turntable
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EX-Turntable.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef Defines_h
#define Defines_h

// ...
///////////////////////////////////////////////////////////////////////////////
extern uint8_t
  errNo,
  executionMode;

extern bool
  debug,
  trace;

extern String
  strX;

// ...
//////////////////////////////////////////////////////////////////////////////
#define FAILURE               -1

// Error codes
//////////////////////////////////////////////////////////////////////////////
#define ERRNO_NONE            0
#define ERRNO_NOSTEPPER       1
#define ERRNO_NOSOUND         2
#define ERRNO_NOMICROSD       3
#define ERRNO_SIGNATURE       4
#define ERRNO_VERSION         5

// Execution modes
//////////////////////////////////////////////////////////////////////////////
#define EXEMODE_NONE          0
#define EXEMODE_IDLE          1
#define EXEMODE_CALIBRATE     2
#define EXEMODE_HOME          3
#define EXEMODE_MOVE          4

// I2C Addresses
//////////////////////////////////////////////////////////////////////////////
#define I2C_GPIO              0x20
#define I2C_EEPROM            0x50
#define I2C_STEPPER           0x60

// Sound FX
//////////////////////////////////////////////////////////////////////////////
#define VS1053_RESET          -1  // VS1053 reset pin (not used!)

#define VS1053_CS             7   // VS1053 chip select pin (output)
#define VS1053_DCS            6   // VS1053 Data/command select pin (output)
#define VS1053_DREQ           3   // VS1053 Data request, ideally an Interrupt pin *if possible* (not possible on 32u4)
#define VS1053_CARDCS         4   // Card chip select pin

#define SNDFX_REPORT          1
#define SNDFX_STARTUP         2
#define SNDFX_SLEW            3
#define SNDFX_SHUTDOWN        4


// Ensure AUTO and MANUAL phase switching has a value to test.
#define AUTO                  1
#define MANUAL                0

// Ensure TURNTABLE and TRAVERSER modes also have a value to test.
#define TURNTABLE             0
#define TRAVERSER             1
#define SWINGBRIDGE           2

#include "config.turntable.h"

// Define global variables here.
#ifndef TURNTABLE_EX_MODE
  #define TURNTABLE_EX_MODE     TURNTABLE   // If the mode isn't defined, put it in turntable mode.
#endif

#ifndef STEPPER_MAX_SPEED
  #define STEPPER_MAX_SPEED     200         // Set default max speed if not defined.
#endif

#ifndef STEPPER_ACCELERATION
  #define STEPPER_ACCELERATION  25          // Set default acceleration if not defined.
#endif

#ifndef SANITY_STEPS
  #define SANITY_STEPS          10000       // Define sanity steps if not in config.h.
#endif

#ifndef HOME_SENSITIVITY
  #define HOME_SENSITIVITY      300         // Define homing sensitivity if not in config.h.
#endif

#ifndef PHASE_SWITCHING
  #define PHASE_SWITCHING       AUTO        // Define automatic phase switching if not in config.h
#endif

#ifndef PHASE_SWITCH_ANGLE
  #define PHASE_SWITCH_ANGLE    45          // Define phase switch at 45 degrees if not in config.h
#endif

#ifndef DEBOUNCE_DELAY                      // Define debounce delay in ms if not in config.h
  #if TURNTABLE_EX_MODE == TRAVERSER
    #define DEBOUNCE_DELAY        10        // If we're a traverser, use a delay because switches likely in use
  #elif TURNTABLE_EX_MODE == TURNTABLE
    #define DEBOUNCE_DELAY        0         // If we're a turntable, use 0 because hall effect sensor likely in use
  #endif
#endif

#ifndef STEPPER_GEARING_FACTOR
  #define STEPPER_GEARING_FACTOR  1         // Define the gearing factor to default of 1 if not in config.h
#endif

// Define current version of EEPROM configuration
#define EEPROM_VERSION    2

#if defined(ROTATE_FORWARD_ONLY) && defined(ROTATE_REVERSE_ONLY)
  #error Both ROTATE_FORWARD_ONLY and ROTATE_REVERSE_ONLY defined, please only define one or the other
#endif

#if (TURNTABLE_EX_MODE == TRAVERSER && defined(ROTATE_FORWARD_ONLY)) || (TURNTABLE_EX_MODE == TRAVERSER && defined(ROTATE_REVERSE_ONLY))
  #error Traverser mode cannot operate with ROTATE_FORWARD_ONLY or ROTATE_REVERSE_ONLY
#endif


//typedef unsigned long DWORD;
//typedef unsigned short WORD;
//#define LOWORD(a) ((WORD)(a))
//#define HIWORD(a) ((WORD)(((DWORD)(a) >> 16) & 0xFFFF))
// 
//    DWORD d=0xABCD1234;
// 
//    printf("Original value:\n");
//    printf("       d  == %X\n",d);
// 
//    printf("\nMacros:\n");
//    printf("LOWORD(d) == %X\n", LOWORD(d));
//    printf("HIWORD(d) == %X\n", HIWORD(d));
//
//    printf("\nSimple hiword:\n");
//    printf("  d >> 16 == %X\n", d >> 16);


#endif
