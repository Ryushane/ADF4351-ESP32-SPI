

![1561810618458](ADF4351笔记.assets/1561810618458.png)

芯片引脚定义

分频器

PHASE FREQUENCY DETECTOR (PFD)![img](ADF4351笔记.assets/12152-33252-01.gif)

![img](ADF4351笔记.assets/12152-33253-02.gif)

![1561812029565](ADF4351笔记.assets/1561812029565.png)

![img](ADF4351笔记.assets/12152-33254-03.gif)

![1561886139384](ADF4351笔记.assets/1561886139384.png)

![1561886190343](ADF4351笔记.assets/1561886190343.png)

Feedback Type

![1563623734640](assets/1563623734640.png)

内部晶振100M

先输出一个200M的方波

| 寄存器名称 | 寄存器值   |
| ---------- | ---------- |
| 0          | 0x00320000 |
| 1          |            |
| 2          |            |
| 3          |            |
| 4          |            |
|            |            |

> VCO的输出频率范围为2200MHz - 4400MHz

![1561965947423](https://github.com/Ryushane/ADF4351-ESP32-SPI/blob/master/Pictures/1561965947423.png)

![1561994741914](https://github.com/Ryushane/ADF4351-ESP32-SPI/blob/master/Pictures/1561994741914.png)

**动态更新**

![1561995493488](https://github.com/Ryushane/ADF4351-ESP32-SPI/blob/master/Pictures/1561995493488.png)

**设置寄存器**![1561995529954](https://github.com/Ryushane/ADF4351-ESP32-SPI/blob/master/Pictures/1561995529954.png)



**SPI写数据**

![1561995561728](https://github.com/Ryushane/ADF4351-ESP32-SPI/blob/master/Pictures/1561995561728.png)

## 2017年F题

所需的ADF4351输出的频率不过250M-300M

所需改变的几个寄存器

R0，R1，R3

UI界面：按钮改变频率，同时可以直接使用键盘输入

先加上10.7MHz

ADF4351有不同的晶振频率 25M或者100M，只要更改R counter的大小就可以

25M R counter = 1；100M R counter = 4;

prescaler什么作用其实到现在我也不是很d懂

还有phase value

需要改的主要是Mod value

目前只写了整数部分，分数还需要测试一下

INT范围为23-65535

