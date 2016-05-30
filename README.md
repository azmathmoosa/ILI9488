# ILI9488

Sming graphics library for ILI9488.

#How To

If you are using ESP8266 then connect your ILI9488 to the ESP8266 like this:

Pin Mapping

|9488|  ESP |
|:---|-----:|
|MISO|GPIO12|
|MOSI|GPIO13|
|CLK |GPIO14|
|CS  |GPIO15|
|DC  | GPIO5|
|RST | GPIO4|


If you are using something else, just figure out the SPI pins and connect them.

Just add this library in your Sming/Sming/Libraries folder.

Then just use it like so in your application code

```C++
Adafruit_ILI9488 tft;
tft.begin();
tft.fillScreen(ILI9488_BLACK);
tft.setTextColor(ILI9488_WHITE);
tft.setTextSize(2);
tft.println("Hello World!");
```

These are the methods that can be used.

```C
 void begin(void); //Initialize
 void fillScreen(uint32_t color); //Fillscreen with a color
 void drawPixel(int16_t x, int16_t y, uint32_t color); //Fill pixel @x,y with color
 void drawFastVLine(int16_t x, int16_t y, int16_t h, uint32_t color); //Draw vertical line starting from x,y of length h
 void drawFastHLine(int16_t x, int16_t y, int16_t w, uint32_t color); //Draw horizontal line starting form x,y of len w
 void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color); //Draw rectangle @x,y of wid=w,ht=h and fill with color
 void setRotation(uint8_t r); //Rotate display 0,1,2,3
 void invertDisplay(bool i); //Invert pixels
 void setBrightness(uint8_t level); //Doesn't Work!!  Try tweaking.
 void sleep(); //Saves power (won't turn off display - just disable refresh clocks)
 void wakeUp(); // renables clocks
```

These are some color definitions. Format is R-G-B 8-8-8 You can add your own too.

```C++
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
#define ILI9488_WHITE       0x00FFFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0x00DB7100      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0x0000AFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0x0000F81F
```
#Note

Adafruit has nothing to do with this, I was too lazy to change the names while modifying code form the Adafruit ILI9341 library :D


