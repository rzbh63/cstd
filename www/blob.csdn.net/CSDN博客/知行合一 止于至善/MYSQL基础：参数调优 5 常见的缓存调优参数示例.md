
# MYSQL基础：参数调优:5:常见的缓存调优参数示例 - 知行合一 止于至善 - CSDN博客

2019年03月20日 21:27:32[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：237标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[调优																](https://so.csdn.net/so/search/s.do?q=调优&t=blog)[缓存																](https://so.csdn.net/so/search/s.do?q=缓存&t=blog)[示例																](https://so.csdn.net/so/search/s.do?q=示例&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=示例&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=缓存&t=blog)个人分类：[数据库																](https://blog.csdn.net/liumiaocn/article/category/6328292)
[
																								](https://so.csdn.net/so/search/s.do?q=缓存&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=调优&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=调优&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)

![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这篇文章在前文对缓存调优参数说明的基础上，根据一个具体的设定示例给出查询和设定的命令。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 确认系统当前设定
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)使用如下命令确认系统的当前状态
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
show
```
```python
variables
```
```python
where
```
```python
variable_name
```
```python
in
```
```python
(
```
```python
'tmp_table_size'
```
```python
,
```
```python
\
```
```python
'max_heap_table_size'
```
```python
,
```
```python
\
```
```python
'back_log'
```
```python
,
```
```python
\
```
```python
'read_buffer_size'
```
```python
,
```
```python
\
```
```python
'read_rnd_buffer_size'
```
```python
,
```
```python
\
```
```python
'thread_cache_size'
```
```python
,
```
```python
\
```
```python
'query_cache_type'
```
```python
,
```
```python
\
```
```python
'query_cache_size'
```
```python
,
```
```python
\
```
```python
'wait_timeout'
```
```python
,
```
```python
\
```
```python
'join_buffer_size'
```
```python
,
```
```python
\
```
```python
'sort_buffer_size'
```
```python
)
```
```python
;
```
```python
*
```
```python
执行示例
```
```python
`
```
```python
`
```
```python
`
```
```python
sql
```
```python
mysql
```
```python
>
```
```python
show
```
```python
variables
```
```python
where
```
```python
variable_name
```
```python
in
```
```python
(
```
```python
'tmp_table_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'max_heap_table_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'back_log'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'read_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'read_rnd_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'thread_cache_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'query_cache_type'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'query_cache_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'wait_timeout'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'join_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'sort_buffer_size'
```
```python
)
```
```python
;
```
```python
+
```
```python
----------------------+----------+
```
```python
|
```
```python
Variable_name
```
```python
|
```
```python
Value
```
```python
|
```
```python
+
```
```python
----------------------+----------+
```
```python
|
```
```python
back_log
```
```python
|
```
```python
80
```
```python
|
```
```python
|
```
```python
join_buffer_size
```
```python
|
```
```python
262144
```
```python
|
```
```python
|
```
```python
max_heap_table_size
```
```python
|
```
```python
16777216
```
```python
|
```
```python
|
```
```python
query_cache_size
```
```python
|
```
```python
1048576
```
```python
|
```
```python
|
```
```python
query_cache_type
```
```python
|
```
```python
OFF
```
```python
|
```
```python
|
```
```python
read_buffer_size
```
```python
|
```
```python
131072
```
```python
|
```
```python
|
```
```python
read_rnd_buffer_size
```
```python
|
```
```python
262144
```
```python
|
```
```python
|
```
```python
sort_buffer_size
```
```python
|
```
```python
262144
```
```python
|
```
```python
|
```
```python
thread_cache_size
```
```python
|
```
```python
9
```
```python
|
```
```python
|
```
```python
tmp_table_size
```
```python
|
```
```python
16777216
```
```python
|
```
```python
|
```
```python
wait_timeout
```
```python
|
```
```python
28800
```
```python
|
```
```python
+
```
```python
----------------------+----------+
```
```python
11
```
```python
rows
```
```python
in
```
```python
set
```
```python
(
```
```python
0.00
```
```python
sec
```
```python
)
```
```python
mysql
```
```python
>
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 设定建议
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)修改配置文件
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 为了能够持久化的保持，而不至于重启或者容器重新生成后恢复默认状态，需要设定到配置文件中。

> 官方的mysql镜像配置文件：/etc/mysql/mysql.conf.d/mysqld.cnf
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)建议设定示例（需根据相关性能需求与机器状况进行调整）
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# cache settings
```
```python
tmp_table_size
```
```python
=
```
```python
256M
max_heap_table_size
```
```python
=
```
```python
128M
key_buffer_size
```
```python
=
```
```python
128M
back_log
```
```python
=
```
```python
256
read_buffer_size
```
```python
=
```
```python
1M
read_rnd_buffer_size
```
```python
=
```
```python
1M
thread_cache_size
```
```python
=
```
```python
64
query_cache_type
```
```python
=
```
```python
1
query_cache_size
```
```python
=
```
```python
128M
wait_timeout
```
```python
=
```
```python
120
join_buffer_size
```
```python
=
```
```python
1M
sort_buffer_size
```
```python
=
```
```python
1M
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 设定示例
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)由于官方镜像中没有vi，所以可以使用cat + Heredocument直接追加到设定文件。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# cat <<EOF >>/etc/mysql/mysql.conf.d/mysqld.cnf
```
```python
>
```
```python
# cache settings
```
```python
tmp_table_size
```
```python
=
```
```python
256M
max_heap_table_size
```
```python
=
```
```python
128M
key_buffer_size
```
```python
=
```
```python
128M
back_log
```
```python
=
```
```python
256
read_buffer_size
```
```python
=
```
```python
1M
read_rnd_buffer_size
```
```python
=
```
```python
1M
thread_cache_size
```
```python
=
```
```python
64
query_cache_type
```
```python
=
```
```python
1
query_cache_size
```
```python
=
```
```python
128M
wait_timeout
```
```python
=
```
```python
120
join_buffer_size
```
```python
=
```
```python
1M
sort_buffer_size
```
```python
=
```
```python
1M
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
>
```
```python
EOF
```
```python
#
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)重新启动mysql服务，再次确认设定
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
mysql
```
```python
>
```
```python
show
```
```python
variables
```
```python
where
```
```python
variable_name
```
```python
in
```
```python
(
```
```python
'tmp_table_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'max_heap_table_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'back_log'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'read_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'read_rnd_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'thread_cache_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'query_cache_type'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'query_cache_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'wait_timeout'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'join_buffer_size'
```
```python
,
```
```python
\
```
```python
-
```
```python
>
```
```python
'sort_buffer_size'
```
```python
)
```
```python
;
```
```python
+
```
```python
----------------------+-----------+
```
```python
|
```
```python
Variable_name
```
```python
|
```
```python
Value
```
```python
|
```
```python
+
```
```python
----------------------+-----------+
```
```python
|
```
```python
back_log
```
```python
|
```
```python
256
```
```python
|
```
```python
|
```
```python
join_buffer_size
```
```python
|
```
```python
1048576
```
```python
|
```
```python
|
```
```python
max_heap_table_size
```
```python
|
```
```python
134217728
```
```python
|
```
```python
|
```
```python
query_cache_size
```
```python
|
```
```python
134217728
```
```python
|
```
```python
|
```
```python
query_cache_type
```
```python
|
```
```python
ON
```
```python
|
```
```python
|
```
```python
read_buffer_size
```
```python
|
```
```python
1048576
```
```python
|
```
```python
|
```
```python
read_rnd_buffer_size
```
```python
|
```
```python
1048576
```
```python
|
```
```python
|
```
```python
sort_buffer_size
```
```python
|
```
```python
1048576
```
```python
|
```
```python
|
```
```python
thread_cache_size
```
```python
|
```
```python
64
```
```python
|
```
```python
|
```
```python
tmp_table_size
```
```python
|
```
```python
268435456
```
```python
|
```
```python
|
```
```python
wait_timeout
```
```python
|
```
```python
120
```
```python
|
```
```python
+
```
```python
----------------------+-----------+
```
```python
11
```
```python
rows
```
```python
in
```
```python
set
```
```python
(
```
```python
0.00
```
```python
sec
```
```python
)
```
```python
mysql
```
```python
>
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)设定总结
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|设定参数|修改前|修改后|
|---|---|---|
|tmp_table_size|16M|256M|
|max_heap_table_size|16M|128M|
|back_log|80|256|
|read_buffer_size|128K|1M|
|read_rnd_buffer_size|256K|1M|
|thread_cache_size|9|64|
|query_cache_type|ON|OFF|
|query_cache_size|1M|128M|
|wait_timeout|28800|120|
|join_buffer_size|256K|1M|
|sort_buffer_size|256K|1M|
注：后续会对此设定进行跟踪。
[
            ](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

