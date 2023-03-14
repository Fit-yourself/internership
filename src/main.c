#include "header.h"
#include "bitmap.h"
#include "lcd.h"
#include "touch.h"
#include <pthread.h>
#include "thread.h"

#include "mode.h"
// void Welcome_layout(void)
// {

//     LCD_DrawBmp(atmos_BMP, 0, 0, 0);
//     LCD_DrawBmp(hum_BMP, 0, 0, 0);
//     LCD_DrawBmp(light_off_BMP, 0, 0, 0);
//     LCD_DrawBmp(music_BMP, 0, 0, 0);
//     LCD_DrawBmp(pause_BMP, 0, 0, 0);
//     LCD_DrawBmp(play_BMP, 0, 0, 0);
//     LCD_DrawBmp(temp_BMP, 0, 0, 0);
// }
extern void thread_2048(void);
int main(int argc, char const *argv[])
{
    LCD_Init();
    // Welcome_layout(); LCD_DrawBmp(temp_BMP, 0, 0, 0); // OK 
    
    // main_thread();
    thread_2048();

    for (;;)
    {
            
    }
    return 0;
}

    // printf("LCD init success!\n");

    // if (argc == 2)
    // {
    //     /* 显示开启图面 */
    //     char img_path[64] = {0};
    //     strcpy(img_path, argv[1]);
    //     LCD_DrawBmp(img_path, 0, 0, 0);
    // }