#if !defined(TOUCH_H)
#define TOUCH_H

#include "header.h"
#include "lcd.h"
#include <pthread.h>
#include <linux/input.h>
#define TOUCH_DEV_PATH "/dev/input/event0"

typedef struct 
{
    int mutex;
    int is_valid;
    int x;
    int y;
}touch_info;
extern touch_info info;

void *input_thread(void *arg);
#endif // TOUCH_H
