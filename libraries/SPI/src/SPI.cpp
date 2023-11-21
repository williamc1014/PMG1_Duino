#include "SPI.h"

void SPIClass::begin()
{
	spiPinInit();
}

void SPIClass_PmgDuino::setBitOrder(BIT_ORDER_TYPE order)
{
	BitOrder = order;
	return;
}

BIT_ORDER_TYPE SPIClass_PmgDuino::getBitOrder()
{
	return BitOrder;
}

void SPIClass_PmgDuino::begin()
{
	if (BitOrder==BIT_ORDER_TYPE_MSB)
		initMaster(true);
	else
		initMaster(false);
	return;
}

uint8_t SPIClass_PmgDuino::transferPacket(uint8_t cmd)
{
	uint8_t tx_buffer[SIZE_OF_PACKET] = {PACKET_SOP, 0, PACKET_EOP};
	uint8_t rx_buffer[SIZE_OF_PACKET] = {0, 0, 0};

	tx_buffer[PACKET_CMD_POS] = cmd;
	return sendPacketEx(tx_buffer, rx_buffer, SIZE_OF_PACKET);
}

uint8_t SPIClass_PmgDuino::transfer(uint8_t data)
{
	uint8_t tx_buffer, rx_buffer;

	tx_buffer = data;
	return sendPacket(&tx_buffer, &rx_buffer, 1);
}

void SPIClass_PmgDuino::transfer(uint8_t* buf, uint8_t count)
{
	uint8_t *rx_buffer = new uint8_t[count];

	sendPacket(buf, rx_buffer, count);

	delete[] rx_buffer;

	return;
}

uint16_t SPIClass_PmgDuino::transfer16(uint16_t data)
{
	uint8_t rx_buffer[2] = {0, 0};
	uint8_t tx_buf[2] = {0, 0};

	if (BitOrder==BIT_ORDER_TYPE_LSB)
	{
		tx_buf[0] = data;
		tx_buf[1] = (data>>8);
	}
	else
	{
		tx_buf[0] = (data>>8);
		tx_buf[1] = data;
	}

	sendPacket(tx_buf, rx_buffer, 2);
	return 0;
}
