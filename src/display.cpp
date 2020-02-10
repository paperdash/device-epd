#include <Arduino.h>
#include "display.h"

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK ->   (18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0


GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
uint8_t _buffer[(GxEPD2_750::WIDTH / 8) * GxEPD2_750::HEIGHT];

void setupDisplay()
{
	Serial.println("setupDisplay");
	delay(100);
	display.init(115200);

	Serial.println("setup done");
}


void displayOpenFramebuffer()
{
	memset(_buffer, 0, sizeof(_buffer));
	//display.setFullWindow();
	//display.firstPage();
	//display.fillScreen(GxEPD_WHITE);
}

// TODO
void displayWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	for (int i = 0; i < c; i++)
	{
		_buffer[offset] = bitmap[i];
		offset++;
	}

	/*
	//if (offset + sizeof(*bitmap) <= sizeof(_buffer))
	if (true)
	{
		memcpy((&_buffer) + offset, bitmap, 128);
	}
	else
	{
		Serial.println("!!!!! displayWriteFramebuffer overflow");
	}
	*/
}


void displayFlushFramebuffer()
{
	Serial.println("displayFlushFramebuffer");
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);
	display.drawInvertedBitmap(0, 0, _buffer, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
	display.nextPage();
}


/**
 * @todo fertigstellen
 */
void displayWriteFramebuffer__(int offset, uint8_t bitmap[])
{
	// taken from Adafruit_GFX.cpp, modified
	int16_t w = display.epd2.WIDTH;
	int16_t h = display.epd2.HEIGHT;

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;
	for (int16_t j = 0; j < h; j++)
	{
		for (int16_t i = 0; i < w; i++ )
		{
			if (i & 7) byte <<= 1;
			else
			{
				byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			}

			if (!(byte & 0x80))
			{
				display.drawPixel(0 + i, 0 + j, GxEPD_BLACK);
			}
		}
	}
}



void printSplash()
{
	const char Hello[] = "Hello Paperdash!";

	display.setRotation(1);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby;
	uint16_t tbw, tbh;
	display.getTextBounds(Hello, 0, 0, &tbx, &tby, &tbw, &tbh);
	// center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	display.setFullWindow();
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.print(Hello);
	} while (display.nextPage());
}