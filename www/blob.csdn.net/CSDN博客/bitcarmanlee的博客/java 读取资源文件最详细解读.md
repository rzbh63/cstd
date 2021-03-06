
# java 读取资源文件最详细解读 - bitcarmanlee的博客 - CSDN博客


2017年05月01日 18:27:10[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：10346


java项目中，经常需要将资源文件打包放在项目中，然后在项目中去读取对应的文件。以下面的一个项目为例，项目的目录结构如下：
```python
├── conf
│   └── krb5
```
```python
.conf
```
```python
├── pom
```
```python
.xml
```
```python
├── src
│   ├── main
│   │   ├── java
│   │   ├── resources
│   │   ├── thrift
│   │   └── thrift-java
│   └── test
│       └── java
└── thriftdemo
```
```python
.iml
```
这是一个maven项目，maven项目默认的结构中，resource位于main目录下面。现在我们希望读取到外面conf目录下的krb5.conf配置文件，该怎么办呢。
## 1.使用ClassLoader.getResource定位资源文件
为了达到读取并解析krb5.conf的目的，我们首先写了一段简单的测试代码。
```python
@Test
```
```python
public
```
```python
void
```
```python
t1
```
```python
() {
        String path = Demo.class.getClassLoader().getResource(
```
```python
""
```
```python
).getPath();
        System.
```
```python
out
```
```python
.println(path);
        String filestr = path +
```
```python
"/conf/krb5.conf"
```
```python
;
```
```python
try
```
```python
{
            BufferedReader br =
```
```python
new
```
```python
BufferedReader(
```
```python
new
```
```python
FileReader(
```
```python
new
```
```python
File(filestr)));
```
```python
while
```
```python
(br.readLine() !=
```
```python
null
```
```python
) {
                System.
```
```python
out
```
```python
.println(br.readLine());
            }
        }
```
```python
catch
```
```python
(FileNotFoundException e) {
            e.printStackTrace();
        }
```
```python
catch
```
```python
(IOException e) {
            e.printStackTrace();
        }
    }
```
对于这段代码，首先我们需要清楚的一点是：ClassLoader.getResource方法的入参必须是从根目录开始查找,这里根目录就是target里的classes!
如果我们代码正常运行起来以后，打印出来的路径为：
```python
/xxx/
```
```python
...
```
```python
/target/classes/
```
在使用ClassLoader.getResource的时候，注意查找必须是基于根目录，例如在Intellij中就是`target/classes`。并且目录结构也需要写对，不能以”/”开头。
## 2.Class.getResource也能定位资源文件
如果我们不使用ClassLoader.getResource的方式，而是用Class.getResource的方法，也能定位到资源文件。代码如下：
```python
@Test
    public void t2() {
        String path = Demo1
```
```python
.class
```
```python
.getResource
```
```python
(
```
```python
""
```
```python
)
```
```python
.getPath
```
```python
()
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(path)
```
```python
;
```
```python
String path2 = Demo1
```
```python
.class
```
```python
.getResource
```
```python
(
```
```python
"/"
```
```python
)
```
```python
.getPath
```
```python
()
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(path2)
```
```python
;
```
```python
}
```
看一下代码的输出结果：
```python
/xxx/
```
```python
...
```
```python
/target/classes/edu/bit/files/
/xxx/
```
```python
...
```
```python
/target/classes/
```
需要注意的就是，如果使用Class.getResource方法，入参为空的时候，是当前.class文件的路径；而当入参为”/”的时候，是从根目录开始查找！
## 3.maven中资源文件的配置
我们希望最终打包的结果为如下的结构：
```python
classes/
├── conf
│   └── krb5
```
```python
.conf
```
```python
├── demo
```
```python
.thrift
```
```python
└── edu
    └── bit
```
构建Maven项目的时候，如果没有进行特殊的配置，Maven会按照标准的目录结构查找和处理各种类型文件。
### src/main/java和src/test/java
这两个目录中的所有*.java文件会分别在comile和test-comiple阶段被编译，编译结果分别放到了target/classes和targe/test-classes目录中，但是这两个目录中的其他文件都会被忽略掉。
### src/main/resouces和src/test/resources
这两个目录中的文件也会分别被复制到target/classes和target/test-classes目录中。
### target/classes
打包插件默认会把这个目录中的所有内容打入到jar包或者war包中。
我们现在想把conf下的kbr5.conf放在conf路径下，这就需要我们在pom.xml中进行一些自定义的配置了。主要有两种方式，下面我们来一一介绍。
## 4.在pom.xml的build的plugins子元素中配置maven-resources-plugin插件。
```python
<
```
```python
plugin
```
```python
>
```
```python
<
```
```python
artifactId
```
```python
>
```
```python
maven-resources-plugin
```
```python
</
```
```python
artifactId
```
```python
>
```
```python
<
```
```python
version
```
```python
>
```
```python
2.5
```
```python
</
```
```python
version
```
```python
>
```
```python
<
```
```python
executions
```
```python
>
```
```python
<
```
```python
execution
```
```python
>
```
```python
<
```
```python
id
```
```python
>
```
```python
copy-conf
```
```python
</
```
```python
id
```
```python
>
```
```python
<
```
```python
phase
```
```python
>
```
```python
process-sources
```
```python
</
```
```python
phase
```
```python
>
```
```python
<
```
```python
goals
```
```python
>
```
```python
<
```
```python
goal
```
```python
>
```
```python
copy-resources
```
```python
</
```
```python
goal
```
```python
>
```
```python
</
```
```python
goals
```
```python
>
```
```python
<
```
```python
configuration
```
```python
>
```
```python
<
```
```python
outputDirectory
```
```python
>
```
```python
${basedir}/target/classes/conf
```
```python
</
```
```python
outputDirectory
```
```python
>
```
```python
<
```
```python
resources
```
```python
>
```
```python
<
```
```python
resource
```
```python
>
```
```python
<
```
```python
directory
```
```python
>
```
```python
${project.basedir}/conf
```
```python
</
```
```python
directory
```
```python
>
```
```python
<
```
```python
includes
```
```python
>
```
```python
<
```
```python
include
```
```python
>
```
```python
**/*.xml
```
```python
</
```
```python
include
```
```python
>
```
```python
<
```
```python
include
```
```python
>
```
```python
**/*.conf
```
```python
</
```
```python
include
```
```python
>
```
```python
</
```
```python
includes
```
```python
>
```
```python
</
```
```python
resource
```
```python
>
```
```python
</
```
```python
resources
```
```python
>
```
```python
</
```
```python
configuration
```
```python
>
```
```python
</
```
```python
execution
```
```python
>
```
```python
</
```
```python
executions
```
```python
>
```
```python
</
```
```python
plugin
```
```python
>
```
其中，directory标签配置的是resource文件的源路径，outputDirectory配置的是resource文件的输出路径。因为我们想将资源文件放置在classes的conf目录下面，所以按照上面的配置方式即可。
## 5.配置resources节点
```python
<
```
```python
resources
```
```python
>
```
```python
<
```
```python
resource
```
```python
>
```
```python
<
```
```python
directory
```
```python
>
```
```python
${basedir}/conf
```
```python
</
```
```python
directory
```
```python
>
```
```python
<
```
```python
targetPath
```
```python
>
```
```python
${basedir}/target/classes/conf
```
```python
</
```
```python
targetPath
```
```python
>
```
```python
</
```
```python
resource
```
```python
>
```
```python
</
```
```python
resources
```
```python
>
```
在resources节点里做上面的配置，也能达到上面同样的效果！

