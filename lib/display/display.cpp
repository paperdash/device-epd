#include <Arduino.h>
#include <SPIFFS.h>
#include "display.h"

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// EPD -> ESP
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK ->   (18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(/*CS=*/5, /*DC=*/17, /*RST=*/16, /*BUSY=*/4));
//GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // 800x480

GFXcanvas1 *displayCanvas;
File tmpFileCache;
long startMills;

// bmp filldata
uint8_t bmpFilldata[] = {0x0, 0x23, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0x0};

void setupDisplay()
{
	Serial.println("setupDisplay");
	delay(100);
	display.init(115200);
	display.setRotation(0);
	display.setFullWindow();
	display.firstPage();

	displayCanvas = new GFXcanvas1(display.width(), display.height());

	Serial.println("setup done");
}

GFXcanvas1 *displayGetCanvas()
{
	return displayCanvas;
}

void displayFlush()
{
	display.fillScreen(GxEPD_WHITE);
	display.drawBitmap(0, 0, displayCanvas->getBuffer(), displayCanvas->width(), displayCanvas->height(), GxEPD_WHITE, GxEPD_BLACK);
	display.display();
}

uint16_t displayGetWidth()
{
	return display.width();
}

uint16_t displayGetHeight()
{
	return display.height();
}

size_t write8(uint8_t *buffer, uint8_t v)
{
	memset(buffer, uint8_t(v), sizeof(uint8_t));

	return sizeof(uint8_t);
}

size_t write16(uint8_t *buffer, uint16_t v)
{
	memset(buffer, uint8_t(v), sizeof(uint8_t));
	memset(buffer + sizeof(uint8_t), uint8_t(v >> 8), sizeof(uint8_t));

	return sizeof(uint16_t);
}

size_t write32(uint8_t *buffer, uint32_t v)
{
	memset(buffer, uint8_t(v), sizeof(uint8_t));
	memset(buffer + sizeof(uint8_t), uint8_t(v >> 8), sizeof(uint8_t));
	memset(buffer + sizeof(uint8_t) * 2, uint8_t(v >> 16), sizeof(uint8_t));
	memset(buffer + sizeof(uint8_t) * 3, uint8_t(v >> 24), sizeof(uint8_t));

	return sizeof(uint32_t);
}

int displaySnapshotBMPStream(uint8_t *buffer, size_t maxLen, size_t index)
{
	GFXcanvas1 *_canvas = displayCanvas;

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

	uint32_t pointer = 0;
	if (index == 0)
	{
		pointer += write16(buffer + pointer, 0x4D42);	   // BMP signature
		pointer += write32(buffer + pointer, fileSize);	   // fileSize
		pointer += write32(buffer + pointer, 0);		   // creator bytes
		pointer += write32(buffer + pointer, imageOffset); // image offset
		pointer += write32(buffer + pointer, headerSize);  // Header size
		pointer += write32(buffer + pointer, w);		   // image width
		pointer += write32(buffer + pointer, h);		   // image height
		pointer += write16(buffer + pointer, 1);		   // # planes
		pointer += write16(buffer + pointer, depth);	   // bits per pixel
		pointer += write32(buffer + pointer, 0);		   // format uncompressed

		uint32_t j = 0;
		for (uint32_t i = 34; i < imageOffset; i++)
		{
			pointer += write8(buffer + pointer, bmpFilldata[j++]);
		}

		return pointer;
	}
	else
	{
		// calculate resume point
		size_t row_from = (index - imageOffset) / rowSizeBMP;
		uint32_t rowidx = w * h / 8;
		rowidx -= rowSizeCode * row_from;
		uint32_t colidx_from = (index - imageOffset) - (row_from * rowSizeBMP);

		// export image
		for (uint16_t row = row_from; row < h; row++) // for each line
		{
			rowidx -= rowSizeCode;

			uint32_t colidx;
			for (colidx = colidx_from; colidx < rowSizeCode; colidx++)
			{
				uint8_t data = pgm_read_byte(&bitmap[rowidx + colidx]);
				pointer += write8(buffer + pointer, data);

				if (pointer == maxLen)
				{
					return pointer;
				}
			}

			// reset resume
			colidx_from = 0;
		}

		return pointer;
	}
}