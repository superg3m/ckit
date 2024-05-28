#include "../../core/ckit_types.h"

#pragma pack(push, 1)
typedef struct BmpHeader {
	char signature[2];
	u32 file_size;
	u32 reserved;
	u32 data_offset;
} BmpHeader;
#pragma pack(pop)

typedef struct BmpInfoHeader {
	u32 size;
	u32 width;
	u32 height;
	u16 planes;
	u16 bits_per_pixel;
	u32 compression;
	u32 compressed_image_size;
	u32 x_pixels_per_meter; 	// horizontal resolution: Pixels/meter
	u32 y_pixels_per_meter; 	// vertical resolution: Pixels/meter
	u32 colors_used;            // Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
	u32 important_colors;		// Number of important colors
} BmpInfoHeader;

typedef struct BmpColorTable {
	u8 red;
	u8 blue;
	u8 green;
	u8 alpha;
} BmpColorTable;
/*
Header 	14 bytes 	  	Windows Structure: BITMAPFILEHEADER
  	Signature 	2 bytes 	0000h 	'BM'
FileSize 	4 bytes 	0002h 	File size in bytes
reserved 	4 bytes 	0006h 	unused (=0)
DataOffset 	4 bytes 	000Ah 	Offset from beginning of file to the beginning of the bitmap data
InfoHeader 	40 bytes 	  	Windows Structure: BITMAPINFOHEADER
  	Size 	4 bytes 	000Eh 	Size of InfoHeader =40 
Width 	4 bytes 	0012h 	Horizontal width of bitmap in pixels
Height 	4 bytes 	0016h 	Vertical height of bitmap in pixels
Planes 	2 bytes 	001Ah 	Number of Planes (=1)
Bits Per Pixel 	2 bytes 	001Ch 	Bits per Pixel used to store palette entry information. This also identifies in an indirect way the number of possible colors. Possible values are:
1 = monochrome palette. NumColors = 1  
4 = 4bit palletized. NumColors = 16  
8 = 8bit palletized. NumColors = 256 
16 = 16bit RGB. NumColors = 65536
24 = 24bit RGB. NumColors = 16M
Compression 	4 bytes 	001Eh 	Type of Compression  
0 = BI_RGB   no compression  
1 = BI_RLE8 8bit RLE encoding  
2 = BI_RLE4 4bit RLE encoding
ImageSize 	4 bytes 	0022h 	(compressed) Size of Image 
It is valid to set this =0 if Compression = 0
XpixelsPerM 	4 bytes 	0026h 	horizontal resolution: Pixels/meter
YpixelsPerM 	4 bytes 	002Ah 	vertical resolution: Pixels/meter
Colors Used 	4 bytes 	002Eh 	Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
Important Colors 	4 bytes 	0032h 	Number of important colors 
0 = all
ColorTable 	4 * NumColors bytes 	0036h 	present only if Info.BitsPerPixel less than 8  
colors should be ordered by importance
  		Red 	1 byte 	  	Red intensity
Green 	1 byte 	  	Green intensity
Blue 	1 byte 	  	Blue intensity
reserved 	1 byte 	  	unused (=0)
repeated NumColors times
Pixel Data 	InfoHeader.ImageSize bytes 	  	The image data

*/