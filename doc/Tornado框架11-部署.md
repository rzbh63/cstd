# Tornado框架11-部署

 

为了充分利用多核CPU，并且为了减少同步代码中的阻塞影响，在部署Tornado的时候需要开启多个进程（最好为每个CPU核心开启一个进程）

因为Tornado自带的服务器性能很高，所以我们只需开启多个Tornado进程。为了对外有统一的接口，并且可以分发用户的请求到不同的Tornado进程上，我们用Nginx来进行代理。 
 ![这里写图片描述](https://img-blog.csdn.net/20171020155818478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzE1MTgxNjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 01-supervisor

为了统一管理Tornado的多个进程，我们可以借助supervisor工具。

## 安装

> sudo pip install supervisor

## 配置

运行echo_supervisord_conf命令输出默认的配置项，可以如下操作将默认配置保存到文件中

```
echo_supervisord_conf > supervisord.conf
12
```

vim 打开编辑supervisord.conf文件，修改

```
[include]
files = relative/directory/*.ini12
```

为

```
[include]
files = /etc/supervisor/*.conf12
```

include选项指明包含的其他配置文件。

将编辑后的supervisord.conf文件复制到/etc/目录下

```
sudo cp supervisord.conf /etc/1
```

然后我们在/etc目录下新建子目录supervisor（与配置文件里的选项相同），并在/etc/supervisor/中新建tornado管理的配置文件tornado.conf。

```
[group:tornadoes]
programs=tornado-8000,tornado-8001,tornado-8002,tornado-8003

[program:tornado-8000]
command=/home/python/.virtualenvs/tornado_py2/bin/python /home/python/Documents/demo/chat    /server.py --port=8000
directory=/home/python/Documents/demo/chat
user=python
autorestart=true
redirect_stderr=true
stdout_logfile=/home/python/tornado.log
loglevel=info

[program:tornado-8001]
command=/home/python/.virtualenvs/tornado_py2/bin/python /home/python/Documents/demo/chat    /server.py --port=8001
directory=/home/python/Documents/demo/chat
user=python
autorestart=true
redirect_stderr=true
stdout_logfile=/home/python/tornado.log
loglevel=info

[program:tornado-8002]
command=/home/python/.virtualenvs/tornado_py2/bin/python /home/python/Documents/demo/chat    /server.py --port=8002
directory=/home/python/Documents/demo/chat
user=python
autorestart=true
redirect_stderr=true
stdout_logfile=/home/python/tornado.log
loglevel=info

[program:tornado-8003]
command=/home/python/.virtualenvs/tornado_py2/bin/python /home/python/Documents/demo/chat    /server.py --port=8003
directory=/home/python/Documents/demo/chat
user=python
autorestart=true
redirect_stderr=true
stdout_logfile=/home/python/tornado.log
loglevel=info
```

## 启动

```
supervisord -c /etc/supervisord.conf1
```

查看 supervisord 是否在运行：

```
ps aux | grep supervisord1
```

**supervisorctl**

我们可以利用supervisorctl来管理supervisor。

```
supervisorctl

> status    # 查看程序状态
> stop tornadoes:*   # 关闭 tornadoes组 程序
> start tornadoes:*  # 启动 tornadoes组 程序
> restart tornadoes:*    # 重启 tornadoes组 程序
> update    ＃ 重启配置文件修改过的程序1234567
```

执行status命令时，显示如下信息说明tornado程序运行正常：

```
supervisor> status
tornadoes:tornado-8000 RUNNING pid 32091, uptime 00:00:02
tornadoes:tornado-8001 RUNNING pid 32092, uptime 00:00:02
tornadoes:tornado-8002 RUNNING pid 32093, uptime 00:00:02
tornadoes:tornado-8003 RUNNING pid 32094, uptime 00:00:0212345
```

# 02-nginx

对于使用ubuntu apt-get 安装nginx，其配置文件位于/etc/nginx/sites-available中，修改default文件如下：

```
upstream tornadoes {
    server 127.0.0.1:8000;
    server 127.0.0.1:8001;
    server 127.0.0.1:8002;
    server 127.0.0.1:8003;
}

upstream websocket {
    server 127.0.0.1:8000;
}

server {
    listen 80 default_server;
    listen [::]:80 default_server;
    server_name _;
    location /static/ {
        root /home/python/Documents/demo/chat;
        if ($query_string) {
            expires max;
        }
    }

    location /chat {
        proxy_pass http://websocket/chat;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }

    location / {
        proxy_pass_header Server;
        proxy_set_header Host $http_host;
        proxy_redirect off;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Scheme $scheme;  # 协议 http https
        proxy_pass http://tornadoes;
    }
}1234567891011121314151617181920212223242526272829303132333435363738
```

启动nginx

```
service nginx start   # 启动
service nginx stop    # 停止
service nginx restart # 重启123
```

源码安装版本

```
启动：sudo sbin/nginx
停止：sudo sbin/nginx -s stop
重启：sudo sbin/nginx -s reload
```