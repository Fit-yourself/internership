#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define atmos_BMP "atmos.bmp"
#define hum_BMP "hum.bmp"
#define light_on_BMP "light_on.bmp"
#define light_off_BMP "light_off.bmp"
#define music_BMP "music.bmp"
#define pause_BMP "pause.bmp"
#define play_BMP "play.bmp"
#define temp_BMP "temp.bmp"

#define x_ligh_BMP 280
#define y_ligh_BMP 187

#define x_play_BMP 108
#define y_play_BMP 278
#define x_bb_BMP 25
#define y_bb_BMP 198

#define x_smoke 500 // 右上 only numbers
#define y_smoke 0

////////////////////////////////////////////
#define x_temp_BMP (514) //
#define y_temp_BMP 167
#define x_temp (x_temp_BMP + 50) //
#define y_temp (y_temp_BMP + 25)

#define x_humi_BMP (500)
#define y_humi_BMP 276
#define x_humi (x_humi_BMP + 85)
#define y_humi (y_humi_BMP + 25)

#define x_press_BMP (485)
#define y_press_BMP 385
#define x_press (x_press_BMP + 100)
#define y_press (y_press_BMP + 25)

int Systerm_Init(void);
int main_thread();
/* LCD END*/
#endif