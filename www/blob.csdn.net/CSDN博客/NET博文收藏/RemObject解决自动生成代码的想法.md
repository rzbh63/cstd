
# RemObject解决自动生成代码的想法. - .NET博文收藏 - CSDN博客


2008年06月10日 12:52:00[hejishan](https://me.csdn.net/hejishan)阅读数：1965


用过DELPHI写过多层框架的,可能能RemObject比较熟悉. RemObjects Service Builder 自动生成代码的同时,也给我们带来困扰. 每个类都要定义在intf文件里, 接口只有一个, 这些显然对我们开发系统来讲, 支持的不够, 我想使用多个接口,也不想没完没了的定义结构.
前一段时间写了一个自动生成元数据的小工具,  可以和界面StringGrid和控件自动绑定,实现数据集的效果,而且在客户端完全放弃了数据集, 使用纯对象,  自己觉得还算不错. 然而在传输的时候, 我又想使用RemObject支持序列化的功能.  这个问题如何解决呢?
后来我想摸索之后发现, 可以通过下面的方法来进行改进.
1. RemObject的工程改为普通工程,  把定义的元数据分离出来, 在interface下面uses一下.
2. 如果直接传递对象, 接口和代码怎么解决?　（因为分离了元数据对象单元，所以编译的时候，不会生成代码．　解决问题的办法在于，RPCServerLibrary. Rodl文件．　正是因为这个文件，所以RemObjects Service Builder 自动生成代码根据其ＸＭＬ文件进行解析，最后生成代码．
3. 方法定义的地方：



4. 结构体定义的地方：






有了上面的描述，　你应该想到怎么办了吧，对，就是在这里动手，前面我们不是自己做过动态生成元数据代码吗？　现在只要在其中修改这个ＲＯＤＬ文件，把元数据和方法加进去，　然后编译的时候，RemObjects Service Builder 就会自动帮我们生成代码了．
最后，我们把inter文件（自动生成）改一下．删除元数据的声明，建一个新的工程，把所有代码拷进去．呵呵，大功告成．




