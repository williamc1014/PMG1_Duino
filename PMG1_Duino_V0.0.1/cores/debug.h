/******************************************************************************
* File Name: debug.h
*
* Description: This file defines UART based debug APIs.
*
* Related Document: See README.md
*
*******************************************************************************
* $ Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#ifndef __debug_h__
#define __debug_h__

#include "config.h"

#if DEBUG_LOG
void debug_init(cy_stc_scb_uart_context_t *uartContext);
void debug_print(const char* string);
void debug_print_byte(uint8_t byte);
void debug_print_hword(uint16_t hword);
void debug_print_word(uint32_t word);
#endif

#endif /*__debug_h__ */
