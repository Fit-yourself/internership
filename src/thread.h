#if !defined(_THREAD_H)
#define _THREAD_H
#include <pthread.h>
#include "touch.h"
#include "GY39.h"
#include "MQ2.h" //smoke

void *input_thread(void *arg);
void *gy39_thread(void *arg);
void *mq2_thread(void *arg);
void MQ2_layout_display(int* nd);
void GY39_layout_display(GY39_DATA* data);
void buttons_method(Touch_info *info);


#endif // _THREAD_H
