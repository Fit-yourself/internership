#include "touch.h"
touch_info info = {1,1,0,0};

void button_task(void)
{
    if (info.x > 0 && info.x < 200 && info.y > 0 && info.y < 200)
    {
        printf("YES x = %d, y = %d\n", info.x, info.y);
    }
    else
    {
        // printf("NO x = %d, y = %d\n", info.x, info.y);
    }
}

// 创建线程，实现触摸屏 输入事件处理
void *input_thread(void *arg)
{
    int fd = *(int *)arg;
    struct input_event ev;
    int ret;

    /* 将输入设备设置为非阻塞模式 */
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

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

            touch_task();
            // info.mutex = 1;
            // info.is_valid = 1;
            // printf("info.mutex = %d, info.is_valid = %d\n", info.mutex, info.is_valid);
        }
    }

    /* 恢复输入设备的阻塞模式 */
    fcntl(fd, F_SETFL, flags);

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
