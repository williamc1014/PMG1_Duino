#include <cy_gpio.h>
#include <cy_scb_spi.h>

#if CY7113_BOARD
#define SPI_HW SCB3
#define SPI_IRQ scb_3_interrupt_IRQn

#define SPI_CLOCK_BLOCK PCLK_SCB3_CLOCK

#define SPI_CK_PORT GPIO_PRT1
#define SPI_CK_PORT_NUM 1U
#define SPI_CK_PIN 1U
#define SPI_CK_NUM 1U
#define SPI_CK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM P1_1_SCB3_SPI_CLK //HSIOM_SEL_GPIO
#endif
#define SPI_CK_HSIOM ioss_0_port_1_pin_1_HSIOM
#define SPI_CK_IRQ ioss_interrupt_gpio_IRQn

#define SPI_COPI_PORT GPIO_PRT1
#define SPI_COPI_PORT_NUM 1U
#define SPI_COPI_PIN 3U
#define SPI_COPI_NUM 3U
#define SPI_COPI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_COPI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_3_HSIOM
    #define ioss_0_port_1_pin_3_HSIOM P1_3_SCB3_SPI_MOSI //HSIOM_SEL_GPIO
#endif
#define SPI_COPI_HSIOM ioss_0_port_1_pin_3_HSIOM
#define SPI_COPI_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CIPO_PORT GPIO_PRT1
#define SPI_CIPO_PORT_NUM 1U
#define SPI_CIPO_PIN 2U
#define SPI_CIPO_NUM 2U
#define SPI_CIPO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define SPI_CIPO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM P1_2_SCB3_SPI_MISO //HSIOM_SEL_GPIO
#endif
#define SPI_CIPO_HSIOM ioss_0_port_1_pin_2_HSIOM
#define SPI_CIPO_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CS_PORT GPIO_PRT1
#define SPI_CS_PORT_NUM 1U
#define SPI_CS_PIN 4U
#define SPI_CS_NUM 4U
#define SPI_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
    #define ioss_0_port_1_pin_4_HSIOM P1_4_SCB3_SPI_SELECT0//HSIOM_SEL_GPIO
#endif
#define SPI_CS_HSIOM ioss_0_port_1_pin_4_HSIOM
#define SPI_CS_IRQ ioss_interrupt_gpio_IRQn
#endif

#if PMGDUINO_BOARD
#define SPI_HW SCB2
#define SPI_IRQ scb_2_interrupt_IRQn

#define SPI_CLOCK_BLOCK PCLK_SCB2_CLOCK

#define SPI_CK_PORT GPIO_PRT5
#define SPI_CK_PORT_NUM 5U
#define SPI_CK_PIN 0U
#define SPI_CK_NUM 0U
#define SPI_CK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
    #define ioss_0_port_5_pin_0_HSIOM P5_0_SCB2_SPI_CLK //HSIOM_SEL_GPIO
#endif
#define SPI_CK_HSIOM ioss_0_port_5_pin_0_HSIOM
#define SPI_CK_IRQ ioss_interrupt_gpio_IRQn

#define SPI_COPI_PORT GPIO_PRT5
#define SPI_COPI_PORT_NUM 5U
#define SPI_COPI_PIN 1U
#define SPI_COPI_NUM 1U
#define SPI_COPI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_COPI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
    #define ioss_0_port_5_pin_1_HSIOM P5_1_SCB2_SPI_MOSI //HSIOM_SEL_GPIO
#endif
#define SPI_COPI_HSIOM ioss_0_port_5_pin_1_HSIOM
#define SPI_COPI_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CIPO_PORT GPIO_PRT5
#define SPI_CIPO_PORT_NUM 5U
#define SPI_CIPO_PIN 2U
#define SPI_CIPO_NUM 2U
#define SPI_CIPO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define SPI_CIPO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_2_HSIOM
    #define ioss_0_port_5_pin_2_HSIOM P5_2_SCB2_SPI_MISO //HSIOM_SEL_GPIO
#endif
#define SPI_CIPO_HSIOM ioss_0_port_5_pin_2_HSIOM
#define SPI_CIPO_IRQ ioss_interrupt_gpio_IRQn

#define SPI_CS_PORT GPIO_PRT5
#define SPI_CS_PORT_NUM 5U
#define SPI_CS_PIN 3U
#define SPI_CS_NUM 3U
#define SPI_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPI_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_3_HSIOM
    #define ioss_0_port_5_pin_3_HSIOM P5_3_SCB2_SPI_SELECT0//HSIOM_SEL_GPIO
#endif
#define SPI_CS_HSIOM ioss_0_port_5_pin_3_HSIOM
#define SPI_CS_IRQ ioss_interrupt_gpio_IRQn
#endif

const cy_stc_gpio_pin_config_t SPI_CK_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_CK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_COPI_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_COPI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_CIPO_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = SPI_CIPO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t SPI_CS_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPI_CS_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};


const cy_stc_gpio_pin_config_t SPI_GPIO_HIZ_config = 
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = HSIOM_SEL_GPIO,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

cy_stc_scb_spi_config_t SPI_config = 
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA0_CPOL0,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = 4,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableFreeRunSclk = false,
    .enableInputFilter = true,
    
    .enableMisoLateSample = true,
    .enableTransferSeperation = false,
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                   (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = CY_SCB_SPI_SS_SETUP_DELAY_0_75_CYCLES,
    .ssHoldDelay = CY_SCB_SPI_SS_HOLD_DELAY_0_75_CYCLES,
    .ssInterDataframeDelay = CY_SCB_SPI_SS_INTERFRAME_DELAY_1_5_CYCLES,
    .enableWakeFromSleep = false,
    .mosiIdleHigh = true,
    .rxFifoTriggerLevel = 7UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 0UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

const cy_stc_sysint_t SPI_SCB_IRQ_cfg =
{
    .intrSrc      = SPI_IRQ,
    .intrPriority = 3
};

#define SPI_CLOCK_DIV2      2
#define SPI_CLOCK_DIV4      4
#define SPI_CLOCK_DIV8      8
#define SPI_CLOCK_DIV16     16
#define SPI_CLOCK_DIV32     32
#define SPI_CLOCK_DIV64     64
#define SPI_CLOCK_DIV128    128

#define SPI_BUFFER_MAX_SIZE 32

void spiPinInit(void);
void spiInit(uint32_t rate, bool msbFirst, uint8_t mode);
void spiDeInit(void);
void spiDisable(void);
void spiSetPeripheralClock(uint8_t div);
void spiSendData(uint8_t *txData, uint8_t *rxData, uint8_t size);

