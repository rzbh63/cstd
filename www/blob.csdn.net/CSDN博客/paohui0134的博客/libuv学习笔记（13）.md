
# libuv学习笔记（13） - paohui0134的博客 - CSDN博客


2016年06月17日 21:24:26[paohui0134](https://me.csdn.net/paohui0134)阅读数：1484


# libuv学习笔记（13）
## uv_tcp_t数据结构与相关函数(2)
本篇主要内容是上一篇未能学习的listen、accept与write
### 相关函数
#### uv_listen，导出函数，在uv.h中声明，在stream.c中定义
```python
int
```
```python
uv_listen(uv_stream_t* stream,
```
```python
int
```
```python
backlog, uv_connection_cb cb) {
```
```python
int
```
```python
err;
  err = ERROR_INVALID_PARAMETER;
```
```python
switch
```
```python
(stream->type) {
```
```python
case
```
```python
UV_TCP:
      err = uv_tcp_listen((uv_tcp_t*)stream, backlog, cb);
```
```python
break
```
```python
;
```
```python
case
```
```python
UV_NAMED_PIPE:
```
```python
//管道
```
```python
err = uv_pipe_listen((uv_pipe_t*)stream, backlog, cb);
```
```python
break
```
```python
;
```
```python
default
```
```python
:
```
```python
assert
```
```python
(
```
```python
0
```
```python
);
  }
```
```python
return
```
```python
uv_translate_sys_error(err);
}
```
对于tcp流，调用uv_tcp_listen
```python
int
```
```python
uv_tcp_listen(uv_tcp_t* handle,
```
```python
int
```
```python
backlog, uv_connection_cb cb) {
  uv_loop_t* loop = handle->loop;
```
```python
unsigned
```
```python
int
```
```python
i, simultaneous_accepts;
  uv_tcp_accept_t* req;
```
```python
int
```
```python
err;
  assert(backlog >
```
```python
0
```
```python
);
```
```python
//正在监听，赋值回调函数
```
```python
if
```
```python
(handle->flags & UV_HANDLE_LISTENING) {
    handle->stream.serv.connection_cb = cb;
  }
```
```python
//正在读取数据，返回错误
```
```python
if
```
```python
(handle->flags & UV_HANDLE_READING) {
```
```python
return
```
```python
WSAEISCONN;
  }
```
```python
//绑定的时候出了错误
```
```python
if
```
```python
(handle->delayed_error) {
```
```python
return
```
```python
handle->delayed_error;
  }
```
```python
//未能绑定
```
```python
if
```
```python
(!(handle->flags & UV_HANDLE_BOUND)) {
```
```python
//绑定默认的地址与端口
```
```python
err = uv_tcp_try_bind(handle,
                          (
```
```python
const
```
```python
struct
```
```python
sockaddr*) &uv_addr_ip4_any_,
```
```python
sizeof
```
```python
(uv_addr_ip4_any_),
```
```python
0
```
```python
);
```
```python
if
```
```python
(err)
```
```python
return
```
```python
err;
```
```python
if
```
```python
(handle->delayed_error)
```
```python
return
```
```python
handle->delayed_error;
  }
```
```python
//获取acceptex函数指针
```
```python
if
```
```python
(!handle->tcp.serv.func_acceptex) {
```
```python
if
```
```python
(!uv_get_acceptex_function(handle->socket, &handle->tcp.serv.func_acceptex)) {
```
```python
return
```
```python
WSAEAFNOSUPPORT;
    }
  }
```
```python
if
```
```python
(!(handle->flags & UV_HANDLE_SHARED_TCP_SOCKET) &&
      listen(handle->socket, backlog) == SOCKET_ERROR) {
```
```python
return
```
```python
WSAGetLastError();
  }
```
```python
//改变状态
```
```python
handle->flags |= UV_HANDLE_LISTENING;
  handle->stream.serv.connection_cb = cb;
  INCREASE_ACTIVE_COUNT(loop, handle);
```
```python
//递增handle的活动计数
```
```python
simultaneous_accepts = handle->flags & UV_HANDLE_TCP_SINGLE_ACCEPT ?
```
```python
1
```
```python
: uv_simultaneous_server_accepts;
```
```python
//初始化内部accept请求
```
```python
if
```
```python
(!handle->tcp.serv.accept_reqs) {
    handle->tcp.serv.accept_reqs = (uv_tcp_accept_t*)
      uv__malloc(uv_simultaneous_server_accepts *
```
```python
sizeof
```
```python
(uv_tcp_accept_t));
```
```python
if
```
```python
(!handle->tcp.serv.accept_reqs) {
      uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
    }
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < simultaneous_accepts; i++) {
      req = &handle->tcp.serv.accept_reqs[i];
      uv_req_init(loop, (uv_req_t*)req);
      req->type = UV_ACCEPT;
      req->accept_socket = INVALID_SOCKET;
      req->data = handle;
      req->wait_handle = INVALID_HANDLE_VALUE;
```
```python
if
```
```python
(handle->flags & UV_HANDLE_EMULATE_IOCP) {
```
```python
//模拟iocp的情况下，需要创建event
```
```python
req->event_handle = CreateEvent(NULL,
```
```python
0
```
```python
,
```
```python
0
```
```python
, NULL);
```
```python
if
```
```python
(!req->event_handle) {
          uv_fatal_error(GetLastError(),
```
```python
"CreateEvent"
```
```python
);
        }
      }
```
```python
else
```
```python
{
        req->event_handle = NULL;
      }
```
```python
//添加accept请求
```
```python
uv_tcp_queue_accept(handle, req);
    }
```
```python
//初始化其他未使用的请求，因为uv_tcp_endgame 并不知道有多少初始化的请求，它只会清理所有请求
```
```python
for
```
```python
(i = simultaneous_accepts; i < uv_simultaneous_server_accepts; i++) {
      req = &handle->tcp.serv.accept_reqs[i];
      uv_req_init(loop, (uv_req_t*) req);
      req->type = UV_ACCEPT;
      req->accept_socket = INVALID_SOCKET;
      req->data = handle;
      req->wait_handle = INVALID_HANDLE_VALUE;
      req->event_handle = NULL;
    }
  }
```
```python
return
```
```python
0
```
```python
;
}
```
添加accept请求
```python
static
```
```python
void
```
```python
uv_tcp_queue_accept(uv_tcp_t* handle, uv_tcp_accept_t* req) {
  uv_loop_t* loop = handle->loop;
  BOOL success;
  DWORD bytes;
  SOCKET accept_socket;
```
```python
short
```
```python
family;
  assert(handle->flags & UV_HANDLE_LISTENING);
  assert(req->accept_socket == INVALID_SOCKET);
```
```python
//选择ip类型
```
```python
if
```
```python
(handle->flags & UV_HANDLE_IPV6) {
    family = AF_INET6;
  }
```
```python
else
```
```python
{
    family = AF_INET;
  }
```
```python
//新建一个socket用来获取接受到的socket
```
```python
accept_socket = socket(family, SOCK_STREAM,
```
```python
0
```
```python
);
```
```python
if
```
```python
(accept_socket == INVALID_SOCKET) {
```
```python
//出现错误，直接将请求添加到loop的请求队列做错误处理
```
```python
SET_REQ_ERROR(req, WSAGetLastError());
    uv_insert_pending_req(loop, (uv_req_t*)req);
    handle->reqs_pending++;
```
```python
return
```
```python
;
  }
```
```python
//使socket不可继承
```
```python
if
```
```python
(!SetHandleInformation((HANDLE) accept_socket, HANDLE_FLAG_INHERIT,
```
```python
0
```
```python
)) {
    SET_REQ_ERROR(req, GetLastError());
    uv_insert_pending_req(loop, (uv_req_t*)req);
    handle->reqs_pending++;
    closesocket(accept_socket);
```
```python
return
```
```python
;
  }
```
```python
//准备重叠结构体
```
```python
memset
```
```python
(&(req->u.io.overlapped),
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
(req->u.io.overlapped));
```
```python
if
```
```python
(handle->flags & UV_HANDLE_EMULATE_IOCP) {
```
```python
//模拟iocp，使用之前创建的event
```
```python
req->u.io.overlapped.hEvent = (HANDLE) ((ULONG_PTR) req->event_handle |
```
```python
1
```
```python
);
  }
```
```python
//异步accept
```
```python
success = handle->tcp.serv.func_acceptex(handle->socket,
                                          accept_socket,
                                          (
```
```python
void
```
```python
*)req->accept_buffer,
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
(
```
```python
struct
```
```python
sockaddr_storage),
```
```python
sizeof
```
```python
(
```
```python
struct
```
```python
sockaddr_storage),
                                          &bytes,
                                          &req->u.io.overlapped);
```
```python
if
```
```python
(UV_SUCCEEDED_WITHOUT_IOCP(success)) {
```
```python
//直接将请求添加到loop的请求队列
```
```python
req->accept_socket = accept_socket;
    handle->reqs_pending++;
    uv_insert_pending_req(loop, (uv_req_t*)req);
  }
```
```python
else
```
```python
if
```
```python
(UV_SUCCEEDED_WITH_IOCP(success)) {
```
```python
//通过iocp处理
```
```python
req->accept_socket = accept_socket;
    handle->reqs_pending++;
```
```python
//如果是模拟iocp，那么注册等待event_handle的事件
```
```python
if
```
```python
(handle->flags & UV_HANDLE_EMULATE_IOCP &&
        req->wait_handle == INVALID_HANDLE_VALUE &&
        !RegisterWaitForSingleObject(&req->wait_handle,
          req->event_handle, post_completion, (
```
```python
void
```
```python
*) req,
          INFINITE, WT_EXECUTEINWAITTHREAD)) {
      SET_REQ_ERROR(req, GetLastError());
      uv_insert_pending_req(loop, (uv_req_t*)req);
      handle->reqs_pending++;
```
```python
return
```
```python
;
    }
  }
```
```python
else
```
```python
{
```
```python
//添加错误信息，直接添加到loop请求队列
```
```python
SET_REQ_ERROR(req, WSAGetLastError());
    uv_insert_pending_req(loop, (uv_req_t*)req);
    handle->reqs_pending++;
```
```python
//关闭socket
```
```python
closesocket(accept_socket);
```
```python
//销毁事件句柄
```
```python
if
```
```python
(handle->flags & UV_HANDLE_EMULATE_IOCP) {
      CloseHandle(req->u.io.overlapped.hEvent);
      req->event_handle = NULL;
    }
  }
}
```
uv_run中调用uv_process_reqs对accpet请求的处理：
```python
case
```
```python
UV_ACCEPT
```
```python
:
        DELEGATE_STREAM_REQ(
```
```python
loop
```
```python
, req, accept, data);
```
```python
break
```
```python
;
```
最终调用uv_process_tcp_accept_req
```python
void
```
```python
uv_process_tcp_accept_req(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_tcp_t
```
```python
*
```
```python
handle
```
```python
,
    uv_req_t
```
```python
*
```
```python
raw_req) {
  uv_tcp_accept_t
```
```python
*
```
```python
req
```
```python
=
```
```python
(uv_tcp_accept_t
```
```python
*
```
```python
) raw_req;
  int err;
  assert(
```
```python
handle
```
```python
->
```
```python
type
```
```python
==
```
```python
UV_TCP);
```
```python
//如果handle->accepted_socket不可用，表示uv_queue_accept 失败了，使用失败信息调用
```
```python
//connection_cb
```
```python
if
```
```python
(req
```
```python
->
```
```python
accept_socket
```
```python
==
```
```python
INVALID_SOCKET) {
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_LISTENING) {
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&=
```
```python
~UV_HANDLE_LISTENING;
      DECREASE_ACTIVE_COUNT(
```
```python
loop
```
```python
,
```
```python
handle
```
```python
);
```
```python
//handle活动计数递减
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
serv
```
```python
.
```
```python
connection_cb) {
        err
```
```python
=
```
```python
GET_REQ_SOCK_ERROR(req);
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
serv
```
```python
.
```
```python
connection_cb((uv_stream_t
```
```python
*
```
```python
)
```
```python
handle
```
```python
,
                                      uv_translate_sys_error(err));
      }
    }
  }
```
```python
else
```
```python
if
```
```python
(REQ_SUCCESS(req)
```
```python
&&
```
```python
setsockopt(req
```
```python
->
```
```python
accept_socket,
                  SOL_SOCKET,
                  SO_UPDATE_ACCEPT_CONTEXT,
                  (char
```
```python
*
```
```python
)
```
```python
&
```
```python
handle
```
```python
->
```
```python
socket,
                  sizeof(
```
```python
handle
```
```python
->
```
```python
socket))
```
```python
==
```
```python
0
```
```python
) {
```
```python
//成功
```
```python
req
```
```python
->
```
```python
next_pending
```
```python
=
```
```python
handle
```
```python
->
```
```python
tcp
```
```python
.
```
```python
serv
```
```python
.
```
```python
pending_accepts;
```
```python
handle
```
```python
->
```
```python
tcp
```
```python
.
```
```python
serv
```
```python
.
```
```python
pending_accepts
```
```python
=
```
```python
req;
```
```python
//调用回调
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
serv
```
```python
.
```
```python
connection_cb) {
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
serv
```
```python
.
```
```python
connection_cb((uv_stream_t
```
```python
*
```
```python
)
```
```python
handle
```
```python
,
```
```python
0
```
```python
);
    }
  }
```
```python
else
```
```python
{
```
```python
//收到的socket的错误将被忽略，因为服务端socket可能还是正常的
```
```python
closesocket(req
```
```python
->
```
```python
accept_socket);
    req
```
```python
->
```
```python
accept_socket
```
```python
=
```
```python
INVALID_SOCKET;
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_LISTENING) {
      uv_tcp_queue_accept(
```
```python
handle
```
```python
, req);
```
```python
//继续监听
```
```python
}
  }
  DECREASE_PENDING_REQ_COUNT(
```
```python
handle
```
```python
);
```
```python
//handle等待处理的请求计数减一
```
```python
}
```
#### uv_accept，导出函数，在uv.h中声明，在stream.c中定义
uv_accept一般在uv_listen的回调函数中调用，对于tcp，最终调用uv_tcp_accept
```python
int
```
```python
uv_tcp_accept(uv_tcp_t* server, uv_tcp_t* client) {
  uv_loop_t* loop = server->loop;
```
```python
int
```
```python
err =
```
```python
0
```
```python
;
```
```python
int
```
```python
family;
  uv_tcp_accept_t* req = server->tcp.serv.pending_accepts;
```
```python
//没有请求，返回错误，比如uv_process_tcp_accept_req中出现了错误
```
```python
if
```
```python
(!req) {
```
```python
return
```
```python
WSAEWOULDBLOCK;
  }
```
```python
//
```
```python
if
```
```python
(req->accept_socket == INVALID_SOCKET) {
```
```python
return
```
```python
WSAENOTCONN;
  }
```
```python
if
```
```python
(server->flags & UV_HANDLE_IPV6) {
    family = AF_INET6;
  }
```
```python
else
```
```python
{
    family = AF_INET;
  }
```
```python
//将获取到的socket与loop的iocp端口联系起来，与client联系起来
```
```python
err = uv_tcp_set_socket(client->loop,
                          client,
                          req->accept_socket,
                          family,
```
```python
0
```
```python
);
```
```python
if
```
```python
(err) {
    closesocket(req->accept_socket);
  }
```
```python
else
```
```python
{
```
```python
//按链接socket初始化获取到的socket，也就是初始化客户端会用到的地方，主要是write以及shutdown相
```
```python
//关的请求
```
```python
uv_connection_init((uv_stream_t*) client);
```
```python
// AcceptEx() 已经绑定了接受到的socket的地址
```
```python
client->flags |= UV_HANDLE_BOUND | UV_HANDLE_READABLE | UV_HANDLE_WRITABLE;
  }
```
```python
//为接下来的acceptex做准备
```
```python
server->tcp.serv.pending_accepts = req->next_pending;
  req->next_pending = NULL;
  req->accept_socket = INVALID_SOCKET;
```
```python
if
```
```python
(!(server->flags & UV__HANDLE_CLOSING)) {
```
```python
//accept的行为并未被改变
```
```python
if
```
```python
(!(server->flags & UV_HANDLE_TCP_ACCEPT_STATE_CHANGING)) {
      uv_tcp_queue_accept(server, req);
```
```python
//继续accept
```
```python
}
```
```python
else
```
```python
{
      assert(server->flags & UV_HANDLE_TCP_SINGLE_ACCEPT);
      server->tcp.serv.processed_accepts++;
```
```python
if
```
```python
(server->tcp.serv.processed_accepts >= uv_simultaneous_server_accepts) {
        server->tcp.serv.processed_accepts =
```
```python
0
```
```python
;
```
```python
//所有之前的accept请求都被处理了，现在只发送一个
```
```python
uv_tcp_queue_accept(server, &server->tcp.serv.accept_reqs[
```
```python
0
```
```python
]);
        server->flags &= ~UV_HANDLE_TCP_ACCEPT_STATE_CHANGING;
        server->flags |= UV_HANDLE_TCP_SINGLE_ACCEPT;
      }
    }
  }
  loop->active_tcp_streams++;
```
```python
//tcp流数量加一
```
```python
return
```
```python
err;
}
```
与一般流程不同的是，libuv在listen时内部就调用了acceptex，而uv_accept一般是在listen的回调中使用的，用来获取acceptsocket并对其进行一些设置，将其用传入的client封装。
#### uv_write,导出函数，在uv.h中声明，在stream.c中定义
内部调用uv_tcp_write
```python
int
```
```python
uv_tcp_write(uv_loop_t* loop,
                 uv_write_t* req,
                 uv_tcp_t* handle,
                 const uv_buf_t bufs[],
                 unsigned int nbufs,
                 uv_write_cb cb) {
  int result;
```
```python
DWORD
```
```python
bytes;
  uv_req_init(loop, (uv_req_t*) req);
  req->
```
```python
type
```
```python
=
```
```python
UV_WRITE
```
```python
;
```
```python
req->handle = (uv_stream_t*) handle;
  req->cb = cb;
  //准备重叠结构体
  memset(&(req->u.io.overlapped),
```
```python
0
```
```python
, sizeof(req->u.io.overlapped));
```
```python
if
```
```python
(handle->flags &
```
```python
UV_HANDLE_EMULATE_IOCP
```
```python
) {
    req->event_handle =
```
```python
CreateEvent
```
```python
(
```
```python
NULL
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
NULL
```
```python
);
```
```python
if
```
```python
(!req->event_handle) {
      uv_fatal_error(
```
```python
GetLastError
```
```python
(),
```
```python
"CreateEvent"
```
```python
);
    }
    req->u.io.overlapped.hEvent = (
```
```python
HANDLE
```
```python
) ((
```
```python
ULONG_PTR
```
```python
) req->event_handle |
```
```python
1
```
```python
);
    req->wait_handle =
```
```python
INVALID_HANDLE_VALUE
```
```python
;
  }
  //异步发送
  result =
```
```python
WSASend
```
```python
(handle->socket,
                   (
```
```python
WSABUF
```
```python
*) bufs,
                   nbufs,
                   &bytes,
```
```python
0
```
```python
,
                   &req->u.io.overlapped,
```
```python
NULL
```
```python
);
```
```python
if
```
```python
(
```
```python
UV_SUCCEEDED_WITHOUT_IOCP
```
```python
(result ==
```
```python
0
```
```python
)) {
    //请求立刻就完成了，直接添加请求道loop请求队列
    req->u.io.queued_bytes =
```
```python
0
```
```python
;
    handle->reqs_pending++;
    handle->stream.conn.write_reqs_pending++;
```
```python
REGISTER_HANDLE_REQ
```
```python
(loop, handle, req);
    uv_insert_pending_req(loop, (uv_req_t*) req);
  }
```
```python
else
```
```python
if
```
```python
(
```
```python
UV_SUCCEEDED_WITH_IOCP
```
```python
(result ==
```
```python
0
```
```python
)) {
    //通过iocp处理
    req->u.io.queued_bytes = uv__count_bufs(bufs, nbufs);
    handle->reqs_pending++;
    handle->stream.conn.write_reqs_pending++;
```
```python
REGISTER_HANDLE_REQ
```
```python
(loop, handle, req);
    handle->write_queue_size += req->u.io.queued_bytes;
```
```python
if
```
```python
(handle->flags &
```
```python
UV_HANDLE_EMULATE_IOCP
```
```python
&&//模拟iocp
        !
```
```python
RegisterWaitForSingleObject
```
```python
(&req->wait_handle,
          req->event_handle, post_write_completion, (void*) req,
```
```python
INFINITE
```
```python
,
```
```python
WT_EXECUTEINWAITTHREAD
```
```python
|
```
```python
WT_EXECUTEONLYONCE
```
```python
)) {
```
```python
SET_REQ_ERROR
```
```python
(req,
```
```python
GetLastError
```
```python
());
      uv_insert_pending_req(loop, (uv_req_t*)req);
    }
  }
```
```python
else
```
```python
{
    //发送错误，直接将包含错误信息的请求添加到loop的请求队列
    req->u.io.queued_bytes =
```
```python
0
```
```python
;
    handle->reqs_pending++;
    handle->stream.conn.write_reqs_pending++;
```
```python
REGISTER_HANDLE_REQ
```
```python
(loop, handle, req);
```
```python
SET_REQ_ERROR
```
```python
(req,
```
```python
WSAGetLastError
```
```python
());
    uv_insert_pending_req(loop, (uv_req_t*) req);
  }
  return
```
```python
0
```
```python
;
}
```
对write请求的处理
```python
void
```
```python
uv_process_tcp_write_req(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_tcp_t
```
```python
*
```
```python
handle
```
```python
,
    uv_write_t
```
```python
*
```
```python
req) {
  int err;
  assert(
```
```python
handle
```
```python
->
```
```python
type
```
```python
==
```
```python
UV_TCP);
  assert(
```
```python
handle
```
```python
->
```
```python
write_queue_size
```
```python
>=
```
```python
req
```
```python
->
```
```python
u
```
```python
.
```
```python
io
```
```python
.
```
```python
queued_bytes);
```
```python
handle
```
```python
->
```
```python
write_queue_size
```
```python
-=
```
```python
req
```
```python
->
```
```python
u
```
```python
.
```
```python
io
```
```python
.
```
```python
queued_bytes;
```
```python
//剩下需要发送的数据
```
```python
UNREGISTER_HANDLE_REQ(
```
```python
loop
```
```python
,
```
```python
handle
```
```python
, req);
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_EMULATE_IOCP) {
```
```python
if
```
```python
(req
```
```python
->
```
```python
wait_handle
```
```python
!=
```
```python
INVALID_HANDLE_VALUE) {
      UnregisterWait(req
```
```python
->
```
```python
wait_handle);
```
```python
//停止监控
```
```python
req
```
```python
->
```
```python
wait_handle
```
```python
=
```
```python
INVALID_HANDLE_VALUE;
    }
```
```python
if
```
```python
(req
```
```python
->
```
```python
event_handle) {
      CloseHandle(req
```
```python
->
```
```python
event_handle);
      req
```
```python
->
```
```python
event_handle
```
```python
=
```
```python
NULL
```
```python
;
    }
  }
```
```python
if
```
```python
(req
```
```python
->
```
```python
cb) {
    err
```
```python
=
```
```python
uv_translate_sys_error(GET_REQ_SOCK_ERROR(req));
```
```python
if
```
```python
(err
```
```python
==
```
```python
UV_ECONNABORTED) {
      err
```
```python
=
```
```python
UV_ECANCELED;
    }
    req
```
```python
->
```
```python
cb(req, err);
```
```python
//调用回调
```
```python
}
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
write_reqs_pending
```
```python
--
```
```python
;
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
!=
```
```python
NULL
```
```python
&&
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
write_reqs_pending
```
```python
==
```
```python
0
```
```python
) {
    uv_want_endgame(
```
```python
loop
```
```python
, (uv_handle_t
```
```python
*
```
```python
)
```
```python
handle
```
```python
);
  }
  DECREASE_PENDING_REQ_COUNT(
```
```python
handle
```
```python
);
}
```
#### uv_shutdown，在uv.h中声明，在stream.c中定义
从写端关闭
```python
int uv_shutdown(uv_shutdown_t
```
```python
*
```
```python
req, uv_stream_t
```
```python
*
```
```python
handle
```
```python
, uv_shutdown_cb cb) {
  uv_loop_t
```
```python
*
```
```python
loop
```
```python
=
```
```python
handle
```
```python
->
```
```python
loop
```
```python
;
```
```python
//只处理可读的socket，也就是说只由建立了链接的tcp才可以，处于accept状态的也不行
```
```python
if
```
```python
(
```
```python
!
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_WRITABLE)) {
```
```python
return
```
```python
UV_EPIPE;
  }
  uv_req_init(
```
```python
loop
```
```python
, (uv_req_t
```
```python
*
```
```python
) req);
  req
```
```python
->
```
```python
type
```
```python
=
```
```python
UV_SHUTDOWN;
  req
```
```python
->
```
```python
handle
```
```python
=
```
```python
handle
```
```python
;
  req
```
```python
->
```
```python
cb
```
```python
=
```
```python
cb;
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&=
```
```python
~UV_HANDLE_WRITABLE;
```
```python
//不可写状态
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
=
```
```python
req;
```
```python
handle
```
```python
->
```
```python
reqs_pending
```
```python
++
```
```python
;
  REGISTER_HANDLE_REQ(
```
```python
loop
```
```python
,
```
```python
handle
```
```python
, req);
  uv_want_endgame(
```
```python
loop
```
```python
, (uv_handle_t
```
```python
*
```
```python
)
```
```python
handle
```
```python
);
```
```python
//将handle添加到loop的关闭队列
```
```python
return
```
```python
0
```
```python
;
}
```
#### uv_close关闭tcp  handle
最终调用uv_tcp_close
```python
void
```
```python
uv_tcp_close(uv_loop_t* loop, uv_tcp_t* tcp) {
  int close_socket =
```
```python
1
```
```python
;
```
```python
if
```
```python
(tcp->flags &
```
```python
UV_HANDLE_READ_PENDING
```
```python
) {//没有进行读操作
```
```python
if
```
```python
(!(tcp->flags &
```
```python
UV_HANDLE_SHARED_TCP_SOCKET
```
```python
)) {
      shutdown(tcp->socket,
```
```python
SD_SEND
```
```python
);//关闭发送功能
    }
```
```python
else
```
```python
if
```
```python
(uv_tcp_try_cancel_io(tcp) ==
```
```python
0
```
```python
) {
      //取消io操作，并等待最后一次读请求的处理中关闭socket
      close_socket =
```
```python
0
```
```python
;
    }
```
```python
else
```
```python
{
      //取消失败
    }
  }
```
```python
else
```
```python
if
```
```python
((tcp->flags &
```
```python
UV_HANDLE_SHARED_TCP_SOCKET
```
```python
) &&
             tcp->tcp.serv.accept_reqs !=
```
```python
NULL
```
```python
) {
    //共享socket并且正在listen
```
```python
if
```
```python
(uv_tcp_try_cancel_io(tcp) !=
```
```python
0
```
```python
) {//取消i/o操作
      //取消失败，关闭已经接受到但是iocp流程尚未走完的socket。在uv_accept中如果acceptsocket为
      //空，那么就不会接着进行accept。
      unsigned int i;
      for (i =
```
```python
0
```
```python
; i < uv_simultaneous_server_accepts; i++) {
        uv_tcp_accept_t* req = &tcp->tcp.serv.accept_reqs[i];
```
```python
if
```
```python
(req->accept_socket !=
```
```python
INVALID_SOCKET
```
```python
&&
            !
```
```python
HasOverlappedIoCompleted
```
```python
(&req->u.io.overlapped)) {
          closesocket(req->accept_socket);
          req->accept_socket =
```
```python
INVALID_SOCKET
```
```python
;
        }
      }
    }
  }
```
```python
if
```
```python
(tcp->flags &
```
```python
UV_HANDLE_READING
```
```python
) {
    tcp->flags &= ~
```
```python
UV_HANDLE_READING
```
```python
;
```
```python
DECREASE_ACTIVE_COUNT
```
```python
(loop, tcp);
  }
```
```python
if
```
```python
(tcp->flags &
```
```python
UV_HANDLE_LISTENING
```
```python
) {
    tcp->flags &= ~
```
```python
UV_HANDLE_LISTENING
```
```python
;
```
```python
DECREASE_ACTIVE_COUNT
```
```python
(loop, tcp);
  }
```
```python
if
```
```python
(close_socket) {
    closesocket(tcp->socket);
    tcp->socket =
```
```python
INVALID_SOCKET
```
```python
;
    tcp->flags |=
```
```python
UV_HANDLE_TCP_SOCKET_CLOSED
```
```python
;
  }
  tcp->flags &= ~(
```
```python
UV_HANDLE_READABLE
```
```python
|
```
```python
UV_HANDLE_WRITABLE
```
```python
);
  uv__handle_closing(tcp);
```
```python
if
```
```python
(tcp->reqs_pending ==
```
```python
0
```
```python
) {
    uv_want_endgame(tcp->loop, (uv_handle_t*)tcp);
  }
}
```
shutdown与closehandle的流程区别：
uv_shutdown——>uv_want_endgame——>uv_tcp_endgame
closehandle——>uv_tcp_close——>tcp  handle上没有活动请求之后调用uv_tcp_endgame
uv_tcp_endgame
```python
void
```
```python
uv_tcp_endgame(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_tcp_t
```
```python
*
```
```python
handle
```
```python
) {
  int err;
  unsigned int i;
  uv_tcp_accept_t
```
```python
*
```
```python
req;
```
```python
//有shutdown相关的请求信息，UV_HANDLE_CONNECTION 表示本socket是accept获取的socket或者链接服
```
```python
//务端的socket
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_CONNECTION
```
```python
&&
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
!=
```
```python
NULL
```
```python
&&
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
write_reqs_pending
```
```python
==
```
```python
0
```
```python
) {
    UNREGISTER_HANDLE_REQ(
```
```python
loop
```
```python
,
```
```python
handle
```
```python
,
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req);
    err
```
```python
=
```
```python
0
```
```python
;
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV__HANDLE_CLOSING) {
      err
```
```python
=
```
```python
ERROR_OPERATION_ABORTED;
    }
```
```python
else
```
```python
if
```
```python
(shutdown(
```
```python
handle
```
```python
->
```
```python
socket, SD_SEND)
```
```python
==
```
```python
SOCKET_ERROR) {
      err
```
```python
=
```
```python
WSAGetLastError();
    }
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
->
```
```python
cb) {
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
->
```
```python
cb(
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req,
                               uv_translate_sys_error(err));
    }
```
```python
handle
```
```python
->
```
```python
stream
```
```python
.
```
```python
conn
```
```python
.
```
```python
shutdown_req
```
```python
=
```
```python
NULL
```
```python
;
    DECREASE_PENDING_REQ_COUNT(
```
```python
handle
```
```python
);
```
```python
return
```
```python
;
  }
```
```python
//有关闭标记并且活动请求数为0，关闭相关资源，否则只能等到最后一个请求处理的时候再次调用
```
```python
//uv_tcp_endgame了（每个请求处理都会调用DECREASE_PENDING_REQ_COUNT宏）
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV__HANDLE_CLOSING
```
```python
&&
```
```python
handle
```
```python
->
```
```python
reqs_pending
```
```python
==
```
```python
0
```
```python
) {
    assert(
```
```python
!
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_CLOSED));
```
```python
if
```
```python
(
```
```python
!
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_TCP_SOCKET_CLOSED)) {
      closesocket(
```
```python
handle
```
```python
->
```
```python
socket);
```
```python
handle
```
```python
->
```
```python
socket
```
```python
=
```
```python
INVALID_SOCKET;
```
```python
handle
```
```python
->
```
```python
flags
```
```python
|=
```
```python
UV_HANDLE_TCP_SOCKET_CLOSED;
    }
```
```python
if
```
```python
(
```
```python
!
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_CONNECTION)
```
```python
&&
```
```python
handle
```
```python
->
```
```python
tcp
```
```python
.
```
```python
serv
```
```python
.
```
```python
accept_reqs) {
```
```python
if
```
```python
(
```
```python
handle
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV_HANDLE_EMULATE_IOCP) {
        for (i
```
```python
=
```
```python
0
```
```python
; i
```
```python
<
```
```python
uv_simultaneous_server_accepts; i
```
```python
++
```
```python
) {
          req
```
```python
=
```
```python
&
```
```python
handle
```
```python
->
```
```python
tcp
```
```python
.
```
```python
serv
```
```python
.
```
```python
accept_reqs
```
```python
[
```
```python
i
```
```python
]
```
```python
;
          if (req->wait_handle != INVALID_HANDLE_VALUE) {
            UnregisterWait(req->wait_handle);
            req->wait_handle = INVALID_HANDLE_VALUE;
          }
          if (req->event_handle) {
            CloseHandle(req->event_handle);
            req->event_handle = NULL;
          }
        }
      }
      uv__free(handle->tcp.serv.accept_reqs);
      handle->tcp.serv.accept_reqs = NULL;
    }
    if (handle->flags & UV_HANDLE_CONNECTION &&
        handle->flags & UV_HANDLE_EMULATE_IOCP) {
      if (handle->read_req.wait_handle != INVALID_HANDLE_VALUE) {
        UnregisterWait(handle->read_req.wait_handle);
        handle->read_req.wait_handle = INVALID_HANDLE_VALUE;
      }
      if (handle->read_req.event_handle) {
        CloseHandle(handle->read_req.event_handle);
        handle->read_req.event_handle = NULL;
      }
    }
    uv__handle_close(handle);
    loop->active_tcp_streams--;
  }
}
```
### 其他内容
#### 1.uv_shutdown与uv_close的区别，
uv_shutdown只是关闭了UV_HANDLE_CONNECTION 标志socket的写功能，并不会修改UV_HANDLE_CLOSING标记，不会处理accept以及read请求
uv_close会调用uv_tcp_close，做全面的清理工作，停止（非立刻）accept以及read，取消所有i/o，修改handle标记为UV_HANDLE_CLOSING，这会导致在最后一个请求处理的时候调用uv_want_endgame，最终导致uv_close回调被调用。
#### 2.
uv_tcp_t内部有一个联合体，
展开后如下：
```python
union
```
```python
{
```
```python
struct
```
```python
{ 
      uv_tcp_accept_t* accept_reqs;
```
```python
//accept请求
```
```python
//已经处理了的请求，最后关闭的时候会用到，需要等所有的acceptex调用的结果信息通过iocp接收到之后
```
```python
//才能关闭
```
```python
unsigned
```
```python
int
```
```python
processed_accepts;                                       
      uv_tcp_accept_t* pending_accepts;                                         
      LPFN_ACCEPTEX func_acceptex;
```
```python
//acceptex函数指针
```
```python
} serv;
```
```python
struct
```
```python
{ 
      uv_buf_t read_buffer;
```
```python
//读取数据的缓存
```
```python
LPFN_CONNECTEX func_connectex;
```
```python
//connectex的函数指针
```
```python
} conn;                               
  } tcp;
```
serv表示server端会用到的功能，也就是listen    accept
conn表示客户端会用到的功能，包括主动连接服务端的socket以及通过accept获取到的socket
#### 3.三种处理模式
1.第一种，UV_SUCCEEDED_WITHOUT_IOCP，也就是不通过iocp，直接将请求添加到loop的请求队列，这种情况个人没有遇到过，推测可能是类似阻塞式的i/o
2.第二种，UV_HANDLE_EMULATE_IOCP，模拟iocp，也就是通过RegisterWaitForSingleObject注册对于event的监控，而event就是自己创建的事件，对应io完成之后，会通知该event，进而被监控到，并在回调函数中向iocp端口发送信号。对于tcp，通过uv_tcp_open初始化的uv_tcp_t且与iocp端口联系失败时（uv_tcp_set_socket）
3.通过iocp，io操作完成之后直接可以通过iocp获取通知。

