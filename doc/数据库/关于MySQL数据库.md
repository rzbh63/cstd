# 关于MySQL数据库



MySQL最流行的关系型数据库管理系统

MySQL官网：http://www.mysql.com

MySQL是WEB应用方面最好的RDBMS应用软件之一

RDBMS：Relational Database Management System关系数据库管理系统 

 

问：什么是数据库？

答：数据库（Database）是按照数据结构来组织、存储和管理数据的仓库，每个数据库都有一个或多个不同的API用于创建，访问，管理，搜索和复制所保存的数据。

 

相对于在文件中读写数据，数据库方式的速度会快很多。

　　因此，选择使用关系型数据库管理系统（RDBMS）来存储和管理的大数据量。

 

关系型数据库是建立在关系模型基础上的数据库，借助于集合代数等数学概念和方法来处理数据库中的数据。

RDBMS即关系数据库管理系统(Relational Database Management System)的特点：

　　①数据以表格的形式出现

　　②每行为各种记录名称

　　③每列为记录名称所对应的数据域

　　④许多的行和列组成一张表单

　　⑤若干的表单组成database

 

RDBMS术语

　　①数据库：数据库是一些关联表的集合。.

　　②数据表：表是数据的矩阵。在一个数据库中的表看起来像一个简单的电子表格。

　　③列：一列(数据元素) 包含了相同的数据, 例如邮政编码的数据。

　　④行：一行（=元组，或记录）是一组相关的数据，例如一条用户订阅的数据。

　　⑤冗余：存储两倍数据，冗余降低了性能，但提高了数据的安全性。

　　⑥主键：主键是唯一的。一个数据表中只能包含一个主键。可以使用主键来查询数据。

　　⑦外键：外键用于关联两个表。

　　⑧复合键：复合键（组合键）将多个列作为一个索引键，一般用于复合索引。

　　⑨索引：使用索引可快速访问数据库表中的特定信息。索引是对数据库表中一列或多列的值进行排序的一种结构。类似于书籍的目录。

　　⑩参照完整性：参照的完整性要求关系中不允许引用不存在的实体。与实体完整性是关系模型必须满足的完整性约束条件，目的是保证数据的一致性。

 

MySQL数据库由瑞典MySQL AB公司开发，目前属于Oracle公司。

　　MySQL是一种关联数据库管理系统，关联数据库将数据保存在不同的表中，而不是将所有数据放在一个大仓库内，这样就增加了速度并提高了灵活性。

优势特点：

　　①开源的，不需要支付额外的费用。

　　②支持大型的数据库，可以处理拥有上千万条记录的大型数据库。

　　　　支持5000万条记录的数据仓库，32位系统表文件最大可支持4GB，64位系统支持最大的表文件为8TB。

　　③使用标准的SQL数据语言形式。

　　④允许于多个系统上，并且支持多种语言,对PHP(Web开发语言)有很好的支持。

　　⑤可以定制的，采用GPL协议，可以修改源码来开发自己的Mysql系统。

 