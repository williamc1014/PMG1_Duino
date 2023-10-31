/******************************************************************************
* File Name:   debug.c
*
* Description: This file implements uart based debugging functionality.
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/

#include "cybsp.h"
#include "debug.h"
#include "config.h"

#if DEBUG_LOG


void debug_init(cy_stc_scb_uart_context_t *uartContext)
{
    /* Configure and enable the UART peripheral. */
    //Cy_GPIO_SetHSIOM(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_NUM, CYBSP_DEBUG_UART_RX_HSIOM);
    //Cy_GPIO_SetHSIOM(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_NUM, CYBSP_DEBUG_UART_TX_HSIOM);

    //Cy_GPIO_SetDrivemode(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_NUM, CY_GPIO_DM_HIGHZ);
    //Cy_GPIO_SetDrivemode(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_NUM, CY_GPIO_DM_STRONG);

    //Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

    //Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM, 51UL);
    //Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

	Cy_SCB_UART_Init(CYBSP_DBG_UART_HW, &CYBSP_DBG_UART_config, uartContext);
	Cy_SCB_UART_Enable(CYBSP_DBG_UART_HW);
}

void debug_print(const char* string)
{
    /* Send a string over serial terminal */
    Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
}

void debug_print_byte(uint8_t byte_var)
{
    const char hex[] = "0123456789ABCDEF" ;

    char string[3] = {0};

    string[0] = hex[(byte_var & 0xF0)>>4];
    string[1] = hex[byte_var & 0x0F];

    Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
}

void debug_print_hword(uint16_t hword)
{
    const char hex[] = "0123456789ABCDEF" ;

    char string[5] = {0};

    string[0] = hex[(hword >> 12) & 0x0F];
    string[1] = hex[(hword >> 8) & 0x0F];
    string[2] = hex[(hword >> 4) & 0x0F];
    string[3] = hex[hword & 0x0F];

    Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
}

void debug_print_word(uint32_t word)
{
    const char hex[] = "0123456789ABCDEF" ;

    char string[9]= {0};

    string[0] = hex[(word >> 28) & 0x0F];
    string[1] = hex[(word >> 24) & 0x0F];
    string[2] = hex[(word >> 20) & 0x0F];
    string[3] = hex[(word >> 16) & 0x0F];
    string[4] = hex[(word >> 12) & 0x0F];
    string[5] = hex[(word >> 8) & 0x0F];
    string[6] = hex[(word >> 4) & 0x0F];
    string[7] = hex[word & 0x0F];

    Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
}


#endif
