//*****************************************************************************
//*
//* EX-Turntable.h
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

#ifndef EX_Turntable_h
#define EX_Turntable_h

#include <Arduino.h>

#include "config.turntable.h"

// Command definitions
//////////////////////////////////////////////////////////////////////////////
#define DCCEX_TT_RESET      0x01
#define DCCEX_TT_HOME       0x02
#define DCCEX_TT_MOVE       0x03
#define DCCEX_TT_STOP       0x04
#define DCCEX_TT_RIGHT      0x05
#define DCCEX_TT_LEFT       0x06
#define DCCEX_TT_CALIBRATE  0x07
#define DCCEX_TT_VERSION    0x08
#define DCCEX_TT_TRACK      0x09
#define DCCEX_TT_UNITTEST   0x0A
#define DCCEX_TT_DEBUG      0x0B

#define DCCEX_TT_ISBUSY     0x01

//=============================================================================
// EXTT_Turntable namespace
//-----------------------------------------------------------------------------
namespace EXTT_Turntable
{
// Implementation
  bool begin(void);
  void run(void);

  bool onReceive(uint8_t msg, uint32_t param = 0);
  uint32_t onRequest(uint8_t msg);

};

#endif
