#include "header.h"
#include "lcd.h"
#include "bitmap.h"
// 打开 LCD 设备文件
#define fd fbfd
// 映射到内存
#define pixels fbp
// char* pixels = (char*)mmap(NULL, width * height * 3, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
extern struct fb_var_screeninfo vinfo;
void show_bitmap(char *filename, int x, int y)
{
    int fb; // BMP 文件描述符
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    // 打开 BMP 文件并读取文件头和信息头
    fb = open(filename, O_RDONLY);
    if (fb == -1)
    {
        perror("open bmp file");
        close(fd);
        return;
    }
    lseek(fb, 0, SEEK_SET);
    read(fb, &file_header, sizeof(BITMAPFILEHEADER));
    read(fb, &info_header, sizeof(BITMAPINFOHEADER));

    // int width = info_header.width;
    // int height = info_header.height;
    // printf("width = %d, height = %d, offset = %d\n", width, height, file_header.offset);
    // if (pixels == MAP_FAILED)
    // {
    //     perror("mmap");
    //     close(fb);
    //     // close(fd);
    //     return;
    // }

    // // 读取像素数据到内存
    // lseek(fb, file_header.offset, SEEK_SET); // lseek(fd,54,SEEK_SET);
    // read(fb, pixels, width * height * 3); // 从54开始，数据读取直接到  pixels
    char buf[info_header.width * info_header.height * 3]; // 存储像素数据
    read(fb, buf, sizeof(buf));

    // 限制范围
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x + info_header.width > info_header.width)
        x = vinfo.width - info_header.width;
    if (y + info_header.height > info_header.height)
        y = vinfo.height - info_header.height;

    // 在指定位置绘制图像
    // char *dst = pixels + (x + y * info_header.width) * 3;
    // for (int i = 0; i < height; i++)
    // {
    //     memcpy(dst, pixels + ((height - i - 1) * info_header.width) * 3, width * 3);
    //     dst += info_header.width * 3;
    // }

    int color = 0;
    int tmp;
    for (int i = 0; i < info_header.height; i++)
    {
        for (int j = 0; j < info_header.width; j++)
        {
            tmp = i * 3;
            color = buf[tmp] | buf[tmp + 1] << 8 | buf[tmp + 2] << 16;
            LCD_Draw_point(j + x, info_header.height - 1 - i + y, color);
            // printf("%d ",color);
        }
    }
    // 关闭文件
    close(fb);
    // close(fd);
}

// int show_bitmap(int x, int y, char *file_name)
// {
//     // 打开 LCD 设备文件
//     int fd = open("/dev/fb0", O_RDWR);
//     if (fd == -1)
//     {
//         perror("open /dev/fb0");
//         return -1;
//     }
//     // 获取 BitMap 文件头信息
//     int fb = open(file_name, O_RDONLY);
//     lseek(fb, 0x12, SEEK_SET); // 定位到宽度的位置
//     int width = 0;
//     read(fb, &width, 4); // 读取4个字节

//     int height = 0;
//     read(fb, &height, 4); // 读取4个字节

//     printf("width = %d, height = %d\n", width, height);

// ////////////////////////////////////////////////////////////////
//     lseek(fb, 54, SEEK_SET);
//     char buf[width * height * 3]; // 读取所有像素数据
//     read(fb, buf, sizeof(buf));

//     int color = 0;
//     for (int i = 0; i < height; i++)
//     {
//         for (int j = 0; j < width; j++)
//         {
//             color = buf[i * 3] | buf[i * 3 + 1] << 8 | buf[i * 3 + 2] << 16;
//             LCD_Draw_point(j + x, height - 1 - i + y, color);
//             // printf("%d ",color);
//         }
//     }
//     return 0;
// }