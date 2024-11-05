//*****************************************************************************
//*
//* EXTT_Serial.cpp
//*   Copyright © 2023 Peter Cole, Michael Schaff
//*   All rights reserved.
//*
//* Brief:
//*   ???.
//*
//* Notes:
//*   This file is part of the DCC-EX family of products.
//*
//*   This is free software: you can redistribute it and/or modify
//*   it under the terms of the GNU General Public License as published by
//*   the Free Software Foundation, either version 3 of the License, or
//*   (at your option) any later version.
//*
//*   It is distributed in the hope that it will be useful,
//*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//*   GNU General Public License for more details.
//*
//*   You should have received a copy of the GNU General Public License
//*   along with the product.  If not, see <https://www.gnu.org/licenses/>.
//*
//*****************************************************************************

#include "EX-Turntable.h"
#include "EXTT_Serial.h"

#include "defines.h"

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXTT_Serial::EXTT_Serial()
{
  serialIsBusy  = false;
  serialNewData = false;
  testCmdSent   = false;

  testActivity  = 0;
  testStepsMSB  = 0;
  testStepsLSB  = 0;

  serialIndex   = 0;
}

//*****************************************************************************
//*
//* Brief:
//*   Start Serial communication.
//*
//* Parameters:
//*   baud - Baud rate (optional).
//*
//* Returns:
//*   true if successful, otherwise false.
//*
//*****************************************************************************
bool EXTT_Serial::begin(unsigned long baud)
{
  // Basic setup, display what this is.
  Serial.begin(baud);
  while (!Serial);

  if (trace)
    Serial.println("EXTT_Serial::begin(" + String(baud, DEC) + ")");

  return (true);
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_Serial::run(void)
{
  char
    startMarker = '<',
    endMarker   = '>',
    serialChar;

  //============================================================================
  // ...
  //----------------------------------------------------------------------------
  while (Serial.available() > 0 && serialNewData == false)
    {
    serialChar = Serial.read();

    if (serialIsBusy)
      {
      if (serialChar != endMarker)
        {
        serialBuffer[serialIndex++] = serialChar;

        if (serialIndex >= 20)
          serialIndex = 20 - 1;
        }
      else
        {
        serialBuffer[serialIndex] = '\0';

        serialIsBusy  = false;
        serialNewData = true;
        serialIndex   = 0;
        }
      }
    else if (serialChar == startMarker)
      serialIsBusy = true;
    }

  //============================================================================
  // ...
  //----------------------------------------------------------------------------
  if (serialNewData)
    {
    serialNewData = false;

    char
      *strtokIndex = strtok(serialBuffer, " "),
      command = strtokIndex[0];     // first parameter is activity

    strtokIndex  = strtok(NULL, " ");  // space separator

    switch (command)
      {
      // Calibrate.
      //============================================================================
      case 'C':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_CALIBRATE);
        break;
        }

      // Debug.
      //============================================================================
      case 'D':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_DEBUG);
        break;
        }

      // Home.
      //============================================================================
      case 'H':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_HOME);
        break;
        }

      // Move.
      //============================================================================
      case 'M':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_MOVE, atol(strtokIndex));
        break;
        }

      // Stop.
      //============================================================================
      case 'S':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_STOP);
        break;
        }

      // Step right (clockwise).
      //============================================================================
      case 'R':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_RIGHT, atol(strtokIndex));
        break;
        }

      // Step left (counter-clockwise).
      //============================================================================
      case 'L':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_LEFT, atol(strtokIndex));
        break;
        }

      // Move to track.
      //============================================================================
      case 'T':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_TRACK, atol(strtokIndex));
        break;
        }

      // Perform unit testing.
      //============================================================================
      case 'U':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_UNITTEST, atol(strtokIndex));
        break;
        }

      // Display version and other info.
      //============================================================================
      case 'V':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_VERSION);
        break;
        }

      // Reset.
      //============================================================================
      case 'X':
        {
        EXTT_Turntable::onReceive(DCCEX_TT_RESET, atol(strtokIndex));
        break;
        }

      // ...
      //============================================================================
      default:
        break;
      }
    }

  return;
}
