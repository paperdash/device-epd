#include <Arduino.h>
#include <SPIFFS.h>
//#include <TJpg_Decoder.h>
#include <JPEGDecoder.h> // JPEG decoder library
#include "imageJPEG.h"
#include "display.h"

//#define DEBUG = 1

File tmpFileBuffer;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);

static constexpr int MAX_WIDTH = 640;
static constexpr uint8_t BLOCK_SIZE = 16;
static uint32_t blockDelta[BLOCK_SIZE * MAX_WIDTH + 1];

//static int16_t curRowDeltaJ[MAX_WIDTH + 1];

unsigned int pixelCount = 0;

#define minimum(a, b) (((a) < (b)) ? (a) : (b))

void setupImageJPEG()
{
	Serial.println("setupJPEG");

}

void jpegOpenFramebuffer()
{
	displayOpen();

	SPIFFS.remove("/tmp.jpeg");

	tmpFileBuffer = SPIFFS.open("/tmp.jpeg", FILE_WRITE);
	if (!tmpFileBuffer)
	{
		Serial.println("Failed to open file for writing");
	}

	memset(blockDelta, 0, sizeof(blockDelta));
	pixelCount = 0;
}

void jpegWriteFramebuffer(int offset, uint8_t bitmap[], int c)
{
	Serial.print(".");
	if (tmpFileBuffer)
	{
		tmpFileBuffer.write(bitmap, c);
	}
}

//====================================================================================
//   Print information about the image
//====================================================================================
void jpegInfo()
{
	Serial.println(F("==============="));
	Serial.println(F("JPEG image info"));
	Serial.println(F("==============="));
	Serial.print(F("Width      :"));
	Serial.println(JpegDec.width);
	Serial.print(F("Height     :"));
	Serial.println(JpegDec.height);
	Serial.print(F("Components :"));
	Serial.println(JpegDec.comps);
	Serial.print(F("MCU / row  :"));
	Serial.println(JpegDec.MCUSPerRow);
	Serial.print(F("MCU / col  :"));
	Serial.println(JpegDec.MCUSPerCol);
	Serial.print(F("Scan type  :"));
	Serial.println(JpegDec.scanType);
	Serial.print(F("MCU width  :"));
	Serial.println(JpegDec.MCUWidth);
	Serial.print(F("MCU height :"));
	Serial.println(JpegDec.MCUHeight);
	Serial.println(F("==============="));
}

//====================================================================================
//   Decode and paint onto the TFT screen
//====================================================================================
void renderJPEG(int xpos, int ypos)
{

	// retrieve infomration about the image
	uint16_t *pImg;
	uint16_t mcu_w = JpegDec.MCUWidth;
	uint16_t mcu_h = JpegDec.MCUHeight;
	uint32_t max_x = JpegDec.width;
	uint32_t max_y = JpegDec.height;

	// Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
	// Typically these MCUs are 16x16 pixel blocks
	// Determine the width and height of the right and bottom edge image blocks
	uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
	uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

	// save the current image block size
	uint32_t win_w = mcu_w;
	uint32_t win_h = mcu_h;

	// record the current time so we can measure how long it takes to draw an image
	uint32_t drawTime = millis();

	// save the coordinate of the right and bottom edges to assist image cropping
	// to the screen size
	max_x += xpos;
	max_y += ypos;

	// read each MCU block until there are no more
	while (JpegDec.read())
	{

		// save a pointer to the image block
		pImg = JpegDec.pImage;

		// calculate where the image block should be drawn on the screen
		int mcu_x = JpegDec.MCUx * mcu_w + xpos;
		int mcu_y = JpegDec.MCUy * mcu_h + ypos;

		// check if the image block size needs to be changed for the right and bottom edges
		if (mcu_x + mcu_w <= max_x)
			win_w = mcu_w;
		else
			win_w = min_w;
		if (mcu_y + mcu_h <= max_y)
			win_h = mcu_h;
		else
			win_h = min_h;

		// calculate how many pixels must be drawn
		//uint32_t mcu_pixels = win_w * win_h;

		// draw image block if it will fit on the screen
		if ((mcu_x + win_w) <= display.width() && (mcu_y + win_h) <= display.height())
		{
			// TODO
			//display.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
			tft_output(mcu_x, mcu_y, win_w, win_h, pImg);

			/*
			// open a window onto the screen to paint the pixels into
			//TFTscreen.setAddrWindow(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
			TFTscreen.setAddrWindow(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
			// push all the image block pixels to the screen
			while (mcu_pixels--)
				TFTscreen.pushColor(*pImg++); // Send to TFT 16 bits at a time
				*/
		}

		// stop drawing blocks if the bottom of the screen has been reached
		// the abort function will close the file
		else if ((mcu_y + win_h) >= display.height())
		{
			JpegDec.abort();
		}
	}

	// calculate how long it took to draw the image
	drawTime = millis() - drawTime; // Calculate the time it took

	// print the results to the serial port
	Serial.print("Total render time was    : ");
	Serial.print(drawTime);
	Serial.println(" ms");
	Serial.println("=====================================");
}

