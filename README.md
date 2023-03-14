# 说明
这是脱产实习基于 Linux 进行应用层面的开发学习的项目。

## 项目概述
这是一个基于 Linux 的应用程序，用于在 Linux 系统中实现如下几个任务：
1. 通过驱动来操作 Linux 开发板上的基本外设如 LED、蜂鸣器
2. 认识帧缓冲，基于此原理进行 LCD 屏幕绘制
    - 了解 BMP 图片格式，写入 BMP 图片到 LCD 屏幕
3. 通过串口与 传感器 进行通信，认识 `read`、`write`、`open`、`close` 等系统调用
    - 认识到 `struct` 的底层数据的存储
4. 对 2048 游戏算法，通过 LCD 显示提供前端显示，借用触摸屏来获取上下左右滑输入。

## 使用方法
项目管理比较差，没有再进行源文件的清洗，简单的用了 CMake 进行管理，降低耦合。有效目录结构如下：
```
── lib
│   ├── CMakeLists.txt
│   ├── GY39.c                          # GY39 获取温湿度、光照强度、气压等传感信息
│   ├── LED.c   
│   ├── MQ2.c                           # MQ2 获取烟雾浓度
│   ├── beep.c
│   ├── lcd.c                           # LCD 屏幕驱动
│   ├── lcd_touch.c # LCD 触摸屏驱动
│   ├── serial.c                        # 串口驱动
└── src
    ├── 2048.c                          # 2048 游戏
    ├── CMakeLists.txt
    ├── main.c                          # 入口
    ├── thread.c                        # 实际入口
    ├── thread.h
```
通过 `vscode` 利用 CMake 插件，在`src`文件夹按需修改后可以直接 `build`。

## Q/A

#### 1. `ARM Cortex-A8开发板 /lib/libc.so.6: version 'GLIBC_2.17' not found`
 由于 wsl2 自带编译器过高，导致交叉编译后运行时出现 `GLIBC_2.17` 问题，解决方案是在 Linux 系统中安装低版本的编译器，如 `arm-linux-gcc-5.4.0.tar`。


#### 2. `error while loading shared libraries: libmpfr.so.4: cannot open shared object file: No such file or directory`
在 wsl2 中(虚拟机中)
```bash 
sudo ln -s /usr/lib/x86_64-linux-gnu/libmpfr.so.6 /usr/lib/x86_64-linux-gnu/libmpfr.so.4
```