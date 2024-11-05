//*****************************************************************************
//*
//* EXTT_I2C.h
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

#ifndef obj_I2C_h
#define obj_I2C_h

#include <Arduino.h>
#include <Wire.h>

// I2C Communication
//////////////////////////////////////////////////////////////////////////////
extern uint8_t
  _addrTT;

extern TwoWire
  *_wireTT;

//=============================================================================
// EXTT_I2C class
//-----------------------------------------------------------------------------
class EXTT_I2C
{
public:
// Constructor
  EXTT_I2C();

// Implementation
public:
  bool begin(const uint8_t addr = DCCEX_I2C_ADDRESS, TwoWire *wire = &Wire);

  static void receiveEvent(int received);
  static void requestEvent(void);

// Accessors
public:
  uint8_t getAddr() const  { return (_addrTT); }
  TwoWire* getWire() const { return (_wireTT); }

// Members
private:
  ;

protected:
  ;

};

#endif
