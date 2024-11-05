//*****************************************************************************
//*
//* EXTT_Stepper.h
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

#ifndef obj_Stepper_h
#define obj_Stepper_h

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

// Stepper
//////////////////////////////////////////////////////////////////////////////
#define DIR_CW              1   // Direction of travel (clockwise)
#define DIR_CCW             0   // Direction of travel (counter-clockwise)

//=============================================================================
// EXTT_Stepper class
//-----------------------------------------------------------------------------
class EXTT_Stepper
{
public:
// Constructor
  EXTT_Stepper();

// Implementation
public:
  bool begin(long lastPosition);
  void run(void);

  void moveTo(uint32_t position);
  void stop(void);

  uint16_t step(void);
  void step(uint16_t steps, uint8_t dir);

  bool isBusy(void);

// Members
private:
  bool
    m_isBusy;

  uint16_t
    m_stepDir,
    m_stepCnt;

  float
    m_maxSpeed,
    m_maxAccel;

  ;
  ;
  ;

protected:
  AccelStepper
    *m_objStepper;

  static void stepForward(void);
  static void stepBackward(void);

};

#endif
