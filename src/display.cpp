#include <Arduino.h>
#include "display.h"
#include <SPIFFS.h>

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK ->   (18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/5, /*DC=*/17, /*RST=*/16, /*BUSY=*/4));

void saveScreen();
void writeBitmap(const char filename[], const uint8_t bitmap[], int16_t w, int16_t h, uint16_t depth = 1);
uint8_t filldata[] = {0x0, 0x23, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0x0};
void write16(File &f, uint16_t v);
void write32(File &f, uint32_t v);

void setupDisplay()
{
	Serial.println("setupDisplay");
	delay(100);
	display.init(115200);

	Serial.println("setup done");
}

void displayOpen()
{
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);
}

void displayWritePixel(int16_t x, int16_t y, uint16_t color)
{
	display.drawPixel(x, y, color);
}

void displayWriteFramebuffer(uint8_t bitmap[])
{
	display.drawInvertedBitmap(0, 0, bitmap, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
}

void displayFlush()
{
	display.nextPage();
	/*
	// TODO setting ?
	if (true)
	{
		saveScreen();
	}
	*/
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

// @deprecated
void saveScreen()
{
	unsigned long startMills = millis();
	writeBitmap("/screen.bmp", display.getBuffer(), display.width(), display.height());
	Serial.println(millis() - startMills);
}

// @deprecated
void writeBitmap(const char filename[], const uint8_t bitmap[], int16_t w, int16_t h, uint16_t depth)
{
	File bitmapFile;
	uint32_t rowSizeCode = (w + 8 - depth) * depth / 8;

	// BMP rows are padded (if needed) to 4-byte boundary
	uint32_t rowSizeBMP = (w * depth / 8 + 3) & ~3;
	Serial.print("writeBitmap(\"");
	Serial.print(filename);
	Serial.println("\")");
	//Serial.print("rowSizeCode ");
	//Serial.println(rowSizeCode);
	//Serial.print("rowSizeBMP  ");
	//Serial.println(rowSizeBMP);
	uint32_t headerSize = 40;
	uint32_t imageOffset = 62;
	uint32_t fileSize = imageOffset + h * rowSizeBMP;

	String tmpFile = filename;
	tmpFile += ".tmp";

	bitmapFile = SPIFFS.open(tmpFile, FILE_WRITE);
	if (bitmapFile)
	{
		bitmapFile.seek(0);
		write16(bitmapFile, 0x4D42);	  // BMP signature
		write32(bitmapFile, fileSize);	// fileSize
		write32(bitmapFile, 0);			  // creator bytes
		write32(bitmapFile, imageOffset); // image offset
		write32(bitmapFile, headerSize);  // Header size
		write32(bitmapFile, w);			  // image width
		write32(bitmapFile, h);			  // image height
		write16(bitmapFile, 1);			  // # planes
		write16(bitmapFile, depth);		  // // bits per pixel
		write32(bitmapFile, 0);			  // format uncompressed

		uint32_t j = 0;
		for (uint32_t i = bitmapFile.position(); i < imageOffset; i++)
		{
			bitmapFile.write(filldata[j++]); // remaining header bytes
		}

		uint32_t rowidx = 0;
		for (uint16_t row = 0; row < h; row++) // for each line
		{
			uint32_t colidx;
			for (colidx = 0; colidx < rowSizeCode; colidx++)
			{
				uint8_t data = pgm_read_byte(&bitmap[rowidx + colidx]);
				bitmapFile.write(data);
			}
			rowidx += rowSizeCode;
			while (colidx++ < rowSizeBMP)
			{
				bitmapFile.write(uint8_t(0)); // padding
			}
		}
		bitmapFile.close();

		SPIFFS.remove(filename);
		SPIFFS.rename(tmpFile, filename);

		Serial.println("done");
	}
	else
	{
		Serial.print("open file for write failed!");
	}
}

// @deprecated
void write16(File &f, uint16_t v)
{
	f.write(uint8_t(v));
	f.write(uint8_t(v >> 8));
}

// @deprecated
void write32(File &f, uint32_t v)
{
	f.write(uint8_t(v));
	f.write(uint8_t(v >> 8));
	f.write(uint8_t(v >> 16));
	f.write(uint8_t(v >> 24));
}