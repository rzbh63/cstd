
# SQLite -  芝麻软件工作室的专栏 - CSDN博客


2012年05月02日 20:17:50[seven-soft](https://me.csdn.net/softn)阅读数：302标签：[sqlite																](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[insert																](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[reference																](https://so.csdn.net/so/search/s.do?q=reference&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=function&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=reference&t=blog)个人分类：[SQLite																](https://blog.csdn.net/softn/article/category/1137974)
[
																								](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=insert&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=insert&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)
SQLite，是一款轻型的数据库，是遵守ACID的关联式[数据库管理系统](http://baike.baidu.com/view/68446.htm)，它的设计目标是嵌入式的，而且目前已经在很多嵌入式产品中使用了它，它占用资源非常的低，在嵌入式设备中，可能只需要几百K的内存就够了。它能够支持Windows/Linux/Unix等等主流的[操作系统](http://baike.baidu.com/view/880.htm)，同时能够跟很多程序语言相结合，比如
 Tcl、C\#、PHP、Java等，还有ODBC接口，同样比起Mysql、PostgreSQL这两款开源世界著名的数据库管理系统来讲，它的处理速度比他们都快。SQLite第一个[Alpha版本](http://baike.baidu.com/view/707803.htm)诞生于2000年5月. 至今已经有10个年头，SQLite也迎来了一个版本 SQLite 3已经发布。

## SQLite功能
SQLite虽然很小巧，但是支持的SQL语句不会逊色于其他开源数据库，它支持的SQL包括： 　　ATTACH DATABASE 　　BEGIN TRANSACTION 　　comment 　　COMMIT TRANSACTION 　　COPY 　　CREATE INDEX 　　CREATE TABLE 　　CREATE TRIGGER 　　CREATE VIEW 　　DELETE 　　DETACH DATABASE 　　DROP INDEX 　　DROP TABLE 　　DROP TRIGGER 　　DROP
 VIEW 　　END TRANSACTION 　　EXPLAIN 　　expression 　　INSERT 　　ON CONFLICT clause 　　PRAGMA 　　REPLACE 　　ROLLBACK TRANSACTION 　　SELECT 　　UPDATE 　　同时它还支持事务处理功能等等。也有人说它象Microsoft的Access，有时候真的觉得有点象，但是事实上它们区别很大。比如SQLite 支持跨平台，操作简单，能够使用很多语言直接创建数据库，而不象Access一样需要Office的支持。如果你是个很小型的应用，或者你想做嵌入式开发，没有合适的[数据库系统](http://baike.baidu.com/view/7809.htm)，那么现在你可以考虑使用SQLite。目前它的最新版本是
 3.7.8 。它的官方网站是：http://www.sqlite. org或者http://www.sqlite .com .cn，能在上面获得[源代码](http://baike.baidu.com/view/60376.htm)和文档。同时因为数据库结构简单，系统源代码也不是很多，也适合想研究数据库系统开发的专业人士
## SQLite特性
下面是访问SQLite官方网站: http://www.sqlite. org/ 时第一眼看到关于SQLite的特性. 　　1. ACID事务 　　2. 零配置 – 无需安装和管理配置 　　3. 储存在单一磁盘文件中的一个完整的数据库 　　4.[数据库文件](http://baike.baidu.com/view/1800925.htm)可以在不同字节顺序的机器间自由的共享 　　5. 支持数据库大小至2TB 　　6. 足够小, 大致3万行C代码, 250K 　　7. 比一些流行的数据库在大部分普通数据库操作要快 　　8. 简单, 轻松的API 　　9. 包含TCL绑定, 同时通过Wrapper支持其他语言的绑定 　　10. 良好注释的源代码, 并且有着90%以上的测试覆盖率 　　11. 独立:
 没有额外依赖 　　12. Source完全的Open, 你可以用于任何用途, 包括出售它 　　13. 支持多种开发语言,C, PHP, Perl, Java, C\#,Python
## SQLite类型
SQLite的数据类型 　　首先你会接触到一个让你惊讶的名词: Typelessness(无类型). 对! SQLite是无类型的. 这意味着你可以保存任何类型的数据到你所想要保存的任何表的任何列中, 无论这列声明的数据类型是什么(只有在一种情况下不是, 稍后解释). 对于SQLite来说对字段不指定类型是完全有效的. 如: 　　Create Table ex1(a, b, c); 　　诚然SQLite允许忽略数据类型, 但是仍然建议在你的Create Table语句中指定数据类型. 因为数据类型对于你和其他的程序员交流,
 或者你准备换掉你的数据库引擎时能起到一个提示或帮助的作用. SQLite支持常见的数据类型, 如: 　　CREATE TABLE ex2( 　　a VARCHAR(10), 　　b NVARCHAR(15), 　　c TEXT, 　　d INTEGER, 　　e FLOAT, 　　f BOOLEAN, 　　g CLOB, 　　h BLOB, 　　i TIMESTAMP, 　　j NUMERIC(10,5) 　　k VARYING CHARACTER (24), 　　l NATIONAL VARYING CHARACTER(16)
 　　); 　　前面提到在某种情况下, SQLite的字段并不是无类型的. 即在字段类型为”Integer Primary Key”时.
## 如何连接SQLite？
### 1.用PHP操作sqlite数据库
a、 如何连接sqlite数据库？ 　　if ($db = sqlite_open('mysqlitedb', 0666, $sqliteerror)) { 　　select * from sqlite_master; 　　echo "数据库连接成功！n　　"; 　　} else { 　　die($sqliteerror); 　　} 　　b、 如何列出数据库中所有的表？ 　　if ($db = sqlite_open('mysqlitedb', 0666, $sqliteerror)) { 　　$result
 = sqlite_array_query($db, 'select * from sqlite_master;'); 　　foreach ($result as $entry) { 　　echo 'talbe name='.$entry['name']."　　n"; 　　echo 'sql='.$entry['sql']."　　n"; 　　echo " 　　--------------------------------------------------------------------------------　　";
 　　} 　　sqlite_close($db); 　　} else { 　　die($sqliteerror); 　　}} 　　c、 对sqlite数据库的查询，以及结果集的显示 　　if ($db = sqlite_open('mysqlitedb', 0666, $sqliteerror)) { 　　$result = sqlite_array_query($db, 'select name, email from user ', SQLITE_ASSOC); 　　echo "user表查询结果:　　n";
 　　echo " n name email 　　n"; 　　foreach ($result as $entry) { 　　echo ' '.$entry['name']." " $entry['email']." 　　n"; 　　} 　　echo ' '; 　　sqlite_close($db); 　　} else { 　　die($sqliteerror); 　　} 　　d、 数据库对象记录的增加、删除、修改 　　sqlite_query($db, "INSERT INTO user VALUES('user".$i."'"
 ",'user".$i."@ hichina. com')"); 　　sqlite_query($db, "delete from user where user=’user99’"); 　　sqlite_query($db, 'UPDATE user SET email="lilz@ hichina .com" where name="user1"');
### 2.用JAVA连接SQLite
先下载SQLite数据库的JDBC　　这里给出一个中文站点的URL:（参见扩展阅读）　　将下载到的包解压后得到jar包 sqlitejdbc-v033-nested.jar 放到%JAVA_HOME%\lib 下，　　并且将其添加到classpath系统环境变量中,我的classpath系统环境变量现在为:　　.;%JAVA_HOME%\lib;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\sqlitejdbc-v033-nested.jar　　在你的代码中引用这个驱动:　　Class.forName("org.sqlite.JDBC");　　Connection
 conn = DriverManager.getConnection("jdbc:sqlite:filename");//filename为你的SQLite数据名称　　// ... use the database ...　　conn.close();　　示例程序如下：|importjava.sql.*;
|importorg.sqlite.JDBC;
|/**
|* 这是个非常简单的SQLite的Java程序，
|* 程序中创建数据库、创建表、然后插入数据，
|* 最后读出数据显示出来
|*/
|publicclass TestSQLite
|{
|publicstaticvoid main(String[] args)
|{
|try
|{
|//连接SQLite的JDBC
|Class.forName("org.sqlite.JDBC");
|//建立一个数据库名zieckey.db的连接，如果不存在就在当前目录下创建之
|Connection conn =DriverManager.getConnection("jdbc:sqlite:zieckey.db");
|Statement stat = conn.createStatement();
|stat.executeUpdate("create table tbl1(name varchar(20), salary int);");//创建一个表，两列
|stat.executeUpdate("insert into tbl1 values('ZhangSan',8000);");//插入数据
|stat.executeUpdate("insert into tbl1 values('LiSi',7800);");
|stat.executeUpdate("insert into tbl1 values('WangWu',5800);");
|stat.executeUpdate("insert into tbl1 values('ZhaoLiu',9100);");
|ResultSet rs = stat.executeQuery("select * from tbl1;");//查询数据
|while(rs.next()){//将查询到的数据打印出来
|System.out.print("name = "+ rs.getString("name")+" ");//列属性一
|System.out.println("salary = "+ rs.getString("salary"));//列属性二
|}
|rs.close();
|conn.close();//结束数据库的连接
|}
|catch(Exception e )
|{
|e.printStackTrace();
|}
|}
|}|
编译运行： 　　E:\Coding\java\test>javac TestSQLite.java　　E:\Coding\java\test>java TestSQLite　　name = ZhangSan salary = 8000　　name = LiSi salary = 7800　　name = WangWu salary = 5800　　name = ZhaoLiu salary = 9100　　总结：本文介绍了嵌入式数据库SQLite在Java中的应用，　　通过创建表、插入数据、查询等操作介绍了在Java中对数据库的操纵。
### 3.使用.NET操作SQLLITE
先下载ADO**.NET**2.0 Provider for SQLite。下载binaries zip版就可以了。下载完后解压缩，可以在bin目录下找到System.Data.SQLite.DLL。在vs2008中用Add Refrence功能把System.Data.SQLite.DLL加到工程里就可以了。运行下面代码试试：　　string datasource = "e:/tmp/test.db";　　System.Data.SQLite.SQLiteConnection.CreateFile(datasource);　　//连接数据库　　System.Data.SQLite.SQLiteConnection
 conn = new System.Data.SQLite.SQLiteConnection();　　System.Data.SQLite.SQLiteConnectionStringBuilder connstr = new System.Data.SQLite.SQLiteConnectionStringBuilder();　　connstr.DataSource = datasource;　　connstr.Password = "admin";//设置密码，SQLite ADO**.NET**实现了数据库密码保护　　conn.ConnectionString
 = connstr.ToString();　　conn.Open();　　//创建表　　System.Data.SQLite.SQLiteCommand cmd = new System.Data.SQLite.SQLiteCommand();　　string sql = "CREATE TABLE test(username varchar(20),password varchar(20))";　　cmd.CommandText = sql;　　cmd.Connection = conn;　　cmd.ExecuteNonQuery();　　//插入数据　　sql
 = "INSERT INTO test VALUES('a','b')";　　cmd.CommandText = sql;　　cmd.ExecuteNonQuery();　　//取出数据　　sql = "SELECT * FROM test";　　cmd.CommandText = sql;　　System.Data.SQLite.SQLiteDataReader reader = cmd.ExecuteReader();　　StringBuilder sb = new StringBuilder();　　while
 (reader.Read())　　{　　sb.Append("username:").Append(reader.GetString(0)).Append("\n")　　.Append("password:").Append(reader.GetString(1));　　}　　MessageBox.Show(sb.ToString());
### 4.使用linux下的C操作SQLLITE
由于linux下侧重使用命令，没有win的操作容易上手，所以在测试C操作SQLITE时会比较容易出现错误，给大家做一个简单的程序进行测试，演示怎么应用。　　\#include <stdio.h>　　\#include <sqlite3.h>　　int main( void )　　{　　sqlite3 *db=NULL;　　char *zErrMsg = 0;　　int rc;　　//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件　　rc = sqlite3_open("zieckey.db", &db);
 　　if( rc )　　{　　fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));　　sqlite3_close(db);　　exit(1);　　}　　else printf("You have opened a sqlite3 database named zieckey.db successfully!\nCongratulations! Have fun ! ^-^ \n");　　sqlite3_close(db); //关闭数据库　　return
 0;　　}　　退出，保存。(代码输入完成后，按下 Esc 键，然后输入： :wq ,回车就好拉)　　好拉，现在编译：[root@localhost temp]\# gcc opendbsqlite.c -o db.out　　或者遇到这样的问题：　　[root@localhost temp]\# gcc opendbsqlite.c -o db.out　　opendbsqlite.c:11:21: sqlite3.h: 没有那个文件或目录　　opendbsqlite.c: In function `main':　　opendbsqlite.c:19:
 `sqlite3' undeclared (first use in this function)　　opendbsqlite.c:19: (Each undeclared identifier is reported only once　　opendbsqlite.c:19: for each function it appears in.)　　opendbsqlite.c:19: `db' undeclared (first use in this function)　　这是由于没有找到头文件的原因。　　也许会碰到类似这样的问题：　　[root@localhost
 temp]\# gcc opendbsqlite.c -o db.out　　/tmp/ccTkItnN.o(.text+0x2b): In function `main':　　: undefined reference to `sqlite3_open'　　/tmp/ccTkItnN.o(.text+0x45): In function `main':　　: undefined reference to `sqlite3_errmsg'　　/tmp/ccTkItnN.o(.text+0x67): In function
 `main':　　: undefined reference to `sqlite3_close'　　/tmp/ccTkItnN.o(.text+0x8f): In function `main':　　: undefined reference to `sqlite3_close'　　collect2: ld returned 1 exit status　　这是个没有找到库文件的问题。　　[root@localhost temp]\# gcc opendbsqlite.c -o db.out -lsqlite3
 -L/usr/local/sqlite3/lib -I/usr/local/sqlite3/include　　这样编译应该就可以了　　如果还是有错误的话请搜索：（一）Sqlite数据库连接。有更详细的说明解释
## SQLite的管理
[管理工具](http://baike.baidu.com/view/1251417.htm)也有不少，建议您使用sqlitemanager，用法酷像phpmyadmin.

