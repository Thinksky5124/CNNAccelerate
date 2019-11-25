# -
第三届FPGA创新大赛-加速视觉处理小组
# FPGA 加速 CNN

参考资料：
[HLS 视频开发流程][3]

## CNN 的 C 语言实现

图像处理，像素值/255

电脑端运行 1.92s

### 第一层-卷积

卷积核大小 5\*5

有 6 个卷积核

卷积输入层数 1 层

卷积输出层数 6 层

第一层操作顺序：卷积-激活

### 第二层-池化

池化大小 2\*2

池化输入层数 6 层

池化输出层数 6 层

第二层操作顺序：平均池化

### 第三层-全连接

卷积核大小 5\*5

卷积输入层数 6 层

卷积输出层数 12 层

第一层操作顺序：卷积-激活

### 第四层-第二次池化

池化大小 2\*2

池化输入层数 6 层

池化输出层数 6 层

第四层操作顺序：平均池化

### 第五层-激活输出层

第五层操作顺序：全连接-激活

## 图像灰度处理

使用 Matlab 的 rgb2gray 函数进行灰度处理：
rgb2gray 通过计算 R、G 和 B 分量的加权和，将 RGB 值转换为灰度值，一下是 MATLAB 文件中的说明：

```
0.2989 * R + 0.5870 * G + 0.1140 * B
这些权重与 rgb2ntsc 函数用于计算 Y 分量的权重相同。
在舍入到小数点后 3 位之后，rgb2gray 中用来计算灰度值的系数与 Rec.ITU-R BT.601-7 中用来计算亮度 (E'y) 的系数相同。
```

以下是对比效果：

<center>原图片</center>

![原图片](TestImage/Test1.jpg)

<center>灰度处理后图片-加权平均</center>

![灰度处理后图片-加权平均](TestImage/Test1_Golden_Weight.jpg)

<center>灰度处理后图片-平均值</center>

![灰度处理后图片-平均值](TestImage/Test1_Golden_Mean.jpg)

## RGB 图像灰度化并构造存储灰度矩阵 IP 核

该 IP 核的功能为

- 将颜色空间为 RGB888 流格式的图像转换成灰度图像
- 将灰度图像转化成存储矩阵方便读取

### RGB 转灰度 IP 核设计

针对 RGB888 颜色空间

#### [采用整数灰度转换算法][5]

##### 原理

一、基础
　　对于彩色转灰度，有一个很著名的心理学公式：
Gray = R*0.299 + G*0.587 + B*0.114
二、整数算法
　　而实际应用时，希望避免低速的浮点运算，所以需要整数算法。
　　注意到系数都是 3 位精度的没有，我们可以将它们缩放 1000 倍来实现整数运算算法：
Gray = (R*299 + G*587 + B*114 + 500) / 1000
　　 RGB 一般是 8 位精度，现在缩放 1000 倍，所以上面的运算是 32 位整型的运算。注意后面那个除法是整数除法，所以需要加上 500 来实现四舍五入。
　　就是由于该算法需要 32 位运算，所以该公式的另一个变种很流行：
Gray = (R*30 + G*59 + B*11 + 50) / 100
　　但是，虽说上一个公式是 32 位整数运算，但是根据 80x86 体系的整数乘除指令的特点，是可以用 16 位整数乘除指令来运算的。而且现在 32 位早普及了（AMD64 都出来了），所以推荐使用上一个公式。
三、整数移位算法
　　上面的整数算法已经很快了，但是有一点仍制约速度，就是最后的那个除法。移位比除法快多了，所以可以将系数缩放成 2 的整数幂。
　　习惯上使用 16 位精度，2 的 16 次幂是 65536，所以这样计算系数：
0.299 * 65536 = 19595.264 ≈ 19595
0.587 _ 65536 + (0.264) = 38469.632 + 0.264 = 38469.896 ≈ 38469
0.114 _ 65536 + (0.896) = 7471.104 + 0.896 = 7472
　　可能很多人看见了，使用的舍入方式不是四舍五入。四舍五入会有较大的误差，应该将以前的计算结果的误差一起计算进去，舍入方式是去尾法：
　　写成表达式是：
Gray = (R*19595 + G*38469 + B*7472) >> 16
　　 2 至 20 位精度的系数：
Gray = (R*1 + G*2 + B*1) >> 2
Gray = (R*2 + G*5 + B*1) >> 3
Gray = (R*4 + G*10 + B*2) >> 4
Gray = (R*9 + G*19 + B*4) >> 5
Gray = (R*19 + G*37 + B*8) >> 6
Gray = (R*38 + G*75 + B*15) >> 7
Gray = (R*76 + G*150 + B*30) >> 8
Gray = (R*153 + G*300 + B*59) >> 9
Gray = (R*306 + G*601 + B*117) >> 10
Gray = (R*612 + G*1202 + B*234) >> 11
Gray = (R*1224 + G*2405 + B*467) >> 12
Gray = (R*2449 + G*4809 + B*934) >> 13
Gray = (R*4898 + G*9618 + B*1868) >> 14
Gray = (R*9797 + G*19235 + B*3736) >> 15
Gray = (R*19595 + G*38469 + B*7472) >> 16
Gray = (R*39190 + G*76939 + B*14943) >> 17
Gray = (R*78381 + G*153878 + B*29885) >> 18
Gray = (R*156762 + G*307757 + B*59769) >> 19
Gray = (R*313524 + G*615514 + B*119538) >> 20
　　仔细观察上面的表格，这些精度实际上是一样的：3 与 4、7 与 8、10 与 11、13 与 14、19 与 20
　　所以 16 位运算下最好的计算公式是使用 7 位精度，比先前那个系数缩放 100 倍的精度高，而且速度快：
Gray = (R*38 + G*75 + B\*15) >> 7
　　其实最有意思的还是那个 2 位精度的，完全可以移位优化：
Gray = (R + (WORD)G<<1 + B) >> 2
　　由于误差很大，所以做图像处理绝不用该公式（最常用的是 16 位精度）。但对于游戏编程，场景经常变化，用户一般不可能观察到颜色的细微差别，所以最常用的是 2 位精度。

