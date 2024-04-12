#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>
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

#pragma pack(push, 1) // ���ýṹ���ڴ����
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
	// ��BMP�ļ�
	file = fopen("./BMP/hamster.bmp", "rb");
	if (!file) {
		perror("Error opening file");
		return;
	}

	// ��ȡ�ļ�ͷ
	fread(&header, sizeof(header), 1, file);

	// ��ȡ��Ϣͷ
	fread(&info_header, sizeof(info_header), 1, file);

	// �����ڴ����ڴ洢ͼ������
	image_data = (unsigned char *)malloc(info_header.image_size);
	if (!image_data) {
		perror("Error allocating memory");
		fclose(file);
		return;
	}

	// ���ļ�ָ���ƶ���ͼ�����ݵ���ʼλ��
	fseek(file, header.offset, SEEK_SET);

	// ��ȡͼ������
	fread(image_data, info_header.image_size, 1, file);

	// �ر��ļ�
	fclose(file);

	// �ͷ��ڴ�
	//free(image_data);



	// Save BMP File Bytes into a TXT File
	FILE *file2;
	string str = "";

	// ����ͼ�����ݲ������ɫ��Ϣ
	for (row = 0; row < info_header.height; row++) {
		for (col = 0; col < info_header.width; col++) {
			// ���㵱ǰ������ͼ�����������е�����
			int index = row * info_header.width * (info_header.bit_count / 8) + col * (info_header.bit_count / 8);

			str += std::to_string((int)image_data[index + 0]);
			str += " ";
			str += std::to_string((int)image_data[index + 1]);
			str += " ";
			str += std::to_string((int)image_data[index + 2]);
			str += " ";
			str += "\n";
		}
	}

	file2 = fopen("result.txt", "w");

	// ����ļ��Ƿ�ɹ���
	if (file2 == NULL) {
		printf("�޷����ļ�.\n");
		return;
	}

	// ���ַ���д���ļ�
	fprintf(file2, "%s", str.c_str());

	// �ر��ļ�
	fclose(file2);

	printf("�ַ����ѳɹ�д�뵽 result.txt �ļ���.\n");

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

	// ����ͼ�����ݲ������ɫ��Ϣ
	for (row = 0; row < info_header.height; row++) {
		for (col = 0; col < info_header.width; col++) {
			// ���㵱ǰ������ͼ�����������е�����
			int index = row * info_header.width * (info_header.bit_count / 8) + col * (info_header.bit_count / 8);

			image[(row * 800 + col) * 4 + 0] = image_data[index + 0];
			image[(row * 800 + col) * 4 + 1] = image_data[index + 1];
			image[(row * 800 + col) * 4 + 2] = image_data[index + 2];
		}
	}
}
