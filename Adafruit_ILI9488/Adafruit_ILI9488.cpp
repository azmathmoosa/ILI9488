/***************************************************
  Wasn't able to find any drivers for ILI9488. This is based on ILI9341 Drivers.
  Mainly fixed some color and pixel encoding stuff.

  Written by Azmath for BaseApp Systems
  MIT license, all text above must be included in any redistribution
 ****************************************************/
/********************************
 * ported for Sming by Azmath Moosa.
 * azmathmoosa@gmail.com
 ********************************/

#include "Adafruit_ILI9488.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>
#define SWAPBYTES(i) ((i>>8) | (i<<8))

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

// Constructor when using software SPI.  All output pins are configurable.
Adafruit_ILI9488::Adafruit_ILI9488() : Adafruit_GFX_32(ILI9488_TFTWIDTH, ILI9488_TFTHEIGHT) {
	tabcolor = 0;
}

void Adafruit_ILI9488::transmitCmdData(uint8_t cmd, const uint8_t *data, uint8_t numDataByte)
{
	azhspi_wait_ready();
	TFT_DC_COMMAND;
	azhspi_send_uint8(cmd);
	azhspi_wait_ready();
	TFT_DC_DATA;
	azhspi_send_data(data, numDataByte);
}

#define DELAY 0x80

void Adafruit_ILI9488::begin(void) {
	azhspi_init();
	TFT_DC_INIT;
	TFT_RST_INIT;

	TFT_RST_ACTIVE;
	os_delay_us(10000);
	TFT_RST_DEACTIVE;
	os_delay_us(120000);

	transmitCmd(0x28); //display off

	uint8_t data[15] = {0};

	data[0] = 0x00;
	transmitCmdData(0xB0,data,1); //Interface Mode Control

	data[0] = 0xA0;
	transmitCmdData(0xB1, data ,1); //Frame Rate

	data[0] = 0x02;
	transmitCmdData(0xB4, data, 1); //Display Inversion Control , 2 Dot

	data[0] = 0x02;
	data[1] = 0x02;
	data[2] = 0x3B;
	transmitCmdData(0xB6, data, 3); //RGB/MCU Interface Control

	data[0] = 0xC6;
	transmitCmdData(0xB7, data, 1); //EntryMode


	data[0] = 0x17;		//Power Control 1
	data[1] = 0x15;
	transmitCmdData(0xC0, data, 2);

	data[0] = 0x41;		//Power Control 2
	transmitCmdData(0xC1, data, 1);

	/*data[0] = 0x00;		//Power Control 3
	data[1] = 0x4D;
	data[2] = 0x80;
	transmitCmdData(0xC5, data, 3);*/

	//VCOM Control
	data[0] = 0x00;
	data[1] = 0x12;
	data[2] = 0x80;
	transmitCmdData(0xC5, data, 3);
	
	data[0] = 0x48;
	transmitCmdData(0x36, data, 1);  //Memory Access

	

	data[0] = 0x66; //16bpp DPI and DBI and
	transmitCmdData(0x3A,data,1);  //Interface Pixel Format
	
	

/*	data[0] = 0x18; //default gamma
	data[1] = 0x16;
	transmitCmdData(0xC0, data, 2);
	*/
	
	
	/*data[0] = 0x00;
	data[1] = 0x04;
	transmitCmdData(0xBE, data, 2); //*/


	data[0] = 0x00;			//P-Gamma
	data[1] = 0x03;
	data[2] = 0x09;
	data[3] = 0x08;
	data[4] = 0x16;
	data[5] = 0x0A;
	data[6] = 0x3F;
	data[7] = 0x78;
	data[8] = 0x4C;
	data[9] = 0x09;
	data[10] = 0x0A;
	data[11] = 0x08;
	data[12] = 0x16;
	data[13] = 0x1A;
	data[14] = 0x0F;
	transmitCmdData(0xE0, data, 15);

	data[0] = 0x00;		//N-Gamma
	data[1] = 0X16;
	data[2] = 0X19;
	data[3] = 0x03;
	data[4] = 0x0F;
	data[5] = 0x05;
	data[6] = 0x32;
	data[7] = 0x45;
	data[8] = 0x46;
	data[9] = 0x04;
	data[10] = 0x0E;
	data[11] = 0x0D;
	data[12] = 0x35;
	data[13] = 0x37;
	data[14] = 0x0F;
	transmitCmdData(0xE1, data, 15);


	data[0] = 0x00;
	transmitCmdData(0xE9, data, 1); //Set Image Func

	data[0] = 0xFF;
	transmitCmdData(0x51, data, 1); //Set Brightness to Max

	data[0] = 0xA9;
	data[1] = 0x51;
	data[2] = 0x2C;
	data[3] = 0x82;
	transmitCmdData(0xF7, data, 4); //Adjust Control

	data[0] = MADCTL_MX | MADCTL_BGR;
	transmitCmdData(ILI9488_MADCTL, data, 1); //set default rotation to 0
	
	transmitCmd(0x11);    	//Exit Sleep
	os_delay_us(120000);

	transmitCmd(0x29);    //Display on
	
}

