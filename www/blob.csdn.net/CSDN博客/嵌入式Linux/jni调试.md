
# jni调试 - 嵌入式Linux - CSDN博客

2015年10月20日 11:38:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：642


１２年的时候写过ＪＮＩ但是又忘记得差不多了，现在重新写了一次，发现碰到了几个问题，写下来记录一下

## 第一步
应用程序java代码

```python
package com.example.helloworld;
import java.util.Calendar;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlarmManager;
import android.util.Log;

public class AlarmTest extends Activity
{
	AlarmManager aManager;
	Calendar currentTime = Calendar.getInstance();
	public static final int POWER_OFF_WAKE_UP = 8;//用来设置关机启动的参数　平台这边已经设置好了
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.e("weiqifa", test());
	}
	public native String  test();
	static {
		try{
			Log.i("JNI", "Trying to load libhelloworld.so");
			System.loadLibrary("helloWorld");
		}catch(UnsatisfiedLinkError ule){
			Log.e("JNI", "Warning : could not load the libhelloworld.so");
		}
	}
}
```
编译一下，可以是eclipse编译，也可以是java命令编译
编译后有下面的代码结构和目录
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ ls
AndroidManifest.xml**classes**classes.dex  dexedLibs  helloWorld.apk  res  resources.ap_
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$

## 第二步
生成共享头文件　这一步是最关键的关系到成功和失败

weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ javah -d header -classpath classes -jni com.example.helloworld.AlarmTest
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$

这个命令比较关键　javah -d header 指的是在当前目录下面生成这个头文件，-classpath是指定classes的目录就是第一步里面生成的

-jni指的是生成jni头文件　com.example.helloworld这个是包名　AlarmTest是类名　这些都不能有错

生成后如下
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ ls
AndroidManifest.xml  classes  classes.dex  dexedLibs  header  helloWorld.apk  res  resources.ap_
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ ls header/
com_example_helloworld_AlarmTest.h
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$

```python
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ cat header/com_example_helloworld_AlarmTest.h 
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_helloworld_AlarmTest */
#ifndef _Included_com_example_helloworld_AlarmTest
#define _Included_com_example_helloworld_AlarmTest
#ifdef __cplusplus
extern "C" {
#endif
#undef com_example_helloworld_AlarmTest_POWER_OFF_WAKE_UP
#define com_example_helloworld_AlarmTest_POWER_OFF_WAKE_UP 8L
/*
 * Class:     com_example_helloworld_AlarmTest
 * Method:    test
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_helloworld_AlarmTest_test
  (JNIEnv *, jobject);
#ifdef __cplusplus
}
#endif
#endif
```
刚开始不知道，运行经常出错如下：weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$ javah -jni com.example.helloworld
error: cannot access com.example.helloworld
class file for com.example.helloworld not found
javadoc: error - Class com.example.helloworld not found.
Error: No classes were specified on the command line.  Try -help.
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/bin$
可以看到自动生成对应的函数：Java_com_example_helloworld_AlarmTest_test
**Java_ +****包名（****com****_example_helloworld****）****+****类名****(****AlarmTest****) +****接口名****(****t****est****)****：必须要按此****JNI****规范来操作；**

## 第三步
通过.h来制作.c
看看我们的.c　我们只是需要.h里面那个函数名字，.h没有被引用，也没有参与编译和预编译
helloworld.c如下：

```python
#include <string.h>
#include <jni.h>

JNIEXPORT jstring JNICALL Java_com_example_helloworld_AlarmTest_test
  (JNIEnv *env, jobject thiz)
{
	return (*env)->NewStringUTF(env,"weiqifa hello for JNI");
}
```

## 第四步
生成so文件
看一下我们的mk文件

```python
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := helloWorld
LOCAL_SRC_FILES := helloWorld.c
include $(BUILD_SHARED_LIBRARY)
```
会生成如下的so文件：
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/libs/armeabi$ ls
libhelloWorld.so
weiqifa@weiqifa-Inspiron-3847:~/workspace/helloWorld/libs/armeabi$



## 第五步
执行验证
直接用eclipse安装　或者用　adb push 进去　用adb的话要手动安装so 文件并且改一下权限

10-20 03:06:45.241: E/weiqifa(12194):**weiqifa hello for JNI**
10-20 03:06:45.241: V/ActivityThread(12194): activityCreated r=ActivityRecord{41e7a890 token=android.os.BinderProxy@41e7a060 {com.example.helloworld/com.example.helloworld.AlarmTest}}

问题：
刚开始的时候，我在Android.mk里面写的LOCAL_MODULE生成的模块名称是大写W，helloWorld

```python
LOCAL_MODULE    := helloWorld
```
但是在android源码里面，我写的
```python
System.loadLibrary("helloWorld");
```
里面的helloworld是小写w，然后运行一直加载不成功，后面有个朋友帮我看了一下，找到原因，粗心大意啊。













