#ifndef INCLUDE_HSPI_H_
#define INCLUDE_HSPI_H_

#include "spi_register.h" // from 0.9.4 IoT_Demo
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>

//#define SPI         0
#define HSPI        1
#define SPIFIFOSIZE 16 //16 words length

extern uint32_t *spi_fifo;

extern void azhspi_init(void);
extern void azhspi_send_data(const uint8_t * data, uint8_t datasize);
extern void azhspi_send_uint16_r(const uint16_t data, int32_t repeats);
extern void azhspi_send_rgb_r(uint8_t r, uint8_t g, uint8_t b, uint32_t repeats);
inline void azhspi_wait_ready(void){while (READ_PERI_REG(SPI_FLASH_CMD(HSPI))&SPI_FLASH_USR);}

inline void azhspi_prepare_tx(uint32_t bytecount)
{
	uint32_t bitcount = bytecount * 8 - 1;

	WRITE_PERI_REG(SPI_FLASH_USER1(HSPI), (bitcount & SPI_USR_OUT_BITLEN) << SPI_USR_OUT_BITLEN_S);
}


inline void azhspi_start_tx()
{
	SET_PERI_REG_MASK(SPI_FLASH_CMD(HSPI), SPI_FLASH_USR);   // send
}

inline void azhspi_send_uint8(uint8_t data)
{
	azhspi_prepare_tx(1);
	*spi_fifo = data;
	azhspi_start_tx();
}

inline void azhspi_send_uint16(uint16_t data)
{
	azhspi_prepare_tx(2);
	*spi_fifo = data;
	azhspi_start_tx();
}
inline void azhspi_send_rgb(uint8_t r, uint8_t g, uint8_t b){
	azhspi_prepare_tx(3);
	uint32_t data = b<<16|g<<8|r;
	*spi_fifo = data;
	azhspi_start_tx();

}

inline void azhspi_send_uint32(uint32_t data)
{
	azhspi_prepare_tx(4);
	*spi_fifo = data;
	azhspi_start_tx();
}

#endif /* INCLUDE_HSPI_H_ */
