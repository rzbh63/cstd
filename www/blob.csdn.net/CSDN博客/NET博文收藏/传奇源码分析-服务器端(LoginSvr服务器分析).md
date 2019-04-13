
# 传奇源码分析-服务器端(LoginSvr服务器分析)  - .NET博文收藏 - CSDN博客


2008年06月10日 12:50:00[hejishan](https://me.csdn.net/hejishan)阅读数：1915



**LoginSvr****服务器**
g_gcSock Local5500端口
1．首先从LoginSvr.cpp  WinMain分析：
1) CheckAvailableIOCP :检查是不是NT，2000的系统（IOCP）
2) InitInstance:初始化界面，加载WSAStartup
GetDBManager()->Init( InsertLogMsg, "Mir2_Account", "sa", "prg" );
数据库管理类，做底层数据库操作。
3)MainWndProc窗口回调函数OnCommand:
IDM_STARTSERVICE:
创建LoadAccountRecords线程
a). UPDATE TBL_ACCOUNT重置帐户验证状态。
b). 读服务器列表(TBL_SERVERINFO, selGate服务器)，加入g_xGameServerList
遍历xGameServerList列表，把服务器信息加入到一个字符数组g_szServerList中。
c). 启动InitServerThreadForMsg线程。
d).调用InitServerSocket函数创建两个线程：
AcceptThread线程：
ServerWorkerThread线程：
**调用****InitServerSocket函数创建两个线程：**
1) AcceptThread线程：
Accept之后生成一个CGateInfo对象，CGateInfo->sock = Accept; 客户端Socket值赋值给结构体。记录客户相关信息。新的套接字句柄用CreateIoCompletionPort关联到完成端口，然后发出一个异步的WSASend或者WSARecv调用(pNewUserInfo->Recv();接收客户端消息)，因为是异步函数，WSASend/WSARecv会马上返回，实际的发送或者接收数据的操作由WINDOWS系统去做。然后把CGateInfo对象加入g_xGateList中。在客户accept之后，投递一个I/O(recv)。
分析一下g_xGateList发现，每个CGateInfo里有sock; xUserInfoList，g_SendToGateQ，该网关的相关信息依次（网关对应的sock, 用户列列信息，消息队列），可以为多个LoginGate登录网关服务。
2) ServerWorkerThread线程：
ServerWorkerThread线程工作原理：
循环调用GetQueuedCompletionStatus()函数来得到IO操作结果。阻塞函数。当WINDOWS系统完成WSASend或者WSArecv的操作，把结果发到完成端口。GetQueuedCompletionStatus()马上返回,并从完成端口取得刚完成的WSASend/WSARecv的结果。然后接着发出WSASend/WSARecv，并继续下一次循环阻塞在GetQueuedCompletionStatus()这里。
a).if (g_fTerminated) 线程结束前：循环遍历g_xGateList，取出pGateInfo关闭套接字，并删除节点。dwBytesTransferred =0 ,关闭**该服务器**套接字。
b).while ( pGateInfo->HasCompletionPacket() ) 验证消息格式。
case '-': 发送心跳数据包到每个LoginGate服务器。
case 'A':  处理每个LoginGat服务器转发的客户端的消息增加到各自网关(CGateInfo)g_SendToGateQ队列中，然后ThreadFuncForMsg线程进行验证后再发送消息到各个LoginGate服务器。
pGateInfo->ReceiveSendUser(&szTmp[2]);
case 'O': 处理每个网关Accept客户端后增加pUserInfo用户信息到各自网关的xUserInfoList列表中。
pGateInfo->ReceiveOpenUser(&szTmp[2]);
case 'X': 处理每个网关收到客户端Socket关闭之后发送过来的消息。设置该网关socket相应状态。
pGateInfo->ReceiveCloseUser(&szTmp[2]);
case 'S':  GameSvr服务器发送的消息，更新TBL_ACCOUNT，验证字段，说明用户已下线，下次登录必须先到LoginSvr服务器再次验证。
pGateInfo->ReceiveServerMsg(&szTmp[2]);
case 'M':  GameSvr服务器发送的消息,创建一个用户的消息，把用户ID，密码,名字插入TBL_ACCOUNT表中插入成功返回SM_NEWID_SUCCESS,否则SM_NEWID_FAIL，把在信息前加\#，信息后加!  不做TBL_ACCOUNTADD表的添加，只增加TBL_ACCOUNT表信息。
‘A’:是LoginGate 服务器转发客户端消息到g_xMsgQueue队列, 由ThreadFuncForMsg线程处理后，转发到各个loginGate服务器
继续投递I/O操作。
**启动****InitServerThreadForMsg 创建ThreadFuncForMsg线程。**c
收到loginGate服务器发送过来的消息之后，ServerWorkerThread经过数据包分析之后（case 'A'），把客户端的消息，写入g_SendToGateQ队列中，然后在本线程中再进行处理。
遍历g_SendToGateQ队列中数据，验证数据包是否正确（\#!字符）根据DefaultMsg.wIdent标志
case CM_IDPASSWORD:处理登陆业务
遍历xUserInfoList用户列表信息，到数据库表TBL_ACCOUNT中找相应信息，如果失败发送(SM_ID_NOTFOUND, SM_PASSWD_FAIL)消息，否则发送SM_PASSOK_SELECTSERVER+ g_szServerList（SelGate服务器列表消息）
SelGate服务器列表消息（对应TBL_SERVERINFO数据库表中数据），供用户选择登录的SelGate服务器。
CM_SELECTSERVER:选择服务器(SelGate)
遍历xUserInfoList用户列表信息，根据socket,找到用户密钥，消息解密后，遍历g_xGameServerList列表，把用户选择的SelGate服务器转化为IP地址，发送至LoginGate服务器，再转发至客户端。设置该用户SelServer的标志状态。从该网关的xUserInfoList用户列表中删除该用户。
CM_ADDNEWUSER:  新注册用户
判断用户名是否已存在，失败发送SM_NEWID_FAIL消息，成功，写插入表数据，并发送SM_NEWID_SUCCESS消息到 LoginGate服务器，转发至客户端。
IDM_STOPSERVICE**:****停止服务****(IOCP模型Server响应客户端请求)**
**Close -> OnCommand(IDM_STOPSERVICE, 0L); ->g_fTerminated = TRUE;****三个线程退出。**
主要流程：
服务启动后，LoginSvr启动了AcceptThread,和ServerWorkerThread线程，AcceptThread线程接收loginGate，GameSvr服务器连接，加入g_xGateList网关列表中，ServerWorkerThread线程从完成端口取得刚完成的WSASend/WSARecv的结果后，进行分析处理两个服务器发送来的消息。服务启动同时，启动ThreadFuncForMsg线程，该线程从g_xMsgQueue(iocp读到的loginGate服务器发来的数据)中取出数据，处理数据。投递I/O，利用IOCP模型，发送到loginGate服务器。



