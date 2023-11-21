#include "pmg1_hw_spi.h"

cy_stc_scb_spi_context_t spiContext;

void SPI_Isr(void)
{
    Cy_SCB_SPI_Interrupt(SPI_HW, &spiContext);
}

void spiPinInit(void)
{
    Cy_GPIO_Pin_Init(SPI_CK_PORT, SPI_CK_PIN, &SPI_CK_config);
    Cy_GPIO_Pin_Init(SPI_COPI_PORT, SPI_COPI_PIN, &SPI_COPI_config);
    Cy_GPIO_Pin_Init(SPI_CIPO_PORT, SPI_CIPO_PIN, &SPI_CIPO_config);
    Cy_GPIO_Pin_Init(SPI_CS_PORT, SPI_CS_PIN, &SPI_CS_config);    
}

void spiInit(uint32_t rate, bool msbFirst, uint8_t mode)
{
    uint8_t div = 2;
    // maximum rate is 48MHx / 2 (DIV) / 4 (Oversample) = 6MHz
    // Minimum rate is 48MHx / 128 (DIV) / 4 (Oversample) = 93.750kHz
    if (rate >= 6000000 || rate <= 93750)
        return;

    if (mode > 3)
        return;

    Cy_GPIO_Pin_Init(SPI_CK_PORT, SPI_CK_PIN, &SPI_CK_config);
    Cy_GPIO_Pin_Init(SPI_COPI_PORT, SPI_COPI_PIN, &SPI_COPI_config);
    Cy_GPIO_Pin_Init(SPI_CIPO_PORT, SPI_CIPO_PIN, &SPI_CIPO_config);
    Cy_GPIO_Pin_Init(SPI_CS_PORT, SPI_CS_PIN, &SPI_CS_config);

    // SPI clock source configuration
    div = (uint8_t)((3000000 / rate) - 1);
    Cy_SysClk_PeriphAssignDivider(SPI_CLOCK_BLOCK, CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 6U, div);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 6U);

    // configure mode and bit-order
    SPI_config.sclkMode = mode;
    SPI_config.enableMsbFirst = (msbFirst) ? true : false;

    /* Configure SPI to operate */
    (void) Cy_SCB_SPI_Init(SPI_HW, (cy_stc_scb_spi_config_t const *)&SPI_config, &spiContext);

    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&SPI_SCB_IRQ_cfg, &SPI_Isr);
    NVIC_EnableIRQ(SPI_SCB_IRQ_cfg.intrSrc);

    /* Enable SPI to operate */
    Cy_SCB_SPI_Enable(SPI_HW);
}

void spiDeInit(void)
{
    Cy_SCB_SPI_Disable(SPI_HW, &spiContext);
    Cy_SCB_SPI_DeInit(SPI_HW);
    Cy_GPIO_Pin_Init(SPI_CK_PORT, SPI_CK_PIN, &SPI_GPIO_HIZ_config);
    Cy_GPIO_Pin_Init(SPI_COPI_PORT, SPI_COPI_PIN, &SPI_GPIO_HIZ_config);
    Cy_GPIO_Pin_Init(SPI_CIPO_PORT, SPI_CIPO_PIN, &SPI_GPIO_HIZ_config);
    Cy_GPIO_Pin_Init(SPI_CS_PORT, SPI_CS_PIN, &SPI_GPIO_HIZ_config);      
}

void spiSetPeripheralClock(uint8_t div)
{
    if (div != SPI_CLOCK_DIV2  || 
        div != SPI_CLOCK_DIV4  ||
        div != SPI_CLOCK_DIV8  ||
        div != SPI_CLOCK_DIV16 ||
        div != SPI_CLOCK_DIV64 ||
        div != SPI_CLOCK_DIV128
       )
       return;

    Cy_SysClk_PeriphAssignDivider(SPI_CLOCK_BLOCK, CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 6U, div-1);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 6U);    
}

uint8_t spiRxData[SPI_BUFFER_MAX_SIZE] = {0xFF};

uint8_t spiSendByte(uint8_t data)
{
    (void)Cy_SCB_SPI_Transfer(SPIHW, &data, &spiRxData[0], 1, &spiContext);

    return rxData[0];
}

uint8_t *spiSendArray(uint8_t *data, uint8_t size)
{
    if (size > SPI_BUFFER_MAX_SIZE)
        return;
    (void)Cy_SCB_SPI_Transfer(SPIHW, data, spiRxData, size, &spiContext);
    
    return rxData;
}