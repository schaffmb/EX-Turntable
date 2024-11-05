//*****************************************************************************
//*
//* EX-Turntable.ino
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

#include <Arduino.h>
#include <StateMachine.h>

#include "defines.h"
#include "version.h"

#include "EX-Turntable.h"
#include "EXTT_AutoReverser.h"
#include "EXTT_Cache.h"
#include "EXTT_I2C.h"
#include "EXTT_Serial.h"
#include "EXTT_SoundFX.h"
#include "EXTT_Stepper.h"

// Turntable
//////////////////////////////////////////////////////////////////////////////
#if !defined(ARDUINO_UNOR4_MINIMA) && !defined(ARDUINO_UNOR4_WIFI)
  #warning Arduino UNO R4 recommended for EX-Turntable use.
#endif

// ...
///////////////////////////////////////////////////////////////////////////////
uint8_t
  errNo         = ERRNO_NONE,
  executionMode = EXEMODE_IDLE;

bool
  debug,
  trace;

String
  strX;

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
void setup(void)
{
  debug = true;
  trace = true;

  if (EXTT_Turntable::begin())
    {
    EXTT_Turntable::onReceive(DCCEX_TT_VERSION);
    EXTT_Turntable::onReceive(DCCEX_TT_HOME);
    EXTT_Turntable::onReceive(DCCEX_TT_MOVE, 301);
    }

  //EXTT_Turntable::onReceive(DCCEX_TT_RESET, 2);
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
void loop(void)
{
   //============================================================================
   // ...
   //----------------------------------------------------------------------------
   if (errNo)
      {
      pinMode(LED_BUILTIN, OUTPUT);

      #ifdef DEBUG
         switch (errNo)
            {
            case ERRNO_NOSTEPPER:
               Serial.println("Error: No stepper shield found.");
               break;

            // case ERRNO_NOEEPROM:
            //    Serial.println("Error: No EEPROM found.");
            //    break;

            case ERRNO_NOSOUND:
               Serial.println("Error: No Sound decoder found.");
               break;

            case ERRNO_NOMICROSD:
               Serial.println("Error: No SD Card found.");
               break;

            case ERRNO_VERSION:
               Serial.println("Error: Application version mismatch.");
               break;

            case ERRNO_SIGNATURE:
               Serial.println("Error: Invalid application signature.");
               break;
            }
      #endif

      // Flash the internal LED to indicate error number
      while (true)
         {
         for (int16_t blink = 0; blink != errNo; blink++)
            {
            digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on (HIGH is the voltage level)
            delay(300);                       // Wait
            digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off by making the voltage LOW
            delay(300);                       // Wait for a second
            }

         delay(1200);
         }
      }

  EXTT_Turntable::run();
  return;
}

//=============================================================================
// EXTT_Turntable namespace
//-----------------------------------------------------------------------------
namespace EXTT_Turntable
{
  namespace
  {
    //=============================================================================
    // Members (private)
    //-----------------------------------------------------------------------------
    bool
      m_isBusy = false;

    int
      tripCount = 0;

    EXTT_Cache
      m_objCache;

    EXTT_Serial
      m_objSerial;

    EXTT_I2C
      m_objI2C;

    EXTT_AutoReverser
      m_objAR;

    EXTT_Stepper
      m_objStepper;

    StateMachine
      m_objFSM = StateMachine();

    State
      *m_stateNode[5];

    //=============================================================================
    // Implementation (private)
    //-----------------------------------------------------------------------------
    void(* resetFunc) (void) = 0; //declare reset function at address 0

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
    void nodeIdle(void)
    {
      #ifdef HEARTBEAT
        if (tripCount++ % HEARTBEAT == 0)
          Serial.println("EXTT_Turntable::nodeIdle");
      #endif

      return;
    }
    ///////////////////////////////////////////////////////////////////////////////
    bool transitionIdle(void)
    {
      return (1); //digitalRead(tripPin) == 0 ? true : false);
    }
    ///////////////////////////////////////////////////////////////////////////////
    void nodeReport(void)
    {
      #ifdef HEARTBEAT
        if (tripCount++ % HEARTBEAT == 0)
          Serial.println("EXTT_Turntable::nodeReport");
      #endif

      return;
    }
    ///////////////////////////////////////////////////////////////////////////////
    bool transitionReport(void)
    {
      return (true);
    }
    ///////////////////////////////////////////////////////////////////////////////
    void nodeStartup(void)
    {
      #ifdef HEARTBEAT
        if (tripCount++ % HEARTBEAT == 0)
          Serial.println("EXTT_Turntable::nodeStartup");
      #endif

      return;
    }
    ///////////////////////////////////////////////////////////////////////////////
    bool transitionStartup(void)
    {
      return (true);
    }
    ///////////////////////////////////////////////////////////////////////////////
    void nodeSlew(void)
    {
      #ifdef HEARTBEAT
        if (tripCount++ % HEARTBEAT == 0)
          Serial.println("EXTT_Turntable::nodeSlew");
      #endif

      return;
    }
    ///////////////////////////////////////////////////////////////////////////////
    bool transitionSlew(void)
    {
      return (true);
    }
    ///////////////////////////////////////////////////////////////////////////////
    void nodeShutdown(void)
    {
      #ifdef HEARTBEAT
        if (tripCount++ % HEARTBEAT == 0)
          Serial.println("EXTT_Turntable::nodeShutdown");
      #endif

      return;
    }
    ///////////////////////////////////////////////////////////////////////////////
    bool transitionShutdown(void)
    {
      return (true);
    }

  }

  //=============================================================================
  // Implementation (public)
  //-----------------------------------------------------------------------------

  //*****************************************************************************
  //*
  //* Brief:
  //*   ???.
  //*
  //* Parameters:
  //*   None.
  //*
  //* Returns:
  //*   ???.
  //*
  //*****************************************************************************
  bool begin()
  {
    m_objSerial.begin();
    // // Run startup configuration
    // startupConfiguration();

    // // Set up the stepper driver
    // setupStepperDriver();

    // // If we're not sensor testing, start Wire()
    // if (!sensorTesting)
    //   setupWire();

    if (trace)
      Serial.println("EXTT_Turntable::begin()");

    m_objI2C.begin();
    m_objAR.begin();

    //============================================================================
    // ...
    //----------------------------------------------------------------------------
    if (m_objCache.begin())
      {
      ;
      ;
      ;

      // ...
      //////////////////////////////////////////////////////////////////////////////
      if (m_objStepper.begin(m_objCache.getLastPosition()))
        {
        // Instantiate and define the state machine
        //////////////////////////////////////////////////////////////////////////////
        m_stateNode[0] = m_objFSM.addState(&nodeIdle);
        m_stateNode[1] = m_objFSM.addState(&nodeReport);
        m_stateNode[2] = m_objFSM.addState(&nodeStartup);
        m_stateNode[3] = m_objFSM.addState(&nodeSlew);
        m_stateNode[4] = m_objFSM.addState(&nodeShutdown);

        m_stateNode[0]->addTransition(&transitionIdle, m_stateNode[1]);
        m_stateNode[1]->addTransition(&transitionReport, m_stateNode[2]);
        m_stateNode[2]->addTransition(&transitionStartup, m_stateNode[3]);
        m_stateNode[3]->addTransition(&transitionSlew, m_stateNode[4]);
        m_stateNode[4]->addTransition(&transitionShutdown, m_stateNode[0]);
        }
      else
        errNo = ERRNO_NOSTEPPER;
      }

    return (errNo == ERRNO_NONE || errNo == ERRNO_NOSOUND ? true : false);
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
  void run(void)
  {
    // // If we're only testing sensors, don't do anything else.
    // if (sensorTesting)
    //   {
    //   bool
    //     testHomeSensorState = getHomeState();

    //   if (testHomeSensorState != homeSensorState)
    //     {
    //     if (testHomeSensorState == HOME_SENSOR_ACTIVE_STATE)
    //       Serial.println(F("Home sensor ACTIVATED"));
    //     else
    //       Serial.println(F("Home sensor DEACTIVATED"));

    //     homeSensorState = testHomeSensorState;
    //     }

    //   getHomeState();

    //   #if TURNTABLE_EX_MODE == TRAVERSER
    //     bool
    //       testLimitSensorState = getLimitState();

    //     if (testLimitSensorState != limitSensorState)
    //       {
    //       if (testLimitSensorState == LIMIT_SENSOR_ACTIVE_STATE)
    //         Serial.println(F("Limit sensor ACTIVATED"));
    //       else
    //         Serial.println(F("Limit sensor DEACTIVATED"));
        
    //       limitSensorState = testLimitSensorState;
    //       }
    //   #endif
    //   }
    // else
    //   {
    //   #if TURNTABLE_EX_MODE == TRAVERSER
    //     // If we hit our limit switch when not calibrating, stop!
    //     if (getLimitState() == LIMIT_SENSOR_ACTIVE_STATE && !calibrating && stepper.isRunning() && stepper.targetPosition() < 0)
    //       {
    //       Serial.println(F("ALERT! Limit sensor activitated, halting stepper"));

    //       if (!homed)
    //         homed = 1;

    //       stepper.stop();
    //       stepper.setCurrentPosition(stepper.currentPosition());
    //       }

    //     // If we hit our home switch when not homing, stop!
    //     if (getHomeState() == HOME_SENSOR_ACTIVE_STATE && homed && !calibrating && stepper.isRunning() && stepper.distanceToGo() > 0)
    //       {
    //       Serial.println(F("ALERT! Home sensor activitated, halting stepper"));

    //       stepper.stop();
    //       stepper.setCurrentPosition(0);
    //       }
    //   #endif

    //   // If we haven't successfully homed yet, do it.
    //   if (!homed)
    //     moveHome();

    //   // If flag is set for calibrating, do it.
    //   if (calibrating)
    //     calibration();

    //   // Process the stepper object continuously.
    //   stepper.run();

    //   // Process our LED.
    //   processLED();

    //   // If disabling on idle is enabled, disable the stepper.
    //   #ifdef DISABLE_OUTPUTS_IDLE
    //     if (stepper.isRunning() != lastRunningState)
    //       {
    //       lastRunningState = stepper.isRunning();

    //       if (!lastRunningState)
    //         stepper.disableOutputs();
    //       }
    //   #endif
    //   }

    m_objSerial.run();
    m_objFSM.run();

    //m_objAR.run();
    m_objStepper.run();
    //m_objSoundFX.run();

    return;
  }

  //*****************************************************************************
  //*
  //* Brief:
  //*   ???.
  //*
  //* Parameters:
  //*   msg   - ???.
  //*   param - ???.
  //*
  //* Returns:
  //*   ???.
  //*
  //*****************************************************************************
  bool onReceive(uint8_t msg, uint32_t param)
  {
    if (trace)
      Serial.print("EXTT_Turntable::onReceive(");

    //============================================================================
    // ...
    //----------------------------------------------------------------------------
    switch (msg)
      {
      // Reset the CPU.
      //============================================================================
      case DCCEX_TT_RESET:
        {
        if (trace)
          Serial.println("DCCEX_TT_RESET, " + String(param) + ")");

        switch (param)
          {
          // Soft reset.
          //----------------------------------------------------------------------------
          case 1:
            {
            #if defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
              NVIC_SystemReset();
            #else
              resetFunc();
            #endif

            break;
            }

          // Hard reset.
          //----------------------------------------------------------------------------
          case 2:
            {
            m_objCache.clearEEPROM();

            if (debug)
              m_objCache.forceEEPROM();

            ;
            ;
            ;

            break;
            }
          }

        break;
        }

      // Move Stepper to Home Position.
      //============================================================================
      case DCCEX_TT_HOME:
        {
        if (trace)
          Serial.println("DCCEX_TT_HOME, " + String(param) + ")");

        if (m_objStepper.isBusy())
          {
          if (debug)
            Serial.println("Stepper is busy");
          }
        // else if (!calibrating || homed == 2)
        //   initiateHoming();

        break;
        }

      // Move Stepper to Position.
      //============================================================================
      case DCCEX_TT_MOVE:
        {
        if (trace)
          Serial.println("DCCEX_TT_MOVE, " + String(param) + ")");

        if (m_objStepper.isBusy())
          {
          if (debug)
            Serial.println("Stepper is busy.");
          }
        else
          m_objStepper.moveTo(param);

        break;
        }

      // Move to Track.
      //============================================================================
      case DCCEX_TT_TRACK:
        {
        if (trace)
          Serial.println("DCCEX_TT_TRACK, " + String(param) + ")");

        if (m_objStepper.isBusy())
          {
          if (debug)
            Serial.println("Stepper is busy.");
          }
        else
          {
          int8_t
            indx = m_objCache.getTrackToIndx(param);

          if (indx != FAILURE)
            m_objStepper.moveTo(m_objCache.getIndxToPosition(indx));

          if (debug)
            Serial.println("Invalid track index.");
          }

        break;
        }

      // Stop Stepper
      //============================================================================
      case DCCEX_TT_STOP:
        {
        if (trace)
          Serial.println("DCCEX_TT_STOP, " + String(param) + ")");

        m_objStepper.stop();
        break;
        }

      // Step right (clockwise)
      //============================================================================
      case DCCEX_TT_RIGHT:
        {
        if (trace)
          Serial.println("DCCEX_TT_RIGHT, " + String(param) + ")");

        ;
        ;
        ;

        break;
        }

      // Step left (counter-clockwise)
      //============================================================================
      case DCCEX_TT_LEFT:
        {
        if (trace)
          Serial.println("DCCEX_TT_LEFT, " + String(param) + ")");

        ;
        ;
        ;

        break;
        }

      // Perform Stepper Calibration.
      //============================================================================
      case DCCEX_TT_CALIBRATE:
        {
        if (trace)
          Serial.println("DCCEX_TT_CALIBRATE, " + String(param) + ")");

        if (m_objStepper.isBusy())
          {
          if (debug)
            Serial.println("Stepper is busy, cannot Calibrate.");
          }
        else if (1) //!calibrating || homed == 2)
          // initiateCalibration();
          m_objStepper.step(4096, 0);

        ;
        ;
        ;

        break;
        }

      // Show Version/Configuration Information.
      //============================================================================
      case DCCEX_TT_VERSION:
        {
        if (trace)
          Serial.println("DCCEX_TT_VERSION, " + String(param) + ")");

        Serial.println("\n\n== EX-Turntable ============================================================");
        Serial.println("License GPLv3 fsf.org © dcc-ex.com\n");
        Serial.println("Version: " + String(VERSION));
        Serial.println("I2C address: 0x" + String(DCCEX_I2C_ADDRESS, HEX));

        Serial.print("Mode: ");
            #if TURNTABLE_EX_MODE == TRAVERSER
              Serial.println("TRAVERSER");
            #elif TURNTABLE_EX_MODE == SWINGBRIDGE
              Serial.println("SWINGBRIDGE");
            #else
              Serial.println("TURNTABLE");
            #endif

        Serial.print("Rotation: ");
            #ifdef ROTATE_FORWARD_ONLY
              Serial.println("CW");
            #elif defined(ROTATE_REVERSE_ONLY)
              Serial.println("CWW");
            #else
              Serial.println("SHORTEST");
            #endif

        Serial.println("Invert");
            Serial.print("  Direction: ");
            #ifdef INVERT_DIRECTION
              Serial.println("enabled");
            #else
              Serial.println("disabled");
            #endif
            Serial.print("  Step: ");
            #ifdef INVERT_STEP
              Serial.println("INVERT_STEP enabled");
            #else
              Serial.println("disabled");
            #endif

        // #ifdef INVERT_ENABLE
        //   Serial.println(F("INVERT_ENABLE enabled"));
        // #endif

        // if (fullTurnSteps == 0)
        //   Serial.println(F("EX-Turntable has not been calibrated yet"));
        // else
        //   {
        //   #ifdef FULL_STEP_COUNT
        //     Serial.print(F("Manual override has been set for "));
        //   #else
        //     Serial.print(F("EX-Turntable has been calibrated for "));
        //   #endif

        //   Serial.print(fullTurnSteps);
        //   Serial.println(F(" steps per revolution"));
        //   }

        // Serial.print(F("Gearing factor set to "));
        // Serial.println(gearingFactor);

        Serial.print("Phase Switching\n  Mode: ");
            #if PHASE_SWITCHING == AUTO
              Serial.println("Auto Reverser");
            #elif PHASE_SWITCHING == STEP
              Serial.println("Step (from Home)");
              Serial.println("  Angle: " + String(PHASE_SWITCH_ANGLE) + " degrees");
              Serial.println("  Switch: " + String(phaseSwitchStartSteps));
              Serial.println("  Revert: " + String(phaseSwitchStopSteps));
            #else
              Serial.println("Manual");
            #endif

        Serial.println("Stepper");
            Serial.println("  Model: ");
            Serial.println("  Speed: " + String(STEPPER_MAX_SPEED));
            Serial.println("  Accel: " + String(STEPPER_ACCELERATION));

        // if (debug)
        //   {
        //   Serial.print(F("DEBUG: maxSpeed()|acceleration(): "));
        //   Serial.print(stepper.maxSpeed());
        //   Serial.print(F("|"));
        //   Serial.println(stepper.acceleration());
        //   }

        // // If in sensor testing mode, display this, don't enable stepper or I2C
        // if (sensorTesting)
        //   {
        //   Serial.println(F("SENSOR TESTING ENABLED, EX-Turntable operations disabled"));
        //   Serial.print(F("Home/limit switch current state: "));
        //   Serial.print(homeSensorState);
        //   Serial.print(F("/"));
        //   Serial.println(limitSensorState);
        //   Serial.print(F("Debounce delay: "));
        //   Serial.println(DEBOUNCE_DELAY);
        //   }

        Serial.println("============================================================================\n\n");
        break;
        }

      // Perform Unit Testing.
      //============================================================================
      case DCCEX_TT_UNITTEST:
        {
        if (trace)
          Serial.println("DCCEX_TT_UNITTEST, " + String(param) + ")");

        // if (stepper.isRunning())
        //   Serial.println(F("Stepper is running, ignoring <T>"));
        // else
        //   {
        //   if (sensorTesting)
        //     {
        //     Serial.println(F("Disabling sensor testing mode, reboot required"));
        //     sensorTesting = false;
        //     }
        //   else
        //     {
        //     Serial.println(F("Enabling sensor testing mode, taking EX-Turntable offline"));

        //     Wire.end();
        //     sensorTesting = true;
        //     }
        //   }

        break;
        }

      // Toggle Debug Mode.
      //============================================================================
      case DCCEX_TT_DEBUG:
        {
        if (trace)
          Serial.println("DCCEX_TT_DEBUG, " + String(param) + ")");

        if (debug)
          Serial.println("Disabling debug output");
        else
          Serial.println("Enabling debug output");

        debug = !debug;
        break;
        }

      // Unknown command
      //============================================================================
      default:
        {
        if (trace)
          Serial.println("DCCEX_TT_UNKNOWN, " + String(param) + ")");

        return (false);
        }
      }

  return (true);
  }

  //*****************************************************************************
  //*
  //* Brief:
  //*   ???.
  //*
  //* Parameters:
  //*   msg - ???.
  //*
  //* Returns:
  //*   ???.
  //*
  //*****************************************************************************
  uint32_t onRequest(uint8_t msg)
  {
    ;
    ;
    ;

    return (0);
  }










};
