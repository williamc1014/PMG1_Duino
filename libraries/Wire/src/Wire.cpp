//#include "Wire.h"
#include "cy_scb_i2c.h"
#include "config.h"


#if CY7113_BOARD
#define I2C_HW SCB0
#define I2C_IRQ scb_0_interrupt_IRQn
#define I2C_SDA_PORT GPIO_PRT4
#define I2C_SDA_PIN 1U
#define I2C_SDA_HSIOM P4_1_SCB0_I2C_SDA
#define I2C_SCL_PORT GPIO_PRT4
#define I2C_SCL_PIN 0U
#define I2C_SCL_HSIOM P4_0_SCB0_I2C_SCL
#endif

#if PMGDUINO_BOARD
#define I2C_HW SCB1
#define I2C_IRQ scb_1_interrupt_IRQn
#define I2C_SDA_PORT GPIO_PRT1
#define I2C_SDA_PIN 5U
#define I2C_SDA_HSIOM P1_5_SCB1_I2C_SDA
#define I2C_SCL_PORT GPIO_PRT1
#define I2C_SCL_PIN 6U
#define I2C_SCL_HSIOM P1_6_SCB1_I2C_SCL
#endif

cy_stc_scb_i2c_config_t I2C_config =
{
    .i2cMode = CY_SCB_I2C_SLAVE,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 16,
    .slaveAddressMask = 254,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 0,
    .highPhaseDutyCycle = 0,
    .delayInFifoAddress = 0,
};

cy_stc_gpio_pin_config_t I2C_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2C_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

const cy_stc_gpio_pin_config_t I2C_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2C_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

//****************************************************************************
// @Global Variables
//****************************************************************************
cy_stc_scb_i2c_context_t I2C_context;

// Initialize Class Variables //////////////////////////////////////////////////
uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);
//****************************************************************************
// @Local Functions
//****************************************************************************

// Constructors ////////////////////////////////////////////////////////////////
TwoWire::TwoWire()
{

}

// Public Methods //////////////////////////////////////////////////////////////
void I2C_Init(void)
{
    Cy_GPIO_Pin_Init(I2C_SDA_PORT, I2C_SDA_PIN, &I2C_SDA_config);
    Cy_GPIO_Pin_Init(I2C_SCL_PORT, I2C_SCL_PIN, &I2C_SCL_config);

    //clock 400kbps
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB1_CLOCK, CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 7U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 7U, 4U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 7U);
}

void I2C_Interrupt(void)
{
    Cy_SCB_I2C_Interrupt(I2C_HW, &I2C_context);
}

void TwoWire::begin(void)
{
    cy_en_scb_i2c_status_t initStatus;
    cy_en_sysint_status_t sysStatus;
    cy_stc_sysint_t I2C_SCB_IRQ_cfg =
    {
        .intrSrc = I2C_IRQ,
        .intrPriority = 0u
    };

    I2C_config.i2cMode = CY_SCB_I2C_MASTER;
    I2C_Init();

    /* Initialize and enable the I2C in master mode */
    initStatus = Cy_SCB_I2C_Init(I2C_HW, &I2C_config, &I2C_context);
    if(initStatus != CY_SCB_I2C_SUCCESS)
    {
        return;
    }

    /* Hook interrupt service routine */
    sysStatus = Cy_SysInt_Init(&I2C_SCB_IRQ_cfg, &I2C_Interrupt);
    if(sysStatus != CY_SYSINT_SUCCESS)
    {
        return;
    }
    NVIC_EnableIRQ((IRQn_Type) I2C_SCB_IRQ_cfg.intrSrc);

    Cy_SCB_I2C_Enable(I2C_HW, &I2C_context);
}

