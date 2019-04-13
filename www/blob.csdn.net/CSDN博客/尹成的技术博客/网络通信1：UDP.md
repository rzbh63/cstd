
# 网络通信1：UDP - 尹成的技术博客 - CSDN博客

2018年11月16日 11:16:29[尹成](https://me.csdn.net/yincheng01)阅读数：167


\#UDP协议
\#\#\#1.简介
UDP（User Datagram Protocol），用户数据报协议，是OSI(Open System Interconnection，开放式系统互联) 参考模型中一种无连接的传输层协议，提供面向事务的简单不可靠信息传送服务，IETF RFC 768是UDP的正式规范。UDP提供了无连接通信，且不对传送数据包进行可靠性保证，适合于一次传输少量数据，UDP传输的可靠性由应用层负责。常用的UDP端口号有:
![这里写图片描述](https://img-blog.csdn.net/20180912233036606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
UDP报文没有可靠性保证、顺序保证和流量控制字段等，可靠性较差。但是正因为UDP协议的控制选项较少，在**数据传输过程中延迟小、数据传输效率高**，适合对可靠性要求不高的应用程序，或者可以保障可靠性的应用程序，如DNS、TFTP、SNMP等。
UDP在IP报文中的位置如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180912233217867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#\#2.UDP使用
在选择使用协议的时候，选择UDP必须要谨慎。在网络质量令人十分不满意的环境下，UDP协议数据包丢失会比较严重。但是由于UDP的特性:它不属于连接型协议，因而具有资源消耗小，处理速度快的优点，所以通常音频、视频和普通数据在传送时使用UDP较多，因为它们即使偶尔丢失一两个数据包，也不会对接收结果产生太大影响。比如我们聊天用的QQ就是使用的UDP协议。
既然UDP是一种不可靠的网络协议，那么还有什么使用价值或必要呢?其实不然，在有些情况下UDP协议可能会变得非常有用。因为UDP具有TCP所望尘莫及的速度优势。虽然TCP协议中植入了各种安全保障功能，但是在实际执行的过程中会占用大量的系统开销，无疑使速度受到严重的影响。反观UDP由于排除了信息可靠传递机制，将安全和排序等功能移交给上层应用来完成，极大降低了执行时间，使速度得到了保证。
关于UDP协议的最早规范是RFC768，1980年发布。尽管时间已经很长，但是UDP协议仍然继续在主流应用中发挥着作用。包括视频电话会议系统在内的许多应用都证明了UDP协议的存在价值。因为相对于可靠性来说，这些应用更加注重实际性能，所以为了获得更好的使用效果(例如，更高的画面帧刷新速率)往往可以牺牲一定的可靠性(例如，画面质量)。这就是UDP和TCP两种协议的权衡之处。根据不同的环境和特点，两种传输协议都将在今后的网络世界中发挥更加重要的作用。
\#\#\#3.UDP报头
UDP报头由4个域组成，其中每个域各占用2个字节，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180912233349806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
UDP协议使用端口号为不同的应用保留其各自的数据传输通道。UDP和TCP协议正是采用这一机制实现对同一时刻内多项应用同时发送和接收数据的支持。数据发送一方(可以是客户端或服务器端)将UDP数据包通过源端口发送出去，而数据接收一方则通过目标端口接收数据。有的网络应用只能使用预先为其预留或注册的静态端口;而另外一些网络应用则可以使用未被注册的动态端口。因为UDP报头使用两个字节存放端口号，所以端口号的有效范围是从0到65535。一般来说，大于49151的端口号都代表动态端口。
数据报的长度是指包括报头和数据部分在内的总字节数。因为报头的长度是固定的，所以该域主要被用来计算可变长度的数据部分(又称为数据负载)。数据报的最大长度根据操作环境的不同而各异。从理论上说，包含报头在内的数据报的最大长度为65535字节。不过，一些实际应用往往会限制数据报的大小，有时会降低到8192字节。（对于一次发送多少字节比较好，后面会讲到）
UDP协议使用报头中的校验值来保证数据的安全。校验值首先在数据发送方通过特殊的算法计算得出，在传递到接收方之后，还需要再重新计算。如果某个数据报在传输过程中被第三方篡改或者由于线路噪音等原因受到损坏，发送和接收方的校验计算值将不会相符，由此UDP协议可以检测是否出错。这与TCP协议是不同的，后者要求必须具有校验值。
许多链路层协议都提供错误检查，包括流行的以太网协议，也许你想知道为什么UDP也要提供检查和校验。其原因是链路层以下的协议在源端和终端之间的某些通道可能不提供错误检测。虽然UDP提供有错误检测，但检测到错误时，UDP不做错误校正，只是简单地把损坏的消息段扔掉，或者给应用程序提供警告信息。
\#\#\#UDP特性
UDP是一个无连接协议，传输数据之前源端和终端不建立连接，当UDP它想传送时就简单地去抓取来自应用程序的数据，并尽可能快地把它扔到网络上。在发送端，UDP传送数据的速度仅仅是受应用程序生成数据的速度、计算机的能力和传输带宽的限制;在接收端，UDP把每个消息段放在队列中，应用程序每次从队列中读一个消息段。
由于传输数据不建立连接，因此也就不需要维护连接状态，包括收发状态等，因此一台服务机可同时向多个客户机传输相同的消息。
UDP信息包的标题很短，只有8个字节，相对于TCP的20个字节信息包的额外开销很小。
吞吐量不受拥挤控制算法的调节，只受应用软件生成数据的速率、传输带宽、源端和终端主机性能的限制。
UDP使用尽最大努力交付，即不保证可靠交付，因此主机不需要维持复杂的链接状态表(这里面有许多参数)。
UDP是面向报文的。发送方的UDP对应用程序交下来的报文，在添加首部后就向下交付给IP层。既不拆分，也不合并，而是保留这些报文的边界，因此，应用程序需要选择合适的报文大小。
\#\#\#实例
\#\#\#\#服务端实现
import (
	"os"
	"fmt"
	"net"
)
//处理错误信息
func checkError(err error) {
	if err != nil { //指针不为空
		fmt.Println("Error", err.Error())
		os.Exit(1)
	}
}
func receiveUDPMsg(udpConn *net.UDPConn) {
	//声明30字节的缓冲区
	buffer := make([]byte, 30)
	//从udpConn读取客户端发送过来的数据，放在缓冲区中(阻塞方法)
	//返回值：n=读到的字节长度,remoteAddr=客户端地址,err=错误
	n, remoteAddr, err := udpConn.ReadFromUDP(buffer) //从udp接收数据
	if err != nil {
		fmt.Println("Error", err.Error()) //打印错误信息
		return
	}
	fmt.Printf("接收到来自%v的消息：%s", remoteAddr,string(buffer[0:n]))
	//向远程地址中写入数据
	_, err = udpConn.WriteToUDP([]byte("hao nimei"), remoteAddr)
	checkError(err)
}
func main() {
	//解析IP和端口得到UDP地址
	udp_addr, err := net.ResolveUDPAddr("udp", "127.0.0.1:8848")
	checkError(err)
	//在解析得到的地址上建立UDP监听
	udpConn, err := net.ListenUDP("udp", udp_addr)
	defer udpConn.Close() //关闭链接
	checkError(err)
	//从udpConn中接收UDP消息
	receiveUDPMsg(udpConn) //收消息
}\#\#\#\#客户端实现
import (
	"net"
	"fmt"
	"os"
)
func main() {
	//请求连接服务器，得到连接对象
	conn,err:=net.Dial("udp","127.0.0.1:8848")
	defer  conn.Close()
	if err!=nil{
		fmt.Println("网络连接出错")
		os.Exit(1)
	}
	//向连接中写入消息
	conn.Write([]byte("hello nimei"))
	fmt.Println("发送消息","hello nimei")
	//读取代表收取消息(阻塞)
	buffer := make([]byte, 30)
	n, err := conn.Read(buffer)
	if err!=nil{
		fmt.Println("读取消息错误：err=",err)
		os.Exit(1)
	}
	fmt.Println("收到消息",string(buffer[:n]))
}学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116111614928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

