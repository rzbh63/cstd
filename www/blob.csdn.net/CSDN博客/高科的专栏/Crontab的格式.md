
# Crontab的格式 - 高科的专栏 - CSDN博客

2015年01月14日 19:36:52[高科](https://me.csdn.net/pbymw8iwm)阅读数：4746



第1列分钟1～59
第2列小时1～23（0表示子夜）
第3列日1～31
第4列月1～12
第5列星期0～6（0表示星期天）
第6列要运行的命令
下面是crontab的格式：
分 时 日 月 星期 要运行的命令
这里有crontab文件条目的一些例子：
30 21 * * * /usr/local/apache/bin/apachectl restart
上面的例子表示每晚的21:30重启apache。
45 4 1,10,22 * * /usr/local/apache/bin/apachectl restart
上面的例子表示每月1、10、22日的4 : 45重启apache。
10 1 * * 6,0 /usr/local/apache/bin/apachectl restart
上面的例子表示每周六、周日的1 : 10重启apache。
0,30 18-23 * * * /usr/local/apache/bin/apachectl restart
上面的例子表示在每天18 : 00至23 : 00之间每隔30分钟重启apache。
0 23 * * 6 /usr/local/apache/bin/apachectl restart
上面的例子表示每星期六的11 : 00 pm重启apache。
* */1 * * * /usr/local/apache/bin/apachectl restart
每一小时重启apache
* 23-7/1 * * * /usr/local/apache/bin/apachectl restart
晚上11点到早上7点之间，每隔一小时重启apache
0 11 4 * mon-wed /usr/local/apache/bin/apachectl restart
每月的4号与每周一到周三的11点重启apache
0 4 1 jan * /usr/local/apache/bin/apachectl restart
一月一号的4点重启apache


