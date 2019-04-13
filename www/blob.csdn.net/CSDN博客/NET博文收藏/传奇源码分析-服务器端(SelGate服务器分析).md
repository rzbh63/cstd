
# 传奇源码分析-服务器端(SelGate服务器分析)  - .NET博文收藏 - CSDN博客


2008年06月10日 12:49:00[hejishan](https://me.csdn.net/hejishan)阅读数：1480标签：[服务器																](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)[timer																](https://so.csdn.net/so/search/s.do?q=timer&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[socket																](https://so.csdn.net/so/search/s.do?q=socket&t=blog)[server																](https://so.csdn.net/so/search/s.do?q=server&t=blog)[command																](https://so.csdn.net/so/search/s.do?q=command&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=server&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=timer&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
**SelGate****服务器**
注：客户端从LoginSvr服务器得到SelGate服务器IP之后，连接SelGate服务器，进行角
色创建，删除，选择操作，然后发送数据到DBSrv服务器。
g_ssock  Local7100客户端登陆端口
g_csock Remote5100发送到DBSrv服务器上的套接字
1．首先从SelGate.cpp WinMain分析：
1) CheckAvailableIOCP :检查是不是NT，2000的系统（IOCP）
2) InitInstance:初始化界面，加载WSAStartup
3) MainWndProc窗口回调函数.
2．MainWndProc.CPP中分析回调函数MainWndProc
switch (nMsg)
{
case _IDM_CLIENTSOCK_MSG:
case WM_COMMAND:
case WM_CLOSE：
1）_IDM_CLIENTSOCK_MSG 消息：
处理与SelGate回调通讯事件。
调用：OnClientSockMsg,该函数是一个回调函数：
当启动服务之后，ConnectToServer函数将(_IDM_CLIENTSOCK_MSG消息 FD_CONNECT|FD_READ|FD_CLOSE)传入WSAAsyncSelect函数。在与hWnd窗口句柄对应的窗口例程中以Windows消息的形式接收网络事件通知。函数OnClientSockMsg，主要完成与DBSrv服务器之间的通信（心跳，转发客户端数据包等）
switch (WSAGETSELECTEVENT(lParam))
{
case FD_CONNECT:
case FD_CLOSE:
case FD_READ:
FD_CONNECT：(重新连接情况)
A. CheckSocketError返回正常时：
a). ConnectToServer函数首先在服务启动的时候执行一次。回调
FD_CONNECT
b)．连接DBSrv时,开启ThreadFuncForMsg线程，把从客户端发送的数据(g_xMsgQueue, FD_READ事件读到的DBSrv服务器发来的数据)投递I/O，利用IOCP模型，发送到客户端。SleepEx挂起线程,至到一个I/O 完成回调函数被调用。一个异步过程调用排队到此线程。
ThreadFuncForMsg线程检测(从DBSrv收到的g_xMsgQueue数据包-心跳，处理包)。i/o 投递，利用IOCP发送给客户端。
if (nSocket = AnsiStrToVal(pszFirst + 1)) //得到socket
WSASend((SOCKET)nSocket, &Buf, 1, &dwSendBytes, 0, NULL, NULL);
c)．终止定时器_ID_TIMER_CONNECTSERVER
KillTimer(g_hMainWnd, _ID_TIMER_CONNECTSERVER);
d)．设置_ID_TIMER_KEEPALIVE定时器 （心跳数据包）
SetTimer(g_hMainWnd, _ID_TIMER_KEEPALIVE
调用定时器回调函数OnTimerProc: 定时发关心跳数据包到DBSrv服务器。SendExToServer(PACKET_KEEPALIVE);

**B.**如果socket断开，设置_ID_TIMER_CONNECTSERVER定时器
ConnectToServer尝试重新连接服务器。
_ID_TIMER_CONNECTSERVER, (TIMERPROC)OnTimerProc);
FD_CLOSE:
断开SOCKET连接，OnCommand(IDM_STOPSERVICE, 0); 回调函数处理IDM_STOPSERVICE。
case FD_READ:
接收DBSrv服务器发送的数据包（心跳,登陆验证，selCur服务器地址），把数据加入缓冲区(g_xMsgQueue)中。

