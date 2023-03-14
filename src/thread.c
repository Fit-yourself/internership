#include "header.h"
#include "thread.h"
#include "bitmap.h"
#include "lcd.h"
#include "touch.h"
#define MESSAGE_MAX_LEN 100
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// 全局变量，存储发送的消息
// char message[MESSAGE_MAX_LEN];
GY39_DATA GY39_data = {0};
int nd = 0; // 烟雾浓度数据
// 全局变量，表示是否有消息可读
int gy39_message_ready = 0;
int mq2_message_ready = 0;

// 互斥量和条件变量，用于线程同步
pthread_mutex_t gy39_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gy39_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mq2_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mq2_cond = PTHREAD_COND_INITIALIZER;
#include "touch.h"

void timer_handler(int signum)
{
    printf("Timer expired.\n");
}
#include "mode.h"


int main_thread()
{
    pthread_t thread_id_touch; // 触摸屏线程
    pthread_t thread_id_gy39;  //
    pthread_t thread_id_mq2;   //

    int fd_touch = Touch_init();
    int fd_mq2 = MQ2_init(); // smoke
    int fd_gy39 = GY39_init();

    LCD_Clear(0xffffff);

    LCD_DrawBmp(music_BMP, x_bb_BMP, y_bb_BMP, 1);
    LCD_DrawBmp(pause_BMP, x_play_BMP, y_play_BMP, 1);

    for (int x = 0; x < 4; x++) // 烟雾
    {
        LCD_DrawWord((x_smoke - 200) + x * 48, y_smoke, 48, 48, nongdu[x], 0xff0000);
    }
    LCD_DrawBmp(temp_BMP, x_temp_BMP, y_temp_BMP, 0);
    LCD_DrawBmp(hum_BMP, x_humi_BMP, y_humi_BMP, 0);
    LCD_DrawBmp(atmos_BMP, x_press_BMP, y_press_BMP, 0);

    pthread_create(&thread_id_touch, NULL, input_thread, &fd_touch);
    pthread_create(&thread_id_mq2, NULL, mq2_thread, &fd_mq2);
    pthread_create(&thread_id_gy39, NULL, gy39_thread, &fd_gy39);

    return 0;
}
// 创建线程，实现触摸屏 输入事件处理
void *input_thread(void *arg)
{
    int fd = *(int *)arg;
    Touch_info info = {0, 0};

    while (1)
    {
        if (Touch_get(fd, &info) != 0)
            continue;
        buttons_method(&info); // 检查按键按下
    }
    return NULL;
}

void *gy39_thread(void *arg)
{
    int fd_gy39 = *(int *)arg;
    int time = 0; // 初始化计数器
    int mode = 1;
    GY39_mod(fd_gy39, mode); // 设置模式
    GY39_read(fd_gy39, &GY39_data); // 激活
    printf("wait for GY39 to start\n");
    sleep(1);
    int light_count = 0; // 初始化光照设置次数
    for (;;)
    {
        // 发送消息
        // sleep(1);
        if (GY39_read(fd_gy39, &GY39_data) != 0)
            continue;

        if (mode == 1)
        {
            if (light_count++ > 1500)
            {
                GY39_mod(fd_gy39, 2); // 设置光照
                light_count = 0;
                mode = 2;
            }
            GY39_layout_display(&GY39_data);
        }
        else
        {
            if (light_count++ > 1000)
            {
                GY39_mod(fd_gy39, 1); // 设置温度
                light_count = 0;
                mode = 1;
            }
            // printf("light: %lf\n", GY39_data.light);
            if (GY39_data.light > 50)
            {
                LCD_DrawBmp(light_on_BMP, x_ligh_BMP, y_ligh_BMP, 0);
            }
            else
            {
                LCD_DrawBmp(light_off_BMP, x_ligh_BMP, y_ligh_BMP, 0);
            }
        }
    }
}
void *mq2_thread(void *arg)
{
    int fd_smoke = *(int *)arg;
    char w_buf[9] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    char r_buf[9] = {0};

    printf("wait for MQ2 to start\n");
    sleep(1);
    for (;;)
    {
        if (write(fd_smoke, w_buf, 9) != 9)
            continue;
        usleep(100);
        int r = read(fd_smoke, r_buf, 9);
        if (r != 9)
            continue;
        // pthread_mutex_lock(&mq2_mutex);
        if (r_buf[0] == 0xff && r_buf[1] == 0x86)
        {
            nd = r_buf[2] << 8 | r_buf[3];
        }
        MQ2_layout_display(&nd);
    }
}

