/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Copyright (c) 2018 Infineon Technologies AG
  This file has been modified for the XMC microcontroller series.
*/
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
// @Std Includes
//****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <types.h>
#include <math.h>
#include <pgmspace.h>
#include "cy_pdl.h"
#include "debug.h"

typedef struct
{
  GPIO_PRT_Type *port;
  uint32_t  pin;
} ifx_gpio_port_pin_t;

typedef struct
{
  uint8_t pin;
  uint8_t pwm_num;

} ifx_pwm_port_pin_t;

typedef struct
{
  uint8_t vplus_port;
  uint8_t vplus_pin;
} ifx_adc_vplus_t;

#if PMGDUINO_BOARD

#define D0    0
#define D1    1
#define D2    2
#define D3    3
#define D4    4
#define D5    5
#define D6    6
#define D7    7
#define D8    8
#define D9    9
#define D10   10
#define D11   11
#define D12   12
#define D13   13
#define D14   14
#define D15   15
#define D16   16
#define D17   17
#define D18   18
#define D19   19
#define D20   20
#define D21   21

#define A0    15
#define A1    16
#define A2    17
#define A3    18
#define A4    19
#define A5    20
#define A6    21

#ifndef IFX_DUINO_MAX_NUM_GPIO  
#define IFX_DUINO_MAX_NUM_GPIO 22
#endif

#ifndef IFX_DUINO_MAX_NUM_PWM
#define IFX_DUINO_MAX_NUM_PWM 7
#endif

#ifndef IFX_DUINO_MAX_NUM_ADC
#define IFX_DUINO_MAX_NUM_ADC 7
#endif

#ifndef LED_BUILTIN_PORT
#define LED_BUILTIN_PORT  GPIO_PRT7
#endif

#ifndef LED_BUILTIN_PIN_NUM
#define LED_BUILTIN_PIN_NUM   6
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 22
#endif

#ifndef LED1_PORT
#define LED1_PORT  GPIO_PRT7
#endif

#ifndef LED1_PIN_NUM
#define LED1_PIN_NUM   4
#endif

#ifndef LED1
#define LED1 23
#endif


#ifndef HSIOM_SEL_PWM
#define HSIOM_SEL_PWM 8
#endif

#ifndef PWM_MAX_FREQ
#define PWM_MAX_FREQ  (8000000u)
#endif

extern void (*pin_intr_handler[IFX_DUINO_MAX_NUM_GPIO])(void);

const ifx_gpio_port_pin_t gpio_pin_mapping[IFX_DUINO_MAX_NUM_GPIO] =
{
    {GPIO_PRT1, 3},     // D0
    {GPIO_PRT1, 4},     // D1
    {GPIO_PRT2, 1},     // D2
    {GPIO_PRT3, 3},     // D3
    {GPIO_PRT6, 2},     // D4
    {GPIO_PRT0, 4},     // D5
    {GPIO_PRT0, 0},     // D6
    {GPIO_PRT5, 5},     // D7
    {GPIO_PRT5, 1},     // D8
    {GPIO_PRT5, 0},     // D9
    {GPIO_PRT5, 2},     // D10
    {GPIO_PRT1, 5},     // D11
    {GPIO_PRT1, 6},     // D12
    {GPIO_PRT3, 5},     // D13
    {GPIO_PRT3, 6},     // D14
    {GPIO_PRT2, 4},     // D15, A0
    {GPIO_PRT2, 7},     // D16, A1
    {GPIO_PRT2, 2},     // D17, A2
    {GPIO_PRT2, 5},     // D18, A3
    {GPIO_PRT2, 3},     // D19, A4
    {GPIO_PRT2, 0},     // D20, A5
    {GPIO_PRT2, 6}      // D21, A6
};

const ifx_pwm_port_pin_t pwm_pin_mapping[IFX_DUINO_MAX_NUM_PWM] = 
{
  { 0, 4},   // D0, PWM4
  { 1, 5},   // D1, PWM5
  { 2, 2},   // D2, PWM2
  { 3, 7},   // D3, PWM7
  { 4, 0},   // D4, PWM0
  { 5, 1},   // D5, PWM1
  {15, 3}    // D15, PWM3
};

#ifndef pwm_INPUT_DISABLED
#define pwm_INPUT_DISABLED (0x07u)
#endif

