#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include "Main.h"

using namespace std;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int IMAGE_WIDTH;
extern const int IMAGE_HEIGHT;
extern const int IMAGE_SIZE;
extern BYTE *image;

/*
** BMP Header & InfoHeader
*/

#pragma pack(push, 1) // 禁用结构体内存对齐
typedef struct {
	char     type[2];
	unsigned int  size;
	unsigned short int  reserved1, reserved2;
	unsigned int  offset;
} BMPHeader;

typedef struct {
	unsigned int  size;
	unsigned int  width, height;
	unsigned short int  planes;
	unsigned short int  bit_count;
	unsigned int  compression;
	unsigned int  image_size;
	unsigned int  x_pixels_per_meter;
	unsigned int  y_pixels_per_meter;
	unsigned int  colors_used;
	unsigned int  colors_important;
} BMPInfoHeader;
#pragma pack(pop)

char color = 128;

FILE *file;
BMPHeader header;
BMPInfoHeader info_header;
unsigned char *image_data = NULL;
int row, col;

void Setup() {
	// 打开BMP文件
	file = fopen("./BMP/hamster.bmp", "rb");
	if (!file) {
		perror("Error opening file");
		return;
	}

	// 读取文件头
	fread(&header, sizeof(header), 1, file);

	// 读取信息头
	fread(&info_header, sizeof(info_header), 1, file);

	// 分配内存用于存储图像数据
	image_data = (unsigned char *)malloc(info_header.image_size);
	if (!image_data) {
		perror("Error allocating memory");
		fclose(file);
		return;
	}

	// 将文件指针移动到图像数据的起始位置
	fseek(file, header.offset, SEEK_SET);

	// 读取图像数据
	fread(image_data, info_header.image_size, 1, file);

	// 关闭文件
	fclose(file);

	//// 遍历图像数据并输出颜色信息
	//for (row = 0; row < info_header.height; row++) {
	//	for (col = 0; col < info_header.width; col++) {
	//		// 计算当前像素在图像数据数组中的索引
	//		int index = row * info_header.width * (info_header.bit_count / 8) + col * (info_header.bit_count / 8);

	//		// 输出当前像素的颜色信息
	//		printf("Pixel at (%d, %d): ", col, row);
	//		for (int i = 0; i < (info_header.bit_count / 8); i++) {
	//			printf("%02X ", image_data[index + i]);
	//		}
	//		printf("\n");
	//	}
	//}

	// 释放内存
	free(image_data);
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

	// 遍历图像数据并输出颜色信息
	for (row = 0; row < info_header.height; row++) {
		for (col = 0; col < info_header.width; col++) {
			// 计算当前像素在图像数据数组中的索引
			int index = row * info_header.width * (info_header.bit_count / 8) + col * (info_header.bit_count / 8);

			image[(row * 800 + col) * 4 + 0] = image_data[index + 0];
			image[(row * 800 + col) * 4 + 1] = image_data[index + 1];
			image[(row * 800 + col) * 4 + 2] = image_data[index + 2];
			//image[(row * 800 + col) * 4 + 3] = image_data[index + 3];

			//// 输出当前像素的颜色信息
			//printf("Pixel at (%d, %d): ", col, row);
			//for (int i = 0; i < (info_header.bit_count / 8); i++) {
			//	printf("%02X ", image_data[index + i]);
			//}
			//printf("\n");
		}
	}
}
