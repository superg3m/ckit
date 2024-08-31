#include "../../ckit.h"

#if defined(PLATFORM_WINDOWS)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		int stride = window->bitmap.width * window->bitmap.bytes_per_pixel;

		u8* row = window->bitmap.memory;    
		for(u32 y = 0; y < window->bitmap.height; y++)
		{
			u32* pixel = (u32*)row;
			for(u32 x = 0; x < window->bitmap.width; x++)
			{
				const u32 red = ((100 + x + x_offset) << 16);
				const u32 green = (10 << 8);
				const u32 blue = ((100 + y + y_offset) << 0);
				
				const u32 rgb = red|green|blue;

				*pixel++ = rgb;
			}
			row += stride;
		}
	}

	void DrawAlphaBlend (HWND hWnd, HDC hdcwnd) {
		HDC hdc;               // handle of the DC we will create  
		BLENDFUNCTION bf;      // structure for alpha blending 
		HBITMAP hbitmap;       // bitmap handle 
		BITMAPINFO bmi;        // bitmap header 
		VOID *pvBits;          // pointer to DIB section 
		ULONG   ulWindowWidth, ulWindowHeight;      // window width/height 
		ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height 
		RECT    rt;            // used for getting window dimensions 
		UINT32   x,y;          // stepping variables 
		UCHAR ubAlpha;         // used for doing transparent gradient 
		UCHAR ubRed;        
		UCHAR ubGreen;
		UCHAR ubBlue;
		float fAlphaFactor;    // used to do premultiply 
				
		// get window dimensions 
		GetClientRect(hWnd, &rt);
		
		// calculate window width/height 
		ulWindowWidth = rt.right - rt.left;  
		ulWindowHeight = rt.bottom - rt.top;  

		// make sure we have at least some window size 
		if ((!ulWindowWidth) || (!ulWindowHeight))
			return;

		// divide the window into 3 horizontal areas 
		ulWindowHeight = ulWindowHeight / 3;

		// create a DC for our bitmap -- the source DC for AlphaBlend  
		hdc = CreateCompatibleDC(hdcwnd);
		
		// zero the memory for the bitmap info 
		ZeroMemory(&bmi, sizeof(BITMAPINFO));

		// setup bitmap info  
		// set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth - (ulWindowWidth/5)*2;
		bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight/5)*2;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

		// create our DIB section and select the bitmap into the dc 
		hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
		SelectObject(hdc, hbitmap);

		// in top window area, constant alpha = 50%, but no source alpha 
		// the color format for each pixel is 0xaarrggbb  
		// set all pixels to blue and set source alpha to zero 
		for (y = 0; y < ulBitmapHeight; y++)
			for (x = 0; x < ulBitmapWidth; x++)
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x000000ff; 

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency 
		bf.AlphaFormat = 0;             // ignore source alpha channel 

		if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5, 
						ulBitmapWidth, ulBitmapHeight, 
						hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
			return;                     // alpha blend failed 
		
		// in middle window area, constant alpha = 100% (disabled), source  
		// alpha is 0 in middle of bitmap and opaque in rest of bitmap  
		for (y = 0; y < ulBitmapHeight; y++)
			for (x = 0; x < ulBitmapWidth; x++)
				if ((x > (int)(ulBitmapWidth/5)) && (x < (ulBitmapWidth-ulBitmapWidth/5)) &&
					(y > (int)(ulBitmapHeight/5)) && (y < (ulBitmapHeight-ulBitmapHeight/5)))
					//in middle of bitmap: source alpha = 0 (transparent). 
					// This means multiply each color component by 0x00. 
					// Thus, after AlphaBlend, we have a, 0x00 * r,  
					// 0x00 * g,and 0x00 * b (which is 0x00000000) 
					// for now, set all pixels to red 
					((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x00ff0000; 
				else
					// in the rest of bitmap, source alpha = 0xff (opaque)  
					// and set all pixels to blue  
					((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0xff0000ff; 

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha  
		bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha) 
	
		if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5+ulWindowHeight, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
			return;

		// bottom window area, use constant alpha = 75% and a changing 
		// source alpha. Create a gradient effect using source alpha, and  
		// then fade it even more with constant alpha 
		ubRed = 0x00;
		ubGreen = 0x00;
		ubBlue = 0xff;
		
		for (y = 0; y < ulBitmapHeight; y++)
			for (x = 0; x < ulBitmapWidth; x++)
			{
				// for a simple gradient, base the alpha value on the x  
				// value of the pixel  
				ubAlpha = (UCHAR)((float)x / (float)ulBitmapWidth * 255);
				//calculate the factor by which we multiply each component 
				fAlphaFactor = (float)ubAlpha / (float)0xff; 
				// multiply each pixel by fAlphaFactor, so each component  
				// is less than or equal to the alpha value. 
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] 
					= (ubAlpha << 24) |                       //0xaa000000 
					((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000 
					((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00 
					((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb 
			}

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha  
		bf.SourceConstantAlpha = 0xbf;   // use constant alpha, with  
										// 75% opaqueness 

		AlphaBlend(hdcwnd, ulWindowWidth/5, 
				ulWindowHeight/5+2*ulWindowHeight, ulBitmapWidth, 
				ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, 
				ulBitmapHeight, bf);

		// do cleanup 
		DeleteObject(hbitmap);
		DeleteDC(hdc);
		
	}
#elif defined(PLATFORM_LINUX)
	void set_bitmap_gradient(CKIT_Window* window, u32 x_offset, u32 y_offset) {
		
	}
#endif

int main() {
	ckit_init();

	u32 width = 800;
	u32 height = 800;
	u32 width_padding = 18;
	u32 height_padding = 42;
	u32 width_with_padding = width - width_padding;
	u32 height_with_padding = height - height_padding;

	LOG_FATAL("TESTING\n");
	LOG_ERROR("TESTING\n");
	LOG_WARN("TESTING\n");
	LOG_DEBUG("TESTING\n");
	LOG_SUCCESS("TESTING\n");

	ckit_window_bind_icon("../../../assets/c_original_logo_icon_146611.ico");
	CKIT_Window* window = ckit_window_create(width, height, "GameEngine");

	float x_pos = 40;
	float y_pos = 40;

	float x_velocity = 1.0;
	float y_velocity = 1.0;

	u32 player_width = 40;
	u32 player_height = 40;
	u32 half_player_width = player_width / 2;
	u32 half_player_height = player_height / 2;

	u32 center_width = 10;
	u32 center_height = 10;
	u32 half_center_width = center_width / 2;
	u32 half_center_height = center_height / 2;

	u32 border_size = 5; 
	u32 close_factor = 0;

	size_t file_size = 0;
	u8* ckit_sword_bmp_data = ckit_os_read_entire_file("../../../assets/Learn_About_BMP.bmp", &file_size);
	CKIT_Bitmap sword_bitmap = ckit_graphics_load_bmp(ckit_sword_bmp_data, file_size);
	
	int mouse_x = 0;
	int mouse_y = 0;
	while (!ckit_window_should_quit(window)) {
		// set_bitmap_gradient(window, x_offset, y_offset);

		{ // UPDATE
			s32 left_threshold   = 0 + close_factor;
			s32 right_threshold  = width_with_padding - close_factor;
			s32 top_threshold    =  0 + close_factor;
			s32 bottom_threshold = height_with_padding - close_factor;

			Boolean left_check   = x_pos < left_threshold - 1;
			Boolean right_check  = (x_pos + player_width) > right_threshold + 1;
			Boolean top_check    = (y_pos) < top_threshold - 1;
			Boolean bottom_check = (y_pos + player_height) > bottom_threshold + 1;


			if (left_check || right_check) {
				x_velocity *= -1;
				close_factor += 2;
				x_pos += x_velocity * 5;
			}

			if (bottom_check || top_check) {
				y_velocity *= -1;
				close_factor += 2;
				y_pos += y_velocity * 5;
			}

			x_pos += x_velocity;
			y_pos += y_velocity;

			ckit_window_get_mouse_position(window, &mouse_x, &mouse_y);
		}


		{ // RENDER
			ckit_window_clear_color(window, (CKIT_Color){55, 55, 55, 255});

			// ckit_window_draw_bitmap(window, 10, 0, sword_bitmap);

			ckit_window_draw_quad_custom(window, 0 + close_factor, 0 + close_factor, border_size, height_with_padding - (close_factor * 2), CKIT_COLOR_GREEN); // left
			ckit_window_draw_quad_custom(window, 0 + close_factor, 0 + close_factor, width_with_padding - (close_factor * 2), border_size, CKIT_COLOR_PURPLE); // top
			ckit_window_draw_quad_custom(window, 0 + close_factor, height_with_padding - close_factor, width_with_padding - (close_factor * 2), border_size, CKIT_COLOR_RED); // bottom
			ckit_window_draw_quad_custom(window, width_with_padding - close_factor, 0 + close_factor, border_size, height_with_padding - (close_factor * 2), CKIT_COLOR_BLUE); // right

			float offset_to_center_x = ((float)x_pos + (half_player_width)) - half_center_width;
			float offset_to_center_y = ((float)y_pos + (half_player_height)) - half_center_height;

			ckit_window_draw_quad_custom(window, (s32)width / 4, (s32)height / 4, 400, 200, ((CKIT_Color){20, 20, 20, 255}));
			ckit_window_draw_quad_custom(window, (s32)x_pos, (s32)y_pos, player_width, player_height, ((CKIT_Color){0, 255, 0, 10}));

			//ckit_window_draw_quad_custom(window, (s32)mouse_x, (s32)mouse_y, player_width, player_height, ((CKIT_Color){0, 255, 0, 10}));

			ckit_window_draw_quad_custom(window, (s32)offset_to_center_x, (s32)offset_to_center_y, center_width, center_height, CKIT_COLOR_PURPLE);


			ckit_window_swap_buffers(window);
		}
	}

	ckit_window_free(window); // hmm how can I make this safer?
	ckit_free(ckit_sword_bmp_data);

	ckit_cleanup();
	return 0;
}