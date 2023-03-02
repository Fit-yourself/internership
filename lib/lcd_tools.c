#include "header.h"
#include "lcd.h"
#include <math.h>
#include "fonts.h"
#define abs(x) ((x) > 0 ? (x) : -(x))

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;
long int screensize = 0;
int fbfd;

// void LCD_Draw_Num(int x, int y, int num, int color)
// {
//     int w = 16, h = 32;
//     // 每个数字占据 16x32 像素点
//     if (num < 0 || num > 9)
//     {
//         return;
//     }
//     // // 字模数据的起始地址
//         // 获取数字字模
//     const unsigned char* font = fonts[num*144];

//     // // 遍历像素点，逐个绘制
//     // for (int i = 0; i < 24; i++) {
//     //     for (int j = 0; j < 48; j++) {
//     //         // 判断该像素点是否需要绘制
//     //         if (font[j * 3 + i / 8] & (1 << (7 - i % 8))) {
//     //             // 绘制像素点
//     //             LCD_Draw_point(x + i, y + j, color);
//     //         }
//     //     }
//     // }
//     // const unsigned char *data = &fonts[num];
//     // // 绘制数字
//     // for (int i = 0; i < h; i++)
//     // {
//     //     for (int j = 0; j < w; j++)
//     //     {
//     //         // 获取像素点的值
//     //         unsigned char value = data[i * 2 + j / 8];
//     //         unsigned char bit = 1 << (7 - j % 8);
//     //         unsigned char pixel = (value & bit) ? 1 : 0;
//     //         // 绘制像素点
//     //         if (pixel)
//     //         {
//     //             LCD_Draw_point(x + j, y + i, color);
//     //         }
//     //     }
//     // }
//         // 遍历像素点，逐个绘制
//     for (int i = 0; i < 24; i++) {
//         for (int j = 0; j < 48; j++) {
//             // 判断该像素点是否需要绘制
//             if (font[j * 3 + i / 8] & (1 << (7 - i % 8))) {
//                 // 绘制像素点
//                 LCD_Draw_point(x + i, y + j, color);
//             }
//         }
//     }
// }
// void LCD_Draw_Num_64(int x, int y, int num, int color)
// {
//     if (num < 0 || num > 9) {
//         return;
//     }

//     const unsigned char *data = &AsciiLib64[num * 32];
//     const unsigned char *blank = &AsciiLib64[num * 32 + 16];

//     int w = 32, h = 64;
//     int bg_color = 0x000000;

//     for (int j = 0; j < h; j++) {
//         for (int i = 0; i < w; i++) {
//             unsigned char byte = data[(j / 8) * 4 + i / 8];
//             int bit = 1 << (7 - (i % 8));
//             if (byte & bit) {
//                 LCD_Draw_point(x + i, y + j, color);
//             }
//             else {
//                 LCD_Draw_point(x + i, y + j, bg_color);
//             }
//         }
//     }

//     for (int j = 0; j < h; j++) {
//         for (int i = 0; i < w; i++) {
//             unsigned char byte = blank[(j / 8) * 4 + i / 8];
//             int bit = 1 << (7 - (i % 8));
//             if (byte & bit) {
//                 LCD_Draw_point(x + w + i, y + j, bg_color);
//             }
//         }
//     }
// }

// void LCD_Draw_Number(int x, int y, int number, int color)
// {
//     // 每个数字占据 16x32 像素点
//     int w = 16, h = 32;
//     // 如果数字是负数，先绘制负号，并将数字转为正数
//     if (number < 0)
//     {
//         LCD_Draw_point(x, y + h / 2, color);
//         number = -number;
//         x += w;
//     }
//     // 如果数字是 0，直接绘制数字 0
//     if (number == 0)
//     {
//         LCD_Draw_Num(x, y, 0, color);
//         return;
//     }
//     // 将数字转为字符串
//     char str[32];
//     snprintf(str, sizeof(str), "%d", number);
//     // 计算字符串长度
//     int len = strlen(str);
//     // 绘制每个数字
//     for (int i = 0; i < len; i++)
//     {
//         int digit = str[i] - '0';
//         LCD_Draw_Num(x + i * w, y, digit, color);
//     }
// }


int LCD_init(void)
{
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1)
    {
        printf("Error: cannot open framebuffer device.\n");
        return -1;
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        printf("Error reading fixed information.\n");
        return -1;
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        printf("Error reading variable information.\n");
        return -1;
    }
    if (vinfo.bits_per_pixel != 32)
    {
        printf("Only support 32 bits per pixel\n");
        return -1;
    }
    // xres = 800, yres = 480, bpp = 32
    // printf("xres = %d, yres = %d, bpp = %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    // screensize = vinfo.xres * vinfo.yres * 3;
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == NULL)
    {
        printf("Error: failed to map framebuffer device to memory.\n");
        return -1;
    }
    return fbfd;
}
void LCD_exit(void)
{
    munmap(fbp, screensize);
    close(fbfd);
}


