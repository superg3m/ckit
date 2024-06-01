#include "../../core/ckit_types.h"

#pragma pack(push, 1)
typedef struct BmpHeader { 	// total 40 bytes
	char signature[2]; 			// 2 bytes
	u32 file_size; 				// 4 bytes
	u32 reserved;  				// 4 bytes
	u32 data_offset; 			// 4 bytes

	u32 size;					// 4 bytes
	u32 width;					// 4 bytes
	u32 height;					// 4 bytes
	u16 planes;					// 2 bytes
	u16 bits_per_pixel; 		// 2 bytes
	u32 compression;			// 4 bytes
	u32 compressed_image_size;	// 4 bytes
	u32 x_pixels_per_meter; 	// 4 bytes // horizontal resolution: Pixels/meter
	u32 y_pixels_per_meter; 	// 4 bytes // vertical resolution: Pixels/meter
	u32 colors_used;            // 4 bytes // Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.
	u32 important_colors;		// 4 bytes // Number of important colors
} BmpHeader;
#pragma pack(pop)

// THEN:
// Pixel Data | SIZE: (InfoHeader.ImageSize bytes)
