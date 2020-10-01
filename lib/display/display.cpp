#include <Arduino.h>
#include <SPIFFS.h>
#include "display.h"
#include "jpec.h"
#include "esp_task_wdt.h"

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK ->   (18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// mapping of Waveshare Universal e-Paper Raw Panel Driver Shield for Arduino / NUCLEO
// BUSY -> 7, RST -> 8, DC -> 9, CS-> 10, CLK -> 13, DIN -> 11

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class

GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/5, /*DC=*/17, /*RST=*/16, /*BUSY=*/4));

GFXcanvas1 *displayCanvas;
File tmpFileCache;
long startMills;

// bmp
void write16(File &f, uint16_t v);
void write32(File &f, uint32_t v);
uint8_t filldata3[] = {0x0, 0x23, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0x0};
// bmp

void setupDisplay()
{
	Serial.println("setupDisplay");
	delay(100);
	//display.init(115200); TODO

	displayCanvas = new GFXcanvas1(display.width(), display.height());

	Serial.println("setup done");
}

GFXcanvas1 *displayGetCanvas()
{
	return displayCanvas;
}

void displayOpen()
{
	displayCanvas->setRotation(0);
	displayCanvas->fillScreen(GxEPD_WHITE);
}

void displayWritePixel(int16_t x, int16_t y, uint16_t color)
{
	displayCanvas->drawPixel(x, y, color);
}

void displayFlush()
{
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();
	display.fillScreen(GxEPD_WHITE);

	display.drawBitmap(0, 0, displayCanvas->getBuffer(), displayCanvas->width(), displayCanvas->height(), GxEPD_BLACK);
}

/*
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
*/

void exportJPG(GFXcanvas1 *_canvas, const char *fileName, uint8_t q)
{
	Serial.println(F("exportJPG"));
	long start = millis();

	SPIFFS.remove(fileName);
	tmpFileCache = SPIFFS.open(fileName, FILE_WRITE);
	if (!tmpFileCache)
	{
		Serial.println("Failed to open file for writing");
		return;
	}

	/* Create a JPEG encoder provided image data */
	jpec_enc_t *e = jpec_enc_new2(_canvas->getBuffer(), _canvas->width(), _canvas->height(), q, [](int offset, uint8_t val) {
		tmpFileCache.write(val);

		// every 1000ms
		if (startMills + 1000 < millis())
		{
			startMills = millis();
			esp_task_wdt_reset();
		}
	});

	/* Compress */
	jpec_enc_run(e);
	Serial.print(millis() - start);
	Serial.println("ms");

	/* Release the encoder */
	jpec_enc_del(e);
	tmpFileCache.close();
}

void exportBMP(GFXcanvas1 *_canvas, const char *fileName)
{
	Serial.println(F("exportBMP"));
	startMills = millis();

	SPIFFS.remove(fileName);
	tmpFileCache = SPIFFS.open(fileName, FILE_WRITE);
	if (!tmpFileCache)
	{
		Serial.println("Failed to open file for writing");
		return;
	}

	uint8_t *bitmap = _canvas->getBuffer();
	int16_t w = _canvas->width();
	int16_t h = _canvas->height();

	uint16_t depth = 1;
	uint32_t rowSizeCode = (w + 8 - depth) * depth / 8;

	// BMP rows are padded (if needed) to 4-byte boundary
	uint32_t rowSizeBMP = (w * depth / 8 + 3) & ~3;
	uint32_t headerSize = 40;
	uint32_t imageOffset = 62;
	uint32_t fileSize = imageOffset + h * rowSizeBMP;

	write16(tmpFileCache, 0x4D42);		// BMP signature
	write32(tmpFileCache, fileSize);	// fileSize
	write32(tmpFileCache, 0);			// creator bytes
	write32(tmpFileCache, imageOffset); // image offset
	write32(tmpFileCache, headerSize);	// Header size
	write32(tmpFileCache, w);			// image width
	write32(tmpFileCache, h);			// image height
	write16(tmpFileCache, 1);			// # planes
	write16(tmpFileCache, depth);		// bits per pixel
	write32(tmpFileCache, 0);			// format uncompressed

	uint32_t j = 0;
	for (uint32_t i = 34; i < imageOffset; i++)
	{
		tmpFileCache.write(filldata3[j++]); // remaining header bytes
	}

	uint32_t rowidx = w * h / 8;
	for (uint16_t row = 0; row < h; row++) // for each line
	{
		rowidx -= rowSizeCode;

		uint32_t colidx;
		for (colidx = 0; colidx < rowSizeCode; colidx++)
		{
			uint8_t data = pgm_read_byte(&bitmap[rowidx + colidx]);
			tmpFileCache.write(data);
		}

		while (colidx++ < rowSizeBMP)
		{
			tmpFileCache.write(uint8_t(0)); // padding
		}

		esp_task_wdt_reset();
	}

	Serial.print(millis() - startMills);
	Serial.println("ms");

	tmpFileCache.close();
}

void write16(File &f, uint16_t v)
{
	f.write(uint8_t(v));
	f.write(uint8_t(v >> 8));
}

void write32(File &f, uint32_t v)
{
	f.write(uint8_t(v));
	f.write(uint8_t(v >> 8));
	f.write(uint8_t(v >> 16));
	f.write(uint8_t(v >> 24));
}

uint8_t displayPixelBWRatio()
{
	uint8_t *buffer = displayCanvas->getBuffer();
	uint32_t pixelWhite = 0;
	for (uint16_t y = 0; y < displayCanvas->height(); y++)
	{
		for (uint16_t x = 0; x < displayCanvas->width(); x++)
		{

			uint8_t *ptr = (uint8_t *)&buffer[(x / 8) + y * ((displayCanvas->width() + 7) / 8)];
			// return ((*ptr) & (0x80 >> (x & 7))) != 0 ? 0xFF : 0;
			pixelWhite += ((*ptr) & (0x80 >> (x & 7))) != 0 ? 1 : 0;
		}
	}

	uint32_t pixelCount = displayCanvas->width() * displayCanvas->height();
	uint8_t pixelRatio = (uint8_t)round(100.0 / pixelCount * pixelWhite);
	Serial.println(100.0 / pixelCount * pixelWhite);
	Serial.printf("Pixel count: %d, white pixel: %d, ratio: %d\n", pixelCount, pixelWhite, pixelRatio);

	return pixelRatio;
}

void displayPrintScreenJPG(const char *fileName, uint8_t q)
{
	exportJPG(displayCanvas, fileName, q);
}

void displayPrintScreenBMP(const char *fileName)
{
	exportBMP(displayCanvas, fileName);
}