const cy_stc_tcpwm_pwm_config_t pwm_config_default = 
{
  .pwmMode = CY_TCPWM_PWM_MODE_PWM,
  .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
  .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
  .deadTimeClocks = 0,
  .runMode = CY_TCPWM_PWM_CONTINUOUS,
  .period0 = 32768,
  .period1 = 32768,
  .enablePeriodSwap = false,
  .compare0 = 16384,
  .compare1 = 16384,
  .enableCompareSwap = false,
  .interruptSources = CY_TCPWM_INT_NONE,
  .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
  .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
  .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
  .swapInputMode = pwm_INPUT_DISABLED & 0x3U,
  .swapInput = CY_TCPWM_INPUT_0,
  .reloadInputMode = pwm_INPUT_DISABLED & 0x3U,
  .reloadInput = CY_TCPWM_INPUT_0,
  .startInputMode = pwm_INPUT_DISABLED & 0x3U,
  .startInput = CY_TCPWM_INPUT_0,
  .killInputMode = pwm_INPUT_DISABLED & 0x3U,
  .killInput = CY_TCPWM_INPUT_0,
  .countInputMode = pwm_INPUT_DISABLED & 0x3U,
  .countInput = CY_TCPWM_INPUT_1,
};

const uint32_t adc_routing_mask[7] = {1, 4, 8, 16, 32, 64, 128};

const ifx_adc_vplus_t adc_VPLUS_mapping[IFX_DUINO_MAX_NUM_ADC] = 
{
  {SAR0_VPLUS0_PORT, SAR0_VPLUS0_PIN},
  {SAR0_VPLUS1_PORT, SAR0_VPLUS1_PIN},
  {SAR0_VPLUS2_PORT, SAR0_VPLUS2_PIN},
  {SAR0_VPLUS3_PORT, SAR0_VPLUS3_PIN},
  {SAR0_VPLUS4_PORT, SAR0_VPLUS4_PIN},
  {SAR0_VPLUS5_PORT, SAR0_VPLUS5_PIN},
  {SAR0_VPLUS6_PORT, SAR0_VPLUS6_PIN}
};

#endif

#if CY7113_BOARD

#ifndef APP_SWITCH_PORT
#define APP_SWITCH_PORT GPIO_PRT3
#endif

#ifndef APP_SWITCH_PIN
#define APP_SWITCH_PIN  3
#endif

#ifndef APP_SWITCH_NUM
#define APP_SWITCH_NUM  3
#endif

#define D0    0
#define D1    1
#define D2    2
#define D3    3
#define D4    4
#define D5    5
#define D6    6
#define D7    7
#define D8    8
#define D9    9
#define D10   10
#define D11   11
#define D12   12
#define D13   13
#define D14   14
#define D15   15
#define D16   16
#define D17   17
#define D18   18
#define D19   19
#define D20   20
#define D21   21
#define D22   22

#define A0    5
#define A1    6

#ifndef IFX_DUINO_MAX_NUM_GPIO  
#define IFX_DUINO_MAX_NUM_GPIO 23
#endif

#ifndef IFX_DUINO_MAX_NUM_PWM
#define IFX_DUINO_MAX_NUM_PWM 6
#endif

#ifndef IFX_DUINO_MAX_NUM_ADC
#define IFX_DUINO_MAX_NUM_ADC 2
#endif

#ifndef LED_BUILTIN_PORT
#define LED_BUILTIN_PORT  GPIO_PRT3
#endif

#ifndef LED_BUILTIN_PIN_NUM
#define LED_BUILTIN_PIN_NUM   3
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 23
#endif

#ifndef HSIOM_SEL_PWM
#define HSIOM_SEL_PWM 8
#endif

#ifndef PWM_MAX_FREQ
#define PWM_MAX_FREQ  (8000000u)
#endif

extern void (*pin_intr_handler[IFX_DUINO_MAX_NUM_GPIO])(void);