void TwoWire::begin(uint8_t address)
{
    cy_en_scb_i2c_status_t initStatus;
    cy_en_sysint_status_t sysStatus;
    cy_stc_sysint_t I2C_SCB_IRQ_cfg =
    {
        .intrSrc = I2C_IRQ,
        .intrPriority = 0u
    };

    I2C_config.i2cMode = CY_SCB_I2C_SLAVE;
    I2C_config.slaveAddress = address;
    I2C_Init();

    /* Initialize and enable the I2C in slave mode */
    initStatus = Cy_SCB_I2C_Init(I2C_HW, &I2C_config, &I2C_context);
    if(initStatus != CY_SCB_I2C_SUCCESS)
    {
        return;
    }

    /* Hook interrupt service routine */
    sysStatus = Cy_SysInt_Init(&I2C_SCB_IRQ_cfg, &I2C_Interrupt);
    if(sysStatus != CY_SYSINT_SUCCESS)
    {
        return;
    }
    NVIC_EnableIRQ((IRQn_Type) I2C_SCB_IRQ_cfg.intrSrc);

    Cy_SCB_I2C_SlaveConfigWriteBuf(I2C_HW, rxBuffer, BUFFER_LENGTH, &I2C_context);
    Cy_SCB_I2C_SlaveConfigReadBuf(I2C_HW, txBuffer, BUFFER_LENGTH, &I2C_context);
    Cy_SCB_I2C_Enable(I2C_HW, &I2C_context);
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

void TwoWire::end(void)
{
    Cy_SCB_I2C_Disable(I2C_HW, &I2C_context);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
    uint8_t i = 0;

    if (isize > 0)
    {
        // send internal address; this mode allows sending a repeated start to access
        // some devices' internal registers. This function is executed by the hardware
        // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)
        beginTransmission(address);

        // the maximum size of internal address is 3 bytes
        if (isize > 3){
            isize = 3;
        }

        // write internal register address - most significant byte first
        while (isize-- > 0)
        {
            Cy_SCB_I2C_MasterWriteByte(I2C_HW, (uint8_t)(iaddress >> (isize*8)), 10, &I2C_context);
        }

        Cy_SCB_I2C_MasterSendReStart(I2C_HW, address, CY_SCB_I2C_READ_XFER, 10, &I2C_context);
    }
    else
    {
         Cy_SCB_I2C_MasterSendStart(I2C_HW, address, CY_SCB_I2C_READ_XFER, 10, &I2C_context);
    }

    // clamp to buffer length
    if(quantity > BUFFER_LENGTH)
    {
        quantity = BUFFER_LENGTH;
    }

    // perform blocking read into buffer
    while(i < (quantity - 1))
    {
        Cy_SCB_I2C_MasterReadByte(I2C_HW, CY_SCB_I2C_ACK , &rxBuffer[i], 10, &I2C_context);
        i++;
    }

    Cy_SCB_I2C_MasterReadByte(I2C_HW, CY_SCB_I2C_NAK , &rxBuffer[i], 10, &I2C_context);

    if(sendStop)
    {
        Cy_SCB_I2C_MasterSendStop(I2C_HW, 10, &I2C_context);
    }

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return quantity;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;

    Cy_SCB_I2C_MasterSendStart(I2C_HW, address, CY_SCB_I2C_WRITE_XFER, 10, &I2C_context);
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    uint8_t i = 0;

    while(i < txBufferLength)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_HW, txBuffer[i], 10, &I2C_context);
        i++;
    }

    if(sendStop)
    {
        Cy_SCB_I2C_MasterSendStop(I2C_HW, 10, &I2C_context);
    }

    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
    // indicate that we are done transmitting
    transmitting = 0;

    return 0;
}

//  This provides backwards compatibility with the original
//  definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
    if (txBufferLength >= BUFFER_LENGTH)
    {
        //TODO: setWriteError();
        return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer
    txBufferLength = txBufferIndex;

    return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t* data, size_t quantity)
{
    for (size_t i = 0; i < quantity; ++i)
    {
        write(data[i]);
    }

    return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
    int value = -1;

    // get each successive byte on each call
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }

    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
    int value = -1;

    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }

    return value;
}

void TwoWire::flush(void)
{

}

// behind the scenes callback function that is called when a data block is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
    // don't bother if user hasn't registered a callback
    if (!user_onReceive)
    {
        return;
    }
    // don't bother if rx buffer is in use by a master requestFrom() op
    // i know this drops data, but it allows for slight stupidity
    // meaning, they may not have read all the master requestFrom() data yet
    if (rxBufferIndex < rxBufferLength)
    {
        return;
    }

    // copy twi rx buffer into local read buffer
    // this enables new reads to happen in parallel
    for (uint8_t i = 0; i < numBytes; ++i)
    {
        rxBuffer[i] = inBytes[i];
    }
    // set rx iterator vars
    rxBufferIndex = 0;
    rxBufferLength = numBytes;
    // alert user program
    user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if (!user_onRequest)
    {
        return;
    }
    // reset tx buffer iterator vars
    // !!! this will kill any pending pre-master sendTo() activity
    txBufferIndex = 0;
    txBufferLength = 0;
    // alert user program
    user_onRequest();
}

void TwoWire::slaveStatus(void)
{
    int state;

    state = Cy_SCB_I2C_SlaveGetStatus(I2C_HW, &I2C_context);

    if((0u != (state & CY_SCB_I2C_SLAVE_WR_CMPLT)) && (0u == (state & CY_SCB_I2C_SLAVE_BUS_ERR)))
    {
        rxBufferIndex = 0;
        rxBufferLength = Cy_SCB_I2C_SlaveGetWriteTransferCount(I2C_HW, &I2C_context);
        Cy_SCB_I2C_SlaveClearWriteStatus(I2C_HW, &I2C_context);
        user_onReceive(rxBufferLength);
    }
}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(int) )
{
    user_onReceive = function;
    Cy_SCB_I2C_RegisterEventCallback(I2C_HW, (cy_cb_scb_i2c_handle_events_t)slaveStatus, &I2C_context);
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
    user_onRequest = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();

//****************************************************************************
//                                 END OF FILE
//****************************************************************************
