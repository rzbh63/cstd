# MySQL最常用分组聚合函数



一、聚合函数（aggregation function）---也就是组函数

　　在一个行的集合（一组行）上进行操作，对每个组给一个结果。

常用的组函数：

| [AVG([distinct\] expr)](https://www.cnblogs.com/geaozhang/p/6745147.html#sum-avg) | 求平均值     |
| ------------------------------------------------------------ | ------------ |
| [COUNT({*\|[distinct\] } expr)](https://www.cnblogs.com/geaozhang/p/6745147.html#count) | 统计行的数量 |
| [MAX([distinct\] expr)](https://www.cnblogs.com/geaozhang/p/6745147.html#max-min) | 求最大值     |
| [MIN([distinct\] expr)](https://www.cnblogs.com/geaozhang/p/6745147.html#max-min) | 求最小值     |
| [SUM([distinct\] expr)](https://www.cnblogs.com/geaozhang/p/6745147.html#sum-avg) | 求累加和     |

　　①每个组函数接收一个参数

　　②默认情况下，组函数忽略列值为null的行，不参与计算

　　③有时，会使用关键字distinct剔除字段值重复的条数

注意：

　　1）当使用组函数的select语句中没有group by子句时，中间结果集中的所有行自动形成一组，然后计算组函数；

　　2）组函数不允许嵌套，例如：count(max(…))；

　　3）组函数的参数可以是列或是函数表达式；

　　4）一个SELECT子句中可出现多个聚集函数。

 

实验演示用表：

```
mysql> select * from salary_tab;
+--------+---------+
| userid | salary  |
+--------+---------+
|      1 | 1000.00 |
|      2 | 2000.00 |
|      3 | 3000.00 |
|      4 |    NULL |
|      5 | 1000.00 |
+--------+---------+
5 rows in set (0.00 sec) 
mysql> use TENNIS
mysql> show tables;
+-------------------+
| Tables_in_TENNIS  |
+-------------------+
| COMMITTEE_MEMBERS |
| MATCHES           |
| PENALTIES         |
| PLAYERS           |
| TEAMS             |
+-------------------+
5 rows in set (0.00 sec)
```

 

1、count函数

①count(*)：返回表中满足where条件的行的数量

```
mysql> select count(*) from salary_tab where salary='1000';
+----------+
| count(*) |
+----------+
|        2 |
+----------+

mysql> select count(*) from salary_tab;　　#没有条件，默认统计表数据行数
+----------+
| count(*) |
+----------+
|        5 |
+----------+
```

②count(列)：返回列值非空的行的数量

```
mysql> select count(salary) from salary_tab;
+---------------+
| count(salary) |
+---------------+
|             4 |
+---------------+
```

③count(distinct 列)：返回列值非空的、并且列值不重复的行的数量

```
mysql> select count(distinct salary) from salary_tab;
+------------------------+
| count(distinct salary) |
+------------------------+
|                      3 |
+------------------------+
```

④count(expr)：根据表达式统计数据

```
mysql> select * from TT;
+------+------------+
| UNIT | DATE       |
+------+------------+
| a    | 2018-04-03 |
| a    | 2017-12-12 |
| b    | 2018-01-01 |
| b    | 2018-04-03 |
| c    | 2016-06-06 |
| d    | 2018-03-03 |
+------+------------+
6 rows in set (0.00 sec)

mysql> select UNIT as '单位',
    ->     COUNT(TO_DAYS(DATE)=TO_DAYS(NOW()) or null) as '今日统计',
    ->     COUNT(YEAR(DATE)=YEAR(NOW()) or null) as '今年统计'
    -> from v_jjd
    -> group by JJDW;
+------+----------+----------+
| 单位  | 今日统计  | 今年统计  |
+------+----------+----------+
| a    |        1 |        1 |
| b    |        1 |        2 |
| c    |        0 |        0 |
| d    |        0 |        1 |
+------+----------+----------+
4 rows in set (0.00 sec)
```

 

2、max和min函数---统计列中的最大最小值

```
mysql> select max(salary) from salary_tab;
+-------------+
| max(salary) |
+-------------+
|     3000.00 |
+-------------+

mysql> select min(salary) from salary_tab;
+-------------+
| min(salary) |
+-------------+
|     1000.00 |
+-------------+
```

注意：如果统计的列中只有NULL值，那么MAX和MIN就返回NULL

 

3、sum和avg函数---求和与求平均

！！表中列值为null的行不参与计算

```
mysql> select sum(salary) from salary_tab;
+-------------+
| sum(salary) |
+-------------+
|     7000.00 |
+-------------+

mysql> select avg(salary) from salary_tab;
+-------------+
| avg(salary) |
+-------------+
| 1750.000000 |
+-------------+

mysql> select avg(ifnull(salary,0)) from salary_tab;
+-----------------------+
| avg(ifnull(salary,0)) |
+-----------------------+
|           1400.000000 |
+-----------------------+
```

注意：要想列值为NULL的行也参与组函数的计算，必须使用IFNULL函数对NULL值做转换。

 

 

二、分组SELECT

```
SELECT select_expr [, select_expr ...]

    [FROM table_references

      [PARTITION partition_list]

    [WHERE where_condition]

    [GROUP BY {col_name | expr | position}

      [ASC | DESC], ... [WITH ROLLUP]]

    [HAVING where_condition]

    [ORDER BY {col_name | expr | position}

      [ASC | DESC], ...]

[LIMIT {[offset,] row_count | row_count OFFSET offset}]
```

分组SELECT的基本格式：

　　select [聚合函数] 字段名 from 表名

　　　　[where 查询条件]

　　　　[group by 字段名]

　　　　[having 过滤条件]

1、group by子句

　　根据给定列或者表达式的每一个不同的值将表中的行分成不同的组，使用组函数返回每一组的统计信息

规则：

　　①出现在SELECT子句中的单独的列，必须出现在GROUP BY子句中作为分组列

　　②分组列可以不出现在SELECT子句中

　　③分组列可出现在SELECT子句中的一个复合表达式中

　　④如果GROUP BY后面是一个复合表达式，那么在SELECT子句中，它必须整体作为一个表达式的一部分才能使用。

1）指定一个列进行分组

```
mysql> select salary,count(*) from salary_tab
    -> where salary>=2000
    -> group by salary;   
+---------+----------+
| salary  | count(*) |
+---------+----------+
| 2000.00 |        1 |
| 3000.00 |        1 |
+---------+----------+
```

2）指定多个分组列，‘大组中再分小组’

```
mysql> select userid,count(salary) from salary_tab
    -> where salary>=2000
    -> group by salary,userid;   
+--------+---------------+
| userid | count(salary) |
+--------+---------------+
|      2 |             1 |
|      3 |             1 |
+--------+---------------+
```

3）根据表达式分组

```
mysql> select year(payment_date),count(*)
    -> from PENALTIES
    -> group by year(payment_date);
+--------------------+----------+
| year(payment_date) | count(*) |
+--------------------+----------+
|               1980 |        3 |
|               1981 |        1 |
|               1982 |        1 |
|               1983 |        1 |
|               1984 |        2 |
+--------------------+----------+
5 rows in set (0.00 sec)
```

4）带有排序的分组：如果分组列和排序列相同，则可以合并group by和order by子句

```
mysql> select teamno,count(*)
    -> from MATCHES
    -> group by teamno
    -> order by teamno desc;
+--------+----------+
| teamno | count(*) |
+--------+----------+
|      2 |        5 |
|      1 |        8 |
+--------+----------+
2 rows in set (0.00 sec)

mysql> select teamno,count(*)
    -> from MATCHES
    -> group by teamno desc;　　#可以把desc(或者asc)包含到group by子句中简化
+--------+----------+
| teamno | count(*) |
+--------+----------+
|      2 |        5 |
|      1 |        8 |
+--------+----------+
2 rows in set (0.00 sec)
```

对于分组聚合注意：

　　通过select在返回集字段中，这些字段要么就要包含在group by语句后面，作为分组的依据，要么就要被包含在聚合函数中。我们可以将group by操作想象成如下的一个过程：首先系统根据select语句得到一个结果集，然后根据分组字段，将具有相同分组字段的记录归并成了一条记录。这个时候剩下的那些不存在与group by语句后面作为分组依据的字段就很有可能出现多个值，但是目前一种分组情况只有一条记录，一个数据格是无法放入多个数值的，所以这个时候就需要通过一定的处理将这些多值的列转化成单值，然后将其放在对应的数据格中，那么完成这个步骤的就是前面讲到的聚合函数，这也就是为什么这些函数叫聚合函数了。

 

2、GROUP_CONCAT()函数

　　函数的值等于属于一个组的指定列的所有值，以逗号隔开，并且以字符串表示。

例1：对于每个球队，得到其编号和所有球员的编号

```
mysql> select teamno,group_concat(playerno)
    -> from MATCHES
    -> group by teamno;
+--------+------------------------+
| teamno | group_concat(playerno) |
+--------+------------------------+
|      1 | 6,6,6,44,83,2,57,8     |
|      2 | 27,104,112,112,8       |
+--------+------------------------+
2 rows in set (0.01 sec)
```

如果没有group by子句，group_concat返回一列的所有值

例2：得到所有的罚款编号列表

```
mysql> select group_concat(paymentno)
    -> from PENALTIES;
+-------------------------+
| group_concat(paymentno) |
+-------------------------+
| 1,2,3,4,5,6,7,8         |
+-------------------------+
1 row in set (0.00 sec)
```

 

3、with rollup子句：用来要求在一条group by子句中进行多个不同的分组

用的比较少点，但是有时可以根据具体的需求使用

　　如果有子句GROUP BY E1,E2,E3,E4 WITH ROLLUP

　　那么将分别执行以下分组：[E1,E2,E3,E4]、[E1,E2,E3]、[E1,E2]、[E1]、[]

注意：[ ]表示所有行都分在一组中

示例：按照球员的性别和居住城市，统计球员的总数；统计每个性别球员的总数；统计所有球员的总数

```
mysql> select sex,town,count(*)
    -> from PLAYERS
    -> group by sex,town with rollup;
+-----+-----------+----------+
| sex | town      | count(*) |
+-----+-----------+----------+
| F   | Eltham    |        2 |
| F   | Inglewood |        1 |
| F   | Midhurst  |        1 |
| F   | Plymouth  |        1 |
| F   | NULL      |        5 |
| M   | Douglas   |        1 |
| M   | Inglewood |        1 |
| M   | Stratford |        7 |
| M   | NULL      |        9 |
| NULL | NULL      |       14 |
+-----+-----------+----------+
10 rows in set (0.00 sec)
```

 

4、HAVING子句：对分组结果进行过滤

注意：

　　不能使用WHERE子句对分组后的结果进行过滤

　　不能在WHERE子句中使用组函数，仅用于过滤行

```
mysql> select playerno
    -> from PENALTIES
    -> where count(*)>1
    -> group by playerno;
ERROR 1111 (HY000): Invalid use of group function
```

因为WHERE子句比GROUP BY先执行，而组函数必须在分完组之后才执行，且分完组后必须使用having子句进行结果集的过滤。

基本语法：

```
SELECT   select_expr [, select_expr ...]

   FROM  table_name

   [WHERE where_condition]

   [GROUP BY {col_name | expr} [ASC | DESC], ... [WITH ROLLUP]]

[HAVING where_condition]
```

！！！having子语句与where子语句区别：

　　where子句在分组前对记录进行过滤；

　　having子句在分组后对记录进行过滤

```
mysql> select salary,count(*) from salary_tab
    -> where salary>=2000
    -> group by salary
    -> having count(*)>=0;
+---------+----------+
| salary  | count(*) |
+---------+----------+
| 2000.00 |        1 |
| 3000.00 |        1 |
+---------+----------+
```

1）HAVING可以单独使用而不和GROUP BY配合,如果只有HAVING子句而没有GROUP BY，表中所有的行分为一组

2）HAVING子句中可以使用组函数

3）HAVING子句中的列，要么出现在一个组函数中，要么出现在GROUP BY子句中(否则出错)

```
mysql> select town,count(*)
    -> from PLAYERS
    -> group by town
    -> having birth_date>'1970-01-01';
ERROR 1054 (42S22): Unknown column 'birth_date' in 'having clause'
mysql> select town,count(*)
    -> from PLAYERS
    -> group by town
    -> having town in ('Eltham','Midhurst');
+----------+----------+
| town     | count(*) |
+----------+----------+
| Eltham   |        2 |
| Midhurst |        1 |
+----------+----------+
2 rows in set (0.00 sec)
```

 

 

三、集合查询操作

　　union用于把两个或者多个select查询的结果集合并成一个

```
SELECT ...

UNION [ALL | DISTINCT]

SELECT ...

[UNION [ALL | DISTINCT]

SELECT ...]
```

默认情况下，UNION = UNION DISTINCT

　　①进行合并的两个查询，其SELECT列表必须在数量和对应列的数据类型上保持一致；

　　②默认会去掉两个查询结果集中的重复行；默认结果集不排序；

　　③最终结果集的列名来自于第一个查询的SELECT列表

UNION  ALL不去掉结果集中重复的行

注：联合查询结果使用第一个select语句中的字段名

```
mysql> select * from t1;
+------+------+
| num  | addr |
+------+------+
|  123 | abc  |
|  321 | cba  |
+------+------+
2 rows in set (0.00 sec)

mysql> select * from t2;
+------+------+
| id   | name |
+------+------+
|    1 | a    |
|    2 | A    |
+------+------+
2 rows in set (0.00 sec)

mysql> select * from t1
    -> union
    -> select * from t2;
+------+------+
| num  | addr |
+------+------+
|  123 | abc  |
|  321 | cba  |
|    1 | a    |
|    2 | A    |
+------+------+
4 rows in set (0.00 sec)
```

如果要对合并后的整个结果集进行排序，ORDER BY子句只能出现在最后面的查询中

注意：

　　在去重操作时，如果列值中包含NULL值，认为它们是相等的

@author：http://www.cnblogs.com/geaozhang/