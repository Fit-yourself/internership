#include "header.h"
#include <sys/mman.h>
#include "lcd.h"

#include "serial.h"
#include "bitmap.h"


#define BEEP_PATH "/sys/kernel/gec_ctrl/beep"


int main()
{
 
    Systerm_Init();
    // show_bitmap("./img.bmp",0,0);

    // 1.初始化串口
    int fd_smoke = serial_init(COM3, 9600);
    if (fd_smoke == -1)
    {
        perror("init smoke error");
        return -1;
    }
    
    // 2.装驱动
    system("insmod kobject_led.ko");
    int fd_beep = open(BEEP_PATH, O_RDWR);
    if (fd_beep == -1)
    {
    }

    // 3.获取烟雾信息
    while (1)
    {
        // 发送请求信息给传感器
        char w_buf[9] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        char r_buf[9] = {0};

        int ret = write(fd_smoke, w_buf, 9);
        if (ret != 9)
        {
            perror("write data error");
            continue;
        }

        // 在写和读数据之间，要给传感器一丢丢反应时间
        usleep(1000);

        // 准备接收数据
        int r = read(fd_smoke, r_buf, 9);
        if (r != 9)
        {
            perror("read data error");
            continue;
        }

        // 验证数据的准确性
        int nd = 0;
        int cmd;
        if (r_buf[0] == 0xff && r_buf[1] == 0x86)
        {
            // 烟雾浓度 r_buf[2] && r_buf[3]
            nd = r_buf[2] << 8 | r_buf[3];
            printf("smoke = %d\n", nd);
            if (nd >50)
            {
                // 打开蜂鸣器
                cmd = 1;
                write(fd_beep, &cmd, 1);
                sleep(1);
            }
            else
            {
                // 关闭蜂鸣器
                //.....
                cmd = 0;
                write(fd_beep, &cmd, 1);
            }
        }
        sleep(1);
    }

    return 0;
}