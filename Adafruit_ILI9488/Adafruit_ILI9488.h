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

#ifndef _ADAFRUIT_ILI9488H_
#define _ADAFRUIT_ILI9488H_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include "../Adafruit_GFX_32/Adafruit_GFX_32.h"

extern "C"
{
#include <osapi.h>
#include <gpio.h>
#include "azhspi.h"
}

#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_PASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR   0x30
#define ILI9488_MADCTL  0x36
#define ILI9488_PIXFMT  0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1
/*
#define ILI9488_PWCTR6  0xFC

*/

// Color definitions
#define ILI9488_BLACK       0x00000000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x00097194      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x001F7300      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x0001857E      /*   0, 128, 128 */
#define ILI9488_MAROON      0x00E770FF      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x00710087      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x00007BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0x0000C618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x00007BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x000D15FF      /*   0,   0, 255 */
#define ILI9488_GREEN       0x0031FF0D      /*   0, 255,   0 */
#define ILI9488_CYAN        0x000007FF      /*   0, 255, 255 */
#define ILI9488_RED         0x00FF1919      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0x00FF5CDE      /* 255,   0, 255 */
#define ILI9488_YELLOW      0x00FAFF5C      /* 255, 255,   0 */
#define ILI9488_WHITE       0x00FFFFFE      /* 255, 255, 255 */
#define ILI9488_ORANGE      0x00DB7100      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0x0000AFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0x0000F81F

#define TFT_DC_DATA		GPIO_OUTPUT_SET(5, 1)
#define TFT_DC_COMMAND	GPIO_OUTPUT_SET(5, 0)
#define TFT_DC_INIT 	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); TFT_DC_DATA

#define TFT_RST_ACTIVE		GPIO_OUTPUT_SET(4, 0)
#define TFT_RST_DEACTIVE 	GPIO_OUTPUT_SET(4, 1)
#define TFT_RST_INIT		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); TFT_RST_DEACTIVE

#define MAKEWORD(b1, b2, b3, b4) (uint32_t(b1) | ((b2) << 8) | ((b3) << 16) | ((b4) << 24))


class Adafruit_ILI9488 : public Adafruit_GFX_32 {

private:
 uint8_t  tabcolor;
 void transmitCmdData(uint8_t cmd, const uint8_t *data, uint8_t numDataByte);
 inline void transmitData(uint16_t data) {azhspi_wait_ready(); azhspi_send_uint16(data);}
 inline void transmitDataRGB(uint32_t color){
	 	 uint8_t r = (color & 0x00FF0000)>>16;
	 	 r&=0xFC;
	 	 uint8_t g = (color & 0x0000FF00)>>8;
	 	 g&=0xFC;
	 	 uint8_t b = (color & 0x000000FF);
	 	 b&=0xFC;
	 	 azhspi_wait_ready();
	 	 azhspi_send_rgb(r,g,b);
 };
 inline void transmitCmdData(uint8_t cmd, uint32_t data) {azhspi_wait_ready(); TFT_DC_COMMAND; azhspi_send_uint8(cmd); azhspi_wait_ready(); TFT_DC_DATA; azhspi_send_uint32(data);}
 inline void transmitData(uint16_t data, int32_t repeats){azhspi_wait_ready(); azhspi_send_uint16_r(data, repeats);}
 inline void transmitDataRGB(uint32_t color, uint32_t repeats){
	 uint8_t r = (color & 0x00FF0000)>>16;
	 r&=0xFC;
	 uint8_t g = (color & 0x0000FF00)>>8;
	 g&=0xFC;
	 uint8_t b = (color & 0x000000FF);
	 b&=0xFC;
	 azhspi_wait_ready();
	 azhspi_send_rgb_r(r,g,b,repeats);
 };
 inline void transmitCmd(uint8_t cmd){azhspi_wait_ready(); TFT_DC_COMMAND; azhspi_send_uint8(cmd);azhspi_wait_ready(); TFT_DC_DATA;}

public:
  Adafruit_ILI9488();

  void     begin(void),
           fillScreen(uint32_t color),
           drawPixel(int16_t x, int16_t y, uint32_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint32_t color),
           setRotation(uint8_t r),
           invertDisplay(bool i),
	   setBrightness(uint8_t level),
           sleep(),
	   wakeUp();
  inline void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {
	  transmitCmdData(ILI9488_CASET, MAKEWORD(x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF));
  	  transmitCmdData(ILI9488_PASET, MAKEWORD(y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF));
	  transmitCmd(ILI9488_RAMWR); // write to RAM
  }
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
};

#endif
