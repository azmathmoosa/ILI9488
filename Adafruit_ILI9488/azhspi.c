#include "azhspi.h"

/*
Pinout:
MISO GPIO12
MOSI GPIO13
CLK GPIO14
CS GPIO15
DC GPIO5
RST GPIO4
*/

#define HSPI_PRESCALER 4// target azhspi clock speed is 40MHz/HSPI_PRESCALER, so that with prescaler 2 the azhspi clock is 30MHz

#define __min(a,b) ((a > b) ? (b):(a))
uint32_t *spi_fifo;

void azhspi_init(void)
{
	spi_fifo = (uint32_t*)SPI_FLASH_C0(HSPI);

	WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //clear bit9

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); // HSPIQ MISO GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); // HSPID MOSI GPIO13
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); // CLK GPIO14
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); // CS GPIO15


	// SPI clock = CPU clock / 10 / 4
	// time length HIGHT level = (CPU clock / 10 / 2) ^ -1,
	// time length LOW level = (CPU clock / 10 / 2) ^ -1
	WRITE_PERI_REG(SPI_FLASH_CLOCK(HSPI),
	   (((HSPI_PRESCALER - 1) & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
	   ((1 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |
	   ((0 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |
	   ((1 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));

	//speeditup
	// Speed things up 
        /*WRITE_PERI_REG(SPI_FLASH_CLOCK(HSPI),
        (((1-1) & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
        (((2-1) & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |
        (((2 / 2 - 1) & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |
        (((2-1) & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));*/

	WRITE_PERI_REG(SPI_FLASH_CTRL1(HSPI), 0);

	uint32_t regvalue = SPI_FLASH_DOUT;
    regvalue &= ~(BIT2 | SPI_FLASH_USR_ADDR | SPI_FLASH_USR_DUMMY | SPI_FLASH_USR_DIN | SPI_USR_COMMAND | SPI_DOUTDIN); //clear bit 2 see example IoT_Demo
	WRITE_PERI_REG(SPI_FLASH_USER(HSPI), regvalue);
}

void azhspi_send_uint16_r(uint16_t data, int32_t repeats)
{
	uint32_t i;
	uint32_t word = data << 16 | data;

	while(repeats > 0)
	{
		uint16_t bytes_to_transfer = __min(repeats * sizeof(uint16_t) , SPIFIFOSIZE * sizeof(uint32_t));
		azhspi_wait_ready();
		azhspi_prepare_tx(bytes_to_transfer);
		for(i = 0; i < (bytes_to_transfer + 3) / 4;i++)
			spi_fifo[i] = word;
		azhspi_start_tx();
		repeats -= bytes_to_transfer / 2;
	}
}
void azhspi_send_rgb_r(uint8_t r, uint8_t g, uint8_t b, uint32_t repeats){
		uint32_t i;
		uint32_t word = b << 16 | g << 8 | r;
		//uint8_t data[3] = {0xFF,0xCC,0xFF};
		//r = 0x00, g = 0x00, b = 0xFF;
		while(repeats > 0)
		{
			//uint16_t bytes_to_transfer = __min(repeats * 3 , SPIFIFOSIZE * sizeof(uint32_t));
			azhspi_wait_ready();
			azhspi_prepare_tx(3);
			*spi_fifo = word;
			azhspi_start_tx();
			repeats--;
		}
}

void azhspi_send_data(const uint8_t * data, uint8_t datasize)
{
	uint32_t *_data = (uint32_t*)data;
	uint8_t i;

	uint8_t words_to_send = __min((datasize + 3) / 4, SPIFIFOSIZE);
	azhspi_prepare_tx(datasize);
	for(i = 0; i < words_to_send;i++)
		spi_fifo[i] = _data[i];
	azhspi_start_tx();
}
