//*****************************************************************************
//*
//* EXTT_Cache.cpp
//*   Copyright © 2023-2024 Peter Cole, Michael Schaff
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

#include <EEPROM.h>

#include "EX-Turntable.h"
#include "EXTT_Cache.h"

#include "defines.h"

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXTT_Cache::EXTT_Cache()
{
  ;
  ;
  ;
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
//*   true if successful, otherwise false.
//*
//*****************************************************************************
bool EXTT_Cache::begin(void)
{
  if (trace)
    Serial.println("EXTT_Cache::begin()");

  // Read our configuration table
  //////////////////////////////////////////////////////////////////////////////
  int
    eeAddr = 0;

  EEPROM.get(eeAddr, m_objLastState);  eeAddr += sizeof(m_objLastState);

  if (debug)
    {
    Serial.println(m_objLastState.signature.eepromFlag);
    Serial.println(m_objLastState.signature.eepromVersion);
    }

  if (strcmp(m_objLastState.signature.eepromFlag, SIGNATURE) == 0)
    {
    if (strcmp(m_objLastState.signature.eepromVersion, VERSION) == 0)
      {
      m_trackXrefHlf = 0;
      m_trackXrefMax = 0;

      do
        {
        EEPROM.get(eeAddr, m_trackXref[m_trackXrefHlf][0]);   eeAddr += sizeof(int16_t);
        EEPROM.get(eeAddr, m_trackXref[m_trackXrefHlf][1]);   eeAddr += sizeof(int16_t);

        } while(m_trackXref[m_trackXrefHlf++][0] > 0);

        Serial.println(m_trackXrefHlf);
        Serial.println(m_trackXrefMax);

      // Invert the table
      //////////////////////////////////////////////////////////////////////////////
      for (m_trackXrefHlf -= 2; m_trackXrefHlf != m_trackXrefMax; m_trackXrefMax++)
        {
        m_trackXref[m_trackXrefHlf + m_trackXrefMax][0] = -m_trackXref[m_trackXrefMax][0];
        m_trackXref[m_trackXrefHlf + m_trackXrefMax][1] = m_trackXref[m_trackXrefMax][1] + m_objLastState.config.stepHalf > m_objLastState.config.stepFull
                ? m_trackXref[m_trackXrefMax][1] - m_objLastState.config.stepHalf
                : m_trackXref[m_trackXrefMax][1] + m_objLastState.config.stepHalf;
        }

      m_trackXrefMax *= 2;
      }
    else
      // Couldn't find SD card
      errNo = ERRNO_VERSION;
    }
  else
    errNo = ERRNO_SIGNATURE;

  return (errNo == ERRNO_NONE);
}

//*****************************************************************************
//*
//* Brief:
//*   Write step count with "TTEX" identifier to EEPROM.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_Cache::writeEEPROM(bool fullWrite)
{
  if (trace)
    Serial.println("EXTT_Cache::writeEEPROM()");

  int
    eeAddr = 0;

  //============================================================================
  //----------------------------------------------------------------------------
  EEPROM.put(eeAddr, m_objLastState);   eeAddr += sizeof(m_objLastState);

  if (fullWrite)
    {
    for (int nIndx = 0; nIndx < m_trackXrefHlf; nIndx++)
      {
      EEPROM.put(eeAddr, m_trackXref[nIndx][0]);  eeAddr += sizeof(int16_t);
      EEPROM.put(eeAddr, m_trackXref[nIndx][1]);  eeAddr += sizeof(int16_t);
      }

    EEPROM.put(eeAddr, (int16_t)-99999);  eeAddr += sizeof(int16_t);
    EEPROM.put(eeAddr, (int16_t)-99999);
    }

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Clear EEPROM image.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_Cache::clearEEPROM(void)
{
  if (trace)
    Serial.println("EXTT_Cache::clearEEPROM()");

  for (int eeAddr = 0, maxAddr = EEPROM.length(); eeAddr < maxAddr; eeAddr++)
    EEPROM.write(eeAddr, 0);

  //============================================================================
  //----------------------------------------------------------------------------
  strcpy(m_objLastState.signature.eepromFlag, SIGNATURE);
  strcpy(m_objLastState.signature.eepromVersion, VERSION);

  m_objLastState.config.stepHalf = 0;
  m_objLastState.config.stepFull = 0;

  m_objLastState.trackIndx = 0;
  m_objLastState.trackLast = 0;

  m_objLastState.lastPosition = 0;

  EEPROM.put(0, m_objLastState);
  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Force an image into the EEPROM.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_Cache::forceEEPROM(void)
{
  if (trace)
    Serial.println("EXTT_Cache::forceEEPROM()");

  m_trackXrefMax = 0;
  m_trackXrefHlf = 0;

  //============================================================================
  //----------------------------------------------------------------------------
  int
    eeAddr = 0;

  m_objLastState.config.stepHalf = 2048;
  m_objLastState.config.stepFull = 4096;

  m_objLastState.trackIndx = 2;
  m_objLastState.trackLast = 2;

  m_objLastState.lastPosition = 500;

  EEPROM.put(eeAddr, m_objLastState);   eeAddr += sizeof(m_objLastState);

  m_trackXref[m_trackXrefHlf][0] = 1;   m_trackXref[m_trackXrefHlf++][1] = 25;
  m_trackXref[m_trackXrefHlf][0] = 4;   m_trackXref[m_trackXrefHlf++][1] = 75;
  m_trackXref[m_trackXrefHlf][0] = 7;   m_trackXref[m_trackXrefHlf++][1] = 150;
  m_trackXref[m_trackXrefHlf][0] = 12;  m_trackXref[m_trackXrefHlf++][1] = 900;
  m_trackXref[m_trackXrefHlf][0] = 15;  m_trackXref[m_trackXrefHlf++][1] = 320;
  m_trackXref[m_trackXrefHlf][0] = 16;  m_trackXref[m_trackXrefHlf++][1] = 360;
  m_trackXref[m_trackXrefHlf][0] = 17;  m_trackXref[m_trackXrefHlf++][1] = 400;

  for (int nIndx = 0; nIndx < m_trackXrefHlf; nIndx++)
    {
    EEPROM.put(eeAddr, m_trackXref[nIndx][0]);  eeAddr += sizeof(int16_t);
    EEPROM.put(eeAddr, m_trackXref[nIndx][1]);  eeAddr += sizeof(int16_t);
    }

  EEPROM.put(eeAddr, (int16_t)-99999);  eeAddr += sizeof(int16_t);
  EEPROM.put(eeAddr, (int16_t)-99999);

  // Invert the table
  //////////////////////////////////////////////////////////////////////////////
  for (m_trackXrefHlf -= 2; m_trackXrefHlf != m_trackXrefMax; m_trackXrefMax++)
      {
      m_trackXref[m_trackXrefHlf + m_trackXrefMax][0] = -m_trackXref[m_trackXrefMax][0];
      m_trackXref[m_trackXrefHlf + m_trackXrefMax][1] = m_trackXref[m_trackXrefMax][1] + m_objLastState.config.stepHalf > m_objLastState.config.stepFull
            ? m_trackXref[m_trackXrefMax][1] - m_objLastState.config.stepHalf
            : m_trackXref[m_trackXrefMax][1] + m_objLastState.config.stepHalf;
      }

  m_trackXrefMax *= 2;

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Accessors.
//*
//* Parameters:
//*   Varies.
//*
//* Returns:
//*   Varies.
//*
//*****************************************************************************
uint32_t EXTT_Cache::getLastPosition(void)    { return (m_objLastState.lastPosition); }
void EXTT_Cache::setLastPosition(uint32_t v)  { m_objLastState.lastPosition = v; writeEEPROM(); }

uint32_t EXTT_Cache::getStepHalf(void)        { return (m_objLastState.config.stepHalf); }
void EXTT_Cache::setStepHalf(uint32_t v)      { m_objLastState.config.stepHalf = v; writeEEPROM(); }

uint32_t EXTT_Cache::getStepFull(void)        { return (m_objLastState.config.stepFull); }
void EXTT_Cache::setStepFull(uint32_t v)      { m_objLastState.config.stepFull = v; writeEEPROM(); }

uint16_t EXTT_Cache::getTrackIndx(void)       { return (m_objLastState.trackIndx); }
void EXTT_Cache::setTrackIndx(uint16_t v)     { m_objLastState.trackIndx = v; writeEEPROM(); }

uint16_t EXTT_Cache::getTrackLast(void)       { return (m_objLastState.trackLast); }
void EXTT_Cache::setTrackLst(uint16_t v)      { m_objLastState.trackLast = v; writeEEPROM(); }

int32_t EXTT_Cache::getIndxToPosition(uint8_t v)  { return (v > m_trackXrefMax ? 0 : m_trackXref[v][1]); }
int32_t EXTT_Cache::getIndxToTrack(uint8_t v)     { return (v > m_trackXrefMax ? 0 : m_trackXref[v][0]); }
int8_t EXTT_Cache::getTrackToIndx(int32_t v)      { for (uint8_t nIndx = 0; nIndx < m_trackXrefMax; nIndx++)
                                                      if (m_trackXref[v][0] == v)
                                                        return (m_trackXref[v][0]);

                                                    return (FAILURE);
                                                  }

