void LCD_Clear(int color)
{
    for (int y = 0; y < vinfo.yres; y++)
    {
        for (int x = 0; x < vinfo.xres; x++)
        {
            LCD_Draw_point(x, y, color);
        }
    }
}
int LCD_Draw_point(int x, int y, int color)
{
    if (x >= vinfo.xres || y >= vinfo.yres)
    {
        return -1;
    }
    long int location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;
    *((unsigned int *)(fbp + location)) = color;
    return 0;
}

void LCD_Draw_line(int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        LCD_Draw_point(x0, y0, color);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// 绘制矩形
int LCD_Draw_rect(int x1, int y1, int x2, int y2, int color)
{
    if (x1 > x2 || y1 > y2)
    {
        return -1;
    }
    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            LCD_Draw_point(x, y, color);
        }
    }
    return 0;
}

// 绘制圆形
void LCD_Draw_circle(int xc, int yc, int r, int color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (x <= y)
    {
        LCD_Draw_point(xc + x, yc + y, color);
        LCD_Draw_point(xc + x, yc - y, color);
        LCD_Draw_point(xc - x, yc + y, color);
        LCD_Draw_point(xc - x, yc - y, color);
        LCD_Draw_point(xc + y, yc + x, color);
        LCD_Draw_point(xc + y, yc - x, color);
        LCD_Draw_point(xc - y, yc + x, color);
        LCD_Draw_point(xc - y, yc - x, color);
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 绘制椭圆
void LCD_Draw_ellipse(int xc, int yc, int a, int b, int color)
{
    int x = 0, y = b;
    int a2 = a * a, b2 = b * b;
    int d = 4 * b2 - 4 * a2 * b + a2;
    while (a2 * y >= b2 * x)
    {
        LCD_Draw_point(xc + x, yc + y, color);
        LCD_Draw_point(xc + x, yc - y, color);
        LCD_Draw_point(xc - x, yc + y, color);
        LCD_Draw_point(xc - x, yc - y, color);
        if (d >= 0)
        {
            d += 4 * b2 * (1 - y) + 4 * a2 * (2 * x - 1);
            y--;
        }
        d += 4 * b2 * (2 * x + 3);
        x++;
    }
    x = a, y = 0;
    d = 4 * a2 - 4 * a * b2 + b2;
    while (b2 * x > a2 * y)
    {
        LCD_Draw_point(xc + x, yc + y, color);
        LCD_Draw_point(xc + x, yc - y, color);
        LCD_Draw_point(xc - x, yc + y, color);
        LCD_Draw_point(xc - x, yc - y, color);
        if (d >= 0)
        {
            d += 4 * a2 * (1 - x) + 4 * b2 * (2 * y - 1);
            x--;
        }
        d += 4 * a2 * (2 * y + 3);
        y++;
    }
}
#define PI 3.14
// 绘制弧线
void LCD_Draw_arc(int xc, int yc, int r, int start_angle, int end_angle, int color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (x <= y)
    {
        int angle = atan2(y, x) * 180 / PI;
        if (angle >= start_angle && angle <= end_angle)
        {
            LCD_Draw_point(xc + x, yc + y, color);
            LCD_Draw_point(xc + x, yc - y, color);
            LCD_Draw_point(xc - x, yc + y, color);
            LCD_Draw_point(xc - x, yc - y, color);
            LCD_Draw_point(xc + y, yc + x, color);
            LCD_Draw_point(xc + y, yc - x, color);
            LCD_Draw_point(xc - y, yc + x, color);
            LCD_Draw_point(xc - y, yc - x, color);
        }
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 绘制多边形
void LCD_Draw_polygon(int x[], int y[], int n, int color)
{
    for (int i = 0; i < n; i++)
    {
        int x1 = x[i];
        int y1 = y[i];
        int x2 = x[(i + 1) % n];
        int y2 = y[(i + 1) % n];
        LCD_Draw_line(x1, y1, x2, y2, color);
    }
}

// 填充圆形
void LCD_Fill_circle(int xc, int yc, int r, int color)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (x <= y)
    {
        for (int i = xc - x; i <= xc + x; i++)
        {
            LCD_Draw_point(i, yc + y, color);
            LCD_Draw_point(i, yc - y, color);
        }
        for (int i = xc - y; i <= xc + y; i++)
        {
            LCD_Draw_point(i, yc + x, color);
            LCD_Draw_point(i, yc - x, color);
        }
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 填充椭圆
void LCD_Fill_ellipse(int xc, int yc, int a, int b, int color)
{
    for (int y = -b; y <= b; y++)
    {
        for (int x = -a; x <= a; x++)
        {
            if (x * x * b * b + y * y * a * a <= a * a * b * b)
            {
                LCD_Draw_point(xc + x, yc + y, color);
            }
        }
    }
}

/***********************************************************************/
