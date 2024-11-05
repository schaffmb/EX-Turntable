//*****************************************************************************
//*
//* EXTT_Stepper.cpp
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
#include "EXTT_Stepper.h"

#include "defines.h"

// Stepper
//////////////////////////////////////////////////////////////////////////////
unsigned long timex;
unsigned long previousAccel = 0;
int interval = 100;  // # of milliseconds between speed increases


#define DCCEX_SM_MOTORPIN1  8   // IN1 on the ULN2003 driver
#define DCCEX_SM_MOTORPIN2  9   // IN2 on the ULN2003 driver
#define DCCEX_SM_MOTORPIN3  10  // IN3 on the ULN2003 driver
#define DCCEX_SM_MOTORPIN4  11  // IN4 on the ULN2003 driver

#define DCCEX_SM_DIRPIN     8
#define DCCEX_SM_STEPPIN    9

#define DCCEX_SM_STEPPER    2

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXTT_Stepper::EXTT_Stepper()
{
  m_maxSpeed = DCCEX_SM_MAXSPEED;
  m_maxAccel = DCCEX_SM_MAXACCEL;

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
//*   lastPosition - Last known position of the stepper.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
bool EXTT_Stepper::begin(long lastPosition)
{
  if (trace)
    Serial.println("EXTT_Stepper::begin()");

  m_stepDir = 0;
  m_stepCnt = 0;

  m_objStepper = 
    #if DCCEX_SM_DRIVER == ULN2003_HALF
      new AccelStepper(AccelStepper::HALF4WIRE, DCCEX_SM_MOTORPIN1, DCCEX_SM_MOTORPIN3, DCCEX_SM_MOTORPIN2, DCCEX_SM_MOTORPIN4);
    #elif DCCEX_SM_DRIVER == ULN2003_FULL
      new AccelStepper(AccelStepper::FULL4WIRE, DCCEX_SM_MOTORPIN1, DCCEX_SM_MOTORPIN3, DCCEX_SM_MOTORPIN2, DCCEX_SM_MOTORPIN4);
    #elif DCCEX_SM_DRIVER == A4988
      new AccelStepper(AccelStepper::DRIVER, DCCEX_SM_STEPPIN, DCCEX_SM_DIRPIN);
    #else
      new AccelStepper(EXTT_Stepper::stepForward, EXTT_Stepper::stepBackward);
    #endif

  ;
  ;
  ;

  // Set the maximum steps per second and acceleration in steps per second
  m_objStepper->setMaxSpeed(m_maxSpeed);
  m_objStepper->setAcceleration(m_maxAccel);

  m_objStepper->setCurrentPosition(lastPosition);

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
void EXTT_Stepper::run(void)
{
  m_objStepper->run();
  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Set the position of the turntable.
//*
//* Parameters:
//*   position - Absolute position on the turntable.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_Stepper::moveTo(uint32_t position)
{
  if (trace)
    Serial.println("EXTT_Stepper::moveTo(" + String(position) + ")");

  m_objStepper->moveTo(position);
  return;
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
void EXTT_Stepper::stop(void)
{
  if (trace)
    Serial.println("EXTT_Stepper::stop()");

  m_objStepper->setMaxSpeed(0);
  m_objStepper->setAcceleration(0);
  m_objStepper->stop();

  m_objStepper->runToPosition();
  m_objStepper->setMaxSpeed(m_maxSpeed);
  m_objStepper->setAcceleration(m_maxAccel);

  return;
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
bool EXTT_Stepper::isBusy(void)
{
  if (trace)
    Serial.println("EXTT_Stepper::isBusy()");

  return (m_objStepper->isRunning());
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
void EXTT_Stepper::stepForward(void)
{
  if (trace)
    Serial.println("EXTT_Stepper::stepForward()");

  ;
  ;
  ;

  return;
}
///////////////////////////////////////////////////////////////////////////////
void EXTT_Stepper::stepBackward(void)
{
  if (trace)
    Serial.println("EXTT_Stepper::stepBackward()");

  ;
  ;
  ;

  return;
}



















//*****************************************************************************
//*****************************************************************************
uint16_t EXTT_Stepper::step(void)
{
  //Serial.println("EXTT_Stepper::step()");

//  m_stepCnt -= STEP_ANIMATION;

  return (m_stepCnt);
}
///////////////////////////////////////////////////////////////////////////////
void EXTT_Stepper::step(uint16_t steps, uint8_t dir)
{
  m_objStepper->move(steps);

  //m_objMotor->step(steps, dir);
  return;
}
