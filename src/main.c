#include "header.h"
#include "bitmap.h"
#include "lcd.h"
#include "touch.h"
#include <pthread.h>
#include "thread.h"
#include "mode.h"

extern void thread_2048(void);
int main(int argc, char const *argv[])
{
    LCD_Init();
    
    // main_thread();
    thread_2048();

    for (;;)
    {
            
    }
    return 0;
}
