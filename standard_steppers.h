/*
 *  © 2022 Peter Cole
 *
 *  These are the standard stepper controller and motor definitions.
*/

#ifndef STANDARD_STEPPERS_h
#define STANDARD_STEPPERS_h

#if STEPPER_CONTROLLER == ULN2003

#define ULN2003_PIN1 8
#define ULN2003_PIN2 9
#define ULN2003_PIN3 10
#define ULN2003_PIN4 11

#elif STEPPER_CONTROLLER == A4988

#define A4988_STEP_PIN 8
#define A4988_DIR_PIN 9

#elif STEPPER_CONTROLLER == DRV8825

#define DRV8825_STEP_PIN 8
#define DRV8825_DIR_PIN 9

#elif STEPPER_CONTROLLER == TMC2208



#endif

#if STEPPER_MOTOR == ULN2003COMBO

#define FULLSTEPS 2048

#endif

#endif
