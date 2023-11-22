//#include "SPI.h"

void SPIClass::begin()
{
	spiPinInit();
}

void SPIClass::end()
{
	spiDeInit();
}

void SPIClass::beginTransaction(SPISettings settings)
{
	spiParameter.clockFreq = settings.clockFreq;
	spiParameter.bitOrder = settings.bitOrder;
	spiParameter.dataMode = settings.dataMode;

	spiInit(spiParameter.clockFreq, spiParameter.bitOrder, spiParameter.dataMode);
}

void SPIClass::endTransaction(void)
{
	spiDisable();
}

void SPIClass::setBitOrder(uint8_t order)
{
	if (spiParameter.bitOrder != order)
	{
		spiParameter.bitOrder = order;
		spiDeInit();
		delay(1);
		spiInit(spiParameter.clockFreq, spiParameter.bitOrder, spiParameter.dataMode);
	}
}

void SPIClass::setDataMode(uint8_t mode)
{
	if (spiParameter.dataMode != mode)
	{
		spiParameter.dataMode = mode;
		spiDeInit();
		delay(1);
		spiInit(spiParameter.clockFreq, spiParameter.bitOrder, spiParameter.dataMode);
	}
}

void SPIClass::setClockDivider(uint8_t div)
{
	uint32_t newRate;

	newRate = 48000000 / 4 / div;

	if (spiParameter.clockFreq != newRate)
	{
		spiParameter.clockFreq = newRate;
		spiDeInit();
		delay(1);
		spiInit(spiParameter.clockFreq, spiParameter.bitOrder, spiParameter.dataMode);
	}
}

uint8_t SPIClass::transfer(uint8_t val)
{
	uint8_t txData, rxData;

	txData = val;

	spiSendData(&txData, &rxData, 1);

	return rxData;
}

void SPIClass::transfer(uint8_t* buffer, uint8_t size)
{
	uint8_t i;
	uint8_t rxData;

	for (i=0; i<size; i++)
	{
		spiSendData(&buffer[i], &rxData, size);
		buffer[i] = rxData;
	}
}

uint16_t SPIClass::transfer16(uint16_t val16)
{
	uint16_t rx_buffer;
	uint8_t tx_buf[2] = {0, 0};

	if (spiParameter.bitOrder == LSBFIRST)
	{
		tx_buf[0] = val16;
		tx_buf[1] = (val16>>8);
	}
	else
	{
		tx_buf[0] = (val16>>8);
		tx_buf[1] = val16;
	}

	spiSendData(tx_buf, (uint8_t *)&rx_buffer, 2);

	return rx_buffer;
}
