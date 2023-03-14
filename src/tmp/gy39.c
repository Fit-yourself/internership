#include "header.h"
#include "fonts.h"
#include "lcd.h"

// void LCD_Draw_Num(int num, int x, int y, int color) {
//   // 每个数字的宽度为 24 像素，高度为 48 像素
//   const int num_width = 24;
//   const int num_height = 48;

//   // 数字之间留出 10 像素的间隔
//   x += 10;

//   // 数字 1 对应的点阵数据在 fonts 数组的第二个元素
//   const unsigned char* num_data = fonts[1];

//   // 遍历点阵数据，画出数字 1
//   for (int j = 0; j < num_height; j++) {
//     for (int i = 0; i < num_width; i++) {
//       int bit_index = (j / 8) * num_width + i;
//       int bit_value = (num_data[bit_index] >> (j % 8)) & 1;
//       if (bit_value == 1) {
//         LCD_Draw_point(x + i, y + j, color);
//       }
//     }
//   }
// }
void LCD_Draw_Font(const unsigned char font[], int x, int y, int color) {
    int i, j;
    for (i = 0; i < 24; i++) {
        for (j = 0; j < 48; j++) {
            if (font[i * 48 + j] == 1) {
                LCD_Draw_point(x + j, y + i, color);
            }
        }
    }
}
#include "GY39.h"
int main(int argc, char const *argv[])
{
    Systerm_Init();


    int fd_gy39 = GY39_init();
    GY39_DATA data = {0};
    GY39_read(fd_gy39,&data); // 激活
    printf("wait for GY39 to start\n");
    sleep(3);
    // GY39_mod(fd_gy39, 0);
    while (1)
    {
        if (GY39_read(fd_gy39,&data) == -1) continue;
        printf("light: %.2f, temp: %.2f, press: %.4f, humi: %.2f, alti: %.2d\n", data.light, data.temp, data.press, data.humi, data.alti);
        
        // if(mode == 1) mode =2;
        // else mode = 1;
        // GY39_mod(fd_gy39, mode);
        sleep(1);
    }
    // LCD_Draw_Num(100,100,2, 0x0000ff);
    // LCD_Draw_Font(FONTS[1], 0, 0, 0x0000ff);
    return 0;
}
