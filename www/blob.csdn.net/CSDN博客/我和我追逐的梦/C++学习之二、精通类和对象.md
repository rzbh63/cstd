
# C++学习之二、精通类和对象 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月27日 18:45:48[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1264标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[编译器																](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=class&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=class&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

**精通类和对象**
**本文是基于大家已经知道类和对象的基础上强化一些知识**

**对象中的动态分配内存：**
如果在对象中动态分配了内存，就应当在析构函数中释放该内存，所以此时需要自己编写析构函数。
**处理复制和赋值：**
如果没有自己编写复制构造函数和赋值操作符，C++会帮你生成一个，这些编译器生成的方法分别递归地调用对象成员上的复制构造函数或赋值操作符。不过对于基本类型（如int、double、指针），则会提供浅或位复制（赋值）：只是从源对象直接将数据成员复制或赋值到目标对象。如果在对象中动态分配了内存，就会带来问题。
针对上述问题：只要在类中动态地分配了内存，就应该编写自己的复制构造函数提供内存的深复制。在复制构造函数中要复制所有数据成员，而不只是指针成员。
对一个动态分配了内存的对象赋值时，因为它已经得到了初始化，所以需要自己编写赋值操作符，在分配新内存之前，需要对其已经分配的内存进行释放。
有时在类中动态分配内存时，最简单的做法就是禁止别人复制或对你的对象赋值，为此可以将复制构造函数或operator=标记为private，此时不必为这二者操作不提供实现。
综上所述：只要内需要动态分配内存，就需要自己编写复制构造函数、析构函数、赋值操作符。
**不同类型的数据成员：**
**静态数据成员：**与类关联而不是与对象关联的数据成员，它是属于类的，而不是属于某个对象的。
**const数据成员**：此数据成员一经创建和初始化就不能改动了。
当常量仅用于类时，应当使用static const 数据成员而不是全局常量。而且这些常量必须在源文件中为其声明空间，此时也是最后一次为其赋值的机会：
class SpreadsheetCell
{
。。。code。。。
public：
static const int kMaxHeight；
。。。code。。。
}；
。。。.cpp。。。
const int SpreadsheetCell：：kMaxHeight = 100；
如果statict const 成员变量是简单类型，C++标准允许在类的定义文件中声明这些变量并同时对其赋值。
**引用数据成员**：如果类的定义中需要一个引用数据成员，并且这个成员的类型是另外一个类，而且必须了解引用的类，而不是使用一个\#include，我们就可以使用使用类的超前引用。
class SpreadsheetApplication；//超前声明
class SpreadsheetCell
{
…code….
SpreadsheetApplication &app;
};
同时SpreadsheetCell的构造函数的初始化列表中必须为app初始化。在复制构造函数中也是如此。但是注意：一旦初始化了，就不能在修改它所引用的对象了。因此，不必再赋值操作符中尝试对此引用赋值。
**const 引用数据成员：**常规的引用可以指向const对象，类似的，引用成员也可以指向const对象 const SpreadsheetApplication &app;
还可以有static引用成员，static const 引用成员，不过一般很少使用。
**深入了解方法：**
**静态方法：**此方法应用于整个类，而不只是某个对象。在类外对其实现是，不需要加上关键字static。它不是在某一个特定的对象上调用，所以没有this指针，而且不能对一个特定对象执行来访问其非静态成员。实际上，静态方法就相当于常规函数。唯一的区别就在于，它可以访问类的private和protected静态数据成员。
切记：不要再静态方法中访问非静态成员。
调用时，需要使用作用域解析操作符。
**const 方法：**const对象就是值不能改变的对象。如果一个const对象或对const对象的应用，编译器就不允许你在改对象上调用任何方法，除非可以保证所调用的方法不会修改任何数据成员。要保证一个方法不会修改任何数据成员，具体做法就是将方法本身用const关键字来标记。如类的一个成员函数：string getString() const; 在类外定义此方法是也要加上const关键字。
如果一个方法声明为const，但它确实会修改数据成员，编译器就会报错。而且不能将静态方法声明为const，因为这是多余的，静态方法根本就没有相应的实例，因此它们不可能修改内部值。如果方法中每个数据成员都有一个const引用，此方法值为const就很有用，一旦修改就会报错。
非const对象可以调用const和非const方法，不过const对象只能调用const方法。
应当养成习惯，将所有不会修改对象的方法都声明为const方法，这样就可以在程序中使用const对象的引用。
注意，const对象也可以撤销，而且可以调用其析构函数。不要把析构函数标记为const。
如果在const方法中确实要修改某个数据成员，则可以在类定义中此数据成员前面加上mutable关键字。 如：mutable int value；
**方法重载：**
C++不允许仅基于返回类型重载一个方法名，因为在许多情况下，编译器无法确定要调用哪一个版本下的方法。
不过可以给予const重载一个方法。也就是说可以编写二个同名的且参数相同的方法，一个声明为const，另一个则不是。如果你提供一个const对象，则会调用const方法，如果你提供一个非const对象，则调用非const方法。
**默认参数：**
C++提供一个类似于方法重载的特性，称为默认参数。可以在原型中为函数和方法参数指定默认值。如果用户指定了实参，则忽略默认值。如果没有指定实参，则使用默认值。默认参数在构造函数中最有用。
注意：只能从最右参数开始的一个连续参数表提供默认参数。否则，编译器将无法将未提供的实参与默认值匹配。
注意：只是在方法声明中指定默认参数，在定义中并不指定。如果想同时声明一个默认构造函数，以及一个多参数默认构造函数且其所有参数都有默认值，则编译器会报错。因为如果没有指定任何参数，它就不知道该调用哪一个构造函数。
**内联方法：**
在C++中，你可以建议某个方法或函数调用实际上不应该当作方法或函数调用。实际上，编译器应当把这个方法或函数体直接插入到代码中方法或函数调用所在的位置。这个过程称为内联，需要由此表现得方法或函数称为内联方法或内联函数。这个过程只是\#define宏的一个更安全的版本。
在函数或方法定义中，通过将关键字inline置于其前，就可以指定一个内联方法和函数。
注意：所有需要调用内联方法或函数的源文件中，都应该提供内联方法和函数定义。因此，如果你编写了内联函数或方法，就应该在头文件中连同内联函数或方法的原型放上其定义。对于方法，这说明，要把方法定义放在包括类定义的.h文件中，这样做是很安全的。这就像是一个\#define宏。
C++中还提供了另一种语法来声明内联方法，其中根本不用inline关键字。具体做法，就是把方法定义直接放在类定义中。
需要提到的是：首先，方法能否成为内联方法，这有许多限制。编译器只会内联最简单的方法或函数。如果定义了一个内联方法或函数，但编译器不想内联此方法或函数，则可能会忽略内联指令，但不做任何提示。其次，内联可能导致代码量膨胀。因此应当谨慎使用内联。
**友元函数：**
C++允许类将其他类或非成员函数声明为友元函数（friend），而且这些友元可以访问protected和private数据成员和方法。不用在此函数定义前加上friend关键字。
**操作符重载：**
请见C++学习之四、重载C++操作符。


