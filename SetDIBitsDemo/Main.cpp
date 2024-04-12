#include <windows.h>
#include "Main.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

// 定义图像大小
extern const int IMAGE_WIDTH;
extern const int IMAGE_HEIGHT;

// 定义图像数据
extern const int IMAGE_SIZE; // RGBA 格式
extern BYTE *image; // 初始化为全黑

char color = 128;

void Setup() {
	;
}

void Update() {
	for (int y = 0; y < 600; y++) {
		for (int x = 0; x < 800; x++) {
			image[(y * 800 + x) * 4 + 0] = 0;  // B
			image[(y * 800 + x) * 4 + 1] = 0;  // G
			image[(y * 800 + x) * 4 + 2] = color;  // R
			image[(y * 800 + x) * 4 + 3] = 0;
		}
	}

	color++;
	if (color > 255) {
		color %= 256;
	}
}
