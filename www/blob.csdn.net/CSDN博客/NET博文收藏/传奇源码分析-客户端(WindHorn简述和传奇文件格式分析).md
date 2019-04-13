
# 传奇源码分析-客户端(WindHorn简述和传奇文件格式分析) - .NET博文收藏 - CSDN博客


2008年06月10日 12:51:00[hejishan](https://me.csdn.net/hejishan)阅读数：2027


**DirectX****类库分析****(WindHorn)：**
1.RegHandler.cpp 注册表访问(读写)。
2.CWHApp派生CWHWindow，CWHWindow完成窗口的注册和创建。CWHWindow派生出CWHDXGraphicWindow,CWHDXGraphicWindow调用CWHWindow完成创建窗口功能，然后再调用CreateDXG()来初始化DirectX。
3.WHDefProcess.cpp在构造函数中获得CWHDXGraphicWindow句柄。
Clear函数中调用在后台缓存上进行绘图操作，换页至屏幕。
ShowStatus函数,显示状态信息。
DefMainWndProc函数，调用CWHDXGraphicWindow->MainWndProcDXG消息处理。
4.WHImage.cpp图象处理。加载位图，位图转换。优化处理。
5.WHSurface.cpp 主页面处理。
6.WHWilTexture.cpp 材质渲染。
WILTextureContainer：WIL容器类。m_pNext指向下一个WILTextureContainer，单链表。
7.WHWilImage.cpp 从Data目录中加载Wix文件（内存映射）。
8.WHDXGraphic.cpp 处理DirectX效果。
文件类型格式探讨：
Wix文件：索引文件，根据索引查找到相应数据地址(数据文件)。
// WIX 文件头格式
typedef struct tagWIXFILEIMAGEINFO
{
CHAR    szTmp[40];     //库文件标题'WEMADE Entertainment inc.' WIL文件头
INT     nIndexCount;   //图片数量
INT*    pnPosition;    //位置
}WIXIMAGEINFO, *LPWIXIMAGEINFO;
我们下载一个Hedit编辑器打开一个Wil文件，分析一下。我们发现Wix文件中，0x23地址(含该地址)以前的内容是都相同的，即为：\#INDX v1.0-WEMADE Entertainment inc.
Ofs44 0x2C的地方：存放着0B 00 00 00，高低位转换后为：0xB转换十进制数为11(图片数量)Ofs48 0x30的地方：存放着38 04 00 00，高低位转换后为：0x438 = 1080, 这个就是图象数据的开始位置。
我们用Wil编辑打开对应的Wil文件，发现，果然有11张图片。另外我们发现，在Ofs = 44 -47之间的数据总是38 04 00 00，终于明白，所有的图片起始位置是相同的。
Wil文件: 数据文件。
前面我们说了图象数据的开始位置为0x438 = 1080, 1080中有文件开头的44字节都是相同的。所以，就是说有另外的1036字节是另有用途。1036中有1024是一个256色的调色板。
我们看到图片位置数据为：20 03 58 02, 转化为十六进制： 0x320, 0x258 刚好就是800*600大小的图片。07 00 D4 FF。图片起始位置为：
Ofs 1088: 0x440 图片大小为480000
起始位置：0x440 1088终止位置：0x7573F 481087 为了验证数据是否正确，我们通过Wil工具，把第一幅图片导出来，然后用Hedit编辑器打开，经过对比，我们发现，数据一致。大小一致。
第二张BMP图片(图片起始位置：0x436 10078) ： F0 01 69 01 , 07 00 D4 FF
刚好大小。第二张Wil起始位置：Ofs:481096  0x75748
知道了图片格式，我们可以写一个抓图片格式的程序了。
**很遗憾,网速太慢, 图片很难贴上去. 如果有图片,大家看得就更清楚了.**




