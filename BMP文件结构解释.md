BMP（Bitmap）文件是一种常见的图像文件格式，它使用位图来存储图像数据。下面是一个简要的BMP文件结构解释：

1. 文件头（File Header）：文件头长度为14个字节，包含文件类型、文件大小等信息。
   - 文件类型（2字节）：指示文件类型，通常为"BM"。
   - 文件大小（4字节）：指示整个BMP文件的大小。
   - 保留字段（4字节）：保留字段，通常为0。
   - 图像数据偏移量（4字节）：指示图像数据相对于文件起始位置的偏移量。
2. 信息头（Bitmap Information Header）：信息头长度可变，一般为40字节，包含图像的宽度、高度、位深度等信息。
   - 信息头长度（4字节）：指示信息头的长度。
   - 图像宽度（4字节）：图像的宽度，以像素为单位。
   - 图像高度（4字节）：图像的高度，以像素为单位。
   - 色彩平面数（2字节）：表示目标设备的颜色平面数，通常为1。
   - 每像素位数（2字节）：表示每个像素所占位数，通常为1、4、8、16、24或32。
   - 压缩方式（4字节）：表示图像的压缩方式，通常为0表示不压缩。
   - 图像数据大小（4字节）：表示图像数据的大小，以字节为单位。
   - 水平分辨率（4字节）：表示目标设备水平分辨率，单位为像素每米。
   - 垂直分辨率（4字节）：表示目标设备垂直分辨率，单位为像素每米。
   - 使用的颜色数（4字节）：表示实际使用的颜色数，如果为0，则表示使用所有颜色。
   - 重要的颜色数（4字节）：表示重要的颜色数，如果为0，则表示都重要。
3. 调色板（Palette）：如果位深度小于等于8位，则会有调色板数据。调色板包含了图像中实际使用的颜色信息。
4. 图像数据（Image Data）：图像数据部分存储了实际的像素信息，每个像素占据一定的空间，根据位深度来确定。

接下来是一个使用纯C语言读取BMP文件的简单示例代码：

```c
#include <stdio.h>
#include <stdlib.h>

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

int main() {
    FILE *file;
    BMPHeader header;
    BMPInfoHeader info_header;
    unsigned char *image_data = NULL;
    int row, col;

    // 打开BMP文件
    file = fopen("example.bmp", "rb");
    if (!file) {
        perror("Error opening file");
        return -1;
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
        return -1;
    }
    
    // 将文件指针移动到图像数据的起始位置
    fseek(file, header.offset, SEEK_SET);
    
    // 读取图像数据
    fread(image_data, info_header.image_size, 1, file);
    
    // 关闭文件
    fclose(file);
    
    // 遍历图像数据并输出颜色信息
    for (row = 0; row < info_header.height; row++) {
        for (col = 0; col < info_header.width; col++) {
            // 计算当前像素在图像数据数组中的索引
            int index = row * info_header.width * (info_header.bit_count / 8) + col * (info_header.bit_count / 8);
            
            // 输出当前像素的颜色信息
            printf("Pixel at (%d, %d): ", col, row);
            for (int i = 0; i < (info_header.bit_count / 8); i++) {
                printf("%02X ", image_data[index + i]);
            }
            printf("\n");
        }
    }
    
    // 释放内存
    free(image_data);
    
    return 0;
}

```

