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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int score = 0;
int game_over = 0;
#define ROW 4
#define COL 4
int board[ROW][COL];
void init_board(int board[][COL]);
void print_board(int board[][COL]);
int slide_left(int board[][COL]);
int slide_right(int board[][COL]);
int slide_up(int board[][COL]);
int slide_down(int board[][COL]);
void generate_random(int board[][COL]);
int check_game_over(int board[][COL]);

void init_board(int board[][COL])
{
    // 初始化棋盘
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            board[i][j] = 0;
        }
    }

    // 随机生成两个数字
    int count = 0;
    while (count < 2)
    {
        int i = rand() % ROW;
        int j = rand() % COL;
        if (board[i][j] == 0)
        {
            board[i][j] = (rand() % 2 + 1) * 2;
            count++;
        }
    }
}
#include "lcd.h"
#define n0_BMP "n0.bmp"
#define n2_BMP "n2.bmp"
#define n4_BMP "n4.bmp"
#define n8_BMP "n8.bmp"
#define n16_BMP "n16.bmp"
#define n32_BMP "n32.bmp"
#define n64_BMP "n64.bmp"
#define n128_BMP "n128.bmp"
#define n256_BMP "n256.bmp"
#define n512_BMP "n512.bmp"
#define n1024_BMP "n1024.bmp"
#define n2048_BMP "n2048.bmp"

void print_board(int board[][COL]) // TODO Using LCD
{
    static char *pic = 0;
    // 打印棋盘
    for (int j = 0; j < COL; j++)
    {
        for (int i = 0; i < ROW; i++)
        {
            // printf("%-5d", board[i][j]);
            switch (board[i][j])
            {
            case 0:
                pic = n0_BMP;
                break;
            case 2:
                pic = n2_BMP;
                break;
            case 4:
                pic = n4_BMP;
                break;
            case 8:
                pic = n8_BMP;
                break;
            case 16:
                pic = n16_BMP;
                break;
            case 32:
                pic = n32_BMP;
                break;
            case 64:
                pic = n64_BMP;
                break;
            case 128:
                pic = n128_BMP;
                break;
            case 256:
                pic = n256_BMP;
                break;
            case 512:
                pic = n512_BMP;
                break;
            case 1024:
                pic = n1024_BMP;
                break;
            case 2048:
                pic = n2048_BMP;
                break;
            default:
                break;
            }
            LCD_DrawBmp(pic, j * 200, i * 120, 1);
        }
        // printf("\n");
    }
    printf("Current score is %d.\n", score);
    // printf("\n");
}

