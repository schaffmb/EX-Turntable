//*****************************************************************************
//*
//* EX_Serial.h
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

#ifndef obj_Serial_h
#define obj_Serial_h

//=============================================================================
// EXTT_Serial class
//-----------------------------------------------------------------------------
class EXTT_Serial
{
public:
// Constructor
  EXTT_Serial();

// Implementation
public:
  bool begin(unsigned long baud = 115200);
  void run(void);

// Members
private:
  ;
  ;
  ;

protected:
  char
    serialBuffer[20];

  bool
    serialIsBusy,
    serialNewData,
    testCmdSent;

  uint8_t
    testActivity,
    testStepsMSB,
    testStepsLSB;

  byte
    serialIndex;

};

#endif
