#include <Arduino.h>
#include <sys/stat.h>

typedef struct {
	uint16_t        x1;
	uint16_t        y1;
	uint16_t        x2;
	uint16_t        y2;
} dispWin_t;

dispWin_t dispWin = {
  .x1 = 0,
  .y1 = 0,
  .x2 = 640, // DEFAULT_TFT_DISPLAY_WIDTH,
  .y2 = 384, // DEFAULT_TFT_DISPLAY_HEIGHT,
};
uint8_t	  image_debug;

// @see https://github.com/Bodmer/TJpg_Decoder/blob/master/examples/SPIFFS_Jpg/SPIFFS_Jpg.ino

// @see https://github.com/loboris/ESP32_TFT_library/blob/aa21772f54a71887ec08b2e8bbaef9e304009891/components/tft/tft.c
void TFT_bmp_image(int x, int y, uint8_t scale, char *fname, uint8_t *imgbuf, int size)
{
	FILE *fhndl = NULL;
	struct stat sb;
	int i, err=0;
	int img_xsize, img_ysize, img_xstart, img_xlen, img_ystart, img_ylen;
	int img_pos, img_pix_pos, scan_lines, rd_len;
	uint8_t tmpc;
	uint16_t wtemp;
	uint32_t temp;
	int disp_xstart, disp_xend, disp_ystart, disp_yend;
	uint8_t buf[56];
	char err_buf[64];
	uint8_t *line_buf[2] = {NULL,NULL};
	uint8_t lb_idx = 0;
	uint8_t *scale_buf = NULL;
	uint8_t scale_pix;
	uint16_t co[3] = {0,0,0};			// RGB sum
	uint8_t npix;

	if (scale > 7) scale = 7;
	scale_pix = scale+1;	// scale factor ( 1~8 )

    if (fname) {
    	// * File name is given, reading image from file
    	if (stat(fname, &sb) != 0) {
			sprintf(err_buf, "opening file");
    		err = -1;
    		goto exit;
    	}
    	size = sb.st_size;
		fhndl = fopen(fname, "r");
		if (!fhndl) {
			sprintf(err_buf, "opening file");
			err = -2;
			goto exit;
		}

		i = fread(buf, 1, 54, fhndl);  // read header
    }
    else {
    	// * Reading image from buffer
    	if ((imgbuf) && (size > 54)) {
    		memcpy(buf, imgbuf, 54);
    		i = 54;
    	}
    	else i = 0;
    }

    sprintf(err_buf, "reading header");
	if (i != 54) {err = -3;	goto exit;}

	// ** Check image header and get image properties
	if ((buf[0] != 'B') || (buf[1] != 'M')) {err=-4; goto exit;} // accept only images with 'BM' id

	memcpy(&temp, buf+2, 4);				// file size
	if (temp != size) {err=-5; goto exit;}

	memcpy(&img_pos, buf+10, 4);			// start of pixel data

	memcpy(&temp, buf+14, 4);				// BMP header size
	if (temp != 40) {err=-6; goto exit;}

	memcpy(&wtemp, buf+26, 2);				// the number of color planes
	if (wtemp != 1) {err=-7; goto exit;}

	memcpy(&wtemp, buf+28, 2);				// the number of bits per pixel
	if (wtemp != 24) {err=-8; goto exit;}

	memcpy(&temp, buf+30, 4);				// the compression method being used
	if (temp != 0) {err=-9; goto exit;}

	memcpy(&img_xsize, buf+18, 4);			// the bitmap width in pixels
	memcpy(&img_ysize, buf+22, 4);			// the bitmap height in pixels


	// * scale image dimensions
/*
	img_xlen = img_xsize / scale_pix;		// image display horizontal size
	img_ylen = img_ysize / scale_pix;		// image display vertical size

	if (x == CENTER) x = ((dispWin.x2 - dispWin.x1 + 1 - img_xlen) / 2) + dispWin.x1;
	else if (x == RIGHT) x = dispWin.x2 + 1 - img_xlen;

	if (y == CENTER) y = ((dispWin.y2 - dispWin.y1 + 1 - img_ylen) / 2) + dispWin.y1;
	else if (y == BOTTOM) y = dispWin.y2 + 1 - img_ylen;
*/
	if ((x < ((dispWin.x2 + 1) * -1)) || (x > (dispWin.x2 + 1)) || (y < ((dispWin.y2 + 1) * -1)) || (y > (dispWin.y2 + 1))) {
		sprintf(err_buf, "out of display area (%d,%d", x, y);
		err = -10;
		goto exit;
	}

	// ** set display and image areas
	if (x < dispWin.x1) {
		disp_xstart = dispWin.x1;
		img_xstart = -x;	// image pixel line X offset
		img_xlen += x;
	}
	else {
		disp_xstart = x;
		img_xstart = 0;
	}
	if (y < dispWin.y1) {
		disp_ystart = dispWin.y1;
		img_ystart = -y;	// image pixel line Y offset
		img_ylen += y;
	}
	else {
		disp_ystart = y;
		img_ystart = 0;
	}
	disp_xend = disp_xstart + img_xlen - 1;
	disp_yend = disp_ystart + img_ylen - 1;
	if (disp_xend > dispWin.x2) {
		disp_xend = dispWin.x2;
		img_xlen = disp_xend - disp_xstart + 1;
	}
	if (disp_yend > dispWin.y2) {
		disp_yend = dispWin.y2;
		img_ylen = disp_yend - disp_ystart + 1;
	}

	if ((img_xlen < 8) || (img_ylen < 8) || (img_xstart >= (img_xsize-2)) || ((img_ysize - img_ystart) < 2)) {
		sprintf(err_buf, "image too small");
		err = -11;
		goto exit;
	}

	// ** Allocate memory for 2 lines of image pixels
	line_buf[0] = static_cast<uint8_t *>(heap_caps_malloc(img_xsize*3, MALLOC_CAP_DMA));
	if (line_buf[0] == NULL) {
	    sprintf(err_buf, "allocating line buffer #1");
		err=-12;
		goto exit;
	}

	line_buf[1] = static_cast<uint8_t *>(heap_caps_malloc(img_xsize*3, MALLOC_CAP_DMA));
	if (line_buf[1] == NULL) {
	    sprintf(err_buf, "allocating line buffer #2");
		err=-13;
		goto exit;
	}

	if (scale) {
		// Allocate memory for scale buffer
		rd_len = img_xlen * 3 * scale_pix;
		scale_buf = static_cast<uint8_t *>(malloc(rd_len*scale_pix));
		if (scale_buf == NULL) {
			sprintf(err_buf, "allocating scale buffer");
			err=-14;
			goto exit;
		}
	}
	else rd_len = img_xlen * 3;

	// ** ***************************************************** **
	// ** BMP images are stored in file from LAST to FIRST line **
	// ** ***************************************************** **

	/* Used variables:
		img_xsize		horizontal image size in pixels
		img_ysize		number of image lines
		img_xlen 		image display horizontal scaled size in pixels
		img_ylen		image display vertical scaled size in pixels
		img_xstart		first pixel in line to be displayed
		img_ystart		first image line to be displayed
		img_xlen		number of pixels in image line to be displayed, starting with 'img_xstart'
		img_ylen		number of lines in image to be displayed, starting with 'img_ystart'
		rd_len			length of color data which are read from image line in bytes
	 */

	// Set position in image to the first color data (beginning of the LAST line)
	img_pos += (img_ystart * (img_xsize*3));
	if (fhndl) {
		if (fseek(fhndl, img_pos, SEEK_SET) != 0) {
			sprintf(err_buf, "file seek at %d", img_pos);
			err = -15;
			goto exit;
		}
	}

	if (image_debug) printf("BMP: image size: (%d,%d) scale: %d disp size: (%d,%d) img xofs: %d img yofs: %d at: %d,%d; line buf: 2* %d scale buf: %d\r\n",
			img_xsize, img_ysize, scale_pix, img_xlen, img_ylen, img_xstart, img_ystart, disp_xstart, disp_ystart, img_xsize*3, ((scale) ? (rd_len*scale_pix) : 0));

	// * Select the display
	//disp_select();

	while ((disp_yend >= disp_ystart) && ((img_pos + (img_xsize*3)) <= size)) {
		if (img_pos > size) {
			sprintf(err_buf, "EOF reached: %d > %d", img_pos, size);
			err = -16;
			goto exit1;
		}
		if (scale == 0) {
			// Read the line of color data into color buffer
			if (fhndl) {
				i = fread(line_buf[lb_idx], 1, img_xsize*3, fhndl);  // read line from file
				if (i != (img_xsize*3)) {
					sprintf(err_buf, "file read at %d (%d<>%d)", img_pos, i, img_xsize*3);
					err = -16;
					goto exit1;
				}
			}
			else memcpy(line_buf[lb_idx], imgbuf+img_pos, img_xsize*3);

			if (img_xstart > 0)	memmove(line_buf[lb_idx], line_buf[lb_idx]+(img_xstart*3), rd_len);
			// Convert colors BGR-888 (BMP) -> RGB-888 (DISPLAY) ===
			for (i=0; i < rd_len; i += 3) {
				tmpc = line_buf[lb_idx][i+2] & 0xfc;				// save R
				line_buf[lb_idx][i+2] = line_buf[lb_idx][i] & 0xfc;	// B -> R
				line_buf[lb_idx][i] = tmpc;							// R -> B
				line_buf[lb_idx][i+1] &= 0xfc;						// G
			}
			img_pos += (img_xsize*3);
		}
		else {
			// scale image, read 'scale_pix' lines and find the average color
			for (scan_lines=0; scan_lines<scale_pix; scan_lines++) {
				if (img_pos > size) break;
				if (fhndl) {
					i = fread(line_buf[lb_idx], 1, img_xsize*3, fhndl);  // read line from file
					if (i != (img_xsize*3)) {
						sprintf(err_buf, "file read at %d (%d<>%d)", img_pos, i, img_xsize*3);
						err = -17;
						goto exit1;
					}
				}
				else memcpy(line_buf[lb_idx], imgbuf+img_pos, img_xsize*3);
				img_pos += (img_xsize*3);

				// copy only data which are displayed to scale buffer
				memcpy(scale_buf + (rd_len * scan_lines), line_buf[lb_idx]+img_xstart, rd_len);
			}

			// Populate display line buffer
			for (int n=0;n<(img_xlen*3);n += 3) {
				memset(co, 0, sizeof(co));	// initialize color sum
				npix = 0;					// initialize number of pixels in scale rectangle

				// sum all pixels in scale rectangle
				for (int sc_line=0; sc_line<scan_lines; sc_line++) {
					// Get colors position in scale buffer
					img_pix_pos = (rd_len * sc_line) + (n * scale_pix);

					for (int sc_col=0; sc_col<scale_pix; sc_col++) {
						co[0] += scale_buf[img_pix_pos];
						co[1] += scale_buf[img_pix_pos + 1];
						co[2] += scale_buf[img_pix_pos + 2];
						npix++;
					}
				}
				// Place the average in display buffer, convert BGR-888 (BMP) -> RGB-888 (DISPLAY)
				line_buf[lb_idx][n+2] = (uint8_t)(co[0] / npix);	// B
				line_buf[lb_idx][n+1] = (uint8_t)(co[1] / npix);	// G
				line_buf[lb_idx][n] = (uint8_t)(co[2] / npix);		// R
			}
		}

		//wait_trans_finish(1);
		//send_data(disp_xstart, disp_yend, disp_xend, disp_yend, img_xlen, (color_t *)line_buf[lb_idx]);
		lb_idx = (lb_idx + 1) & 1;  // change buffer

		disp_yend--;
	}
	err = 0;
exit1:
	//disp_deselect();
exit:
	if (scale_buf) free(scale_buf);
	if (line_buf[0]) free(line_buf[0]);
	if (line_buf[1]) free(line_buf[1]);
	if (fhndl) fclose(fhndl);
	if ((err) && (image_debug)) printf("Error: %d [%s]\r\n", err, err_buf);

	//return err;
}