int slide_left(int board[][COL])
{
    // 向左滑动
    int flag = 0;
    for (int i = 0; i < ROW; i++)
    {
        int k = 0;
        for (int j = 1; j < COL; j++)
        {
            if (board[i][j] != 0)
            {
                if (board[i][k] == 0)
                {
                    board[i][k] = board[i][j];
                    board[i][j] = 0;
                    flag = 1;
                }
                else if (board[i][k] == board[i][j])
                {
                    board[i][k] *= 2;
                    score += board[i][k];
                    board[i][j] = 0;
                    k++;
                    flag = 1;
                }
                else
                {
                    k++;
                    if (k != j)
                    {
                        board[i][k] = board[i][j];
                        board[i][j] = 0;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

int slide_right(int board[][COL])
{
    // 向右滑动
    int flag = 0;
    for (int i = 0; i < ROW; i++)
    {
        int k = COL - 1;
        for (int j = COL - 2; j >= 0; j--)
        {
            if (board[i][j] != 0)
            {
                if (board[i][k] == 0)
                {
                    board[i][k] = board[i][j];
                    board[i][j] = 0;
                    flag = 1;
                }
                else if (board[i][k] == board[i][j])
                {
                    board[i][k] *= 2;
                    board[i][j] = 0;
                    score += board[i][k];
                    k--;
                    flag = 1;
                }
                else
                {
                    k--;
                    if (k != j)
                    {
                        board[i][k] = board[i][j];
                        board[i][j] = 0;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

int slide_up(int board[][COL])
{
    // 向上滑动
    int flag = 0;
    for (int j = 0; j < COL; j++)
    {
        int k = 0;
        for (int i = 1; i < ROW; i++)
        {
            if (board[i][j] != 0)
            {
                if (board[k][j] == 0)
                {
                    board[k][j] = board[i][j];
                    board[i][j] = 0;
                    flag = 1;
                }
                else if (board[k][j] == board[i][j])
                {
                    board[k][j] *= 2;
                    board[i][j] = 0;
                    score += board[k][j];
                    k++;
                    flag = 1;
                }
                else
                {
                    k++;
                    if (k != i)
                    {
                        board[k][j] = board[i][j];
                        board[i][j] = 0;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

int slide_down(int board[][COL])
{
    // 向下滑动
    int flag = 0;
    for (int j = 0; j < COL; j++)
    {
        int k = ROW - 1;
        for (int i = ROW - 2; i >= 0; i--)
        {
            if (board[i][j] != 0)
            {
                if (board[k][j] == 0)
                {
                    board[k][j] = board[i][j];
                    board[i][j] = 0;
                    flag = 1;
                }
                else if (board[k][j] == board[i][j])
                {
                    board[k][j] *= 2;
                    score += board[k][j];
                    board[i][j] = 0;
                    k--;
                    flag = 1;
                }
                else
                {
                    k--;
                    if (k != i)
                    {
                        board[k][j] = board[i][j];
                        board[i][j] = 0;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}
void generate_random(int board[][COL])
{
    // 在棋盘上随机生成一个新的数字
    int i, j;
    do
    {
        i = rand() % ROW;
        j = rand() % COL;
    } while (board[i][j] != 0);
    board[i][j] = (rand() % 2 + 1) * 2;
}

int check_game_over(int board[][COL])
{
    // 检查游戏是否结束
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j] == 0)
            {
                return 0;
            }
            if (i < ROW - 1 && board[i][j] == board[i + 1][j])
            {
                return 0;
            }
            if (j < COL - 1 && board[i][j] == board[i][j + 1])
            {
                return 0;
            }
        }
    }
    return 1;
}

#define _CLEAN LCD_Clear(0xffffff);
#define abs(x) ((x) > 0 ? (x) : -(x))
void slip_method(int fd_touch, Touch_info *info)
{
    static int start_x = 0;
    static int start_y = 0;
    static int end_x = 0;
    static int end_y = 0;
    static int is_sliding = 0;

    static int ret = 0;
    struct input_event ev;
    while (read(fd_touch, &ev, sizeof(ev)) > 0 && game_over != 1)
    {
        if (ev.type == EV_ABS)
        {
            if (ev.code == ABS_X)
            {
                info->x = ev.value * (1.0 * 800 / 1040); // 坐标映射
            }
            else if (ev.code == ABS_Y)
            {
                info->y = ev.value * (1.0 * 480 / 600); // 坐标映射
            }
        }
        else if (ev.type == EV_KEY && ev.code == BTN_TOUCH)
        {
            if (ev.value == 1) // 触摸按下
            {
                start_x = info->x;
                start_y = info->y;
                is_sliding = 1;
                // printf("start_x = %d, start_y = %d", start_x, start_y);
            }
            else if (ev.value == 0 && is_sliding) // 触摸抬起
            {
                end_x = info->x;
                end_y = info->y;
                int dx = end_x - start_x;
                int dy = end_y - start_y;
                // printf("end_x = %d, end_y = %d", end_x,end_y);
                if (abs(dx) > abs(dy) && dx > 0)
                {
                    // 向右滑动
                    // printf("\tSliding right\n");
                    ret = slide_right(board);
                }
                else if (abs(dx) > abs(dy) && dx < 0)
                {
                    // 向左滑动
                    // printf("\tSliding left\n");
                    ret = slide_left(board);
                }
                else if (abs(dy) > abs(dx) && dy > 0)
                {
                    // 向下滑动
                    // printf("\tSliding down\n");
                    ret = slide_down(board);
                }
                else if (abs(dy) > abs(dx) && dy < 0)
                {
                    // 向上滑动
                    // printf("\t  Sliding up\n");
                    ret = slide_up(board);
                }
                is_sliding = 0;
                if (ret)
                {
                    generate_random(board);
                }
                print_board(board);
            }
        }

        if (check_game_over(board))
        {
            game_over = 1;
            printf("Game over! Your score is %d.\n", score);
            return;
        }
    }
}

void signal_handler(int signo) // 进行 `ctrl + c` 后的分数显示
{
    printf("You exited! Your score is %d.\n", score);
    exit(0);
}
int thread_2048(void)
{

    Touch_info info = {0, 0};
    int fd_touch = Touch_init();
    srand(time(NULL));

    signal(SIGINT, signal_handler);
    init_board(board);
    print_board(board);

    while (!game_over)
    {
        slip_method(fd_touch, &info); // 业务逻辑
        if (game_over == 1)
        {
            return 0;
        }
    }

    return 0;
}
