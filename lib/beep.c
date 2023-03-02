#include "header.h"
#include "beep.h"

int fd_beep;
int BEEP_init(void)
{
    int fd_beep;
    fd_beep = open("/sys/kernel/gec_ctrl/beep", O_RDWR);
    if (fd_beep == -1)
    {
        perror("open beep error");
        return -1;
    }
    return 0;
}

void BEEP_Toggle(void)
{
    static int cmd = 0;
    if (cmd == 0)
        write(fd_beep, &cmd, 1);
    else
        write(fd_beep, &cmd, 0);
}
void BEEP_on(void)
{
    int cmd = 1;
    write(fd_beep, &cmd, 1);
}
void BEEP_off(void)
{
    int cmd = 0;
    write(fd_beep, &cmd, 1);
}
void BEEP_exit(void)
{
    close(fd_beep);
}
