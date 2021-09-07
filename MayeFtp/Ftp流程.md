



+ ftpClient

```sh
ftp> open 127.0.0.1 8848
连接到127.0.0.1
220 Microsoft FTP Service
200 OPTS UTF8 command successful - UTF8 encoding now ON.
用户(127.0.0.1:(none)):maye
331 Anonymous access allowed, send identity (e-mail name) as password.
密码:
230 User logged in.
ftp>ls
200 PORT command successful.
125 Data connection already open; Transfer starting.
#425-Cannot open data connection.
01 学习C语言准备.pptx
02 大话CC++.pptx
226 Transfer complete.
```

+ ftpServer

```cpp
main>OPTS UTF8 ON
main>USER 1
main>PASS 1
main>PORT 127,0,0,1,243,16
main>NLST
```

[微软文档](https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/ftp-recv)

https://www.sohu.com/a/372157696_100175238

[**Ftp协议码**](https://blog.csdn.net/guoxilen/article/details/41149289)

[**FTP协议常用COMMAND和状态码**](https://www.iyunv.com/thread-71881-1-49.html)

[状态码](https://www.cnblogs.com/dongiosblogs/p/4548770.html)

[Win10开启FTP与配置（完整无错版）](https://blog.csdn.net/weijifen000/article/details/88904286)

