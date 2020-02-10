#include <Arduino.h>
#include "display.h"

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=5*/ 5, /*DC=*/0, /*RST=*/2, /*BUSY=*/15));

#include "bitmaps/Bitmaps640x384.h" // 7.5"  b/w

void setupDisplay()
{
	//Serial.begin(115200);
	//Serial.println();
	Serial.println("setupDisplay");
	delay(100);
	display.init(115200);
	// first update should be full refresh
	helloWorld();
	delay(1000);
	// partial refresh mode can be used to full screen,
	// effective if display panel hasFastPartialUpdate
	helloFullScreenPartialMode();
	delay(1000);
	helloArduino();
	delay(1000);
	helloEpaper();
	delay(1000);
	//helloValue(123.9, 1);
	//delay(1000);
	showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
	delay(1000);
	drawBitmaps();

	display.powerOff();
	deepSleepTest();
	Serial.println("setup done");
}

void updateDisplay_Neu(const unsigned char *bitmap)
{
	Serial.println("Update Display #");

	display.setFullWindow();

	//drawBitmaps640x384();

	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		//display.drawBitmap(0, 0, bitmap, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
		display.drawInvertedBitmap(0, 0, bitmap, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
	} while (display.nextPage());
}





// note for partial update window and setPartialWindow() method:
// partial update window size and position is on byte boundary in physical x direction
// the size is increased in setPartialWindow() if x or w are not multiple of 8 for even rotation, y or h for odd rotation
// see also comment in GxEPD2_BW.h, GxEPD2_3C.h or GxEPD2_GFX.h for method setPartialWindow()

const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";

void helloWorld()
{
	//Serial.println("helloWorld");
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	// center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	display.setFullWindow();
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(HelloWorld);
	} while (display.nextPage());
	//Serial.println("helloWorld done");
}

void helloWorldForDummies()
{
	//Serial.println("helloWorld");
	const char text[] = "Hello World!";
	// most e-papers have width < height (portrait) as native orientation, especially the small ones
	// in GxEPD2 rotation 0 is used for native orientation (most TFT libraries use 0 fix for portrait orientation)
	// set rotation to 1 (rotate right 90 degrees) to have enough space on small displays (landscape)
	display.setRotation(1);
	// select a suitable font in Adafruit_GFX
	display.setFont(&FreeMonoBold9pt7b);
	// on e-papers black on white is more pleasant to read
	display.setTextColor(GxEPD_BLACK);
	// Adafruit_GFX has a handy method getTextBounds() to determine the boundary box for a text for the actual font
	int16_t tbx, tby;
	uint16_t tbw, tbh;																				 // boundary box window
	display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
	// center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	// full window mode is the initial mode, set it anyway
	display.setFullWindow();
	// here we use paged drawing, even if the processor has enough RAM for full buffer
	// so this can be used with any supported processor board.
	// the cost in code overhead and execution time penalty is marginal
	// tell the graphics class to use paged drawing mode
	display.firstPage();
	do
	{
		// this part of code is executed multiple times, as many as needed,
		// in case of full buffer it is executed once
		// IMPORTANT: each iteration needs to draw the same, to avoid strange effects
		// use a copy of values that might change, don't read e.g. from analog or pins in the loop!
		display.fillScreen(GxEPD_WHITE); // set the background to white (fill the buffer with value for white)
		display.setCursor(x, y);				 // set the postition to start printing text
		display.print(text);						 // print some text
																		 // end of part executed multiple times
	}
	// tell the graphics class to transfer the buffer content (page) to the controller buffer
	// the graphics class will command the controller to refresh to the screen when the last page has been transferred
	// returns true if more pages need be drawn and transferred
	// returns false if the last page has been transferred and the screen refreshed for panels without fast partial update
	// returns false for panels with fast partial update when the controller buffer has been written once more, to make the differential buffers equal
	// (for full buffered with fast partial update the (full) buffer is just transferred again, and false returned)
	while (display.nextPage());
	//Serial.println("helloWorld done");
}