WM_COMMAND:
IDM_STARTSERVICE:启动服务(IOCP模型Server响应客户端请求)
IDM_STOPSERVICE:停止服务(IOCP模型Server)
WM_CLOSE:
IDM_STOPSERVICE:停止服务(IOCP模型Server)
WSACleanup();
PostQuitMessage(0); //WM_DESTROY消息
**IDM_STARTSERVICE:****启动服务****(IOCP模型Server响应客户端请求)**
InitServerSocket：函数：
1) AcceptThread线程：
Accept之后生成一个CSessionInfo对象，pNewUserInfo->sock = Accept; 客户端Socket值赋值给结构体。记录客户相关信息。
新的套接字句柄用CreateIoCompletionPort关联到完成端口，然后发出一个异步的WSASend或者WSARecv调用(pNewUserInfo->Recv();接收客户端消息)，因为是异步函数，WSASend/WSARecv会马上返回，实际的发送或者接收数据的操作由WINDOWS系统去做。然后把CSessionInfo对象加入g_xSessionList中。向DBsrv服务器发送用户Session信息。打包规则‘%0socket/ip$/0’
**在客户****accept之后，总投递一个I/O(recv),然后把相应的数据发往DBSrv服务器。**
2) CreateIOCPWorkerThread函数：
调用CreateIoCompletionPort 并根据处理器数量，创建一个或多个ServerWorkerThread线程。
ServerWorkerThread线程工作原理：
循环调用GetQueuedCompletionStatus()函数来得到IO操作结果。阻塞函数。当WINDOWS系统完成WSASend或者WSArecv的操作，把结果发到完成端口。GetQueuedCompletionStatus()马上返回,并从完成端口取得刚完成的WSASend/WSARecv的结果。然后接着发出WSASend/WSARecv，并继续下一次循环阻塞在GetQueuedCompletionStatus()这里。
a). pSessionInfo为空或者dwBytesTransferred =0 ,在客户端close socket，发相应数据包(异常）到DBSrv服务器(X命令-数据包)，关闭客户端套按字。
b). while ( pSessionInfo->HasCompletionPacket() )如果数据验证正确，就转发数据包(A命令-数据包) DBSrv服务器。
c). if (pSessionInfo->Recv() 继续投递I/O操作。
总结：
我们不停地发出异步的WSASend/WSARecv IO操作，具体的IO处理过程由WINDOWS系统完成，WINDOWS系统完成实际的IO处理后，把结果送到完成端口上（如果有多个IO都完成了，那么就在完成端口那里排成一个队列）。我们在另外一个线程里从完成端口不断地取出IO操作结果，然后根据需要再发出WSASend/WSARecv IO操作。

IDM_STOPSERVICE**:****停止服务****(IOCP模型Server响应客户端请求)**
**Close -> OnCommand(IDM_STOPSERVICE, 0L); ->g_fTerminated = TRUE;****线程退出。**
ClearSocket(g_ssock);
ClearSocket(g_csock);
CloseHandle(g_hIOCP);
总结：SelGate（角色处理服务器），接受客户端连接，并且把用户数据包(角色处理)发送到DBSrv服务器中，由DBSrv服务器处理之后，发送数据包返回给客户端。SelGate之间是通过定时器，定时发送“心跳”数据。验证服务器存活的。客户端与服务器端的数据在传输中，是进行过加密的。
向DBSrv发送‘%A’+Msg+‘$0’消息： 转发客户端消息。
‘%X’+Msg+‘$0’消息： 发送用户连接消息，增加到用户列表。
‘%O’+Msg+‘$0’消息： 发送用户上线消息。
主要流程：
服务启动后，SelGate启动了AcceptThread,和ServerWorkerThread线程，AcceptThread线程接收客户端连接，并把session信息发送给DBSrv服务器，ServerWorkerThread线程从完成端口取得刚完成的WSASend/WSARecv的结果后，把客户端数据转发给DBSrv服务器。服务启动时，WSAAsyncSelect模型连接到DBSrv服务器中。一旦连接成功，就启动ThreadFuncForMsg线程，该线程从g_xMsgQueue(FD_READ事件读到的DBSrv服务器发来的数据)中取出DBSrv服务器处理过的数据。投递I/O，利用IOCP模型，发送到客户端。
ServerWorkerThread转发客户端数据-> WSAAsyncSelect的Read读DBSrv处理后返回的数据-> ThreadFuncForMsg线程，投递WSASend消息，由Windows处理（IOCP），发送数据给客户端。



