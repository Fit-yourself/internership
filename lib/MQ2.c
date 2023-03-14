#include "MQ2.h"
#include "serial.h"
int MQ2_init(void)
{
    int fd_smoke = serial_init(COM3, 9600);
    if (fd_smoke == -1)
    {
        // perror("init smoke error");
        return -1;
    }
    return fd_smoke;
}