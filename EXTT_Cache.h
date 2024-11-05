//*****************************************************************************
//*
//* EEPROMFunctions.cpp
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

#ifndef obj_Cache_h
#define obj_Cache_h

#include "version.h"

//=============================================================================
// EXTT_Cache class
//-----------------------------------------------------------------------------
class EXTT_Cache
{
public:
// Constructor
  EXTT_Cache();

// Implementation
public:
  bool begin(void);

  void clearEEPROM(void);
  void forceEEPROM(void);
  void writeEEPROM(bool fullWrite = false);

// Accessors
public:
  uint32_t getLastPosition(void);
  void setLastPosition(uint32_t v);

  uint32_t getStepHalf(void);
  void setStepHalf(uint32_t v);

  uint32_t getStepFull(void);
  void setStepFull(uint32_t v);

  uint16_t getTrackIndx(void);
  void setTrackIndx(uint16_t v);

  uint16_t getTrackLast(void);
  void setTrackLst(uint16_t v);

  int32_t getIndxToPosition(uint8_t v);
  int32_t getIndxToTrack(uint8_t v);
  int8_t getTrackToIndx(int32_t v);

// Members
private:
  struct
    {
    struct
      {
      char
        eepromFlag[10]    = { SIGNATURE },
        eepromVersion[10] = { VERSION };

      } signature;

    struct
      {
      uint32_t
        stepHalf,
        stepFull;

      } config;

    uint8_t
      trackIndx,
      trackLast;

    uint32_t
      lastPosition;

    } m_objLastState;

  uint8_t
    m_trackXrefMax,
    m_trackXrefHlf;

            //  +--------- Track (-Track = reverse head)
            //  |
            //  |   +---- Stepper index (0-999999)
            //  |\  |
  int32_t   //  vv  v
    m_trackXref[96][2];

protected:
  ;
  ;
  ;

};

#endif
