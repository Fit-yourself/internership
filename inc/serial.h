#ifndef __SERIAL_H__
#define __SERIAL_H__

//6818串口所对应的文件名
#define COM2 "/dev/ttySAC1"
#define COM3 "/dev/ttySAC2"
#define COM4 "/dev/ttySAC3"


int serial_init(char *file,int peed);


#endif 




