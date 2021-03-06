
# 日志级别 debug info warn eirror fatal - 高科的专栏 - CSDN博客

2012年02月28日 18:05:51[高科](https://me.csdn.net/pbymw8iwm)阅读数：5379


### 日志级别 debug info warn eirror fatal

软件中总免不了要使用诸如 Log4net, Log4j, Tracer 等东东来写日志，不管用什么，这些东东大多是大同小异的，一般都提供了这样5个日志级别：
× Debug
× Info
× Warn
× Error
× Fatal
一个等级比一个高，但是在具体开发中，关于应该如何选择适应的等级，却没有找到好的文章进行说明。记录一下自己的一些看法，以便日后使用吧。
=== Debug ===
这个级别最低的东东，一般的来说，在系统实际运行过程中，一般都是不输出的。
因此这个级别的信息，可以随意的使用，任何觉得有利于在调试时更详细的了解系统运行状态的东东，比如变量的值等等，都输出来看看也无妨。
当然，在每一个 Debug 调用之前，一定要加上 If 判断。
=== Info ===
这个应该用来反馈系统的当前状态给最终用户的，所以，在这里输出的信息，应该对最终用户具有实际意义，也就是最终用户要能够看得明白是什么意思才行。
从某种角度上说，Info 输出的信息可以看作是软件产品的一部分（就像那些交互界面上的文字一样），所以需要谨慎对待，不可随便。
=== Warn、Error、Fatal ===
警告、错误、严重错误，这三者应该都在系统运行时检测到了一个不正常的状态，他们之间的区别，要区分还真不是那么简单的事情。我大致是这样区分的：
所谓警告，应该是这个时候进行一些修复性的工作，应该还可以把系统恢复到正常状态中来，系统应该可以继续运行下去。
所谓错误，就是说可以进行一些修复性的工作，但无法确定系统会正常的工作下去，系统在以后的某个阶段，很可能会因为当前的这个问题，导致一个无法修复的错误（例如宕机），但也可能一直工作到停止也不出现严重问题。
所谓Fatal，那就是相当严重的了，可以肯定这种错误已经无法修复，并且如果系统继续运行下去的话，可以肯定必然会越来越乱。这时候采取的最好的措施不是试图将系统状态恢复到正常，而是尽可能地保留系统有效数据并停止运行。
也就是说，选择 Warn、Error、Fatal 中的具体哪一个，是根据当前的这个问题对以后可能产生的影响而定的，如果对以后基本没什么影响，则警告之，如果肯定是以后要出严重问题的了，则Fatal之，拿不准会怎么样，则 Error 之。
=== 一些疑惑 ===
不过在实际使用中，基于上面的这种考虑，也还是有一些具体问题。最常见的就是要在最终产品中将输出日志打开到那种级别才算好呢？
例如在应用中有一个输出窗口，一些系统状态信息将被输出到这个输出窗口中。因为 Info 的级别是如此之低，所以为了让用户能够看到有效的输出信息，必须将日志级别开放到 Info 级别。但是 Warn 的级别比 Info 要高，所以用户不得不被迫看到一些 Warn 的信息。而我们其实已经假定，Warn 信息其实并不影响系统的正常运行，这一般只代表系统中存在一些还没有被发现或者修改的小 Bug。这些 Warn 信息会让最终用户困惑甚至恐慌，系统发出警告了，该怎么办？
个人观点，Info 的级别应该比 Warn 更高才对，Warn 信息和 Debug 一样，应该在产品测试和调试时使用，而 Info、Erro 以及 Fatal 则在产品发布后需要继续使用。
目前我所采用的解决方法是，对于 Warn、Error、Fatal 都添加一个相应的系统断言，这样，可以保证当发生这种问题时，在调试阶段，可以立即得到提示。在软件发布以后，这些信息也能被记录到日志文件中去。



