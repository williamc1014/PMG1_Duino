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

#include "Arduino.h"

#define DEBUG_UART  0
#define DUINO_UART  1

/* Populate configuration structure */
cy_stc_sysint_t uartIntrConfig =
{
    .intrSrc      = scb_4_interrupt_IRQn,
    .intrPriority = 3,
};

#if PMGDUINO_BOARD
static cy_stc_scb_uart_context_t dbgUartContext;
#endif
static cy_stc_scb_uart_context_t duinoUartContext;

ringbuffer_t uartRxRingBuff;
ringbuffer_t uartTxRingBuff;

uint8_t rxData = 0;
uint8_t txData = 0;

void initRingBuffer(ringbuffer_t *buff)
{
    memset((void *)buff->buffer, 0, sizeof(uint8_t) * UART_BUFFER_SIZE);
    buff->head = 0;
    buff->tail = 0;
}

uint8_t peekRingBuffer(ringbuffer_t *buff)
{
    if (buff->head == buff->tail)
        return -1;
    return buff->buffer[buff->tail];
}

void queueRingBuffer(ringbuffer_t *buff, uint8_t data)
{
    int pos = buff->head;
    if (pos >= UART_BUFFER_SIZE)
        pos = 0;
    
    if (pos != buff->tail)
    {
        buff->buffer[buff->head] = data;
        buff->head = pos;
    }
}

uint8_t dequeueRingBuffer(ringbuffer_t *buff)
{
    uint8_t data;

    if (buff->head == buff->tail)
        return -1;
    
    data = buff->buffer[buff->tail];
    buff->tail ++;
    if (buff->tail >= UART_BUFFER_SIZE)
        buff->tail = 0;
    
    return data;
}

bool emptyRingBuffer(ringbuffer_t *buff)
{
    return ((buff->head == buff->tail) ? true : false);
}

HardwareSerial::HardwareSerial()
{
    initRingBuffer(&uartRxRingBuff);
    initRingBuffer(&uartTxRingBuff);
}

void HardwareSerial::begin(uint32_t speed)
{
    begin(speed, SERIAL_8N1);
}

void HardwareSerial::begin(uint32_t speed, uart_cfg_idx_t config)
{
    uart_init(DUINO_UART, (uint8_t)config, speed);
}

void HardwareSerial::end(void)
{
    Cy_SCB_UART_Disable(UART_HW, &duinoUartContext);
    Cy_SCB_UART_DeInit(UART_HW);
}

int HardwareSerial::available(void)
{
    if (uartRxRingBuff.head >= uartRxRingBuff.tail)
        return uartRxRingBuff.head - uartRxRingBuff.tail; 
    
    return UART_BUFFER_SIZE - uartRxRingBuff.tail + uartRxRingBuff.head;
}

int HardwareSerial::availableForWrite(void)
{
    if (uartTxRingBuff.head >= uartTxRingBuff.tail)
        return UART_BUFFER_SIZE - 1 - uartTxRingBuff.head + uartTxRingBuff.tail;
    
    return uartTxRingBuff.tail - uartTxRingBuff.head - 1;
}

int HardwareSerial::peek(void)
{
    return (int)peekRingBuffer(&uartRxRingBuff);
}

int HardwareSerial::read(void)
{
    return (int)dequeueRingBuffer(&uartRxRingBuff);
}

void HardwareSerial::flush(void)
{
    while (uartTxRingBuff.head == uartTxRingBuff.tail);
    while (!(CY_SCB_UART_TRANSMIT_ACTIVE & duinoUartContext.txStatus));
}

size_t HardwareSerial::write(uint8_t val)
{
    if (availableForWrite() != 0)
        queueRingBuffer(&uartTxRingBuff, val);
    else
        return (size_t) 0;

    if (0UL == (CY_SCB_UART_TRANSMIT_ACTIVE & duinoUartContext.txStatus))
    {
        txData = dequeueRingBuffer(&uartTxRingBuff);
        Cy_SCB_UART_Transmit(UART_HW, &txData, 1, &duinoUartContext);
    }

    return (size_t) 1;
}

size_t HardwareSerial::write(char *str)
{
    uint8_t len = (uint8_t)strlen(str);
    uint8_t cnt = 0;

    for (cnt=0; cnt<len; cnt++)
    {
        if(availableForWrite() != 0)
            queueRingBuffer(&uartTxRingBuff, str[cnt]);
        else
            break;
    }

    if (0UL == (CY_SCB_UART_TRANSMIT_ACTIVE & duinoUartContext.txStatus))
    {
        txData = dequeueRingBuffer(&uartTxRingBuff);
        Cy_SCB_UART_Transmit(UART_HW, &txData, 1, &duinoUartContext);
    }

    return (size_t)cnt;
}

size_t HardwareSerial::write(uint8_t *buf, uint8_t len)
{
    uint8_t cnt = 0;

    for (cnt=0; cnt<len; cnt++)
    {
        if(availableForWrite() != 0)
            queueRingBuffer(&uartTxRingBuff, buf[cnt]);
        else
            break;
    }

    if (0UL == (CY_SCB_UART_TRANSMIT_ACTIVE & duinoUartContext.txStatus))
    {
        txData = dequeueRingBuffer(&uartTxRingBuff);
        Cy_SCB_UART_Transmit(UART_HW, &txData, 1, &duinoUartContext);
    }

    return (size_t)cnt;
}

void UART_Isr(void)
{
    Cy_SCB_UART_Interrupt(UART_HW, &duinoUartContext);
}

