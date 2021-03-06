# TCP的超时重传之深入了解RTT与RTO

2018年07月09日 17:09:24 [whgtheone](https://me.csdn.net/whgtheone) 阅读数：5088



TCP提供一种面向连接的、可靠的字节流服务，其中可靠的保证方法之一就是却让从另一端收到的数据。但是数据和确认信号都有可能丢失，。TCP通过在发送数据时设置一个**重传定时器**（注意这里的超时定时器和第四节讲的定时器不一样）来监控数据的丢失状态，如果重传定时器溢出时还没收到确认信号，则重传该数据。这就是建立重传机制的原因。 
下面解释两个名词： 
**RTT(Round Trip Time)：**一个连接的往返时间，即数据发送时刻到接收到确认的时刻的差值； 
**RTO(Retransmission Time Out)：**重传超时时间，即从数据发送时刻算起，超过这个时间便执行重传。 
RTT和RTO 的关系是：由于网络波动的不确定性，每个RTT都是动态变化的，所以RTO也应随着RTT动态变化。

## 一、RTT估计器

首先要明白一个问题，为什么要做一个RTT估计器？在开篇就讲到了TCP连接在重传定时器溢出就会重传数据。那么溢出时间怎么计算，即RTO如何计算？如果设置过短，则会造成重传频繁，加快网络阻塞；设置过长，则会导致性能下降（失序的报文段得不到确认，接收方无法提交给进程）。所以，超时计算的算法应该能够反映当前网络的拥塞情况，而每个连接的RTT恰恰能够反映这一点，所以设计好的RTT估计器是计算 RTO 的第一步。 
由于大多数的TCP实现仅仅在某一时刻为一个已发送但尚未确认的报文段做一次RTT采样，得到一个SampleRTT，而不是为每一个发送的报文段都测量RTT，从而用这个SampleRTT来接近（代表）所有RTT。

- 一个连接中，有且仅有一个测量定时器被使用。也就是说，如果TCP连续发出3组数据，只有一组数据会被测量。
- TCP决不会为已被重传的报文段测量SampleRTT，仅仅为传输一次的报文段测量SampleRTT。
- ACK数据报不会被测量，原因很简单，没有ACK的ACK回应可以供结束定时器测量。

由于路由器的拥塞和端系统负载的变化，由于这种波动，用一个报文段所测的SampleRTT来代表同一段时间内的RTT总是非典型的，为了得到一个典型的RTT，TCP规范中使用低通过滤器来更新一个被平滑的RTT估计器。TCP维持一个估计RTT（称之为EstimatedRTT），一旦获得一个新SampleRTT时，则根据下式来更新EstimatedRTT： 
EstimatedRTT = （1-a）* EstimatedRTT + a * SampleRTT 
其中a通常取值为0.125，即：

```
EstimatedRTT = 0.875 * EstimatedRTT + 0.125 * SampleRTT
```

每个新的估计值的87.5%来自前一个估计值，而12.5%则取自新的测量。

**关于估计器的更新：** 
在一个报文段被发送和确认之前阻止更新估计器。

## 二、RTT的方差跟踪

在最初的RTO算法中，RTO等于一个值为2的时延离散因子与RTT估计值的乘积，即：

```
RTO = 2*EstimatedRTT
```

但这种做法有个很大的缺陷，就是在RTT变化范围很大的时候，使用这个方法无法跟上这种变化，从而引起不必要的重传。怎么理解呢？由于新测量SampleRTT的权值只占EstimatedRTT的12.5%，当实际RTT变化很大的时候，即便测量到的SampleRTT变化也很大，但是所占比重小，最后EstimatedRTT的变化也不大，从而RTO的变化不大，造成RTO过小，容易引起不必要的重传。因此对RTT的方差跟踪则显得很有必要。 
在TCP规范中定义了RTT偏差DevRTT，用于估算SampleRTT一般会偏离EstimatedRTT的程度：

```
DevRTT = (1-B)*DevRTT + B*|SampleRTT - EstimatedRTT|
```

其中B的推荐值为0.25，当RTT波动很大的时候，DevRTT的就会很大。

## 三、设置重传时间间隔RTO

如上面所述得到了EstimatedRTT和DevRTT，很明显超时时间间隔RTO应该大于等于EstimatedRTT，但要大多少才比较合适呢？所以选择DevRTT作为余量，当波动大时余量大，波动小时，余量小。则组后超时重传时间间隔RTO的计算公式为：

```
RTO = EstimatedRTT + 4 * DevRTT
```

在[RFC 6298]中，推荐初始超时重传时间为1秒，当出现超时后，超时重传时间将以指数退避的方法加倍，以免即将被确认的后继报文段过早出现超时。不管如何，一旦报文段收到并更新EstimatedRTT 后，超时重传时间便会按上式计算。具体阐述如下： 
**超时间隔加倍：** 
假设当前超时重传定时器溢出时，与最早的未被确认的报文段相关联的RTO为0.75s，TCP就会重传报文段，并版新的RTO设置为1.5s，如果1.5s后又溢出了，则TCP将再次重传报文段，并把RTO设置为3秒。因此，超时间隔在每次重传后会呈指数上升，然而每当重传定时器在另外两个事件（收到上层应用的数据和收到ACK）中的任意一个启动时，RTO有最近的EstimatedRTT 和DevRTT重新计算。

## 四、实际RTT和RTO测量

首先在这里要区分重传定时器和TCP连接可供调用的时钟定时器。

- 重传定时器：当TCP发送报文段时，就创建这个特定报文段的重传定时器，若在定时器超时之前收到对报文段的确认，则撤销定时器；若在收到对特定报文段的确认之前计时器超时，则重传该报文，并且进行RTO = 2 * RTO进行退避。
- 可供调用的TCP时钟定时器：一个TCP连接只有一个这样的定时器用于测量RTT，一般情况下是500ms定时器，并且只能够被一个报文段占用，即在发送一个报文段时，如果给定连接的定时器已经被使用，则该报文段不被计时（不计算该报文段的RTT，这就解释了不是所有报文段都能被计算RTT）。（这里的定时器应该是调用系统时钟）

![分组交换和RTT测量](https://img-blog.csdn.net/20180709170418569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3doZ3RoZW9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
好，回到RTT的测量上来。实际中RTT的测量并不是那么精确，比如说一个报文段的确认信号在它发送550ms后到达，那么该报文段的往返时间RTT将是500ms或者1000ms（假设使用500ms定时器计时）。为什么会这样呢？因为在调用500ms定时器的同时会增加一个计数器来辅助计时，计数器的每一个滴答（tick）代表定时器转好一圈500ms，两个滴答则表示1000ms。可惜的是定时器和计数器做不到无误差的同时启动，往往计数器会在定时器启动的一段时间内启动，但间隔很小。 
![RTT测量和时钟滴答](https://img-blog.csdn.net/20180709163450113?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3doZ3RoZW9uZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从图上可以看到时间间隔为500ms的时钟滴答，报文段1在0处发出（同时定时器开始启动）假设计数器在定时器启动0.03s后开始计数，报文段1 的确认在1.061s出收到，那么在发送报文段1和接收到报文段1的确认信号之间经历了3个滴答，所以报文段1的RTT为1500ms。之后的RTT也是这么计算。所以说得到的实际RTT总是500ms的倍数。

在公式初始化重传超时公式的时候中，常常使用：

`RTO = EstimatedRTT + 2 * DevRTT`只有在初始化的时候系数为2，之后的系数仍为4。

EstimatedRTT初始化为0，DevRTT的初始值随TCP版本实现不同而定。而后每次根据测量的RTT对RTO进行更新。实际上RTO的值也基本上是500ms的倍数，这是由于通常情况下余量的波动并不会很大。