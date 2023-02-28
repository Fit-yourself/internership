#include "header.h"
#include "bitmap.h"
#include "lcd.h"
#include "touch.h"
#include <pthread.h>
// #include <signal.h>


int fd_touch;


int main(int argc, char const *argv[])
{
    pthread_t thread_id_touch;

    if (argc == 2)
    {
        // printf("Input Error!\tusage: %s src dst\n", argv[0]); // 使用方式
        // return -1;

        /* 显示开启图面 */
        char img_path[100] = {0};
        strcpy(img_path, argv[1]);
        show_bitmap(img_path, 0, 0);
    }

    /* 应用初始化·LCD屏幕输出 */
    Systerm_Init();
    printf("LCD init success!\n");

    /* 触屏输入 */
    fd_touch = open(TOUCH_DEV_PATH, O_RDONLY);
    if (fd_touch < 0)
    {
        perror("Failed to open input device");
        exit(1);
    }


    /* 线程创建 */
    pthread_create(&thread_id_touch, NULL, input_thread, &fd_touch);

    /* 主循环 */
    int quit = 0;
    while (!quit)
    {
        if (info.is_valid > 0 && info.mutex > 0)
        {
            info.mutex = 0; // 互斥锁 | 处理屏幕*点击*坐标信息
            info.is_valid = 1; // 使用过
            info.mutex = 1;
        }

        /* 在主循环中加入适当的等待时间，以免卡住 CPU */
        usleep(10000);

        /* 检查是否需要退出循环 */
        if (getchar() == 'q')
        {
            quit = 1;
        }
    }

    /* 等待触屏输入线程结束 */
    pthread_join(thread_id_touch, NULL);

    /* 关闭输入设备 */
    close(fd_touch);

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>

// #define MAX_MSG_LEN 128

// typedef struct {
//     int is_valid; // 互斥锁 or 条件变量
//     char msg[MAX_MSG_LEN];
// } message_t; 

// message_t send_box;
// message_t recv_box;

// pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t recv_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t send_cond = PTHREAD_COND_INITIALIZER;
// pthread_cond_t recv_cond = PTHREAD_COND_INITIALIZER;

// void *sender_thread(void *arg) {
//     char msg[MAX_MSG_LEN];
//     while (1) {
//         printf("Enter message (or \"exit\" to quit): ");
//         fgets(msg, MAX_MSG_LEN, stdin);
//         if (strcmp(msg, "exit\n") == 0) {
//             break;
//         }
//         pthread_mutex_lock(&send_mutex);
//         while (send_box.is_valid) {
//             pthread_cond_wait(&send_cond, &send_mutex);
//         }
//         strncpy(send_box.msg, msg, MAX_MSG_LEN);
//         send_box.is_valid = 1;
//         pthread_mutex_unlock(&send_mutex);
//         pthread_cond_signal(&recv_cond);
//     }
//     return NULL;
// }

// void *receiver_thread(void *arg) {
//     while (1) {
//         pthread_mutex_lock(&recv_mutex);
//         while (!recv_box.is_valid) {
//             pthread_cond_wait(&recv_cond, &recv_mutex);
//         }
//         printf("Received message: %s", recv_box.msg);
//         recv_box.is_valid = 0;
//         pthread_mutex_unlock(&recv_mutex);
//         pthread_cond_signal(&send_cond);
//     }
//     return NULL;
// }

// int main() {
//     pthread_t sender_id, receiver_id;
//     send_box.is_valid = 0;
//     recv_box.is_valid = 0;
//     pthread_create(&sender_id, NULL, sender_thread, NULL);
//     pthread_create(&receiver_id, NULL, receiver_thread, NULL);
//     pthread_join(sender_id, NULL);
//     pthread_join(receiver_id, NULL);
//     return 0;
// }
