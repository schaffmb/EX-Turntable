//*****************************************************************************
//*
//* EX_SoundFX.cpp
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

#include "EX-Turntable.h"
#include "EXTT_SoundFX.h"

#include "defines.h"

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EX_SoundFX::EX_SoundFX()
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
//*   None.
//*
//*****************************************************************************
bool EX_SoundFX::begin(void)
{
  if (trace)
    Serial.println("EX_SoundFX::begin()");

  //============================================================================
  // ...
  //----------------------------------------------------------------------------
  // if (m_objVS1053.begin())
  //   {
  //   // Make a tone to indicate VS1053 is working
  //   //m_objVS1053.sineTest(0x44, 600);

  //   // Set volume for left, right channels. lower numbers == louder volume!
  //   m_objVS1053.setVolume(5, 5);

  //   if (SD.begin(DCCEX_VS1053_CARDCS))
  //     {
  //     // If DREQ is on an interrupt pin we can do background audio playing
  //     m_objVS1053.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  //     }
  //   else
  //     // Couldn't find SD card
  //     errNo = ERRNO_NOMICROSD;
  //   }
  // else
  //   errNo = ERRNO_NOSOUND;

  // m_objVS1053.playFullFile("/SNDFX_~1.mp3");
  // playFX(SNDFX_REPORT, true);

  return (errNo == ERRNO_NONE ? true : false);
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
void EX_SoundFX::run(void)
{
  ;
  ;
  ;

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   soundTrack - ???.
//*   wait       - ???.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
bool EX_SoundFX::isBusy()
{
  if (trace)
    Serial.println("EX_SoundFX::isBusy()");

  return (false); //m_hasSoundFX == true ? m_objVS1053.playingMusic : false);
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   soundTrack - ???.
//*   wait       - ???.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
bool EX_SoundFX::playFX(uint8_t soundTrack, bool wait)
{
  String
    strFName = "SNDFX_",
    strTrack = "/";

  if (trace)
    Serial.println("EX_SoundFX::playFX(" + String(soundTrack) + ", " + (wait ? "true" : "false") + ")");

  switch (soundTrack)
    {
    // ...
    //============================================================================
    case SNDFX_REPORT:
        {
        strFName.concat("~1");
        break;
        }

    // ...
    //============================================================================
    case SNDFX_STARTUP:
        {
        strFName.concat("~2");
        break;
        }

    // ...
    //============================================================================
    case SNDFX_SLEW:
        {
        strFName.concat("~3");
        break;
        }

    // ...
    //============================================================================
    case SNDFX_SHUTDOWN:
        {
        strFName.concat("~4");
        break;
        }

    // ...
    //============================================================================
    default:
      return (false);
    }

  strTrack.concat(strFName);
  strTrack.concat(".mp3");

  if (debug)
    Serial.println(strTrack);

  // if (m_hasSoundFX)
  //   if (wait)
  //     return (m_objVS1053.playFullFile(strTrack.c_str()));
  //   else
  //     return (m_objVS1053.startPlayingFile(strTrack.c_str()));

  return (true);
}
