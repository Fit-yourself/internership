#if !defined(_BITMAP_H)
#define _BITMAP_H
#include <stdint.h>
// 定义 Windows BMP 文件头结构
#pragma pack(push, 1) // 避免结构体成员对齐
typedef struct {
    uint16_t signature;  // BMP 文件标识 	    //0
    uint32_t size;         // BMP 文件大小      //2
    uint16_t reserved1;  // 保留字段1		    //4
    uint16_t reserved2;  // 保留字段2		    //6
    uint32_t offset;       // 位图数据偏移量    //10
} BITMAPFILEHEADER;//14
#pragma pack(pop) // 2*3+4*2 = 14 bytes

// 定义 Windows BMP 位图信息头结构
#pragma pack(push, 1) // 避免结构体成员对齐
typedef struct {
    uint32_t size;            // 信息头大小     //0	    //14
    int32_t width;            // 图像宽度       //4	    //18
    int32_t height;           // 图像高度       //8	    //22
    uint16_t planes;          // 颜色平面数     //12    //26
    uint16_t bit_count;       // 每像素位数     //14    //28
    uint32_t compression;     // 压缩方式       //16    //30
    uint32_t size_image;      // 图像数据大小   //20    //34
    int32_t x_pixels_per_meter; // 水平分辨率   //24    //38
    int32_t y_pixels_per_meter; // 垂直分辨率   //28    //42
    uint32_t colors_used;     // 使用的颜色数   //32    //46
    uint32_t colors_important; // 重要颜色数    //36    //50
} BITMAPINFOHEADER; 						   //40    //54
#pragma pack(pop) // 4*9+2*2=40  
// 14 + 40 = 54
void show_bitmap(char* filename, int x, int y);
void read_bitmap(void);

#endif // _BITMAP_H
