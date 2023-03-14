#ifndef TOUCH_H
#define TOUCH_H

#include "header.h"

#include <pthread.h>
#include <linux/input.h>
typedef enum Direction
{
    TOUCH_RIGHT,
    TOUCH_LEFT,
    TOUCH_UP,
    TOUCH_DOWN
}Direction;

typedef struct 
{
    // int mutex;
    // int is_valid;
    int x;
    int y;
    Direction type;
}Touch_info;
extern Touch_info info;


typedef struct
{
    int x0, y0;
    int x1, y1;
    int status;
} Button;

int Touch_init(void);
int Touch_get(int fd_touch, Touch_info *info);
#endif // TOUCH_H
    