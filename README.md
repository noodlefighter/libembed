# libembed
toolkit for embeded develop

(working...)

想弄个平台无关的小工具集 ANSI-C
主要做些基础部件 方便裸机开发或是作为RTOS的补充
使这些部件之间没有依赖关系以便裁剪

## 状态机框架
QP-Nano很好 然而是GPL 遂想自己动手实现个
QP的亮点是做出的代码可读性好

其实就是事件驱动+状态机
FSM/HSM(平面/层次状态机) + 主动对象(Active Object) + 主动对象执行器

要方便使用还得加上 调试辅助 + 状态机设计工具(图生成代码, 通过UML设计工具导出的XMI,生成代码应该不难做到)

## 堆内存/内存池


## 环形缓冲区


## 简单事件机制
订阅 - 分发