void Adafruit_ILI9488::drawPixel(int16_t x, int16_t y, uint32_t color) {

	if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
	setAddrWindow(x,y,x+1,y+1);
	transmitDataRGB(color);
}



void Adafruit_ILI9488::drawFastVLine(int16_t x, int16_t y, int16_t h,
		uint32_t color) {

	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;

	if((y+h-1) >= _height)
		h = _height-y;

	setAddrWindow(x, y, x, y+h-1);
	transmitDataRGB(color, h);
}

void Adafruit_ILI9488::drawFastHLine(int16_t x, int16_t y, int16_t w,
		uint32_t color) {

	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;
	if((x+w-1) >= _width)  w = _width-x;
	setAddrWindow(x, y, x+w-1, y);
	transmitDataRGB(color, w);
}

void Adafruit_ILI9488::fillScreen(uint32_t color) {
	fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void Adafruit_ILI9488::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint32_t color) {

	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= _width) || (y >= _height)) return;
	if((x + w - 1) >= _width)  w = _width  - x;
	if((y + h - 1) >= _height) h = _height - y;

	setAddrWindow(x, y, x+w-1, y+h-1);
	transmitDataRGB(color, h*w);
}



// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ILI9488::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void Adafruit_ILI9488::setRotation(uint8_t m) {

	uint8_t data;
	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		data = MADCTL_MX | MADCTL_BGR;
		_width  = ILI9488_TFTWIDTH;
		_height = ILI9488_TFTHEIGHT;
		break;
	case 1:
		data = MADCTL_MV | MADCTL_BGR;
		_width  = ILI9488_TFTHEIGHT;
		_height = ILI9488_TFTWIDTH;
		break;
	case 2:
		data = MADCTL_MY | MADCTL_BGR;
		_width  = ILI9488_TFTWIDTH;
		_height = ILI9488_TFTHEIGHT;
		break;
	case 3:
		data = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
		_width  = ILI9488_TFTHEIGHT;
		_height = ILI9488_TFTWIDTH;
		break;
	}
	transmitCmdData(ILI9488_MADCTL, &data, 1);
}

void Adafruit_ILI9488::setBrightness(uint8_t level){
	transmitCmd(0x28); //display off
	transmitCmdData(0x51, &level, 1);
	transmitCmd(0x29);   //display on
}

void Adafruit_ILI9488::sleep(){
	transmitCmd(0x28); //display off
}

void Adafruit_ILI9488::wakeUp(){
	transmitCmd(0x29);   //display on
}

void Adafruit_ILI9488::invertDisplay(bool i) {
	transmitCmd(i ? ILI9488_INVON : ILI9488_INVOFF);
}
