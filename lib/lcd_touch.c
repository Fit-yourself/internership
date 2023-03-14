#include "touch.h"
#include "lcd.h"
extern Button button[4];
extern int button_color[4];

// Touch_info infos[] = {
//     {0, 0},
//     {0, 0},
//     {0, 0},
//     {0, 0},
// };

// void button_task(void)
// {

//     for (int i = 0; i < 4; i++)
//     {
//         if (info.x > button[i].x0 && info.x < button[i].x1 && info.y > button[i].y0 && info.y < button[i].y1)
//         {
//             // printf("YES x = %d, y = %d\n", info.x, info.y);
//             // printf("%s\n", button[i].info);
//             if (button[i].info == "Start")
//             {
//                 // 播放 mp3
//             }
//         }
//         // else
//         // {
//         //     // printf("NO x = %d, y = %d\n", info.x, info.y);
//         // }
//     }
// }
int Touch_init(void)
{
    int fd_touch;
    fd_touch = open("/dev/input/event0", O_RDONLY);
    if (fd_touch < 0)
    {
        perror("Failed to open input device");
        exit(1);
    }
    return fd_touch;
}

int Touch_get(int fd_touch, Touch_info *info)
{
    /* 读取输入事件 */
    struct input_event ev;
    int ret = read(fd_touch, &ev, sizeof(ev));

    /* 判断是否读取成功 */
    if (ret < 0)
    {
        return -1;
    }
    else if (ret != sizeof(ev))
    {
        fprintf(stderr, "read %d bytes, expected %lu bytes\n", ret, sizeof(ev));
        return -1;
    }

    /* 处理输入事件 */ if (ev.type == EV_ABS)
    {
        // 处理绝对移动事件
        if (ev.code == ABS_X)
        {
            info->x = ev.value * (1.0 * 800 / 1040); // 坐标映射
            printf("X_ABS: %d\n", info->x);
        }
        else if (ev.code == ABS_Y)
        {
            info->y = ev.value * (1.0 * 480 / 600); // 坐标映射
            printf("Y_ABS: %d\n", info->y);
        }
    }
    info->type = ev.type;
    // else if (ev.type == EV_KEY)
    // {
    //     // 处理按键事件
    //     if (ev.code == BTN_TOUCH && ev.value == 1) // 按下
    //     {
    //         printf("Touch key pressed\n");
            
    //     }
    //     else if (ev.code == BTN_TOUCH && ev.value == 0) // 松开
    //     {
    //         printf("Touch key release\n");
    //         info->type = ev.type;
    //     }
    // }
    return 0;
}
// void *input_thread(void *arg)
// {
//     int fd = *(int *)arg;
//     struct input_event ev;

//     while (1)
//     {
//         read(fd, &ev, sizeof(ev));
//         if (ev.type == EV_KEY)
//         {

//             // 处理按键事件
//             if (ev.code == BTN_TOUCH && ev.value == 1) // 按下
//             {
//                 // printf("Touch key pressed\n");
//                 printf("info.mutex = %d, info.is_valid = %d",info.mutex,info.is_valid);
//             }
//             else if (ev.code == BTN_TOUCH && ev.value == 0) // 松开
//             {
//                 // printf("Touch key release\n");
//             }

//         }else if (ev.type == EV_ABS)
//         {
//             if(info.mutex != 1) continue;
//             // 处理绝对移动事件
//             info.mutex = 0;
//             if (ev.code == ABS_X)
//             {
//                 printf("X_ABS: %d\n", ev.value);
//                 info.x = ev.value;
//             }
//             else if (ev.code == ABS_Y)
//             {
//                 printf("Y_ABS: %d\n", ev.value);
//                 info.y = ev.value;
//             }
//             info.is_valid = 1;
//             info.mutex = 1;
//         }
//     }
// }
/*
    get_usr_touch : 获取用户的触摸屏输入坐标
    @x ： 保存的是x轴的坐标值
    @y ： 保存的是y轴的坐标值
    返回值 ：
        无
*/
// void get_usr_touch(int *x, int *y)
// {
//     int fd = open(TOUCH_DEV_PATH, O_RDONLY);
//     if (fd == -1)
//     {
//         perror(" open /dev/input/event0 error");
//         return;
//     }

//     struct input_event ev;
//     while (1)
//     {

//         int ret = read(fd, &ev, sizeof(ev));
//         if (ret != sizeof(ev)) // 假如读出来的字节不是该结构体大小，重新读
//         {
//             continue;
//         }
//         // printf("type = %d ,code = %d ,value = %d\n",ev.type,ev.code,ev.value);

//         if (ev.type == EV_ABS && ev.code == ABS_X)
//         {
//             *x = ev.value * (1.0 * 800 / 1040);
//         }

//         if (ev.type == EV_ABS && ev.code == ABS_Y)
//         {
//             *y = ev.value * (1.0 * 480 / 600);
//         }

//         if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
//         {
//             break;
//         }
//     }

//     printf("( %d,%d )\n", *x, *y);

//     close(fd);
// }
// int main() {
//     int fd = open(DEV_PATH, O_RDONLY);
//     if (fd < 0) {
//         perror("Failed to open input device");
//         exit(1);
//     }

//     pthread_t thread_id;
//     pthread_create(&thread_id, NULL, input_thread, &fd);

//     // 主线程执行其他操作
//     while (1) {
//         // ...
//     }

//     close(fd);
//     return 0;
// }
