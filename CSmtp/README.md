# CSmtp Email

CSmtp Email 是一个C语言编写的基于Windows的邮件发送系统，用于创建可在C/C++中调用电子邮件库。



### 入门

本程序使用VS2019开发，仓库自带Vs的项目文件，克隆之后点击.sln文件，把CSmtp设置为启动，并修改发送人邮箱和密码，以及收件人邮箱即可正常发送邮件**（支持附件发送）**

要使用电子邮件代码，请运行以下命令：

```css
git clone https://github.com/zcmaye/NetWork.git
cd NetWork
点击.sln    
```



如果不是使用Vs，那么将CSmtp中的四个文件拷贝到项目文件夹中[CSmtp_c.h CSmtp_c.c main.c res]，其中res是资源文件夹。

别急还有一步，把NetWork中的文件夹tcpSocket 拷贝到项目文件的上一级目录。

**for exmaple：**

项目文件为MySmtp，则项目目录视图为

+ root
  + tcpSocket
  + MySmtp
    + CSmtp_c.h
    + CSmtp_c.c
    + main.c
    + res



### 版本

+ **master** 为最初版本，最容易理解
+ **v1**         为C语言实现的面向对象调用方法的版本