#ifndef SPI_H_
#define SPI_H_

/*
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
*/

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

class SPISettings {
  public:
  SPISettings(uint32_t speedMaximum, uint8_t dataOrder, uint8_t dataMode)
  {
    clockFreq   = speedMaximum;
    bitOrder    = dataOrder;
    dataMode    = dataMode;
  }

  SPISettings()
  {
    clockFreq   = 4000000;
    bitOrder    = MSBFIRST;
    dataMode    = SPI_MODE0;
  }

  uint32_t  clockFreq;
  uint8_t   bitOrder;
  uint8_t   dataMode;
};


class SPIClass
{
public:
  SPISettings spiParameter;

  SPIClass()
  {
    spiParameter = SPISettings();
  }

  void begin();
  void end();

  void setBitOrder(BitOrder order);
  void setDataMode(uint8_t mode);
	void setClockDivider(uint8_t div);

  void beginTransaction(SPISettings settings);
	void endTransaction(void);
	  

  uint8_t transfer(uint8_t data);
  void transfer(uint8_t* buf, uint8_t count);
  uint16_t transfer16(uint16_t data);
};

#endif /* SPI_CLASS_TEST_H_ */
