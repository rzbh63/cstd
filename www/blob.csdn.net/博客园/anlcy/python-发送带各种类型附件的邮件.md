
# python - 发送带各种类型附件的邮件 - anlcy - 博客园






# [python - 发送带各种类型附件的邮件](https://www.cnblogs.com/camilla/p/7279374.html)
如何发送各种类型的附件。
基本思路就是，使用MIMEMultipart来标示这个邮件是多个部分组成的，然后attach各个部分。如果是附件，则add_header加入附件的声明。
在python中，MIME的这些对象的继承关系如下。
MIMEBase
|-- MIMENonMultipart
|-- MIMEApplication
|-- MIMEAudio
|-- MIMEImage
|-- MIMEMessage
|-- MIMEText
|-- MIMEMultipart
一般来说，不会用到MIMEBase，而是直接使用它的继承类。MIMEMultipart有attach方法，而MIMENonMultipart没有，只能被attach。
MIME有很多种类型，这个略麻烦，如果附件是图片格式，我要用MIMEImage，如果是音频，要用MIMEAudio，如果是word、excel，我都不知道该用哪种MIME类型了，得上google去查。
**最懒的方法就是**，不管什么类型的附件，都用MIMEApplication，MIMEApplication默认子类型是application/octet-stream。
application/octet-stream表明“这是个二进制的文件，希望你们那边知道怎么处理”，然后客户端，比如qq邮箱，收到这个声明后，会根据文件扩展名来猜测。
下面上代码。
假设当前目录下有foo.xlsx/foo.jpg/foo.pdf/foo.mp3这4个文件。
如何发送各种类型的附件。
基本思路就是，使用MIMEMultipart来标示这个邮件是多个部分组成的，然后attach各个部分。如果是附件，则add_header加入附件的声明。
在python中，MIME的这些对象的继承关系如下。
MIMEBase
|-- MIMENonMultipart
|-- MIMEApplication
|-- MIMEAudio
|-- MIMEImage
|-- MIMEMessage
|-- MIMEText
|-- MIMEMultipart
一般来说，不会用到MIMEBase，而是直接使用它的继承类。MIMEMultipart有attach方法，而MIMENonMultipart没有，只能被attach。
MIME有很多种类型，这个略麻烦，如果附件是图片格式，我要用MIMEImage，如果是音频，要用MIMEAudio，如果是word、excel，我都不知道该用哪种MIME类型了，得上google去查。
**最懒的方法就是**，不管什么类型的附件，都用MIMEApplication，MIMEApplication默认子类型是application/octet-stream。
application/octet-stream表明“这是个二进制的文件，希望你们那边知道怎么处理”，然后客户端，比如qq邮箱，收到这个声明后，会根据文件扩展名来猜测。
下面上代码。
假设当前目录下有foo.xlsx/foo.jpg/foo.pdf/foo.mp3这4个文件。
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.application import MIMEApplication
_user = "sigeken@qq.com"
_pwd  = "***"
_to   = "402363522@qq.com"
\#如名字所示Multipart就是分多个部分
msg = MIMEMultipart()
msg["Subject"] = "don't panic"
msg["From"]    = _user
msg["To"]      = _to
\#---这是文字部分---
part = MIMEText("乔装打扮，不择手段")
msg.attach(part)
\#---这是附件部分---
\#xlsx类型附件
part = MIMEApplication(open('foo.xlsx','rb').read())
part.add_header('Content-Disposition', 'attachment', filename="foo.xlsx")
msg.attach(part)
\#jpg类型附件
part = MIMEApplication(open('foo.jpg','rb').read())
part.add_header('Content-Disposition', 'attachment', filename="foo.jpg")
msg.attach(part)
\#pdf类型附件
part = MIMEApplication(open('foo.pdf','rb').read())
part.add_header('Content-Disposition', 'attachment', filename="foo.pdf")
msg.attach(part)
\#mp3类型附件
part = MIMEApplication(open('foo.mp3','rb').read())
part.add_header('Content-Disposition', 'attachment', filename="foo.mp3")
msg.attach(part)
s = smtplib.SMTP("smtp.qq.com", timeout=30)\#连接smtp邮件服务器,端口默认是25
s.login(_user, _pwd)\#登陆服务器
s.sendmail(_user, _to, msg.as_string())\#发送邮件
s.close()





