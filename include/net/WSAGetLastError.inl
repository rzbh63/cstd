WSA_ERROR_DEF(6, WSA_INVALID_HANDLE, 指定的事件对象无效。若使用与Win32函数对应的Winsock函数，便有可能产生这样的Win32错误。它表明传递给WSAWaitForMultipleEvents的一个句柄是无效的。)
WSA_ERROR_DEF(8, WSA_NOT_ENOUGH_MEMORY, 内存不够。这个Win32错误指出内存数量不足，无法完成指定的操作。)
WSA_ERROR_DEF(87, WSA_INVALID_PARAMETER, 一个或多个参数无效。这个Win32错误表明传递到函数内部的参数无效。假若事件计数参数无效，那么在执行WSAWaitForMultipleEvents的时候，也会发生这样的错误。)
WSA_ERROR_DEF(258, WSA_WAIT_TIMEOUT, 操作超时。这个Win32错误指出重叠I / O操作未在规定的时间内完成。)
WSA_ERROR_DEF(995, WSA_OPERATION_ABORTED, 重叠操作被取消。这个Win32错误指出由于套接字的关闭，造成一次重叠I / O操作的取消。除此以外，该错误也可能在执行SIO_FLUSH这个I / O控制命令时出现。)
WSA_ERROR_DEF(996, WSA_IO_INCOMPLETE, 重叠I / O事件对象未处于传信状态。这个Win32错误也和重叠I / O操作密切相关，在调用WSAGetOverlappedResults函数的时候产生，指出重叠I / O操作尚未完成。)
WSA_ERROR_DEF(997, WSA_IO_PENDING, 重叠操作将在以后完成。用Winsock函数发出一次重叠I / O操作时，若出现这样的Win32错误，便表明操作尚未完成，而且会在以后的某个时间完成。)
WSA_ERROR_DEF(10004, WSAEINTR, 函数调用中断。该错误表明由于对WSACancelBlockingCall的调用，造成了一次调用被强行中断。)
WSA_ERROR_DEF(10009, WSAEBADF, 文件句柄错误。该错误表明提供的文件句柄无效。在MicrosoftWindowsCE下，socket函数可能返回这个错误，表明共享串口处于“忙”状态。)
WSA_ERROR_DEF(10013, WSAEACCES, 权限被拒。尝试对套接字进行操作，但被禁止。若试图在sendto或WSASendTo中使用一个广播地址，但是尚未用setsockopt和SO_BROADCAST这两个选项设置广播权限，便会产生这类错误。)
WSA_ERROR_DEF(10014, WSAEFAULT, 地址无效。传给Winsock函数的指针地址无效。若指定的缓冲区太小，也会产生这个错误。)
WSA_ERROR_DEF(10022, WSAEINVAL, 参数无效。指定了一个无效参数。例如，假如为WSAIoctl调用指定了一个无效控制代码，便会产生这个错误。另外，它也可能表明套接字当前的状态有错，例如在一个没有监听的套接字上调用accept或WSAAccept的时候。)
WSA_ERROR_DEF(10024, WSAEMFILE, 打开文件过多。提示打开的套接字太多了。通常，Microsoft提供者只受到系统内可用资源数量的限制。)
WSA_ERROR_DEF(10035, WSAEWOULDBLOCK, 资源暂时不可用。对非锁定套接字来说，如果请求操作不能立即执行的话，通常会返回这个错误。比如说，在一个非暂停套接字上调用connect，就会返回这个错误。因为连接请求不能立即执行。)
WSA_ERROR_DEF(10036, WSAEINPROGRESS, 操作正在进行中。当前正在执行非锁定操作。一般来说不会出现这个错误，除非正在开发16位Winsock应用程序。)
WSA_ERROR_DEF(10037, WSAEALREADY, 操作已完成。一般来说，在非锁定套接字上尝试已处于进程中的操作时，会产生这个错误。比如，在一个已处于连接进程的非锁定套接字上，再一次调用connect或WSAConnect。另外，服务提供者处于执行回调函数（针对支持回调例程的Winsock函数）的进程中时也会出现这个错误。)
WSA_ERROR_DEF(10038, WSAENOTSOCK, 无效套接字上的套接字操作。任何一个把SOCKET句柄当作参数的Winsock函数都会返回这个错误。它表明提供的套接字句柄无效。)
WSA_ERROR_DEF(10039, WSAEDESTADDRREQ, 需要目标地址。这个错误表明没有提供具体地址。比方说，假如在调用sendto时，将目标地址设为INADDR_ANY（任意地址），便会返回这个错误。)
WSA_ERROR_DEF(10040, WSAEMSGSIZE, 消息过长。这个错误的含义很多。如果在一个数据报套接字上发送一条消息，这条消息对内部缓冲区而言太大的话，就会产生这个错误。再比如，由于网络自身的限制，使一条消息过长，也会产生这个错误。最后，如果收到数据报之后，缓冲区太小，不能接收消息时，也会产生这个错误。)
WSA_ERROR_DEF(10041, WSAEPROTOTYPE, 套接字协议类型有误。在socket或WSASocket调用中指定的协议不支持指定的套接字类型。比如，要求建立SOCK_STREAM类型的一个IP套接字，同时指定协议为IPPROTO_UDP，便会产生这样的错误。)
WSA_ERROR_DEF(10042, WSAENOPROTOOPT, 协议选项错误。表明在getsockopt或setsockopt调用中，指定的套接字选项或级别不明、未获支持或者无效。)
WSA_ERROR_DEF(10043, WSAEPROTONOSUPPORT, 不支持的协议。系统中没有安装请求的协议或没有相应的实施方案。比如，如果系统中没有安装TCP / IP，而试着建立TCP或UDP套接字时，就会产生这个错误。)
WSA_ERROR_DEF(10044, WSAESOCKTNOSUPPORT, 不支持的套接字类型。对指定的地址家族来说，没有相应的具体套接字类型支持。比如，在向一个不支持原始套接字的协议请求建立一个SOCK_RAW套接字类型时，就会产生这个错误。)
WSA_ERROR_DEF(10045, WSAEOPNOTSUPP, 不支持的操作。表明针对指定的对象，试图采 ? 〉牟僮魑椿裰С帧Ｍǔ＃绻宰旁谝桓霾恢С值饔肳insock函数的套接字上调用了Winsock时，就会产生这个错误。比如，在一个数据报套接字上调用accept或WSAAccept函数时，就会产生这样的错误。)
WSA_ERROR_DEF(10046, WSAEPFNOSUPPORT, 不支持的协议家族。请求的协议家族不存在，或系统内尚未安装。多数情况下，这个错误可与WSAEAFNOSUPPORT互换（两者等价）；后者出现得更为频繁。)
WSA_ERROR_DEF(10047, WSAEAFNOSUPPORT, 地址家族不支持请求的操作。对套接字类型不支持的操作来说，在试着执行它时，就会出现这个错误。比如，在类型为SOCK_STREAM的一个套接字上调用sendto或WSASendTo函数时，就会产生这个错误。另外，在调用socket或WSASocket函数的时候，若同时请求了一个无效的地址家族、套接字类型及协议组合，也会产生这个错误。)
WSA_ERROR_DEF(10048, WSAEADDRINUSE, 下地址正在使用。正常情况下，每个套接字只允许使用一个套接字地址（例如，一个IP套接字地址由本地IP地址及端口号组成）。这个错误一般和bind、connect和WSAConnect这三个函数有关。可在setsockopt函数中设置套接字选项SO_REUSEADDR，允许多个套接字访问同一个本地IP地址及端口号。)
WSA_ERROR_DEF(10049, WSAEADDRNOTAVAIL, 不能分配请求的地址。API调用中指定的地址对那个函数来说无效时，就会产生这样的错误。例如，若在bind调用中指定一个IP地址，但却没有对应的本地IP接口，便会产生这样的错误。另外，通过connect、WSAConnect、sendto、WSASendTo和WSAJoinLeaf这四个函数为准备连接的远程计算机指定端口0时，也会产生这样的错误。)
WSA_ERROR_DEF(10050, WSAENETDOWN, 网络断开。试图采取一项操作时，却发现网络连接中断。这可能是由于网络堆栈的错误，网络接口的故障，或者本地网络的问题造成的。)
WSA_ERROR_DEF(10051, WSAENETUNREACH, 网络不可抵达。试图采取一项操作时，却发现目标网络不可抵达（不可访问）。这意味着本地主机不知道如何抵达一个远程主机。换言之，目前没有已知的路由可抵达那个目标主机。)
WSA_ERROR_DEF(10052, WSAENETRESET, 网络重设时断开了连接。由于“保持活动”操作检测到一个错误，造成网络连接的中断。若在一个已经无效的连接之上，通过setsockopt函数设置SO_KEEPALIVE选项，也会出现这样的错误。)
WSA_ERROR_DEF(10053, WSAECONNABORTED, 软件造成连接取消。由于软件错误，造成一个已经建立的连接被取消。典型情况下，这意味着连接是由于协议或超时错误而被取消的。)
WSA_ERROR_DEF(10054, WSAECONNRESET, 连接被对方重设。一个已经建立的连接被远程主机强行关闭。若远程主机上的进程异常中止运行（由于内存冲突或硬件故障），或者针对套接字执行了一次强行关闭，便会产生这样的错误。针对强行关闭的情况，可用SO_LINGER套接字选项和setsockopt来配置一个套接字。)
WSA_ERROR_DEF(10055, WSAENOBUFS, 没有缓冲区空间。由于系统缺少足够的缓冲区空间，请求的操作不能执行。)
WSA_ERROR_DEF(10056, WSAEISCONN, 套接字已经连接。表明在一个已建立连接的套接字上，试图再建立一个连接。要注意的是，数据报和数据流套接字均有可能出现这样的错误。使用数据报套接字时，假如事先已通过connect或WSAConnect调用，为数据报通信关联了一个端点的地址，那么以后试图再次调用sendto或WSASendTo，便会产生这样的错误。)
WSA_ERROR_DEF(10057, WSAENOTCONN, 套接字尚未连接。若在一个尚未建立连接的“面向连接”套接字上发出数据收发请求，便会产生这样的错误。)
WSA_ERROR_DEF(10058, WSAESHUTDOWN, 套接字关闭后不能发送。表明已通过对shutdown的一次调用，部分关闭了套接字，但事后又请求进行数据的收发操作。要注意的是，这种错误只会在已经关闭的那个数据流动方向上才会发生。举个例子来说，完成数据发送后，若调用shutdown，那么以后任何数据发送调用都会产生这样的错误。)
WSA_ERROR_DEF(10060, WSAETIMEDOUT, 连接超时。若发出了一个连接请求，但经过规定的时间，远程计算机仍未作出正确的响应（或根本没有任何响应），便会发生这样的错误。要想收到这样的错误，通常需要先在套接字上设置好SO_SNDTIMEO和SO_RCVTIMEO选项，然后调用connect及WSAConnect函数。要想了解在套接字上设置SO_SNDTIMEO和SO_RCVTIMEO选项的详情，可参考第9章。)
WSA_ERROR_DEF(10061, WSAECONNREFUSED, 连接被拒。由于被目标机器拒绝，连接无法建立。这通常是由于在远程机器上，没有任何应用程序可在那个地址之上，为连接提供服务。)
WSA_ERROR_DEF(10064, WSAEHOSTDOWN, 主机关闭。这个错误指出由于目标主机关闭，造成操作失败。然而，应用程序此时更有可能收到的是一条WSAETIMEDOUT（连接超时）错误，因为对方关机的情况通常是在试图建立一个连接的时候发生的。)
WSA_ERROR_DEF(10065, WSAEHOSTUNREACH, 没有到主机的路由。应用程序试图访问一个不可抵达的主机。该错误类似于WSAENETUNREACH。)
WSA_ERROR_DEF(10067, WSAEPROCLIM, 进程过多。有些Winsock服务提供者对能够同时访问它们的进程数量进行了限制。)
WSA_ERROR_DEF(10091, WSASYSNOTREADY, 网络子系统不可用。调用WSAStartup时，若提供者不能正常工作（由于提供服务的基层系统不可用），便会返回这种错误。)
WSA_ERROR_DEF(10092, WSAVERNOTSUPPORTED, Winsock.dll版本有误。表明不支持请求的Winsock提供者版本。)
WSA_ERROR_DEF(10093, WSANOTINITIALISED, Winsock尚未初始化。尚未成功完成对WSAStartup的一次调用。)
WSA_ERROR_DEF(10101, WSAEDISCON, 正在从容关闭。这个错误是由WSARecv和WSARecvFrom返回的，指出远程主机已初始化了一次从容关闭操作。该错误是在像ATM这样的“面向消息”协议上发生的。)
WSA_ERROR_DEF(10102, WSAENOMORE, 找不到更多的记录。这个错误自WSALookupServiceNext函数返回，指出已经没有留下更多的记录。这个错误通常可与WSA_E_NO_MORE互换使用。在应用程序中，应同时检查这个错误以及WSA_E_NO_MORE。)
WSA_ERROR_DEF(10103, WSAECANCELLED, 操作被取消。这个错误指出当WSALookupServiceNext调用仍在处理期间，发出了对WSALookupServiceEnd（服务中止）的一个调用。此时，WSALookupServiceNext便会返回这个错误。这个错误代码可与WSA_E_CANCELLED互换使用。作为应用程序，应同时检查这个错误以及WSA_E_CANCELLED)
WSA_ERROR_DEF(10105, WSAEINVALIDPROVIDER, 无效的服务提供者。这个错误同服务提供者关联在一起，在提供者不能建立正确的Winsock版本，从而无法正常工作的前提下产生。)
WSA_ERROR_DEF(10106, WSAEPROVIDERFAILEDINIT, 提供者初始化失败。这个错误同服务提供者关联在一起，通常见于提供者不能载入需要的DLL时。)
WSA_ERROR_DEF(10107, WSASYSCALLFAILURE, 系统调用失败。表明绝对不应失败的一个系统调用却令人遗憾地失败了。)
WSA_ERROR_DEF(10108, WSASERVICE_NOT_FOUND, 找不到这样的服务。这个错误通常与注册和名字解析函数相关，在查询服务时产生（第10章对这些函数进行了详尽解释）。该错误表明，在给定的名字空间内，找不到请求的服务。)
WSA_ERROR_DEF(10109, WSATYPE_NOT_FOUND, 找不到类的类型。该错误也与注册及名字解析函数关联在一起，在处理服务类（Service Class）时发生。若注册好一个服务的实例，它必须引用一个以前通过WSAInstallServiceClass安装好的服务。)
WSA_ERROR_DEF(10110, WSA_E_NO_MORE, 找不到更多的记录。这个错误是自WSALookupServiceNext调用返回的，指出已经没有剩下的记录。该错误通常可与WSAENOMORE互换使用。作为一个应用程序，应同时检查这个错误以及WSAENOMORE。)
WSA_ERROR_DEF(10111, WSA_E_CANCELLED, 操作被取消。该错误指出在对WSALookupServiceNext的调用尚未完成的时候，又发出了对WSALookupServiceEnd（中止服务）的一个调用。这样，WSALookupServiceNext就会返回该错误。这个错误代码可与WSAECANCELLED互换使用。作为一个应用程序，应同时检查这个错误以及WSAECANCELLED。)
WSA_ERROR_DEF(10112, WSAEREFUSED, 查询被拒。由于被主动拒绝，所以一个数据库查询操作失败。)
WSA_ERROR_DEF(11001, WSAHOST_NOT_FOUND, 主机没有找到。这个错误是在调用gethostbyname和gethostbyaddr时产生的，表明没有找到一个授权应答主机（AuthoritativeAnswerHost）。)
WSA_ERROR_DEF(11002, WSATRY_AGAIN, 非授权主机没有找到。这个错误也是在调用gethostbyname和gethostbyaddr时产生的，表明没有找到一个非授权主机，或者遇到了服务器故障。)
WSA_ERROR_DEF(11003, WSANO_RECOVERY, 遇到一个不可恢复的错误。这个错误也是在调用gethostbyname和gethostbyaddr时产生的，指出遇到一个不可恢复的错误，应再次尝试操作。)
WSA_ERROR_DEF(11004, WSANO_DATA, 没有找到请求类型的数据记录。这个错误也是在调用gethostbyname和gethostbyaddr时产生的，指出尽管提供的名字有效，但却没有找到与请求类型对应的数据记录。)
WSA_ERROR_DEF(11005, WSA_QOS_RECEIVERS, 至少有一条预约消息抵达。这个值同IP服务质量（QoS）有着密切的关系，其实并不是一个真正的“错误”（QoS的详情见第12章）。它指出网络上至少有一个进程希望接收QoS通信。)
WSA_ERROR_DEF(11006, WSA_QOS_SENDERS, 至少有一条路径消息抵达。这个值同QoS关联在一起，其实更像一种状态报告消息。它指出在网络上，至少有一个进程希望进行QoS数据的发送。)
WSA_ERROR_DEF(11007, WSA_QOS_NO_SENDERS, 没有QoS发送者。这个值同QoS关联在一起，指出不再有任何进程对QoS数据的发送有兴趣。请参阅第12章，了解在发生这样的错误时，对所发生情况的一系列完整说明。)
WSA_ERROR_DEF(11008, WSA_QOS_NO_RECEIVERS, 没有QoS接收者。这个值同QoS关联在一起，指出不再有任何进程对QoS数据的接收有兴趣。请参阅第12章，查阅对这个错误的完整说明。)
WSA_ERROR_DEF(11009, WSA_QOS_REQUEST_CONFIRMED, 预约请求已被确认。QoS应用可事先发出请求，希望在批准了自己对网络带宽的预约请求后，收到通知。若发出了这样的请求，一旦批准，便会收到这样的消息。请参阅第12章，了解对此消息的详细说明。)
WSA_ERROR_DEF(11010, WSA_QOS_ADMISSION_FAILURE, 缺乏资源致错。资源不够，以至于无法满足QoS带宽请求。)
WSA_ERROR_DEF(11011, WSA_QOS_POLICY_FAILURE, 证书无效。表明发出QoS预约请求的时候，要么用户并不具备正确的权限，要么提供的证书无效。)
WSA_ERROR_DEF(11012, WSA_QOS_BAD_STYLE, 未知或冲突的样式。QoS应用程序可针对一个指定的会话，建立不同的过滤器样式。若出现这一错误，表明指定的样式类型要么未知，要么存在冲突。请参阅第12章，了解对过滤器样式的详细说明。)
WSA_ERROR_DEF(11013, WSA_QOS_BAD_OBJECT, 无效的FILTERSPEC结构或者提供者特有对象。假如为QoS对象提供的FILTERSPEC结构无效，或者提供者特有的缓冲区无效，便会返回这样的错误，详见第12章。)
WSA_ERROR_DEF(11014, WSA_QOS_TRAFFIC_CTRL_ERROR, FLOWSPEC有问题。假如通信控制组件发现指定的FLOWSPEC参数存在问题（作为QoS对象的一个成员传递），便会返回这样的错误。)
WSA_ERROR_DEF(11015, WSA_QOS_GENERIC_ERROR, 常规QoS错误。这是一个比较泛泛的错误；假如其他QoS错误都不适合，便返回这个错误。)

