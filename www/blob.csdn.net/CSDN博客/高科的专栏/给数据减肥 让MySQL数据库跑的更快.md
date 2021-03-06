
# 给数据减肥 让MySQL数据库跑的更快 - 高科的专栏 - CSDN博客

2012年10月18日 14:34:37[高科](https://me.csdn.net/pbymw8iwm)阅读数：5702


在数据库优化工作中，使数据尽可能的小，使表在硬盘上占据的空间尽可能的小，这是最常用、也是最有效的手段之一。因为缩小数据，相对来说可以提高硬盘的读写速度，并且在查询过程中小表的内容处理时所占用的系统资源比较少。同理，如果在比较小的列上设置索引的话，其索引所占用的资源也会比较少。那么数据库管理员该如何给自己的数据减肥呢?对此笔者有如下几个建议。
建议一：空值并不一定不占用空间
在这里笔者先给大家扫盲一下。有些数据库管理员，认为空值不会占用系统资源，其实这是一个错误的认识。他们在数据库设计时，不喜欢将字段的属性设置为NOT NULL。而让用户根据自己的需要来输入数据。笔者认为，这种做法对于数据库的性能是不利的。
笔者的意见是，如果有可能的话，尽量将列设置为NOT NULL，即不允许有空值。这么做的话，可以加快后续处理的速度，同时从数据存储来看还可以使得每列节省一位，从而达到数据减肥的目的。在实际工作中，如果有些情况不需要用户输入数据时，还可以通过默认字段来达到非空的目的。如在薪资系统中，可以将用户的工作年限默认设置为0，而不是空白。当然，如果确实需要NULL的话，也没有办法。但是作为数据库工程师来说，要尽量避免使用NULL值。
建议二：使用尽量小的数据类型
数据类型的大小也会影响到基础表的大小。如对于MEDIUMINT和INT两个数据类型，其都可以用来保存整数型的数据，只是其能够保存的精度不同而已。但是从存储数据的角度来看，前者所需要的存储空间要比后者节省25%左右。为此在能够使用MEDIUMINT的情况下，就不要使用INT。
另外在定义数据长度的时候，在满足需求的情况下，也要尽量的短。如现在薪资考核系统中有员工编码一个字段。如果企业员工编码已经确定，有五位字符构成。那么在定义字段时，只需要定义5个字符的长度。这不仅可以缩小存储空间，而且还可以起到一定的数据校对功能。当用户输入的编码长度超过5位时，数据将无法保存。
虽然说保存某个数据可以有很多数据类型可以选择，也可以定义比较大的字符位数。但是选择尽量小的数据类型，可以帮助降低数据存储空间，达到数据减肥的目的。从而进一步提升数据库的性能。
建议三：索引与数据表大小的关系
笔者在文章一开头就谈到过，如果对于比较小的列设置索引，那么索引也将占用比较少的资源。可见，索引与数据表大小也有紧密的联系。在合适的地方、合适的时机设置合适的索引，也可以实现对数据减肥的目的。
如通常情况下，每张数据表可能会有多个索引，但是主索引往往只有一个。为此对于每张表的主索引应该考虑尽量的短小精悍。这可以帮助数据库更快的进行识别。
再如尽量对前缀进行索引。如现在有一张表，需要对某个列设置索引。而这个列有一个特点，即在头几个字符上有唯一的前缀。如果存在这种情况的话，那么紧紧索引这个前缀，而不是全部，效果会更好。在MySQL数据库中，支持对一个字符列的最左边部分创建一个索引。这也就是说，数据库会将某个字段根据一定的规则拆分为前后两个部分。拆分后前面一部分的数据如果能够保持唯一，那么就只需要对前面一部分设置索引即可，而不需要对整个字段的数据设置索引。这无疑可以缩小索引所占用的资源，实现减肥的目的。更短的索引，能够提供更快的查询速度。因为它们所占用的硬盘空间更少，而且他们将在索引缓存中保存更多的访问。从而降低硬盘的搜索次数，提高查询的效率。
最后需要注意的就是，索引不能够滥用。使用索引确实可以提高数据的处理能力，但是索引同时也会带来额外的开销。只有这个收益大于开销时，使用索引才能够提升数据库的性能。否则的话，则会起到相反的效果。如某个表需要进行快速的存储，如果在这个表上设置过多的索引，索引就会起到副作用。对此笔者建议，如果主要通过搜索列的组合来存取一个表，那么最好对他们只设置一个索引。当然，这个索引部分应该是日常工作中最常用的列。在不得已的情况下，如果需要使用多个索引的话，那么最好能够以更多的副本使用列来获得更好的索引压缩。从而降低因为使用了多个索引而增加的资源消耗。
建议四：在需要“丰满”的地方还是不能够节省
一个女人，该瘦的地方要瘦，该丰满的地方要丰满。其实数据库也是如此。能够节省硬盘空间的地方，就要节省。而不能够节省的地方，则不能够为了减肥而将其精简下来。有时候这会起到适得其反的效果。
笔者以Varchar为例。如在MyISAM标中，如果没有任何可变长的列，那么最好使用固定大小的数据类型。虽然采用固定长度的数据类型，往往会浪费一定的存储空间。因为如果用户输入的数据不足，采用固定长度的话，数据存储时仍然会按这个固定的长度来存储。但是在这种情况下，能够用固定长度的，还是要使用固定长度。因为这种情况下虽然会浪费一定的硬盘空间，但是却可以提高数据的查询速度。
可见，并不是在任何情况下对数据减肥都可以提高数据库的性能。这就好像节支开源，这个节省要节省在刀刃上。否则的话，不但不能够节支，而且还会搬起石头砸自己的脚。通俗的说，就是该瘦的地方要瘦，该丰满的地方要丰满。记住这句话，就对了。
建议五：将表分割以实现减肥的目的
蚂蚁在搬食物时，如果某块食物过大，无法搬动的话，蚂蚁则可能会将这个块食物进行分割，直到其搬得动为止。这就是分蛋糕原理。其实这种现象在日常工作中经常常见。如我们有一张数据库表格，如果里面的纪录非常多，那么表格的允许速度会非常的慢。在这种情况下，可以根据一定的规则将表分为多个工作簿。如现在有一份企业员工的考勤信息。对这个表进行查询、排序、统计时，等待时间非常的长。此时就可以根据部门将其分割成不同的工作簿，然后再对其进行相关的数据分析。此时虽然工作量会大一点，但是其处理的速度会变快许多。
根据这个原理，在数据库优化时，可以将一个经常被扫描的大表分割为2个或者2个以上的表示非常有益的。如在日常工作中，笔者现在有一个动态格式的数据表，并且这个数据是使用一个扫描表时，就会用这个来找出相关行的比较小的静态格式的表。
通过这个表的拆分，可以将一块大蛋糕分为几块小的蛋糕，以利于后续数据的统计与分析。当然这个效果的好坏，直接跟这个拆分的规则有关。关于表如何拆分才能够达到理想的效果，这又是一个比较大的话题。由于这里篇幅有限，笔者不做过多的说明。或许在后续的文章中，笔者会以这命题进行展开，给大家做详细的说明。

