#include "header.h"
#include "serial.h"
#include "GY39.h"
/* 初始化 GY39 */
// static int fd_gy39;
int GY39_init(void)
{
    // 使用串口通信，打开串口设备文件
    int fd_gy39 = serial_init(COM4, 9600);
    if (fd_gy39 == -1)
    {
        perror("open COM4 error");
        return -1;
    }
    return fd_gy39;
}

int GY39_cmd(int fd_gy39, GY39_CMD *cmd)
{
    int ret = write(fd_gy39, cmd, sizeof(GY39_CMD) / sizeof(char));
    if (ret == -1)
    {
        perror("write GY39 error");
        return -1;
    }
    return 0;
}
int GY39_mod(int fd_gy39, int mode)
{
#define AUTO 0b10000000
#define BME 0b00000010
#define MAX 0b00000001
    static GY39_CMD cmd = {0xA5, 0, 0};
    switch (mode)
    {
    case 0:
        cmd.cmd = MAX | BME | AUTO;
        break;
    case 1: // 温度、气压、湿度、海拔
        cmd.cmd = BME | AUTO;
        break;
    case 2: // 光照
        cmd.cmd = MAX | AUTO;
        break;
    default:
        break;
    }
    cmd.sum = (cmd.head + cmd.cmd) & 0xFF;
    GY39_cmd(fd_gy39, &cmd);
    return 0;
}

/* 读取 GY39 的数据 */
int GY39_read(int fd_gy39, GY39_DATA *data)
{
    // 读取数据
    static GY39 buf;
    int ret = read(fd_gy39, &buf, sizeof(buf) / sizeof(char));
    if (ret == -1)
    {
        perror("read GY39 error");
        return -1;
    }
    if (buf.head[0] == 0x5A && buf.head[1] == 0x5A)
    {
        switch (buf.type)
        {
        case 0x15:
            /* 光照 */
            data->light = (buf.data[0] << 24) | (buf.data[1] << 16) | (buf.data[2] << 8) | buf.data[3];
            data->light = ((float)data->light) / 100; // 单位 lux
            break;
        case 0x45:
            /* 温度、气压、湿度、海拔 */
            data->temp = (float)((buf.data[0] << 8) | buf.data[1]) / 100;                                              // 温度 C
            data->press = (float)((buf.data[2] << 24) | (buf.data[3] << 16) | (buf.data[4] << 8) | buf.data[5]) / 100; // 气压 pa
            data->humi = (float)((buf.data[6] << 8) | buf.data[7]) / 100;                                              // 湿度 %
            data->alti = ((buf.data[8] << 8) | buf.data[9]);                                                    // 海拔 m
            break;  

        case 0x55:
            /* IIC 地址 */
            break;

        default:
            break;
        }
    }else{
        return -1;
    }

    // 解析数据
    // sscanf(buf, "%d %d %d", &data->light, &data->temp, &data->humi);

    return 0;
}

/* 退出 GY39 */
void GY39_exit(int fd)
{
    close(fd);
}
