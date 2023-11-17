/*
 * SPI_Class_Test.h
 *
 *  Created on: Aug 21, 2023
 *      Author: chenjoshua
 */

#ifndef SPI_CLASS_TEST_H_
#define SPI_CLASS_TEST_H_

#include "cy_pdl.h"

#if defined(__cplusplus)
extern "C" {
#endif
	uint32_t initMaster(bool);
	uint32_t sendPacket(uint8_t *, uint8_t *, uint32_t);
	uint32_t sendPacketEx(uint8_t *, uint8_t *, uint32_t);
	uint8_t getBitOrder(void);
#if defined(__cplusplus)
}
#endif

typedef enum {
  SPI_MODE0 = 0,
  SPI_MODE1 = 1,
  SPI_MODE2 = 2,
  SPI_MODE3 = 3,
} SPIMode;

typedef enum {
  LSBFIRST = 0,
  MSBFIRST = 1,
} BitOrder;

typedef enum
{
	BIT_ORDER_TYPE_MSB = 0x00,
	BIT_ORDER_TYPE_LSB = 0x01
} BIT_ORDER_TYPE;

class SPISettings {
  public:
#if 0
  SPISettings(uint32_t clock, BitOrder bitOrder, SPIMode dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }


  SPISettings(uint32_t clock, BitOrder bitOrder, int dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, (SPIMode)dataMode);
    } else {
      init_MightInline(clock, bitOrder, (SPIMode)dataMode);
    }
  }
#endif

  // Default speed set to 4MHz, SPI mode set to MODE 0 and Bit order set to MSB first.
  //SPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0); }

#if 0
  bool operator==(const SPISettings& rhs) const
  {
    if ((this->clockFreq == rhs.clockFreq) &&
        (this->bitOrder == rhs.bitOrder) &&
        (this->dataMode == rhs.dataMode)) {
      return true;
    }
    return false;
  }

  bool operator!=(const SPISettings& rhs) const
  {
    return !(*this == rhs);
  }
#endif
  uint32_t getClockFreq() const {
    return clockFreq;
  }
  SPIMode getDataMode() const {
    return dataMode;
  }

  BitOrder getBitOrder() const {
    return (bitOrder);
  }

  private:
  void init_MightInline(uint32_t clock, BitOrder bitOrder, SPIMode dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }

  // Core developer MUST use an helper function in beginTransaction() to use this data
  void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, SPIMode dataMode) __attribute__((__always_inline__)) {
    this->clockFreq = clock;
    this->dataMode = dataMode;
    this->bitOrder = bitOrder;
  }

  uint32_t clockFreq;
  SPIMode dataMode;
  BitOrder bitOrder;
};

//struct SPIClass_PmgDuino
class SPIClass_PmgDuino
{
public:
    void begin();
    void end();

    uint8_t transfer(uint8_t data);
    void transfer(uint8_t* buf, uint8_t count);
    uint16_t transfer16(uint16_t data);
    uint8_t transferPacket(uint8_t cmd);

    void setBitOrder(BIT_ORDER_TYPE order);
    void setDataMode(uint8_t mode);
	void setClockDivider(uint8_t div);

	void endTransaction(void);
	void beginTransaction(SPISettings settings);

    SPIClass_PmgDuino(void);
    ~SPIClass_PmgDuino(void);

private:
    BIT_ORDER_TYPE getBitOrder();

    bool initialized;
    uint8_t interruptMode;
    char interruptSave;
    uint32_t interruptMask;
    BIT_ORDER_TYPE BitOrder = BIT_ORDER_TYPE_MSB;
};

#endif /* SPI_CLASS_TEST_H_ */
