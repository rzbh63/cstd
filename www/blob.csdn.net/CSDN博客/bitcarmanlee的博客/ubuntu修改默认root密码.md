
# ubuntu修改默认root密码 - bitcarmanlee的博客 - CSDN博客


2017年11月24日 10:43:51[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：6647


想在自己的ubuntu里做su root的操作，猛然发现自己不知道root密码，然后也意识到，好像自己也一直没有给ubuntu设置过root密码。特意搜了一把，发现ubuntu的默认root密码是随机的，每次开机都会有一个新的root密码。为了设置新的root密码，可以做如下操作：
1.输入`sudo passwd`命令，然后会提示输入当前用户的密码。
2.按enter键，终端会提示输入新的密码并确认，此时的密码就是新的root密码。
3.修改完毕以后，在执行su root命令，此时输入新的root密码即可。