void helloFullScreenPartialMode()
{
	//Serial.println("helloFullScreenPartialMode");
	const char fullscreen[] = "full screen update";
	const char fpm[] = "fast partial mode";
	const char spm[] = "slow partial mode";
	const char npm[] = "no partial mode";
	display.setPartialWindow(0, 0, display.width(), display.height());
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	const char *updatemode;
	if (display.epd2.hasFastPartialUpdate)
	{
		updatemode = fpm;
	}
	else if (display.epd2.hasPartialUpdate)
	{
		updatemode = spm;
	}
	else
	{
		updatemode = npm;
	}
	// do this outside of the loop
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// center update text
	display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t utx = ((display.width() - tbw) / 2) - tbx;
	uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
	// center update mode
	display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t umx = ((display.width() - tbw) / 2) - tbx;
	uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
	// center HelloWorld
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
	uint16_t hwy = ((display.height() - tbh) / 2) - tby;
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(hwx, hwy);
		display.print(HelloWorld);
		display.setCursor(utx, uty);
		display.print(fullscreen);
		display.setCursor(umx, umy);
		display.print(updatemode);
	} while (display.nextPage());
	//Serial.println("helloFullScreenPartialMode done");
}

void helloArduino()
{
	//Serial.println("helloArduino");
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// align with centered HelloWorld
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	// height might be different
	display.getTextBounds(HelloArduino, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t y = ((display.height() / 4) - tbh / 2) - tby; // y is base line!
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	uint16_t wy = (display.height() / 4) - wh / 2;
	display.setPartialWindow(0, wy, display.width(), wh);
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		//display.drawRect(x, y - tbh, tbw, tbh, GxEPD_BLACK);
		display.setCursor(x, y);
		display.print(HelloArduino);
	} while (display.nextPage());
	delay(1000);
	//Serial.println("helloArduino done");
}

void helloEpaper()
{
	//Serial.println("helloEpaper");
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// align with centered HelloWorld
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	// height might be different
	display.getTextBounds(HelloEpaper, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t y = (display.height() * 3 / 4) + tbh / 2; // y is base line!
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	uint16_t wy = (display.height() * 3 / 4) - wh / 2;
	display.setPartialWindow(0, wy, display.width(), wh);
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(HelloEpaper);
	} while (display.nextPage());
	//Serial.println("helloEpaper done");
}

#if defined(ESP8266) || defined(ESP32)
#include <StreamString.h>
#define PrintString StreamString
#else
class PrintString : public Print, public String
{
public:
	size_t write(uint8_t data) override
	{
		return concat(char(data));
	};
};
#endif

void helloValue(double v, int digits)
{
	//Serial.println("helloValue");
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	PrintString valueString;
	valueString.print(v, digits);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	display.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = (display.height() * 3 / 4) + tbh / 2; // y is base line!
	// show what happens, if we use the bounding box for partial window
	uint16_t wx = (display.width() - tbw) / 2;
	uint16_t wy = (display.height() * 3 / 4) - tbh / 2;
	display.setPartialWindow(wx, wy, tbw, tbh);
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(valueString);
	} while (display.nextPage());
	delay(2000);
	// make the partial window big enough to cover the previous text
	uint16_t ww = tbw; // remember window width
	display.getTextBounds(HelloEpaper, 0, 0, &tbx, &tby, &tbw, &tbh);
	// adjust, because HelloEpaper was aligned, not centered (could calculate this to be precise)
	ww = max(ww, uint16_t(tbw + 12)); // 12 seems ok
	wx = (display.width() - tbw) / 2;
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	wy = (display.height() * 3 / 4) - wh / 2;
	display.setPartialWindow(wx, wy, ww, wh);
	// alternately use the whole width for partial window
	//display.setPartialWindow(0, wy, display.width(), wh);
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(valueString);
	} while (display.nextPage());
	//Serial.println("helloValue done");
}

void deepSleepTest()
{
	//Serial.println("deepSleepTest");
	const char hibernating[] = "hibernating ...";
	const char wokeup[] = "woke up";
	const char from[] = "from deep sleep";
	const char again[] = "again";
	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// center text
	display.getTextBounds(hibernating, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	display.setFullWindow();
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(hibernating);
	} while (display.nextPage());
	display.hibernate();
	delay(5000);
	display.getTextBounds(wokeup, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t wx = (display.width() - tbw) / 2;
	uint16_t wy = (display.height() / 3) + tbh / 2; // y is base line!
	display.getTextBounds(from, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t fx = (display.width() - tbw) / 2;
	uint16_t fy = (display.height() * 2 / 3) + tbh / 2; // y is base line!
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(wx, wy);
		display.print(wokeup);
		display.setCursor(fx, fy);
		display.print(from);
	} while (display.nextPage());
	delay(5000);
	display.getTextBounds(hibernating, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t hx = (display.width() - tbw) / 2;
	uint16_t hy = (display.height() / 3) + tbh / 2; // y is base line!
	display.getTextBounds(again, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t ax = (display.width() - tbw) / 2;
	uint16_t ay = (display.height() * 2 / 3) + tbh / 2; // y is base line!
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(hx, hy);
		display.print(hibernating);
		display.setCursor(ax, ay);
		display.print(again);
	} while (display.nextPage());
	display.hibernate();
	//Serial.println("deepSleepTest done");
}

void showBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial)
{
	//Serial.println("showBox");
	display.setRotation(1);
	if (partial)
	{
		display.setPartialWindow(x, y, w, h);
	}
	else
	{
		display.setFullWindow();
	}
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.fillRect(x, y, w, h, GxEPD_BLACK);
	} while (display.nextPage());
	//Serial.println("showBox done");
}

