/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* Configurator Backend 3.0.0
* device-db 4.7.0.4251
* mtb-pdl-cat2 2.5.0.9691
*
********************************************************************************
* Copyright 2023 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)

#if defined(__cplusplus)
extern "C" {
#endif

#define APP_SWITCH_ENABLED 1U
#define APP_SWITCH_PORT GPIO_PRT1
#define APP_SWITCH_PORT_NUM 1U
#define APP_SWITCH_PIN 0U
#define APP_SWITCH_NUM 0U
#define APP_SWITCH_DRIVEMODE CY_GPIO_DM_PULLUP
#define APP_SWITCH_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
    #define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define APP_SWITCH_HSIOM ioss_0_port_1_pin_0_HSIOM
#define APP_SWITCH_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define APP_SWITCH_HAL_PORT_PIN P1_0
    #define APP_SWITCH P1_0
    #define APP_SWITCH_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define APP_SWITCH_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define APP_SWITCH_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif //defined (CY_USING_HAL)
#define CYBSP_SWCLK_ENABLED 1U
#define CYBSP_SWCLK_PORT GPIO_PRT1
#define CYBSP_SWCLK_PORT_NUM 1U
#define CYBSP_SWCLK_PIN 1U
#define CYBSP_SWCLK_NUM 1U
#define CYBSP_SWCLK_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWCLK_HSIOM ioss_0_port_1_pin_1_HSIOM
#define CYBSP_SWCLK_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_SWCLK_HAL_PORT_PIN P1_1
    #define CYBSP_SWCLK P1_1
    #define CYBSP_SWCLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_SWCLK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_SWCLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT1
#define CYBSP_SWDIO_PORT_NUM 1U
#define CYBSP_SWDIO_PIN 2U
#define CYBSP_SWDIO_NUM 2U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_1_pin_2_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_SWDIO_HAL_PORT_PIN P1_2
    #define CYBSP_SWDIO P1_2
    #define CYBSP_SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

#define CYBSP_BUCK_ENABLE_ENABLED 1U
#define CYBSP_BUCK_ENABLE_PORT GPIO_PRT4
#define CYBSP_BUCK_ENABLE_PORT_NUM 4U
#define CYBSP_BUCK_ENABLE_PIN 0U
#define CYBSP_BUCK_ENABLE_NUM 0U
#define CYBSP_BUCK_ENABLE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BUCK_ENABLE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_0_HSIOM
    #define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BUCK_ENABLE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define CYBSP_BUCK_ENABLE_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_BUCK_ENABLE_HAL_PORT_PIN P4_0
    #define CYBSP_BUCK_ENABLE P4_0
    #define CYBSP_BUCK_ENABLE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_BUCK_ENABLE_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
    #define CYBSP_BUCK_ENABLE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_NMOS_ENABLE_ENABLED 1U
#define CYBSP_NMOS_ENABLE_PORT GPIO_PRT4
#define CYBSP_NMOS_ENABLE_PORT_NUM 4U
#define CYBSP_NMOS_ENABLE_PIN 1U
#define CYBSP_NMOS_ENABLE_NUM 1U
#define CYBSP_NMOS_ENABLE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_NMOS_ENABLE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_1_HSIOM
    #define ioss_0_port_4_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_NMOS_ENABLE_HSIOM ioss_0_port_4_pin_1_HSIOM
#define CYBSP_NMOS_ENABLE_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_NMOS_ENABLE_HAL_PORT_PIN P4_1
    #define CYBSP_NMOS_ENABLE P4_1
    #define CYBSP_NMOS_ENABLE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_NMOS_ENABLE_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
    #define CYBSP_NMOS_ENABLE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define PFET_SNK_CTRL_P0_ENABLED 1U
#define PFET_SNK_CTRL_P0_PORT GPIO_PRT5
#define PFET_SNK_CTRL_P0_PORT_NUM 5U
#define PFET_SNK_CTRL_P0_PIN 4U
#define PFET_SNK_CTRL_P0_NUM 4U
#define PFET_SNK_CTRL_P0_DRIVEMODE CY_GPIO_DM_STRONG
#define PFET_SNK_CTRL_P0_INIT_DRIVESTATE 0
#ifndef ioss_0_port_5_pin_4_HSIOM
    #define ioss_0_port_5_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define PFET_SNK_CTRL_P0_HSIOM ioss_0_port_5_pin_4_HSIOM
#define PFET_SNK_CTRL_P0_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define PFET_SNK_CTRL_P0_HAL_PORT_PIN P5_4
    #define PFET_SNK_CTRL_P0 P5_4
    #define PFET_SNK_CTRL_P0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define PFET_SNK_CTRL_P0_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define PFET_SNK_CTRL_P0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

#define PFET_SNK_CTRL_P1_ENABLED 1U
#define PFET_SNK_CTRL_P1_PORT GPIO_PRT7
#define PFET_SNK_CTRL_P1_PORT_NUM 7U
#define PFET_SNK_CTRL_P1_PIN 2U
#define PFET_SNK_CTRL_P1_NUM 2U
#define PFET_SNK_CTRL_P1_DRIVEMODE CY_GPIO_DM_STRONG
#define PFET_SNK_CTRL_P1_INIT_DRIVESTATE 0
#ifndef ioss_0_port_7_pin_2_HSIOM
    #define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define PFET_SNK_CTRL_P1_HSIOM ioss_0_port_7_pin_2_HSIOM
#define PFET_SNK_CTRL_P1_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define PFET_SNK_CTRL_P1_HAL_PORT_PIN P7_2
    #define PFET_SNK_CTRL_P1 P7_2
    #define PFET_SNK_CTRL_P1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define PFET_SNK_CTRL_P1_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define PFET_SNK_CTRL_P1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define TEST_LED_1_ENABLED 1U
#define TEST_LED_1_PORT GPIO_PRT7
#define TEST_LED_1_PORT_NUM 7U
#define TEST_LED_1_PIN 4U
#define TEST_LED_1_NUM 4U
#define TEST_LED_1_DRIVEMODE CY_GPIO_DM_STRONG
#define TEST_LED_1_INIT_DRIVESTATE 0
#ifndef ioss_0_port_7_pin_4_HSIOM
    #define ioss_0_port_7_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define TEST_LED_1_HSIOM ioss_0_port_7_pin_4_HSIOM
#define TEST_LED_1_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define TEST_LED_1_HAL_PORT_PIN P7_4
    #define TEST_LED_1 P7_4
    #define TEST_LED_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define TEST_LED_1_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define TEST_LED_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_USER_LED_ENABLED 1U
#define CYBSP_USER_LED_PORT GPIO_PRT7
#define CYBSP_USER_LED_PORT_NUM 7U
#define CYBSP_USER_LED_PIN 5U
#define CYBSP_USER_LED_NUM 5U
#define CYBSP_USER_LED_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_USER_LED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_5_HSIOM
    #define ioss_0_port_7_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USER_LED_HSIOM ioss_0_port_7_pin_5_HSIOM
#define CYBSP_USER_LED_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define CYBSP_USER_LED_HAL_PORT_PIN P7_5
    #define CYBSP_USER_LED P7_5
    #define CYBSP_USER_LED_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define CYBSP_USER_LED_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define CYBSP_USER_LED_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define TEST_LED_ENABLED 1U
#define TEST_LED_PORT GPIO_PRT7
#define TEST_LED_PORT_NUM 7U
#define TEST_LED_PIN 6U
#define TEST_LED_NUM 6U
#define TEST_LED_DRIVEMODE CY_GPIO_DM_STRONG
#define TEST_LED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_6_HSIOM
    #define ioss_0_port_7_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define TEST_LED_HSIOM ioss_0_port_7_pin_6_HSIOM
#define TEST_LED_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define TEST_LED_HAL_PORT_PIN P7_6
    #define TEST_LED P7_6
    #define TEST_LED_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define TEST_LED_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
    #define TEST_LED_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

#define ioss_0_port_2_pin_4_ENABLED 1U
#define ioss_0_port_2_pin_4_PORT GPIO_PRT2
#define ioss_0_port_2_pin_4_PORT_NUM 2U
#define ioss_0_port_2_pin_4_PIN 4U
#define ioss_0_port_2_pin_4_NUM 4U
#define ioss_0_port_2_pin_4_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_2_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
    #define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_2_pin_4_IRQ ioss_interrupt_gpio_IRQn
#if defined (CY_USING_HAL)
    #define ioss_0_port_2_pin_4_HAL_PORT_PIN P2_4
    #define ioss_0_port_2_pin_4 P2_4
    #define ioss_0_port_2_pin_4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
    #define ioss_0_port_2_pin_4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
    #define ioss_0_port_2_pin_4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t APP_SWITCH_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t APP_SWITCH_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_SWCLK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t CYBSP_BUCK_ENABLE_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_BUCK_ENABLE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_NMOS_ENABLE_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_NMOS_ENABLE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t PFET_SNK_CTRL_P0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t PFET_SNK_CTRL_P0_obj;
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t PFET_SNK_CTRL_P1_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t PFET_SNK_CTRL_P1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t TEST_LED_1_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t TEST_LED_1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USER_LED_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t CYBSP_USER_LED_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t TEST_LED_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t TEST_LED_obj;
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t ioss_0_port_2_pin_0_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t ioss_0_port_2_pin_0_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);
void reserve_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
