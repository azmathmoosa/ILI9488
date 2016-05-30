# ILI9488

Sming graphics library for ILI9488.

#How To

Just add this library in your Sming/Sming/Libraries folder.

Then just use it like so in your application code

```C++
Adafruit_ILI9488 tft;
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

#Note

Adafruit has nothing to do with this, I was too lazy to change the names while modifying code form the Adafruit ILI9341 library :D


