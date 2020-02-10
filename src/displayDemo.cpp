#include <Arduino.h>
#include "displayDemo.h"

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK ->   (18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// base class GxEPD2_GFX can be used to pass references or pointers to the diplay2 instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
//#define ENABLE_GxEPD2_GFX 1
#include "bitmaps/Bitmaps640x384.h" // 7.5"  b/w


GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> diplay2(GxEPD2_750(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));


void setupdiplay2Demo()
{
	Serial.println("setupdiplay2Demo");
	delay(100);
	diplay2.init(115200);

	// first update should be full refresh
	helloWorld();
	delay(1000);
	diplay2.powerOff();
	// partial refresh mode can be used to full screen,
	// effective if diplay2 panel hasFastPartialUpdate
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

	diplay2.powerOff();
	deepSleepTest();

	Serial.println("setup done");
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
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	diplay2.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	// center bounding box by transposition of origin:
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t y = ((diplay2.height() - tbh) / 2) - tby;
	diplay2.setFullWindow();
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(x, y);
		diplay2.print(HelloWorld);
	} while (diplay2.nextPage());
	//Serial.println("helloWorld done");
}

void helloWorldForDummies()
{
	//Serial.println("helloWorld");
	const char text[] = "Hello World!";
	// most e-papers have width < height (portrait) as native orientation, especially the small ones
	// in GxEPD2 rotation 0 is used for native orientation (most TFT libraries use 0 fix for portrait orientation)
	// set rotation to 1 (rotate right 90 degrees) to have enough space on small diplay2s (landscape)
	diplay2.setRotation(1);
	// select a suitable font in Adafruit_GFX
	diplay2.setFont(&FreeMonoBold9pt7b);
	// on e-papers black on white is more pleasant to read
	diplay2.setTextColor(GxEPD_BLACK);
	// Adafruit_GFX has a handy method getTextBounds() to determine the boundary box for a text for the actual font
	int16_t tbx, tby;
	uint16_t tbw, tbh;																				 // boundary box window
	diplay2.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
	// center bounding box by transposition of origin:
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t y = ((diplay2.height() - tbh) / 2) - tby;
	// full window mode is the initial mode, set it anyway
	diplay2.setFullWindow();
	// here we use paged drawing, even if the processor has enough RAM for full buffer
	// so this can be used with any supported processor board.
	// the cost in code overhead and execution time penalty is marginal
	// tell the graphics class to use paged drawing mode
	diplay2.firstPage();
	do
	{
		// this part of code is executed multiple times, as many as needed,
		// in case of full buffer it is executed once
		// IMPORTANT: each iteration needs to draw the same, to avoid strange effects
		// use a copy of values that might change, don't read e.g. from analog or pins in the loop!
		diplay2.fillScreen(GxEPD_WHITE); // set the background to white (fill the buffer with value for white)
		diplay2.setCursor(x, y);				 // set the postition to start printing text
		diplay2.print(text);						 // print some text
																		 // end of part executed multiple times
	}
	// tell the graphics class to transfer the buffer content (page) to the controller buffer
	// the graphics class will command the controller to refresh to the screen when the last page has been transferred
	// returns true if more pages need be drawn and transferred
	// returns false if the last page has been transferred and the screen refreshed for panels without fast partial update
	// returns false for panels with fast partial update when the controller buffer has been written once more, to make the differential buffers equal
	// (for full buffered with fast partial update the (full) buffer is just transferred again, and false returned)
	while (diplay2.nextPage());
	//Serial.println("helloWorld done");
}

void helloFullScreenPartialMode()
{
	//Serial.println("helloFullScreenPartialMode");
	const char fullscreen[] = "full screen update";
	const char fpm[] = "fast partial mode";
	const char spm[] = "slow partial mode";
	const char npm[] = "no partial mode";
	diplay2.setPartialWindow(0, 0, diplay2.width(), diplay2.height());
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(GxEPD_BLACK);
	const char *updatemode;
	if (diplay2.epd2.hasFastPartialUpdate)
	{
		updatemode = fpm;
	}
	else if (diplay2.epd2.hasPartialUpdate)
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
	diplay2.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t utx = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t uty = ((diplay2.height() / 4) - tbh / 2) - tby;
	// center update mode
	diplay2.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t umx = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t umy = ((diplay2.height() * 3 / 4) - tbh / 2) - tby;
	// center HelloWorld
	diplay2.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t hwx = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t hwy = ((diplay2.height() - tbh) / 2) - tby;
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(hwx, hwy);
		diplay2.print(HelloWorld);
		diplay2.setCursor(utx, uty);
		diplay2.print(fullscreen);
		diplay2.setCursor(umx, umy);
		diplay2.print(updatemode);
	} while (diplay2.nextPage());
	//Serial.println("helloFullScreenPartialMode done");
}