#include "lcd.h"

void LCD_CleanWord(int x, int y, int width, int height)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            LCD_DrawPoint(i, j, 0xffffff);
        }
    }
}

void MQ2_layout_display(int *nd)
{
    LCD_CleanWord(x_smoke, y_smoke, 50, 50);
    LCD_DrawNum(*nd, x_smoke, y_smoke, 0xff0000);
}

void GY39_layout_display(GY39_DATA *data)
{
    // Temp
    LCD_CleanWord(x_temp, y_temp, 120, 50);
    // LCD_DrawString(x_, y_temp, "Temperature:");
    LCD_DrawNum((int)data->temp, x_temp, y_temp, 0xff0000);

    // Humidity
    LCD_CleanWord(x_humi, y_humi, 100, 50);
    // LCD_DrawString(x_, y_humi, "Humidity:");
    LCD_DrawNum((int)data->humi, x_humi, y_humi, 0xff0000);

    // Pressure
    LCD_CleanWord(x_press, y_press, 150, 50);
    // LCD_DrawString(x_, y_press, "Pressure:");
    LCD_DrawNum((int)data->press, x_press, y_press, 0xff0000);

    // LCD_CleanWord(x_, y_alti, 100, 50);
    // // LCD_DrawString(x_, y_alti, "Altitude:");
    // LCD_DrawNum((int)data->alti, x_ + 100, y_alti, 0xff0000);

    // LCD_CleanWord(x_, y_light, 100, 100);
    // // LCD_DrawString(x_, y_light, "Light:");
    // LCD_DrawNum((int)data->light, x_ + 60, y_light, 0xff0000);
}
// #define START_Music "START"
// #define STOP_Music "STOP"
#include <stdbool.h>
#define MP3_FILE "mm.mp3"
#include "header.h" 
#define START 1 // 暂停/播放 切换
#define STOP 0
#define Continue 2
#define BUTTONS_NUM 1 // 按钮数量
Button button[BUTTONS_NUM] = {
    {x_play_BMP, y_play_BMP, x_play_BMP + 65, y_play_BMP + 65, START}, // 音乐播放按键坐标 为一个控件
};
// TODO
#define WINDOW_SIZE 5 // 滑动窗口大小
#define THRESHOLD 2   // 阈值，用于判断是否触发按钮事件

typedef struct {
    int x0;
    int x1;
    int y0;
    int y1;
    int status;
} Button;

void buttons_method(Touch_info *info)
{
    static int button_status[BUTTONS_NUM] = {0};
    static int debounce_count[BUTTONS_NUM] = {0};
    static int button_history[BUTTONS_NUM][WINDOW_SIZE] = {{0}};
    static int button_history_index[BUTTONS_NUM] = {0};

    for (int i = 0; i < BUTTONS_NUM; i++)
    {
        if (info->x > button[i].x0 && info->x < button[i].x1 && info->y > button[i].y0 && info->y < button[i].y1)
        {
            button_history[i][button_history_index[i]] = 1;
        }
        else
        {
            button_history[i][button_history_index[i]] = 0;
        }

        button_history_index[i] = (button_history_index[i] + 1) % WINDOW_SIZE;

        int sum = 0;
        for (int j = 0; j < WINDOW_SIZE; j++)
        {
            sum += button_history[i][j];
        }

        if (sum >= THRESHOLD && debounce_count[i] == 0)
        {
            switch (button[i].status)
            {
            case START:
                button[i].status = STOP;
                LCD_DrawBmp(play_BMP, x_play_BMP, y_play_BMP, 0);
                break;
            case STOP:
                button[i].status = Continue;
                // system("killall -STOP madplay");
                LCD_DrawBmp(pause_BMP, x_play_BMP, y_play_BMP, 0);
                break;
            case Continue:
                // system("killall -CONT madplay");
                button[i].status = STOP;
                LCD_DrawBmp(play_BMP, x_play_BMP, y_play_BMP, 0);
                break;
            default:
                break;
            }

            debounce_count[i] = WINDOW_SIZE; // 设置 debounce 计数器
        }

        if (debounce_count[i] > 0)
        {
            debounce_count[i]--;
        }
    }
}