void handleUartEvent(uint32_t event)
{
    if (event == CY_SCB_RX_INTR_LEVEL)
    {
        queueRingBuffer(&uartRxRingBuff, rxData);
    }

    if ((event == CY_SCB_TX_INTR_LEVEL) && !emptyRingBuffer(&uartTxRingBuff))
    {
        txData = dequeueRingBuffer(&uartTxRingBuff);
        Cy_SCB_UART_Transmit(UART_HW, &txData, 1, &duinoUartContext);
    }
}

void uart_init(uint8_t inst, uint8_t cfg, uint32_t bitrate)
{
    cy_stc_scb_uart_config_t uart_cfg;
    uint8_t i;
    uint32_t divNum = 0;

    /* Configure and enable the UART peripheral. */
    if (inst == DUINO_UART)
    {
        memcpy((void *)&uart_cfg, (const void *)&serial_config_default, sizeof(cy_stc_scb_uart_config_t));

        Cy_GPIO_SetHSIOM(UART_RX_PORT, UART_RX_PIN_NUM, UART_RX_HSIOM);
        Cy_GPIO_SetHSIOM(UART_TX_PORT, UART_TX_PIN_NUM, UART_TX_HSIOM);

        Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
        Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_STRONG);

        Cy_SysClk_PeriphAssignDivider(PCLK_UART_CLOCK, CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

        for (i=0; i<UART_BIT_RATE_MAX_NUM; i++)
        {
            if(brMapping[i].bitRate == bitrate)
            {
                divNum = brMapping[i].divNum;
                break;
            }
        }

        if (divNum == 0)
        {
            Cy_GPIO_SetDrivemode(UART_RX_PORT, UART_RX_PIN_NUM, CY_GPIO_DM_HIGHZ);
            Cy_GPIO_SetDrivemode(UART_TX_PORT, UART_TX_PIN_NUM, CY_GPIO_DM_HIGHZ);
            return;
        }

        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM, divNum);
        Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, CLK_UART_NUM);

        uart_cfg.dataWidth = uartCfgs[cfg].dataBit;
        uart_cfg.parity = uartCfgs[cfg].parity;
        uart_cfg.stopBits = uartCfgs[cfg].stopBit;

        Cy_SCB_UART_Init(UART_HW, &uart_cfg, &duinoUartContext);

        // setup the callback function point in the context
        Cy_SCB_UART_RegisterCallback(UART_HW, handleUartEvent, &duinoUartContext);

        /* Hook interrupt service routine and enable interrupt */
        (void) Cy_SysInt_Init(&uartIntrConfig, &UART_Isr);
        NVIC_EnableIRQ(uartIntrConfig.intrSrc);

	    Cy_SCB_UART_Enable(UART_HW);

        // start to receive
        Cy_SCB_UART_Receive(UART_HW, &rxData, 1, &duinoUartContext);
    }
#if PMGDUINO_BOARD
    else if (inst == DEBUG_UART)
    {
        Cy_GPIO_SetHSIOM(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_NUM, CYBSP_DEBUG_UART_RX_HSIOM);
        Cy_GPIO_SetHSIOM(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_NUM, CYBSP_DEBUG_UART_TX_HSIOM);

        Cy_GPIO_SetDrivemode(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_NUM, CY_GPIO_DM_HIGHZ);
        Cy_GPIO_SetDrivemode(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_NUM, CY_GPIO_DM_STRONG);

        Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_8_BIT, CLK_DBG_UART_NUM);

        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, CLK_DBG_UART_NUM, 51UL);
        Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, CLK_DBG_UART_NUM);

        Cy_SCB_UART_Init(CYBSP_DBG_UART_HW, &CYBSP_DBG_UART_config, &dbgUartContext);
	    Cy_SCB_UART_Enable(CYBSP_DBG_UART_HW);
    }
#endif
    else 
        return;
}

#if PMGDUINO_BOARD
void uart_print(uint8_t inst, const char* string)
{
    /* Send a string over serial terminal */
    if (inst == DUINO_UART)
        Cy_SCB_UART_PutString(UART_HW, string);
#if PMGDUINO_BOARD
    else
        Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
#endif
}

void uart_print_byte(uint8_t inst, uint8_t byte_var)
{
    const char hex[] = "0123456789ABCDEF" ;

    char string[3] = {0};

    string[0] = hex[(byte_var & 0xF0)>>4];
    string[1] = hex[byte_var & 0x0F];

    if (inst == DUINO_UART)
        Cy_SCB_UART_PutString(UART_HW, string);
#if PMGDUINO_BOARD
    else
        Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
#endif
}

void uart_print_hword(uint8_t inst, uint16_t hword)
{
    const char hex[] = "0123456789ABCDEF" ;

    char string[5] = {0};

    string[0] = hex[(hword >> 12) & 0x0F];
    string[1] = hex[(hword >> 8) & 0x0F];
    string[2] = hex[(hword >> 4) & 0x0F];
    string[3] = hex[hword & 0x0F];

    if (inst == DUINO_UART)
        Cy_SCB_UART_PutString(UART_HW, string);
#if PMGDUINO_BOARD
    else
        Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
#endif
}

void uart_print_word(uint8_t inst, uint32_t word)
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

    if (inst == DUINO_UART)
        Cy_SCB_UART_PutString(UART_HW, string);
#if PMGDUINO_BOARD
    else
        Cy_SCB_UART_PutString(CYBSP_DBG_UART_HW, string);
#endif
}
#endif // PMGDUINO_BOARD


