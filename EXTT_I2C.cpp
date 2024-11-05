//*****************************************************************************
//*
//* EXTT_I2C.cpp
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

#include "EX-Turntable.h"
#include "EXTT_I2C.h"

#include "defines.h"

// I2C Communication
//////////////////////////////////////////////////////////////////////////////
uint8_t
  _addrTT;

TwoWire
  *_wireTT;

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXTT_I2C::EXTT_I2C()
{
  ;
  ;
  ;
}

//*****************************************************************************
//*
//* Brief:
//*   Start I2C communication.
//*
//* Parameters:
//*   addr - I2C address (optional).
//*   wire - TwoWire bus reference to use (optional).
//*
//* Returns:
//*   true if successful, otherwise false.
//*
//*****************************************************************************
bool EXTT_I2C::begin(const uint8_t addr, TwoWire *wire)
{
  if (trace)
    Serial.println("EXTT_I2C::begin(0x" + String(addr, HEX) + ")");

  _addrTT = addr;
  _wireTT = wire;

  _wireTT->begin(_addrTT);
  _wireTT->setClock(100000);

  _wireTT->onReceive(EXTT_I2C::receiveEvent);
  _wireTT->onRequest(EXTT_I2C::requestEvent);

  return (true);
}

//*****************************************************************************
//*
//* Brief:
//*   Function to define the action on a received I2C event.
//*
//* Parameters:
//*   received - ???.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_I2C::receiveEvent(int received)
{
  if (trace)
    Serial.println("EXTT_I2C::receiveEvent(" + String(received, HEX) + ")");

  int16_t
    receivedSteps,
    receivedStepsMSB,
    receivedStepsLSB,
    activity;

  long
    steps;

  ;
  ;
  ;

  EXTT_Turntable::run();

  if (debug)
    {
    Serial.print(F("DEBUG: Received "));
    Serial.print(received);
    Serial.println(F(" bytes"));
    }

  // We need 3 received bytes in order to care about what's received.
  if (received == 3)
    {
    // Get our 3 bytes of data, bit shift into steps.
    receivedStepsMSB = _wireTT->read();
    receivedStepsLSB = _wireTT->read();
    activity         = _wireTT->read();

    receivedSteps = (receivedStepsMSB << 8) + receivedStepsLSB;

    // if (gearingFactor > 10)
    //   gearingFactor = 10;

    // steps = receivedSteps * gearingFactor;

    // if (debug)
    //   {
    //   Serial.print(F("DEBUG: receivedStepsMSB|receivedStepsLSB|activity: "));
    //   Serial.print(receivedStepsMSB);
    //   Serial.print(F("|"));
    //   Serial.print(receivedStepsLSB);
    //   Serial.print(F("|"));
    //   Serial.println(activity);
    //   Serial.print(F("DEBUG: gearingFactor|receivedSteps|steps: "));
    //   Serial.print(gearingFactor);
    //   Serial.print(F("|"));
    //   Serial.print(receivedSteps);
    //   Serial.print(F("|"));
    //   Serial.println(steps);
    //   }

    //=============================================================================        
    // ...
    //-----------------------------------------------------------------------------
    switch (activity)
      {
      // ...
      //=============================================================================        
      case 0: //Turn
      case 1: //Turn_PInvert
        {
        EXTT_Turntable::onReceive(DCCEX_TT_MOVE, receivedSteps);

        // if (steps <= fullTurnSteps && !stepper.isRunning() && !calibrating)
        //   {
        //   // Activities 0/1 require turning and setting phase, process only if stepper is not running.
        //   if (debug)
        //     {
        //     Serial.print(F("DEBUG: Requested valid step move to: "));
        //     Serial.print(steps);
        //     Serial.print(F(" with phase switch: "));
        //     Serial.println(activity);
        //     }

        //   moveToPosition(steps, activity);
        //   }
        ;
        ;
        ;

        break;
        }

      // ...
      //=============================================================================        
      case 2: //Home
        {
        EXTT_Turntable::onReceive(DCCEX_TT_HOME);
        break;
        }

      // ...
      //=============================================================================        
      case 3: //Calibrate
        {
        EXTT_Turntable::onReceive(DCCEX_TT_CALIBRATE);
        break;
        }

      // ...
      //=============================================================================        
      case 4:
      case 5:
      case 6:
      case 7:
        {
        // Activities 4 through 7 set LED state.
        if (debug)
          {
          Serial.print("DEBUG: Set LED state to: ");
          Serial.println(activity);
          }

        // setLEDActivity(activity);
        ;
        ;
        ;

        break;
        }

      // ...
      //=============================================================================        
      case 8: //Acc_on
      case 9: //Acc_off
        {
        // Activity 8 turns accessory pin on at any time.
        // if (debug)
        //   {
        //   Serial.print("DEBUG: Turn accessory pin ");
        //   Serial.println(activity == 8 ? "on" ? "off");
        //   }

        // setAccessory(activity == 8 ? HIGH : LOW);
        ;
        ;
        ;

        break;
        }

      // ...
      //=============================================================================        
      default:
        {
        // if (debug)
        //   {
        //   Serial.print("DEBUG: Invalid step count or activity provided, or turntable still moving: ");
        //   Serial.print(steps);
        //   Serial.print(" steps, activity: ");
        //   Serial.println(activity);
        //   }
        ;
        ;
        ;
        }
      }

    }
  else
    {
    // Even if we have nothing to do, we need to read and discard all the bytes to avoid timeouts in the CS.
    if (debug)
      Serial.println("DEBUG: Incorrect number of bytes received, discarding");

    while (_wireTT->available())
      _wireTT->read();
    }

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Function to return the stepper status when requested by the IO_TurntableEX.h device driver.
//*     0 = Finished moving to the correct position.
//*     1 = Still moving.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
void EXTT_I2C::requestEvent()
{
  uint8_t
    stepperStatus;

  // if (stepper.isRunning())
  //   stepperStatus = 1;
  // else
  //   stepperStatus = 0;

  _wireTT->write(stepperStatus);
  return;
}
