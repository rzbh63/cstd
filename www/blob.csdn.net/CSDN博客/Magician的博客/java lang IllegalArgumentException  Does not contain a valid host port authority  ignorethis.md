
# java.lang.IllegalArgumentException: Does not contain a valid host:port authority: ignorethis - Magician的博客 - CSDN博客


2017年11月08日 23:38:56[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：363


执行Hive语句运行MapReduce程序时突然出现这样的异常：
```python
Total MapReduce jobs
```
```python
=
```
```python
1
```
```python
Launching Job
```
```python
1
```
```python
out of
```
```python
1
```
```python
Number of reduce tasks
```
```python
not
```
```python
specified
```
```python
.
```
```python
Estimated from input
```
```python
data
```
```python
size:
```
```python
1
```
```python
In
```
```python
order
```
```python
to
```
```python
change the
```
```python
average
```
```python
load for a reducer (
```
```python
in
```
```python
bytes
```
```python
):
```
```python
set
```
```python
hive
```
```python
.
```
```python
exec
```
```python
.
```
```python
reducers
```
```python
.
```
```python
bytes
```
```python
.
```
```python
per
```
```python
.
```
```python
reducer
```
```python
=<
```
```python
number
```
```python
>
```
```python
In
```
```python
order
```
```python
to
```
```python
limit the maximum number of reducers:
```
```python
set
```
```python
hive
```
```python
.
```
```python
exec
```
```python
.
```
```python
reducers
```
```python
.
```
```python
max
```
```python
=<
```
```python
number
```
```python
>
```
```python
In
```
```python
order
```
```python
to
```
```python
set
```
```python
a constant number of reducers:
```
```python
set
```
```python
mapred
```
```python
.
```
```python
reduce
```
```python
.
```
```python
tasks
```
```python
=<
```
```python
number
```
```python
>
```
```python
Starting Job
```
```python
=
```
```python
job_1510149586421_0024, Tracking URL
```
```python
=
```
```python
http:
```
```python
//shizhan:8088/proxy/application_1510149586421_0024/
```
```python
Kill Command
```
```python
=
```
```python
/root/apps/hadoop
```
```python
-
```
```python
2.6
```
```python
.4
```
```python
/bin/hadoop job
```
```python
-Dmapred
```
```python
.
```
```python
job
```
```python
.
```
```python
tracker
```
```python
=
```
```python
ignorethis
```
```python
-kill
```
```python
job_1510149586421_0024
Hadoop job information for Stage
```
```python
-
```
```python
1
```
```python
: number of mappers:
```
```python
1
```
```python
; number of reducers:
```
```python
1
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
28
```
```python
,
```
```python
184
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
0
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
37
```
```python
,
```
```python
507
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
38
```
```python
,
```
```python
539
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
39
```
```python
,
```
```python
564
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
40
```
```python
,
```
```python
588
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
41
```
```python
,
```
```python
615
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
42
```
```python
,
```
```python
644
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
43
```
```python
,
```
```python
679
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
44
```
```python
,
```
```python
709
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
45
```
```python
,
```
```python
737
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
46
```
```python
,
```
```python
770
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
47
```
```python
,
```
```python
794
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
0
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
48
```
```python
,
```
```python
823
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
100
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
55
```
```python
:
```
```python
49
```
```python
,
```
```python
855
```
```python
Stage
```
```python
-
```
```python
1
```
```python
map
```
```python
=
```
```python
100
```
```python
%
```
```python
,  reduce
```
```python
=
```
```python
100
```
```python
%
```
```python
, Cumulative CPU
```
```python
1.4
```
```python
sec
MapReduce Total cumulative CPU time:
```
```python
1
```
```python
seconds
```
```python
400
```
```python
msec
Ended Job
```
```python
=
```
```python
job_1510149586421_0024
```
```python
with
```
```python
errors
Error during job, obtaining debugging information
```
```python
...
```
```python
Examining task ID: task_1510149586421_0024_m_000000 (
```
```python
and
```
```python
more) from job job_1510149586421_0024
Exception
```
```python
in
```
```python
thread
```
```python
"Thread-414"
```
```python
java
```
```python
.
```
```python
lang
```
```python
.
```
```python
IllegalArgumentException: Does
```
```python
not
```
```python
contain a valid host:port authority: ignorethis
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
net
```
```python
.
```
```python
NetUtils
```
```python
.
```
```python
createSocketAddr(NetUtils
```
```python
.
```
```python
java:
```
```python
212
```
```python
)
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
net
```
```python
.
```
```python
NetUtils
```
```python
.
```
```python
createSocketAddr(NetUtils
```
```python
.
```
```python
java:
```
```python
164
```
```python
)
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
net
```
```python
.
```
```python
NetUtils
```
```python
.
```
```python
createSocketAddr(NetUtils
```
```python
.
```
```python
java:
```
```python
153
```
```python
)
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
hive
```
```python
.
```
```python
ql
```
```python
.
```
```python
exec
```
```python
.
```
```python
JobTrackerURLResolver
```
```python
.
```
```python
getURL(JobTrackerURLResolver
```
```python
.
```
```python
java:
```
```python
42
```
```python
)
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
hive
```
```python
.
```
```python
ql
```
```python
.
```
```python
exec
```
```python
.
```
```python
JobDebugger
```
```python
.
```
```python
showJobFailDebugInfo(JobDebugger
```
```python
.
```
```python
java:
```
```python
209
```
```python
)
        at org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
hive
```
```python
.
```
```python
ql
```
```python
.
```
```python
exec
```
```python
.
```
```python
JobDebugger
```
```python
.
```
```python
run(JobDebugger
```
```python
.
```
```python
java:
```
```python
92
```
```python
)
        at java
```
```python
.
```
```python
lang
```
```python
.
```
```python
Thread
```
```python
.
```
```python
run(
```
```python
Thread
```
```python
.
```
```python
java:
```
```python
744
```
```python
)
FAILED: Execution Error,
```
```python
return
```
```python
code
```
```python
2
```
```python
from org
```
```python
.
```
```python
apache
```
```python
.
```
```python
hadoop
```
```python
.
```
```python
hive
```
```python
.
```
```python
ql
```
```python
.
```
```python
exec
```
```python
.
```
```python
MapRedTask
MapReduce Jobs Launched: 
Job
```
```python
0
```
```python
:
```
```python
Map
```
```python
:
```
```python
1
```
```python
Reduce:
```
```python
1
```
```python
Cumulative CPU:
```
```python
1.4
```
```python
sec   HDFS Read:
```
```python
1125
```
```python
HDFS Write:
```
```python
0
```
```python
FAIL
Total MapReduce CPU Time Spent:
```
```python
1
```
```python
seconds
```
```python
400
```
```python
msec
hive
```
```python
>
```
```python
date
```
```python
-s
```
```python
'2017-11-8 22:58:00'
```
```python
>
```
```python
;
FAILED: Parse Error: line
```
```python
1
```
```python
:
```
```python
0
```
```python
cannot recognize input near
```
```python
'date'
```
```python
'-'
```
```python
's'
```
平时怎么操作都没问题，今天突然就抛出这样的异常，也有点让人摸不着头脑。重点是执行某些简单的查询语句时没有任何问题，一旦查询语句复杂点，就会抛出这样的异常。根据抛出的异常信息发现不了什么，于是就去查看hadoop打印的log日志，发现了问题的原因。
关键日志如下：
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
54
```
```python
:
```
```python
52
```
```python
,
```
```python
543
```
```python
INFO org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.recovery
```
```python
.RMStateStore
```
```python
: Updating info for app: application_1510149586421_0023
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
54
```
```python
:
```
```python
52
```
```python
,
```
```python
545
```
```python
INFO org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.rmapp
```
```python
.RMAppImpl
```
```python
: Application application_1510149586421_0023 failed
```
```python
2
```
```python
times due to Error launching appattempt_1510149586421_0023_000002. Got exception: org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.exceptions
```
```python
.YarnException
```
```python
: Unauthorized request to start container. 
This token is expired. current time is
```
```python
1510181693928
```
```python
found
```
```python
1510153492530
```
```python
Note:
```
```python
System times on machines may be
```
```python
out
```
```python
of sync. Check system time
```
```python
and
```
```python
time zones.
    at sun
```
```python
.reflect
```
```python
.GeneratedConstructorAccessor
```
```python
47
```
```python
.newInstance
```
```python
(Unknown Source)
    at sun
```
```python
.reflect
```
```python
.DelegatingConstructorAccessorImpl
```
```python
.newInstance
```
```python
(DelegatingConstructorAccessorImpl
```
```python
.java
```
```python
:
```
```python
45
```
```python
)
    at java
```
```python
.lang
```
```python
.reflect
```
```python
.Constructor
```
```python
.newInstance
```
```python
(Constructor
```
```python
.java
```
```python
:
```
```python
526
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.api
```
```python
.records
```
```python
.impl
```
```python
.pb
```
```python
.SerializedExceptionPBImpl
```
```python
.instantiateException
```
```python
(SerializedExceptionPBImpl
```
```python
.java
```
```python
:
```
```python
168
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.api
```
```python
.records
```
```python
.impl
```
```python
.pb
```
```python
.SerializedExceptionPBImpl
```
```python
.deSerialize
```
```python
(SerializedExceptionPBImpl
```
```python
.java
```
```python
:
```
```python
106
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.amlauncher
```
```python
.AMLauncher
```
```python
.launch
```
```python
(AMLauncher
```
```python
.java
```
```python
:
```
```python
123
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.amlauncher
```
```python
.AMLauncher
```
```python
.run
```
```python
(AMLauncher
```
```python
.java
```
```python
:
```
```python
251
```
```python
)
    at java
```
```python
.util
```
```python
.concurrent
```
```python
.ThreadPoolExecutor
```
```python
.runWorker
```
```python
(ThreadPoolExecutor
```
```python
.java
```
```python
:
```
```python
1145
```
```python
)
    at java
```
```python
.util
```
```python
.concurrent
```
```python
.ThreadPoolExecutor
```
```python
$Worker
```
```python
.run
```
```python
(ThreadPoolExecutor
```
```python
.java
```
```python
:
```
```python
615
```
```python
)
    at java
```
```python
.lang
```
```python
.Thread
```
```python
.run
```
```python
(Thread
```
```python
.java
```
```python
:
```
```python
744
```
```python
)
. Failing the application.
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
54
```
```python
:
```
```python
52
```
```python
,
```
```python
545
```
```python
INFO org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.rmapp
```
```python
.RMAppImpl
```
```python
: application_1510149586421_0023 State change from FINAL_SAVING to FAILED
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
54
```
```python
:
```
```python
52
```
```python
,
```
```python
545
```
```python
WARN org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.RMAuditLogger
```
```python
: USER=root OPERATION=Application Finished - Failed TARGET=RMAppManager RESULT=FAILURE  DESCRIPTION=App failed with state: FAILED   PERMISSIONS=Application application_1510149586421_0023 failed
```
```python
2
```
```python
times due to Error launching appattempt_1510149586421_0023_000002. Got exception: org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.exceptions
```
```python
.YarnException
```
```python
: Unauthorized request to start container. 
This token is expired. current time is
```
```python
1510181693928
```
```python
found
```
```python
1510153492530
```
```python
Note:
```
```python
System times on machines may be
```
```python
out
```
```python
of sync. Check system time
```
```python
and
```
```python
time zones.
    at sun
```
```python
.reflect
```
```python
.GeneratedConstructorAccessor
```
```python
47
```
```python
.newInstance
```
```python
(Unknown Source)
    at sun
```
```python
.reflect
```
```python
.DelegatingConstructorAccessorImpl
```
```python
.newInstance
```
```python
(DelegatingConstructorAccessorImpl
```
```python
.java
```
```python
:
```
```python
45
```
```python
)
    at java
```
```python
.lang
```
```python
.reflect
```
```python
.Constructor
```
```python
.newInstance
```
```python
(Constructor
```
```python
.java
```
```python
:
```
```python
526
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.api
```
```python
.records
```
```python
.impl
```
```python
.pb
```
```python
.SerializedExceptionPBImpl
```
```python
.instantiateException
```
```python
(SerializedExceptionPBImpl
```
```python
.java
```
```python
:
```
```python
168
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.api
```
```python
.records
```
```python
.impl
```
```python
.pb
```
```python
.SerializedExceptionPBImpl
```
```python
.deSerialize
```
```python
(SerializedExceptionPBImpl
```
```python
.java
```
```python
:
```
```python
106
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.amlauncher
```
```python
.AMLauncher
```
```python
.launch
```
```python
(AMLauncher
```
```python
.java
```
```python
:
```
```python
123
```
```python
)
    at org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.amlauncher
```
```python
.AMLauncher
```
```python
.run
```
```python
(AMLauncher
```
```python
.java
```
```python
:
```
```python
251
```
```python
)
    at java
```
```python
.util
```
```python
.concurrent
```
```python
.ThreadPoolExecutor
```
```python
.runWorker
```
```python
(ThreadPoolExecutor
```
```python
.java
```
```python
:
```
```python
1145
```
```python
)
    at java
```
```python
.util
```
```python
.concurrent
```
```python
.ThreadPoolExecutor
```
```python
$Worker
```
```python
.run
```
```python
(ThreadPoolExecutor
```
```python
.java
```
```python
:
```
```python
615
```
```python
)
    at java
```
```python
.lang
```
```python
.Thread
```
```python
.run
```
```python
(Thread
```
```python
.java
```
```python
:
```
```python
744
```
```python
)
. Failing the application.  APPID=application_1510149586421_0023
```
```python
2017
```
```python
-
```
```python
11
```
```python
-
```
```python
08
```
```python
22
```
```python
:
```
```python
54
```
```python
:
```
```python
52
```
```python
,
```
```python
545
```
```python
INFO org
```
```python
.apache
```
```python
.hadoop
```
```python
.yarn
```
```python
.server
```
```python
.resourcemanager
```
```python
.RMAppManager
```
```python
$ApplicationSummary:
```
可以看到这样的一行：Note: System times on machines may be out of sync. Check system time and time zones.
原来是节点间的时间不同步。。。
这样也能解释下为什么有些简单的语句可以执行，有些复杂的却执行不了。
原因就是当我执行简单语句时，maptask和reducetask都在同一个节点上执行，不会有时间不同步问题；而当我执行复杂的语句时，也许有一个或多个reducetask，如果是多个的话，那么多个reducetask分布在不同的节点上，时间就会不同步，最终写出数据到文件时就会有时间差异，所以才会报错。如果仍是一个，那这一个reducetask肯定在另一个节点上，时间不同步，才会抛出异常。（比如我reduce的时间要早于map的时间）
但是从最初的异常信息来看，我个人认为是在最终将reduce的结果输出到文件的这个阶段出现了异常（reduce阶段已经达到100%）。当然，如果想确认在那一块出现了问题，还得去查看源码。