void drawCornerTest()
{
	display.setFullWindow();
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	for (uint16_t r = 0; r <= 4; r++)
	{
		display.setRotation(r);
		display.firstPage();
		do
		{
			display.fillScreen(GxEPD_WHITE);
			display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
			display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
			display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
			display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);
			display.setCursor(display.width() / 2, display.height() / 2);
			display.print(display.getRotation());
		} while (display.nextPage());
		delay(2000);
	}
}

void showFont(const char name[], const GFXfont *f)
{
	display.setFullWindow();
	display.setRotation(0);
	display.setTextColor(GxEPD_BLACK);
	display.firstPage();
	do
	{
		drawFont(name, f);
	} while (display.nextPage());
}

void drawFont(const char name[], const GFXfont *f)
{
	//display.setRotation(0);
	display.fillScreen(GxEPD_WHITE);
	display.setTextColor(GxEPD_BLACK);
	display.setFont(f);
	display.setCursor(0, 0);
	display.println();
	display.println(name);
	display.println(" !\"#$%&'()*+,-./");
	display.println("0123456789:;<=>?");
	display.println("@ABCDEFGHIJKLMNO");
	display.println("PQRSTUVWXYZ[\\]^_");
	if (display.epd2.hasColor)
	{
		display.setTextColor(GxEPD_RED);
	}
	display.println("`abcdefghijklmno");
	display.println("pqrstuvwxyz{|}~ ");
}

// note for partial update window and setPartialWindow() method:
// partial update window size and position is on byte boundary in physical x direction
// the size is increased in setPartialWindow() if x or w are not multiple of 8 for even rotation, y or h for odd rotation
// see also comment in GxEPD2_BW.h, GxEPD2_3C.h or GxEPD2_GFX.h for method setPartialWindow()
// showPartialUpdate() purposely uses values that are not multiples of 8 to test this

void showPartialUpdate()
{
	// some useful background
	helloWorld();
	// use asymmetric values for test
	uint16_t box_x = 10;
	uint16_t box_y = 15;
	uint16_t box_w = 70;
	uint16_t box_h = 20;
	uint16_t cursor_y = box_y + box_h - 6;
	float value = 13.95;
	uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	// show where the update box is
	for (uint16_t r = 0; r < 4; r++)
	{
		display.setRotation(r);
		display.setPartialWindow(box_x, box_y, box_w, box_h);
		display.firstPage();
		do
		{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
			//display.fillScreen(GxEPD_BLACK);
		} while (display.nextPage());
		delay(2000);
		display.firstPage();
		do
		{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		} while (display.nextPage());
		delay(1000);
	}
	//return;
	// show updates in the update box
	for (uint16_t r = 0; r < 4; r++)
	{
		display.setRotation(r);
		display.setPartialWindow(box_x, box_y, box_w, box_h);
		for (uint16_t i = 1; i <= 10; i += incr)
		{
			display.firstPage();
			do
			{
				display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
				display.setCursor(box_x, cursor_y);
				display.print(value * i, 2);
			} while (display.nextPage());
			delay(500);
		}
		delay(1000);
		display.firstPage();
		do
		{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		} while (display.nextPage());
		delay(1000);
	}
}

void drawBitmaps()
{
	display.setFullWindow();

	drawBitmaps640x384();
}

void drawBitmaps640x384()
{
	const unsigned char *bitmaps[] =
			{
					Bitmap640x384_1, Bitmap640x384_2};

	if ((display.epd2.panel == GxEPD2::GDEW075T8) || (display.epd2.panel == GxEPD2::GDEW075Z09))
	{
		for (uint16_t i = 0; i < sizeof(bitmaps) / sizeof(char *); i++)
		{
			display.firstPage();
			do
			{
				display.fillScreen(GxEPD_WHITE);
				display.drawInvertedBitmap(0, 0, bitmaps[i], display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
			} while (display.nextPage());
			delay(2000);
		}
	}
}
