//*****************************************************************************
//*
//* EX_SoundFX.h
//*   Copyright © 2024 Michael Schaff
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

#ifndef Obj_SoundFX_h
#define Obj_SoundFX_h

// #include <SPI.h>
// #include <SD.h>
// #include <Adafruit_VS1053.h>

//=============================================================================
// EX_SoundFX class
//-----------------------------------------------------------------------------
class EX_SoundFX
{
public:
// Constructor
  EX_SoundFX();

// Implementation
public:
  bool begin(void);
  void run(void);

  bool playFX(uint8_t soundTrack, bool wait);
  bool isBusy();

// Members
private:
  bool
    m_hasSoundFX;

  uint8_t
    m_soundTrack;

protected:
  // Adafruit_VS1053_FilePlayer
  //   m_objVS1053 = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, VS1053_CARDCS);
};

#endif
