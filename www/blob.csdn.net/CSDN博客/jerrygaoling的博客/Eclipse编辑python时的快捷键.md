
# Eclipse编辑python时的快捷键 - jerrygaoling的博客 - CSDN博客


2018年07月17日 01:44:15[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：371


## 使用eclipse编辑python时的快捷键
### 常用快捷键
Ctrl+1 快速修复(最经典的快捷键,就不用多说了)
Ctrl+D: 删除当前行
Ctrl+Alt+↓ 复制当前行到下一行(复制增加)
Ctrl+Alt+↑ 复制当前行到上一行(复制增加)
Alt+↓ 当前行和下面一行交互位置(特别实用,可以省去先剪切,再粘贴了)
Alt+↑ 当前行和上面一行交互位置(同上)
Alt+← 前一个编辑的页面
Alt+→ 下一个编辑的页面(当然是针对上面那条来说了)
Alt+Enter 显示当前选择资源(工程,or 文件 or文件)的属性
Shift+Enter 在当前行的下一行插入空行(这时鼠标可以在当前行的任一位置,不一定是最后)
Shift+Ctrl+Enter 在当前行插入空行(原理同上条)
Ctrl+Q 定位到最后编辑的地方
Ctrl+L 定位在某行 (对于程序超过100的人就有福音了)
Ctrl+M 最大化当前的Edit或View (再按则反之)
Ctrl+/ 注释当前行,再按则取消注释
Ctrl+O 快速显示 OutLine
Ctrl+T 快速显示当前类的继承结构
Ctrl+W 关闭当前Editer
Ctrl+K 参照选中的Word快速定位到下一个
Ctrl+E 快速显示当前Editer的下拉列表(如果当前页面没有显示的用黑体表示)
Ctrl+/(小键盘) 折叠当前类中的所有代码
Ctrl+×(小键盘) 展开当前类中的所有代码
Ctrl+Space 代码助手完成一些代码的插入(但一般和输入法有冲突,可以修改输入法的热键,也可以暂用Alt+/来代替)
Ctrl+Shift+E 显示管理当前打开的所有的View的管理器(可以选择关闭,激活等操作)
Ctrl+J 正向增量查找(按下Ctrl+J后,你所输入的每个字母编辑器都提供快速匹配定位到某个单词,如果没有,则在stutes line中显示没有找到了,查一个单词时,特别实用,这个功能Idea两年前就有了)
Ctrl+Shift+J 反向增量查找(和上条相同,只不过是从后往前查)
Ctrl+Shift+F4 关闭所有打开的Editer
Ctrl+Shift+X 把当前选中的文本全部变味小写
Ctrl+Shift+Y 把当前选中的文本全部变为小写
Ctrl+Shift+F 格式化当前代码
Ctrl+Shift+P 定位到对于的匹配符(譬如{}) (从前面定位后面时,光标要在匹配符里面,后面到前面,则反之)
下面的快捷键是重构里面常用的,本人就自己喜欢且常用的整理一下(注:一般重构的快捷键都是Alt+Shift开头的了)
Alt+Shift+R 重命名 (是我自己最爱用的一个了,尤其是变量和类的Rename,比手工方法能节省很多劳动力)
Alt+Shift+M 抽取方法 (这是重构里面最常用的方法之一了,尤其是对一大堆泥团代码有用)
Alt+Shift+C 修改函数结构(比较实用,有N个函数调用了这个方法,修改一次搞定)
Alt+Shift+L 抽取本地变量( 可以直接把一些魔法数字和字符串抽取成一个变量,尤其是多处调用的时候)
Alt+Shift+F 把Class中的local变量变为field变量 (比较实用的功能)
Alt+Shift+I 合并变量(可能这样说有点不妥Inline)
Alt+Shift+V 移动函数和变量(不怎么常用)
Alt+Shift+Z 重构的后悔药(Undo)
**编辑**
|快捷键|说明|补充|
|---|---|---|
|Ctrl+1|快速修复|（最经典的快捷键,就不用多说了，可以解决很多问题，比如import类、try|
|Ctrl+/|注释当前行,再按则取消注释|
|Ctrl+D|删除当前行或者多行|
|Ctrl+Z|撤销|
|Ctrl+Y|重做|（与撤销Ctrl+Z相反）|
|Ctrl+Shift+F|格式化当前代码|注意与输入法冲突，如搜狗中此快捷键默认开启是繁简切换，可以选择修改或关闭；而Windows10中自带的微软输入法也是繁简切换，暂无法关闭|
|Ctrl+Shift+M|添加类的import导入|
|Ctrl+Shift+O|组织类的import导入|（既有Ctrl+Shift+M的作用，又可以帮你去除没用的导入，很有用）|
|Alt+/|内容辅助|（帮你省了多少次键盘敲打，太常用了）|
|Shift+Enter|在当前行的下一行插入空行|（这时鼠标可以在当前行的任一位置）|
**选择、移动、查找**
|快捷键|说明|补充|
|---|---|---|
|Alt+↓/↑|当前行和下/上面一行交互位置|（特别实用,可以省去先剪切,再粘贴了）|
|Ctrl+↑/↓|文本编辑器视图向上/下滚动一行|
|Ctrl+Alt+↓/↑|复制当前行到下/上一行|（复制增加）|
|Alt+Shift+↑|选择封装元素|
|Alt+Shift+←/→|选择上/下一个元素|
|Shift+←/→|从光标处开始往左/右选择字符|
|Ctrl+Shift+←/→|选中光标左/右边的单词|
|Ctrl+←/→|光标移到左/右边单词的开头|
|Alt+←/→|前/后一个编辑的页面|
|F3|跳转到声明处|
|Ctrl+左键|查看方法的实现|（可用Alt+←返回）|
|Ctrl+F|查找后替换字符、单词等|
|Ctrl+H|搜索方法等|
**运行、调试**
|快捷键|说明|补充|
|---|---|---|
|Ctrl+F11|运行Run|
|F11|调试Debug|
|F5|单步跳入|
|F6|单步跳过|
|F7|单步返回|
|F8|继续|
|Ctrl+Shift+D|显示变量的值|
|Ctrl+Shift+B|在当前行设置或者去掉断点|
|Ctrl+R|运行至行|（超好用，可以节省好多的断点）|
**导航、重构、视图**
|快捷键|说明|补充|
|---|---|---|
|Ctrl+E|快速显示当前Editer的下拉列表|（如果当前页面没有显示的用黑体表示）|
|Alt+Shift+R|重构，重命名方法名、属性或者变量名|（重构的快捷键一般Alt+Shift+xx）|
|Alt+Shift+Z|撤销重构|（重构的后悔药）|
|Alt+Enter|显示当前选择资源的属性|windows下的查看文件的属性就是这个快捷键，通常用来查看文件在windows中的实际路径|
|Ctrl+M|最大化当前的Edit或View|（再按则反之）|
|Ctrl+L|文本编辑器,转至某行|
|Ctrl+O|快速显示|OutLine|
|Ctrl+T|快速显示当前类的继承结构|
|Ctrl+W|关闭当前Editer|（Windows下关闭打开的对话框、浏览器等都可以）|

