#include "touch.h"
extern Button button[4];
extern int button_color[4];
Touch_info info = {0, 0};

// Touch_info infos[] = {
//     {0, 0},
//     {0, 0},
//     {0, 0},
//     {0, 0},
// };

void button_task(void)
{

    for (int i = 0; i < 4; i++)
    {
        if (info.x > button[i].x0 && info.x < button[i].x1 && info.y > button[i].y0  && info.y < button[i].y1)
        {
            // printf("YES x = %d, y = %d\n", info.x, info.y);
            printf("%s\n", button[i].info);
        }
        else
        {
            // printf("NO x = %d, y = %d\n", info.x, info.y);
        }
    }
}

// 创建线程，实现触摸屏 输入事件处理
void *input_thread(void *arg)
{
    int fd = *(int *)arg;
    struct input_event ev;
    int ret;

    /* 将输入设备设置为非阻塞模式 */
    // int flags = fcntl(fd, F_GETFL);
    // fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    // 生成 按钮
    // for (int i = 0; i < 4; i++)
    // {
    //     LCD_Draw_rect(button[i].x0, button[i].y0, button[i].x1, button[i].y1, button_color[i]);
    // }
    while (1)
    {
        /* 读取输入事件 */
        ret = read(fd, &ev, sizeof(ev));

        /* 判断是否读取成功 */
        if (ret < 0)
        {
            if (errno == EAGAIN)
            {
                /* 没有输入事件，等待一段时间 */
                usleep(10000);
                continue;
            }
            else
            {
                perror("read input event");
                break;
            }
        }
        else if (ret != sizeof(ev))
        {
            fprintf(stderr, "read %d bytes, expected %lu bytes\n", ret, sizeof(ev));
            break;
        }

        /* 处理输入事件 */
        if (ev.type == EV_KEY)
        {
            // 处理按键事件
            if (ev.code == BTN_TOUCH && ev.value == 1) // 按下
            {
                // printf("Touch key pressed\n");
            }
            else if (ev.code == BTN_TOUCH && ev.value == 0) // 松开
            {
                // printf("Touch key release\n");
            }
        }
        else if (ev.type == EV_ABS)
        {
            // 处理绝对移动事件
            if (ev.code == ABS_X)
            {
                // printf("X_ABS: %d\n", ev.value);
                info.x = ev.value * (1.0 * 800 / 1040); // 坐标映射

                // printf("info.x: %d\n", info.x);
            }
            else if (ev.code == ABS_Y)
            {
                // printf("Y_ABS: %d\n", ev.value);
                info.y = ev.value * (1.0 * 480 / 600); // 坐标映射
                // printf("info.y: %d\n", info.y);
            }

            button_task();
        }
    }

    /* 恢复输入设备的阻塞模式 */
    // fcntl(fd, F_SETFL, flags);

    return NULL;
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
void get_usr_touch(int *x, int *y)
{
    int fd = open(TOUCH_DEV_PATH, O_RDONLY);
    if (fd == -1)
    {
        perror(" open /dev/input/event0 error");
        return;
    }

    struct input_event ev;
    while (1)
    {

        int ret = read(fd, &ev, sizeof(ev));
        if (ret != sizeof(ev)) // 假如读出来的字节不是该结构体大小，重新读
        {
            continue;
        }
        // printf("type = %d ,code = %d ,value = %d\n",ev.type,ev.code,ev.value);

        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            *x = ev.value * (1.0 * 800 / 1040);
        }

        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            *y = ev.value * (1.0 * 480 / 600);
        }

        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
        {
            break;
        }
    }

    printf("( %d,%d )\n", *x, *y);

    close(fd);
}
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
