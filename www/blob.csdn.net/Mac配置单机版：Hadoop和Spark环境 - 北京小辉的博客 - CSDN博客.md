
# Mac���õ����棺Hadoop��Spark���� - ����С�ԵĲ��� - CSDN����
ԭ
# Mac���õ����棺Hadoop��Spark����
�ö�2018��04��27�� 12:58:10[silentwolfyh](https://me.csdn.net/silentwolfyh)�Ķ�����1041


��Ȩ����������Ϊ����ԭ�����£�����Ϊ http://blog.csdn.net/silentwolfyh					https://yuhui.blog.csdn.net/article/details/80106526
## Ŀ¼
### 1������
### 2�����
### 3���ο��ĵ�����������
### 4�����ù���
#### 1)��������
#### 2)Zookeeper(zookeeper-3.4.8)�����ļ���˵
#### 3)hadoop(hadoop-2.7.3)�����ļ���˵
#### 4)Hive(hive-2.3.2)�����ļ���˵
#### 5)Hbase(hbase-1.2.6)�����ļ���˵
#### 6)Spark(spark-2.0.0-bin-hadoop2.7)��Scala(scala-2.11.11)��װ
#### 7)Storm(apache-storm-1.1.2)��װ
#### 8)Kafka(kafka_2.10-0.8.1.1)��װ
#### 9)Kafka(kafka_2.11-1.0.0)��װ
### 5������չʾ
### 1������
�һ���������Hadoop��Spark����
### 2�����
hadoop-2.7.3
zookeeper-3.4.8
hive-2.3.2
hbase-1.2.6
scala-2.11.11
spark-2.0.0-bin-hadoop2.7
apache-storm-1.1.2
kafka_2.11-1.0.0
```
http://archive.apache.org/dist/zookeeper/zookeeper-3.4.8/zookeeper-3.4.8.tar.gz
http://archive.apache.org/dist/hadoop/common/hadoop-2.7.3/hadoop-2.7.3.tar.gz
http://archive.apache.org/dist/hbase/1.2.6/hbase-1.2.6-bin.tar.gz
http://archive.apache.org/dist/phoenix/apache-phoenix-4.14.0-cdh5.14.2/bin/apache-phoenix-4.14.0-cdh5.14.2-bin.tar.gz
http://archive.apache.org/dist/flink/flink-1.6.0/flink-1.6.0-bin-hadoop27-scala_2.11.tgz
```
### 3���ο��ĵ�����������
**���pdf�ļ���ϸ��**
hadoop-Apache2.7.3+Spark2.0��Ⱥ�.pdf��
���ص�ַ��[https://download.csdn.net/download/silentwolfyh/10607814](https://download.csdn.net/download/silentwolfyh/10607814)
### 4�����ù���
��hadoop-Apache2.7.3+Spark2.0��Ⱥ�.pdf ������ĵ�����Ҫ����һ����Ⱥ�����ù��̣�����ϸ����֮���ٿ���������á�
#### 1����������
```
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_151.jdk/Contents/Home
export HADOOP_HOME=/Users/huiyu/DevTools/hadoop-2.7.3
export HIVE_HOME=/Users/huiyu/DevTools/apache-hive-2.3.2-bin
export HBASE_HOME=/Users/huiyu/DevTools/hbase-1.2.6
export SCALA_HOME=/Users/huiyu/DevTools/scala-2.11.11
export SPARK_HOME=/Users/huiyu/DevTools/spark-2.0.0-bin-hadoop2.7
export STORM_HOME=/Users/huiyu/DevTools/apache-storm-1.1.2
export KAFKA_HOME=/Users/huiyu/DevTools/kafka_2.11-1.0.0
export PATH=$PATH:$SPARK_HOME/bin:$HIVE_HOME/bin:$HADOOP_HOME/bin:$HBASE_HOME/bin:$JAVA_HOME/bin:$SCALA_HOME/bin:$STORM_HOME/bin:$KAFKA_HOME/bin
```
#### 2��Zookeeper�����ļ���˵
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/Users/huiyu/DevTools/zookeeper-3.4.8/data
clientPort=2181
```
#### 3��hadoop�����ļ���˵
[hadoop-env.sh](http://hadoop-env.sh)��ֻҪ����Java_home
```
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_151.jdk/Contents/Home
```
core-site.xml
```
<configuration>
<property>
<name>fs.default.name</name>
<value>hdfs://localhost:9000</value>
</property>
<property>
<name>hadoop.tmp.dir</name>
<value>/Users/huiyu/DevTools/hadoop-2.7.3/tmp</value>
</property>
<property>
<name>hadoop.proxyuser.huiyu.hosts</name>
<value>*</value>
</property>
<property>
<name>hadoop.proxyuser.huiyu.groups</name>
<value>*</value>
</property>
</configuration>
```
���У�hadoop.proxyuser.huiyu.hosts��hadoop.proxyuser.huiyu.groups��hive2��Ҫ�����ã�huiyu���û���
hdfs-site.xml
```
<configuration>
<property>
<name>dfs.name.dir</name>
<value>/Users/huiyu/DevTools/hadoop-2.7.3/dfs/name</value>
</property>
<property>
<name>dfs.data.dir</name>
<value>/Users/huiyu/DevTools/hadoop-2.7.3/dfs/data</value>
</property>
<property>
<name>dfs.replication</name>
<value>2</value>
</property>
<property>
<name>dfs.webhdfs.enabled</name>
<value>true</value>
</property>
</configuration>
```
mapred-site.xml
```
<configuration>
<property>
<name>mapreduce.framework.name</name>
<value>yarn</value>
</property>
</configuration>
```
yarn-site.xml
```
<configuration>
<property>
<name>yarn.resourcemanager.address</name>
<value>localhost:8032</value>
</property>
<property>
<name>yarn.nodemanager.aux-services</name>
<value>mapreduce_shuffle</value>
</property>
</configuration>
```
��ע��localhost:8032����Ҫ��hive��ִ��MRʱ���Ҳ���yarn������ָ��localhost:8032��
#### 4��Hive�����ļ���˵
[hive-env.sh](http://hive-env.sh)
```
export HADOOP_HOME=/Users/huiyu/DevTools/hadoop-2.7.3
export HIVE_HOME=/Users/huiyu/DevTools/apache-hive-2.3.2-bin
export HBASE_HOME=/Users/huiyu/DevTools/hbase-1.2.6
```
hive-site.xml
```
<configuration>
<property>
<name>javax.jdo.option.ConnectionURL</name>
<value>jdbc:mysql://localhost:3306/hive?createDatabaseIfNotExist=true</value>
</property>
<property>
<name>javax.jdo.option.ConnectionDriverName</name>
<value>com.mysql.jdbc.Driver</value>
</property>
<property>
<name>javax.jdo.option.ConnectionUserName</name>
<value>root</value>
<description>username to use against metastore database</description>
</property>
<property>
<name>javax.jdo.option.ConnectionPassword</name>
<value>123456</value>
</property>
<property>
<name>hive.server2.transport.mode</name>
<value>binary</value>
</property>
<property>
<name>hive.server2.thrift.sasl.qop</name>
<value>auth</value>
</property>
<property>
<name>hive.metastore.schema.verification</name>
<value>false</value>
</property>
<property>
<name>hive.server2.authentication</name>
<!--
<value>NOSASL</value>
-->
<value>NOSASL</value>
<description>
Expectsoneof[nosasl,none,ldap,kerberos,pam,custom].
Clientauthenticationtypes.
NONE:noauthenticationcheck
LDAP:LDAP/ADbasedauthentication
KERBEROS:Kerberos/GSSAPIauthentication
CUSTOM:Customauthenticationprovider
(Usewithpropertyhive.server2.custom.authentication.class)
PAM:Pluggableauthenticationmodule
NOSASL:Rawtransport
</description>
</property>
</configuration>
```
**����**
```
NestedThrowablesStackTrace:
Required table missing : "`DBS`" in Catalog "" Schema "". DataNucleus requires this table to perform its persistence operations. Either your MetaData is incorrect, or you need to enable "datanucleus.schema.autoCreateTables"
org.datanucleus.store.rdbms.exceptions.MissingTableException: Required table missing : "`DBS`" in Catalog "" Schema "". DataNucleus requires this table to perform its persistence operations. Either your MetaData is incorrect, or you need to enable "datanucleus.schema.autoCreateTables"
```
**���**
[https://www.cnblogs.com/garfieldcgf/p/8134452.html](https://www.cnblogs.com/garfieldcgf/p/8134452.html)
�����Ϳ��Կ�ʼ��ʼ����,������hadoop,Ȼ����binĿ¼��ִ������
```
yuhuideMacBook-Pro:bin huiyu$ ls
beeline        ext            hive           hive-config.sh hiveserver2    hplsql         metatool       schematool
yuhuideMacBook-Pro:bin huiyu$ ./schematool -initSchema -dbType mysql
```
**hive����˳��**
һ�������� metastore
```
hive --service metastore &
```
���������� hiveserver2
```
hive --service hiveserver2 &
```
#### 5��Hbase�����ļ���˵
HbaseҪ�ص�˵˵������Ҫ���ǻ���Hbase�ⲿ��zookeeper�����ڣ�hbase-env.sh���á�export HBASE_MANAGES_ZK=false����hbase-site.xml�����á�hbase.cluster.distributed���͡�base.zookeeper.quorum����������
[hbase-env.sh](http://hbase-env.sh)
```
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_151.jdk/Contents/Home
export HBASE_MANAGES_ZK=false
```
hbase-site.xml
```
<configuration>
<property>
<name>hbase.rootdir</name>
<value>/Users/huiyu/DevTools/hbase-1.2.6/data/hbase</value>
</property>
<property>
<name>hbase.zookeeper.property.dataDir</name>
<value>/Users/huiyu/DevTools/hbase-1.2.6/data/zookeeper</value>
</property>
<property>
<name>hbase.cluster.distributed</name>
<value>true</value>
</property>
<property>
<name>base.zookeeper.quorum</name>
<value>localhost:2181</value>
</property>
</configuration>
```
#### 6��Spark��Scala��װ
Spark���ú�ʹ�ã�Scala�Ͳ�˵�ˡ�
[����spark-env.sh](http://xn--spark-env-9i9x315l.sh)������hadoop���õ�·��
/Users/huiyu/DevTools/spark-2.0.0-bin-hadoop2.7/conf/spark-env.sh
```
HADOOP_CONF_DIR=/Users/huiyu/DevTools/hadoop-2.7.3/etc/hadoop/
```
#### 7)Storm��װ
�������ý���
[https://github.com/apache/storm/blob/v1.1.2/conf/defaults.yaml](https://github.com/apache/storm/blob/v1.1.2/conf/defaults.yaml)
����
[https://www.apache.org/dyn/closer.lua/storm/apache-storm-1.1.2/apache-storm-1.1.2.tar.gz](https://www.apache.org/dyn/closer.lua/storm/apache-storm-1.1.2/apache-storm-1.1.2.tar.gz)
ֻҪ����conf/storm.yaml�ļ�,�޸���������:
```
storm.zookeeper.servers:
     - "127.0.0.1"
storm.zookeeper.port: 2181
nimbus.seeds: ["localhost"]
storm.local.dir: "/Users/huiyu/DevTools/apache-storm-1.1.2"
supervisor.slots.ports:
    - 6700
    - 6701
    - 6702
    - 6703
```
������������:
```
nohup bin/storm nimbus &
nohup bin/storm supervisor &
nohup bin/storm ui &
```
hadoop-Apache2.7.3+Spark2.0��Ⱥ�.pdf��
�ٶ������ص�ַ��
[https://pan.baidu.com/s/13TmW7dITZ9WpYfyg0OZgIw](https://pan.baidu.com/s/13TmW7dITZ9WpYfyg0OZgIw)
#### 8)Kafka(kafka_2.10-0.8.1.1)��װ
��Ҫ�޸ĵ���������: kafka_2.10-0.8.1.1/config/server.properties
```
broker.id=0
log.dirs=/Users/huiyu/DevTools/kafka_2.10-0.8.1.1/kafka-log
zookeeper.connect=localhost:2181
```
����:
```
����topic:
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic yuhui
չʾtopic:
bin/kafka-topics.sh --list --zookeeper localhost:2181
�����ߣ�
bin/kafka-console-producer.sh --broker-list localhost:9092 --topic yuhui
�����ߣ�
kafka-console-consumer.sh --zookeeper localhost:2181 --topic yuhui --from-beginning
```
#### 9)Kafka(kafka_2.11-1.0.0)��װ
��Ҫ�޸ĵ���������:  kafka_2.11-1.0.0/config/server.properties
```
broker.id=1     # ΨһIDͬһ��Ⱥ��broker.id�����ظ�
listeners=PLAINTEXT://localhost:9092   # ������ַ
log.dirs=/opt/kafka_2.11-1.0.1/data      # ����Ŀ¼
log.retention.hours=168   # kafka���ݱ���ʱ�䵥λΪhour Ĭ�� 168Сʱ�� 7�� 
log.retention.bytes=1073741824  #(kafka���������ֵ��������Χ�Զ������� log.retention.hours���ʹ�ã�ע�������ֵ�趨���ɳ����̴�С��
zookeeper.connect:localhost:2181  #(zookeeper����ip��port,����Զ��ŷָ�)
```
����:
```
����topic:
bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic test
չʾtopic:
bin/kafka-topics.sh --list --zookeeper localhost:2181
�����ߣ�
bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
�����ߣ�
bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning
```
### 5������չʾ
HDFS��UI����
![����дͼƬ����](https://img-blog.csdn.net/20180428095703907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Hadoop��UI����
![����дͼƬ����](https://img-blog.csdn.net/20180428095713478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Zookeeper��Shell����
![����дͼƬ����](https://img-blog.csdn.net/20180428095741198?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Hive��Shell����
![����дͼƬ����](https://img-blog.csdn.net/20180428095731854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Hbase��UI����
![����дͼƬ����](https://img-blog.csdn.net/2018042809575428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Spark��UI����
![����дͼƬ����](https://img-blog.csdn.net/20180428095805908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Spark��shell����
![����дͼƬ����](https://img-blog.csdn.net/20180428095813739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Spark��UI����
![����дͼƬ����](https://img-blog.csdn.net/20180428095821677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Storm��UI����
![����дͼƬ����](https://img-blog.csdn.net/20180808161155969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbGVudHdvbGZ5aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

