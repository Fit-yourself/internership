#include "header.h"
#include "lcd.h"

int Systerm_Init(void)
{
    int ret = LCD_init();
    LCD_Clear_screen();
    return ret;
}
