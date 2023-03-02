#ifndef TOUCH_H
#define TOUCH_H

#include "header.h"

#include <pthread.h>
#include <linux/input.h>
#define TOUCH_DEV_PATH "/dev/input/event0"

typedef struct 
{
    // int mutex;
    // int is_valid;
    int x;
    int y;
}Touch_info;
extern Touch_info info;


typedef struct
{
    int x0, y0;
    int x1, y1;
    char* info;
} Button;

void *input_thread(void *arg);
#endif // TOUCH_H