void jpegFlushFramebuffer()
{
	if (tmpFileBuffer)
	{
		tmpFileBuffer.close();

		if (!SPIFFS.exists("/tmp.jpeg"))
		{
			Serial.println("=== /tmp.jpeg missing");
		}

		// initialise the decoder to give access to image information
		int ret = JpegDec.decodeFile("/tmp.jpeg");
		Serial.print("decodeFile ret = ");
		Serial.println(ret);

		// print information about the image to the serial port
		jpegInfo();

		// render the image onto the screen at coordinate 0,0
		if (JpegDec.width > 0 && JpegDec.height) {
			renderJPEG(0, 0);
		} else {
			Serial.println("!!!! unkown jpeg format !!!!");
		}
	}
}

void on_drawPixel(uint32_t x, uint32_t y, uint32_t color)
{
	// collect all mcu blocks for current row
	uint32_t blockPageY = y - ((y / JpegDec.MCUHeight) * JpegDec.MCUHeight);
	blockDelta[(blockPageY * MAX_WIDTH) + x] = color;


	// full mcu row complete
	if (x == JpegDec.width -1 && (y +1) % JpegDec.MCUHeight == 0) {
		// MCU block sizes: 8x8, 16x8 or 16x16

		uint32_t originOffsetY = ((y / JpegDec.MCUHeight) * JpegDec.MCUHeight);
		//Serial.printf("render block: y: %d - %d \n", originOffsetY, originOffsetY + JpegDec.MCUHeight);

		for (uint16_t _y = 0; _y < JpegDec.MCUHeight; _y++)
		{
			for (uint16_t _x = 0; _x < JpegDec.width; _x++)
			{
				uint32_t originX = _x;
				uint32_t originY = originOffsetY + _y;
				uint32_t originColor = blockDelta[(_y * MAX_WIDTH) + _x];

				// beste farbe
				uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
				uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
				uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;

				uint8_t rgba[4] = {r, g, b, 0};
				ImageProcessPixel(originX, originY, rgba);
			}
		}

		// clean buffer
		memset(blockDelta, 0, sizeof(blockDelta));
	}


	pixelCount++;
	// TODO bessere lösung finden !
	// MCU größen: 8x8, 16x8 or 16x16
	if (false && pixelCount == 10240)	// 10240 / 640 = JpegDec.MCUWidth * JpegDec.MCUSPerRow
	{
		// new block
		uint32_t originOffsetY = ((y / 16) * 16);
		//Serial.println("new block");

		for (uint16_t _y = 0; _y <= BLOCK_SIZE; _y++)
		{
			for (uint16_t _x = 0; _x < MAX_WIDTH; _x++)
			{
				uint32_t originX = _x;
				uint32_t originY = originOffsetY + _y + (blockPageY * 16);
				uint8_t originColor = blockDelta[(_y * MAX_WIDTH) + _x];

				uint8_t blue = originColor & 0x001F;  // 5 bits blue
				uint8_t green = originColor & 0x07E0; // 6 bits green
				uint8_t red = originColor & 0xF800;	  // 5 bits red

				if (false && originX <= 70)
				{
					//uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
					//uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
					//uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;
					uint8_t r = red;
					uint8_t g = green;
					uint8_t b = blue;

					Serial.print("Pixel @ y: ");
					Serial.print(y);
					Serial.print(" x: ");
					Serial.print(x);

					Serial.print("  Color, R:");
					Serial.print(r);
					Serial.print(", G:");
					Serial.print(g);
					Serial.print(", B:");
					Serial.println(b);
				}

				uint8_t rgba[4] = {red, green, blue, 0};
				ImageProcessPixel(originX, originY, rgba);
			}
		}

		// clean buffer
		//memset(blockDelta, 0, sizeof(blockDelta));

		pixelCount = 0;
	}
}

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
	//Serial.printf("tft_output, x: %d, y: %d, w: %d, h: %d\n", x, y, w, h);

	if (false && y == 0)
	{
		//tft_output: x = 0 y = 0 w = 16 h = 16
		//tft_output: x = 16 y = 0 w = 16 h = 16
		//tft_output: x = 384 y = 0 w = 6 h = 16
		//tft_output: x = 0 y = 16 w = 16 h = 16
		Serial.print("tft_output: x = ");
		Serial.print(x);
		Serial.print(" y = ");
		Serial.print(y);
		Serial.print(" w = ");
		Serial.print(w);
		Serial.print(" h = ");
		Serial.println(h);
	}

	// Stop further decoding as image is running off bottom of screen
	if (y >= display.height())
	{
		Serial.println("y is out of display range!");
		return 0;
	}

	int16_t _y = y;
	for (int16_t j = 0; j < h; j++, _y++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			// debug
			/*
			if (false && _y == 10 && x + i <= 130)
			{
				uint32_t originColor = bitmap[j * w + i];

/*
geht
				uint8_t b = (originColor & 0x001F);  // 5 bits blue
				uint8_t g = (originColor & 0x07E0) >> 5; // 6 bits green
				uint8_t r = (originColor & 0xF800) >> 11;	  // 5 bits red

				Serial.print("Pixel @ y: ");
				Serial.print(_y);
				Serial.print(" x: ");
				Serial.print(x + i);

				Serial.print("  Color, R:");
				Serial.print(r * 255 / 31);
				Serial.print(", G:");
				Serial.print(g * 255 / 63);
				Serial.print(", B:");
				Serial.print(b * 255 / 31);
*
				//Serial.print(", color:");
				//Serial.println(originColor);


				// geht auch
				uint8_t r = ((((originColor >> 11) & 0x1F) * 527) + 23) >> 6;
				uint8_t g = ((((originColor >> 5) & 0x3F) * 259) + 33) >> 6;
				uint8_t b = (((originColor & 0x1F) * 527) + 23) >> 6;
				Serial.print("Pixel @ y: ");
				Serial.print(_y);
				Serial.print(" x: ");
				Serial.print(x + i);

				Serial.print("  Color, R:");
				Serial.print(r);
				Serial.print(", G:");
				Serial.print(g);
				Serial.print(", B:");
				Serial.println(b);

/*
				unsigned _r = (originColor & 0xF800) >> 8;       // rrrrr... ........ -> rrrrr000
				unsigned _g = (originColor & 0x07E0) >> 3;       // .....ggg ggg..... -> gggggg00
				unsigned _b = (originColor & 0x1F) << 3;         // ............bbbbb -> bbbbb000
				Serial.printf("r: %d, g: %d, b: %d\n", _r, _g, _b);

			}
			*/

			on_drawPixel(x + i, _y, bitmap[j * w + i]);
		}
	}

	// Return 1 to decode next block
	return 1;
}