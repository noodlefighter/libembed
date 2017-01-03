# libembed
toolkit for embeded develop

(working...)

想弄个平台无关的小工具集 ANSI-C
主要做些基础部件 方便裸机开发或是作为RTOS的补充
使这些部件之间没有依赖关系以便裁剪

可用状态
--------
__工具宏集__ 常用宏/位操作/大小端字节操作

__tinyprintf__ 一个轻量的格式化输出函数库 不支持浮点数(考虑以后添加) fork from [tinyprintf](https://github.com/cjlano/tinyprintf)

__Rotary Encoder__ 旋转编码器

进行中
------
__MSM__ UML状态机

__Ring Buffer__ 循环队列缓冲区

__Rx/Tx Control__ 接收/发送流控制



计划
-----
__softPWM__

__堆内存/内存池__

__简单事件机制__

__SLIP帧控制__

__RGBLED__

__灯效控制(单灯/条形/矩阵)__

__按键扫描(单键/矩阵)__