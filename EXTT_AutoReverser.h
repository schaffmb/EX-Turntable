//*****************************************************************************
//*
//* EXTT_AutoReverser.h
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

#ifndef Obj_AutoReverser_h
#define Obj_AutoReverser_h

// ...
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_AR_ACS712     A0 // Analog input pin
#define DCCEX_AR_TRIGGER    3  // Pin connected to NPN transitor 

//=============================================================================
// EXTT_AutoReverser class
//-----------------------------------------------------------------------------
class EXTT_AutoReverser
{
public:
// Constructor
  EXTT_AutoReverser();

// Implementation
public:
  bool begin(void);
  void run(void);

};

#endif
