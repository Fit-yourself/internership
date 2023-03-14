#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "mode.h"
#include <stdlib.h>
#include "lcd.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int fb;
void *plcd;

void LCD_Init()
{
	fb = open("/dev/fb0", O_RDWR);
	if (fb == -1)
	{
		perror("open /dev/fb0 failed");
		return;
	}
	plcd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (plcd == MAP_FAILED)
	{
		perror("mmap failed");
		return;
	}
}
void LCD_DeInit()
{
	munmap(plcd, 800 * 480 * 4);
	close(fb);
}
void LCD_Clear(int color)
{
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 800; x++)
		{
			LCD_DrawPoint(x, y, color);
		}
	}
}
void LCD_DrawPoint(int x, int y, int color)
{
	int *p = plcd;
	if (x >= 0 && x <= 799 && y >= 0 && y <= 479)
	{
		*(p + 800 * y + x) = color;
	}
}
void LCD_DrawRect(int x, int y, int w, int h, int color)
{
	int i, j;
	for (i = x; i <= x + w - 1; i++)
	{
		for (j = y; j <= y + h - 1; j++)
		{
			LCD_DrawPoint(i, j, color);
		}
	}
}

void LCD_DrawCircle(int x, int y, int r, int color)
{
	int i, j;
	for (i = x - r; i <= x + r; i++)
	{
		for (j = y - r; j <= y + r; j++)
		{
			if ((x - i) * (x - i) + (y - j) * (y - j) <= r * r)
			{
				LCD_DrawPoint(i, j, color);
			}
		}
	}
}

void LCD_DrawEllipse(int x, int y, int a, int b, int color)
{
	int i, j;
	for (i = x - a; i <= x + a; i++)
	{
		for (j = y - b; j <= y + b; j++)
		{
			if (b * b * (i - x) * (i - x) + a * a * (j - y) * (j - y) <= a * a * b * b)
			{
				LCD_DrawPoint(i, j, color);
			}
		}
	}
}

void LCD_DrawWord(int x0, int y0, int w, int h, char *data, int color)
{
	int i, k;

	for (i = 0; i < w * h / 8; i++)
	{
		for (k = 0; k < 8; k++)
		{
			if ((data[i] << k) & 0x80)
			{
				LCD_DrawPoint(x0 + (i * 8 + k) % w, y0 + i / (w / 8), color);
			}
		}
	}
}

void LCD_DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
	int x_min = MIN(x1, MIN(x2, x3));
	int y_min = MIN(y1, MIN(y2, y3));
	int x_max = MAX(x1, MAX(x2, x3));
	int y_max = MAX(y1, MAX(y2, y3));

	int s_sum = abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	int i, j;
	for (i = x_min; i <= x_max; i++)
	{
		for (j = y_min; j <= y_max; j++)
		{
			int s1 = abs(i * (y2 - y3) + x2 * (y3 - j) + x3 * (j - y2));
			int s2 = abs(x1 * (j - y3) + i * (y3 - y1) + x3 * (y1 - j));
			int s3 = abs(x1 * (y2 - j) + x2 * (j - y1) + i * (y1 - y2));
			if (s1 + s2 + s3 == s_sum)
			{
				LCD_DrawPoint(i, j, color);
			}
		}
	}
}

void LCD_DrawBmp(const char *bmpname, int x, int y, int flag)
{
	int fd = open(bmpname, O_RDONLY);
	if (fd == -1)
	{
		perror("open bmp failed!");
		return;
	}
	unsigned char buf[2] = {0};
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, 2);
	if (buf[0] != 0x42 || buf[1] != 0x4D)
	{
		printf("the picture is not bmp!\n");
		return;
	}
	int w = 0;
	lseek(fd, 0x12, SEEK_SET);
	read(fd, &w, 4);
	int h = 0;
	lseek(fd, 0x16, SEEK_SET);
	read(fd, &h, 4);
	int color_d = 0;
	lseek(fd, 0x1c, SEEK_SET);
	read(fd, &color_d, 2);
	// printf("bmp:%d*%d*%d\n", w, h, color_d);
	lseek(fd, 54, SEEK_SET);
	int i, j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			int color = 0;
			if (color_d == 24)
			{
				read(fd, &color, 3);
			}
			else if (color_d == 32)
			{
				read(fd, &color, 4);
			}
			if (flag == 1)
			{
				if (color != 0xFF) // 颜色不是蓝色
				{
					LCD_DrawPoint(x + j, y + (h > 0 ? (h - 1 - i) : i), color);
				}
			}
			else
			{
				LCD_DrawPoint(x + j, y + (h > 0 ? (h - 1 - i) : i), color);
			}
		}
		lseek(fd, (4 - (w * color_d / 8) % 4) % 4, SEEK_CUR);
	}
	close(fd);
}

void LCD_DrawNum(int num, int x, int y, int color)
{
	int n[10], i = 0, k = 0;
	do
	{
		n[i] = num % 10;
		i++;
	} while (num /= 10);

	while (i--)
	{
		LCD_DrawWord(x + k * 24 + 3, y, 24, 53, num_mode[n[i]], color);
		k++;
	}
}

void LCD_name(int x, int y, int z, int color)
{
	int i;
	for (i = 0; i < z; i++)
	{
		LCD_DrawWord(x + 48 * i, y, 48, 48, word_mode[i], color);
	}
}

void LCD_yanwu()
{
	int x;
	for (x = 0; x < 5; x++)
	{
		LCD_DrawWord(100 + x * 48, 50, 48, 48, nongdu[x], 0xff0000);
	}
}