const ifx_gpio_port_pin_t gpio_pin_mapping[IFX_DUINO_MAX_NUM_GPIO] =
{
    {GPIO_PRT3, 5},     // D0
    {GPIO_PRT3, 6},     // D1
    {GPIO_PRT3, 0},     // D2
    {GPIO_PRT2, 3},     // D3
    {GPIO_PRT2, 4},     // D4
    {GPIO_PRT2, 2},     // D5, A0
    {GPIO_PRT2, 1},     // D6, A1
    {GPIO_PRT5, 2},     // D7
    {GPIO_PRT5, 3},     // D8
    {GPIO_PRT5, 0},     // D9
    {GPIO_PRT1, 1},     // D10
    {GPIO_PRT1, 2},     // D11
    {GPIO_PRT6, 0},     // D12
    {GPIO_PRT6, 1},     // D13
    {GPIO_PRT0, 0},     // D14
    {GPIO_PRT6, 2},     // D15
    {GPIO_PRT6, 3},     // D16
    {GPIO_PRT1, 3},     // D17
    {GPIO_PRT1, 4},     // D18
    {GPIO_PRT4, 0},     // D19
    {GPIO_PRT4, 1},     // D20
    {GPIO_PRT1, 6},     // D21
    {GPIO_PRT1, 5}      // D22
};

const ifx_pwm_port_pin_t pwm_pin_mapping[IFX_DUINO_MAX_NUM_PWM] = 
{
  { 2, 6},   // D2 , PWM6
  { 4, 3},   // D4 , PWM3
  { 6, 2},   // D6 , PWM2
  {15, 0},   // D15, PWM0
  {17, 4},   // D17, PWM4
  {18, 5}    // D18, PWM5

};

#ifndef pwm_INPUT_DISABLED
#define pwm_INPUT_DISABLED (0x07u)
#endif

const cy_stc_tcpwm_pwm_config_t pwm_config_default = 
{
  .pwmMode = CY_TCPWM_PWM_MODE_PWM,
  .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
  .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
  .deadTimeClocks = 0,
  .runMode = CY_TCPWM_PWM_CONTINUOUS,
  .period0 = 32768,
  .period1 = 32768,
  .enablePeriodSwap = false,
  .compare0 = 16384,
  .compare1 = 16384,
  .enableCompareSwap = false,
  .interruptSources = CY_TCPWM_INT_NONE,
  .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
  .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
  .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
  .swapInputMode = pwm_INPUT_DISABLED & 0x3U,
  .swapInput = CY_TCPWM_INPUT_0,
  .reloadInputMode = pwm_INPUT_DISABLED & 0x3U,
  .reloadInput = CY_TCPWM_INPUT_0,
  .startInputMode = pwm_INPUT_DISABLED & 0x3U,
  .startInput = CY_TCPWM_INPUT_0,
  .killInputMode = pwm_INPUT_DISABLED & 0x3U,
  .killInput = CY_TCPWM_INPUT_0,
  .countInputMode = pwm_INPUT_DISABLED & 0x3U,
  .countInput = CY_TCPWM_INPUT_1,
};

const uint32_t adc_routing_mask[7] = {1, 4, 8, 16, 32, 64, 128};

const ifx_adc_vplus_t adc_VPLUS_mapping[IFX_DUINO_MAX_NUM_ADC] = 
{
  {SAR0_VPLUS0_PORT, SAR0_VPLUS0_PIN},
  {SAR0_VPLUS1_PORT, SAR0_VPLUS1_PIN}
};

#endif

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

	/*
     * \brief Arduino yield function.
     */
	void yield( void );

    /*
     * \brief Arduino Main setup function. Called only once at the beginning.
     */
    extern void setup(void);

    /*
     * \brief Arduino Main loop function. Called in an endless loop.
     */
    extern void loop(void);


//****************************************************************************
// @Arduino Core Includes
//****************************************************************************
#include "duino_basic.h"
//#include "wiring_constants.h"
//#include "binary.h"
//#include "wiring_digital.h"
//#include "wiring_analog.h"
//#include "wiring_shift.h"
//#include "wiring_time.h"
//#include "wiring_pulse.h"
//#include "itoa.h"
//#include "dtostrf.h"
//#include "WCharacter.h"
//#include "WInterrupts.h"

//****************************************************************************
// @Infineon Core Includes
//****************************************************************************
//#include "reset.h"

#ifdef __cplusplus
} // extern "C"
//#include "Tone.h"
//#include "WMath.h"
#endif	// __cplusplus

//#include "Print.h"
//#include "HardwareSerial.h"

//****************************************************************************
// @Board Variant Includes
//****************************************************************************
//#include <pins_arduino.h>

#endif  /*_ARDUINO_H_ */