##### 代码实现

###### Verilog 代码实现

先将 RGB 分割到 24 位（8 位原数据+16 拓展乘法位）的 wire 类型的变量上，然后乘法运算

```
//转换模块
module Image2GayArray(
	iData,//放图像数据24位RGB888颜色空间
  clk,//时钟信号
  rst_n,//复位信号
  ogray,//输出结果8位
  start,//数据准备信号
  Done//输出结果信号
	);
enmodule
//图像地址调用模块
module counter_ctrl(
	CLK,//时钟信号
  RSTn,//复位信号
  start,//开始信号
  oAddr,//地址信号
  oDone//输出结果信号
	);
endmodule
//rgb2gray模块
module rgb2gray(
	CLK,//时钟信号
  RSTn,//复位信号
  Start,//开始信号
  Gray,//结果
  addra,//地址信号
  Done,//完成信号
  aDone,//单像素点完成转换信号
  iData//数据信号
	);
endmodule
```

###### C 代码 HLS 实现

注意事项：
一、数据类型的表示
typedef ap_ufixed<12,8,AP_RND,AP_SAT> IMAGE_DATA_TYPE;
注意 W ，I 的范围选择

### 使用 DMA 存储灰度图像矩阵

#### DMA 的使用方法-Block Memory Generator IP 核的使用

##### 存储类型

三种模式的 RAM：单口 RAM、伪双口 RAM（简单单口 RAM）和真双口 RAM，以及单双口 ROM

###### 单口 RAM

<img alt="" src="https://img-blog.csdn.net/20140930123835172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

###### 伪双口 RAM-简化双口，A 写入，B 读出

<img alt="" src="https://img-blog.csdn.net/20140930123937088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

###### 真双口，A 和 B 都可以读写

<img alt="" src="https://img-blog.csdn.net/20140930124130095?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

#### 配置方法

一、使用 IP 核，确定数据位宽和深度：（超出地址范围将返回无效数据，在对超出地址范围的数据进行操作的时候，不能够 set 或者 reset）

二、设置操作模式：（写优先，读优先，不改变）

##### 操作模式的区别

###### 写优先

这里的写优先的意思就是你写入的数据，会出现在输出端口，不管你给的地址是什么。这种好处就是保证了你读出的数据是最新的。

<img alt="" src="https://img-blog.csdn.net/20140930125832848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

###### 读优先

读优先指的就是：不管你写入的数据，是先把你要读的数据读出。

<img alt="" src="https://img-blog.csdn.net/20140930130117460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

###### 不改变

不改变模式就是正常的模式，该读的时候读，改写的时候写：（一般没有特殊要求就是选这个）

<img alt="" src="https://img-blog.csdn.net/20140930130304289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzYwMjI3MzA5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast">

## 图像卷积处理

### 卷积核的选取

卷积核分类：平均值滤波、高斯平滑、图像锐化、梯度 Prewitt、Soble 边缘检测、梯度 Laplacian

[介绍几种卷积核][1]

#### 第一种卷积核-图像锐化

卷积神经网路拟采用图像锐化的卷积核，卷积核如下（采用了后一种）：
<img alt="" src="https://images2018.cnblogs.com/blog/1150378/201805/1150378-20180523130202222-373641058.png">

```
#C++ Opencv代码：
Mat kern = (Mat_<char>(3, 3) << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0);//采用图像锐化的方式卷积图片
```

此时采用的是 Opencv 的 filter2D 函数进行卷积

卷积效果图：

<center>卷积处理后图片-图像锐化</center>

![卷积处理后图片-图像锐化](TestImage/Test1_Golden_Conv.jpg)

#### 第二种卷积核-边缘 Soble 提取

```
hls::Sobel<1,0,3>(img_0, img_1);
(xorder=1, yorder=0, aperture_size=3)
```

卷积核：
<img width="112" height="88" class="size-full wp-image-2338" alt="OpenCV函数Sobel算子详解" src="https://www.daimadog.com/wp-content/uploads/2018/07/20180715113740.png">

### 编写 Verilog 语言的卷积 IP 核

模块定义代码说明

```

```

### 编写用于 HLS 的 C++的卷积函数

首先是卷积核的构建[参考][2]

```

**/
```

[1]: https://www.cnblogs.com/xiaojianliu/p/9075872.html
[2]: https://blog.csdn.net/qq_29540745/article/details/52487832
[3]: https://blog.csdn.net/Micheal_w/article/details/39272811
[4]: https://blog.csdn.net/c602273091/article/details/39694145
[5]: https://www.cnblogs.com/diewcs/archive/2010/10/03/1841744.html
[6]: https://www.cnblogs.com/happyamyhope/p/5725741.html
