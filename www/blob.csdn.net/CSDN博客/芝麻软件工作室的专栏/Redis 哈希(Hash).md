
# Redis 哈希(Hash) -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:20:19[seven-soft](https://me.csdn.net/softn)阅读数：304个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis hash 是一个string类型的field和value的映射表，hash特别适合用于存储对象。
Redis 中每个 hash 可以存储 232 - 1 键值对（40多亿）。
### 实例
redis127.0.0.1:6379>HMSET w3ckey name"redis tutorial"description"redis basic commands for caching"likes20visitors23000OK
redis127.0.0.1:6379>HGETALL w3ckey1)"name"2)"redis tutorial"3)"description"4)"redis basic commands for caching"5)"likes"6)"20"7)"visitors"8)"23000"在以上实例中，我们设置了 redis 的一些描述信息(name, description, likes, visitors) 到哈希表的 w3ckey 中。
---

## Redis hash 命令
下表列出了 redis hash 基本的相关命令：
|序号|命令及描述|
|1|[HDEL key field2 [field2]](http://www.runoob.com/redis/hashes-hdel.html)
|删除一个或多个哈希表字段|
|2|[HEXISTS key field](http://www.runoob.com/redis/hashes-hexists.html)
|查看哈希表 key 中，指定的字段是否存在。|
|3|[HGET key field](http://www.runoob.com/redis/hashes-hget.html)
|获取存储在哈希表中指定字段的值。|
|4|[HGETALL key](http://www.runoob.com/redis/hashes-hgetall.html)
|获取在哈希表中指定 key 的所有字段和值|
|5|[HINCRBY key field increment](http://www.runoob.com/redis/hashes-hincrby.html)
|为哈希表 key 中的指定字段的整数值加上增量 increment 。|
|6|[HINCRBYFLOAT key field increment](http://www.runoob.com/redis/hashes-hincrbyfloat.html)
|为哈希表 key 中的指定字段的浮点数值加上增量 increment 。|
|7|[HKEYS key](http://www.runoob.com/redis/hashes-hkeys.html)
|获取所有哈希表中的字段|
|8|[HLEN key](http://www.runoob.com/redis/hashes-hlen.html)
|获取哈希表中字段的数量|
|9|[HMGET key field1 [field2]](http://www.runoob.com/redis/hashes-hmget.html)
|获取所有给定字段的值|
|10|[HMSET key field1 value1 [field2 value2 ]](http://www.runoob.com/redis/hashes-hmset.html)
|同时将多个 field-value (域-值)对设置到哈希表 key 中。|
|11|[HSET key field value](http://www.runoob.com/redis/hashes-hset.html)
|将哈希表 key 中的字段 field 的值设为 value 。|
|12|[HSETNX key field value](http://www.runoob.com/redis/hashes-hsetnx.html)
|只有在字段 field 不存在时，设置哈希表字段的值。|
|13|[HVALS key](http://www.runoob.com/redis/hashes-hvals.html)
|获取哈希表中所有值|
|14|HSCAN key cursor [MATCH pattern] [COUNT count]
|迭代哈希表中的键值对。|