void helloArduino()
{
	//Serial.println("helloArduino");
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(diplay2.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// align with centered HelloWorld
	diplay2.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	// height might be different
	diplay2.getTextBounds(HelloArduino, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t y = ((diplay2.height() / 4) - tbh / 2) - tby; // y is base line!
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	uint16_t wy = (diplay2.height() / 4) - wh / 2;
	diplay2.setPartialWindow(0, wy, diplay2.width(), wh);
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		//diplay2.drawRect(x, y - tbh, tbw, tbh, GxEPD_BLACK);
		diplay2.setCursor(x, y);
		diplay2.print(HelloArduino);
	} while (diplay2.nextPage());
	delay(1000);
	//Serial.println("helloArduino done");
}

void helloEpaper()
{
	//Serial.println("helloEpaper");
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(diplay2.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// align with centered HelloWorld
	diplay2.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	// height might be different
	diplay2.getTextBounds(HelloEpaper, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t y = (diplay2.height() * 3 / 4) + tbh / 2; // y is base line!
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	uint16_t wy = (diplay2.height() * 3 / 4) - wh / 2;
	diplay2.setPartialWindow(0, wy, diplay2.width(), wh);
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(x, y);
		diplay2.print(HelloEpaper);
	} while (diplay2.nextPage());
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
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(diplay2.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	PrintString valueString;
	valueString.print(v, digits);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	diplay2.getTextBounds(valueString, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t y = (diplay2.height() * 3 / 4) + tbh / 2; // y is base line!
	// show what happens, if we use the bounding box for partial window
	uint16_t wx = (diplay2.width() - tbw) / 2;
	uint16_t wy = (diplay2.height() * 3 / 4) - tbh / 2;
	diplay2.setPartialWindow(wx, wy, tbw, tbh);
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(x, y);
		diplay2.print(valueString);
	} while (diplay2.nextPage());
	delay(2000);
	// make the partial window big enough to cover the previous text
	uint16_t ww = tbw; // remember window width
	diplay2.getTextBounds(HelloEpaper, 0, 0, &tbx, &tby, &tbw, &tbh);
	// adjust, because HelloEpaper was aligned, not centered (could calculate this to be precise)
	ww = max(ww, uint16_t(tbw + 12)); // 12 seems ok
	wx = (diplay2.width() - tbw) / 2;
	// make the window big enough to cover (overwrite) descenders of previous text
	uint16_t wh = FreeMonoBold9pt7b.yAdvance;
	wy = (diplay2.height() * 3 / 4) - wh / 2;
	diplay2.setPartialWindow(wx, wy, ww, wh);
	// alternately use the whole width for partial window
	//diplay2.setPartialWindow(0, wy, diplay2.width(), wh);
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(x, y);
		diplay2.print(valueString);
	} while (diplay2.nextPage());
	//Serial.println("helloValue done");
}

void deepSleepTest()
{
	//Serial.println("deepSleepTest");
	const char hibernating[] = "hibernating ...";
	const char wokeup[] = "woke up";
	const char from[] = "from deep sleep";
	const char again[] = "again";
	diplay2.setRotation(1);
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	// center text
	diplay2.getTextBounds(hibernating, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t x = ((diplay2.width() - tbw) / 2) - tbx;
	uint16_t y = ((diplay2.height() - tbh) / 2) - tby;
	diplay2.setFullWindow();
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(x, y);
		diplay2.print(hibernating);
	} while (diplay2.nextPage());
	diplay2.hibernate();
	delay(5000);
	diplay2.getTextBounds(wokeup, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t wx = (diplay2.width() - tbw) / 2;
	uint16_t wy = (diplay2.height() / 3) + tbh / 2; // y is base line!
	diplay2.getTextBounds(from, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t fx = (diplay2.width() - tbw) / 2;
	uint16_t fy = (diplay2.height() * 2 / 3) + tbh / 2; // y is base line!
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(wx, wy);
		diplay2.print(wokeup);
		diplay2.setCursor(fx, fy);
		diplay2.print(from);
	} while (diplay2.nextPage());
	delay(5000);
	diplay2.getTextBounds(hibernating, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t hx = (diplay2.width() - tbw) / 2;
	uint16_t hy = (diplay2.height() / 3) + tbh / 2; // y is base line!
	diplay2.getTextBounds(again, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t ax = (diplay2.width() - tbw) / 2;
	uint16_t ay = (diplay2.height() * 2 / 3) + tbh / 2; // y is base line!
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.setCursor(hx, hy);
		diplay2.print(hibernating);
		diplay2.setCursor(ax, ay);
		diplay2.print(again);
	} while (diplay2.nextPage());
	diplay2.hibernate();
	//Serial.println("deepSleepTest done");
}

void showBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial)
{
	//Serial.println("showBox");
	diplay2.setRotation(1);
	if (partial)
	{
		diplay2.setPartialWindow(x, y, w, h);
	}
	else
	{
		diplay2.setFullWindow();
	}
	diplay2.firstPage();
	do
	{
		diplay2.fillScreen(GxEPD_WHITE);
		diplay2.fillRect(x, y, w, h, GxEPD_BLACK);
	} while (diplay2.nextPage());
	//Serial.println("showBox done");
}

void drawCornerTest()
{
	diplay2.setFullWindow();
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(GxEPD_BLACK);
	for (uint16_t r = 0; r <= 4; r++)
	{
		diplay2.setRotation(r);
		diplay2.firstPage();
		do
		{
			diplay2.fillScreen(GxEPD_WHITE);
			diplay2.fillRect(0, 0, 8, 8, GxEPD_BLACK);
			diplay2.fillRect(diplay2.width() - 18, 0, 16, 16, GxEPD_BLACK);
			diplay2.fillRect(diplay2.width() - 25, diplay2.height() - 25, 24, 24, GxEPD_BLACK);
			diplay2.fillRect(0, diplay2.height() - 33, 32, 32, GxEPD_BLACK);
			diplay2.setCursor(diplay2.width() / 2, diplay2.height() / 2);
			diplay2.print(diplay2.getRotation());
		} while (diplay2.nextPage());
		delay(2000);
	}
}

void showFont(const char name[], const GFXfont *f)
{
	diplay2.setFullWindow();
	diplay2.setRotation(0);
	diplay2.setTextColor(GxEPD_BLACK);
	diplay2.firstPage();
	do
	{
		drawFont(name, f);
	} while (diplay2.nextPage());
}

void drawFont(const char name[], const GFXfont *f)
{
	//diplay2.setRotation(0);
	diplay2.fillScreen(GxEPD_WHITE);
	diplay2.setTextColor(GxEPD_BLACK);
	diplay2.setFont(f);
	diplay2.setCursor(0, 0);
	diplay2.println();
	diplay2.println(name);
	diplay2.println(" !\"#$%&'()*+,-./");
	diplay2.println("0123456789:;<=>?");
	diplay2.println("@ABCDEFGHIJKLMNO");
	diplay2.println("PQRSTUVWXYZ[\\]^_");
	if (diplay2.epd2.hasColor)
	{
		diplay2.setTextColor(GxEPD_RED);
	}
	diplay2.println("`abcdefghijklmno");
	diplay2.println("pqrstuvwxyz{|}~ ");
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
	uint16_t incr = diplay2.epd2.hasFastPartialUpdate ? 1 : 3;
	diplay2.setFont(&FreeMonoBold9pt7b);
	diplay2.setTextColor(GxEPD_BLACK);
	// show where the update box is
	for (uint16_t r = 0; r < 4; r++)
	{
		diplay2.setRotation(r);
		diplay2.setPartialWindow(box_x, box_y, box_w, box_h);
		diplay2.firstPage();
		do
		{
			diplay2.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
			//diplay2.fillScreen(GxEPD_BLACK);
		} while (diplay2.nextPage());
		delay(2000);
		diplay2.firstPage();
		do
		{
			diplay2.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		} while (diplay2.nextPage());
		delay(1000);
	}
	//return;
	// show updates in the update box
	for (uint16_t r = 0; r < 4; r++)
	{
		diplay2.setRotation(r);
		diplay2.setPartialWindow(box_x, box_y, box_w, box_h);
		for (uint16_t i = 1; i <= 10; i += incr)
		{
			diplay2.firstPage();
			do
			{
				diplay2.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
				diplay2.setCursor(box_x, cursor_y);
				diplay2.print(value * i, 2);
			} while (diplay2.nextPage());
			delay(500);
		}
		delay(1000);
		diplay2.firstPage();
		do
		{
			diplay2.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		} while (diplay2.nextPage());
		delay(1000);
	}
}

void drawBitmaps()
{
	diplay2.setFullWindow();

	drawBitmaps640x384();
}

void drawBitmaps640x384()
{
	const unsigned char *bitmaps[] =
			{
					Bitmap640x384_1, Bitmap640x384_2};

	if ((diplay2.epd2.panel == GxEPD2::GDEW075T8) || (diplay2.epd2.panel == GxEPD2::GDEW075Z09))
	{
		for (uint16_t i = 0; i < sizeof(bitmaps) / sizeof(char *); i++)
		{
			diplay2.firstPage();
			do
			{
				diplay2.fillScreen(GxEPD_WHITE);
				diplay2.drawInvertedBitmap(0, 0, bitmaps[i], diplay2.epd2.WIDTH, diplay2.epd2.HEIGHT, GxEPD_BLACK);
			} while (diplay2.nextPage());
			delay(2000);
		}
	}
}
