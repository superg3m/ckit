#include "../../Include/Parser_and_Lexer/ckit_bmp_parser.h"

CKIT_BitmapAsset ckit_graphics_load_bmp(u8* bmp_file_data, size_t file_size) {
	CKIT_BitmapAsset ret = {0};

	BmpHeader bmp_header;
	ckit_memory_copy(bmp_file_data, &bmp_header, sizeof(bmp_header), file_size);
	ret.bytes_per_pixel = (bmp_header.bits_per_pixel / 8);
	size_t bitmap_size = (bmp_header.width * bmp_header.height) * ret.bytes_per_pixel;
	u8* bmp_data = bmp_file_data + bmp_header.data_offset;

	size_t bmp_size = file_size - sizeof(BmpHeader);
	ret.width = bmp_header.width;
	ret.height = bmp_header.height;
	ret.memory = bmp_data;
	
	return ret;
}