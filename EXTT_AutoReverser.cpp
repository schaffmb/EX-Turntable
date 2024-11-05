//*****************************************************************************
//*
//* EXTT_AutoReverser.cpp
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
#include "EXTT_AutoReverser.h"

#include "defines.h"

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXTT_AutoReverser::EXTT_AutoReverser()
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
bool EXTT_AutoReverser::begin()
{
  if (trace)
    Serial.println("EXTT_AutoReverser::begin()");

  pinMode(DCCEX_AR_ACS712, INPUT);
  pinMode(DCCEX_AR_TRIGGER, OUTPUT);

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
void EXTT_AutoReverser::run(void)
{
  float
    vpp         = 0.0048828125,   // Measure voltage on your arduino and divide it with 1024 to get this value
    sensitivity = 0.185;          // Sensetivity of in mv

  int
    counts  = analogRead(DCCEX_AR_ACS712) + 5;  // Read analog signal from current sensor

  float
    voltage = counts * vpp;

  voltage -= 2.5;  // Remove offset

  float
    ampere = voltage / sensitivity;  // Divide by sensor sensetivity

  Serial.println("Amps: " + String(ampere));

  if (ampere > 1.0)
    {
    digitalWrite(DCCEX_AR_TRIGGER, !digitalRead(DCCEX_AR_TRIGGER));  // Toggle output
    delay(500);   // Wait to avoid false triggers
    }

  return;
}
