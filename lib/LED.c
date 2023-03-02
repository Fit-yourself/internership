#include "header.h"
#include "LED.h"
#define LED_PATH "/sys/kernel/gec_ctrl/led"

int fd_led;

int LED_init(void)
{
    fd_led = open(LED_PATH,O_RDWR);
    if(fd_led == -1) {
        printf("LED wrong\n");
        return -1;
    }
    return 0;
}

void LED_Toggle(void){
    static int cmd=0;
    if(cmd == 0) write(fd_led,&cmd,1);
    else write(fd_led,&cmd,0);
}

void LED_on(void){
    int cmd=1;
    write(fd_led,&cmd,1);
}
void LED_off(void){
    int cmd=0;
    write(fd_led,&cmd,1);
}

void LED_exit(void){
    close(fd_led);
}