#ifndef _GY39_H_
#define _GY39_H_
#include <stdint.h>
typedef struct GY39
{
    uint8_t head[2]; // 帧头标志
    uint8_t type;    // 本帧数据类型
    uint8_t len;     // 数据量
    uint8_t data[10]; // 4~10 字节数据
    uint8_t sum; // 校验和
} GY39;

typedef struct GY39_DATA
{
    float temp; // 温度
    float humi; // 湿度
    float light; // 光照 lux
    float alti; // 海拔
    float press; // 气压
} GY39_DATA;


typedef struct GY39_CMD
{
    uint8_t head;
    uint8_t cmd;
    uint8_t sum;
}GY39_CMD;

int GY39_init(void);
int GY39_read(int fd_gy39, GY39_DATA *data);
int GY39_mod(int fd_gy39,int mode);
void GY39_exit(int fd);

#endif