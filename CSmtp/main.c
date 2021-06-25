#include"CSmtp_c.h"

int main()
{
	initSmtp("842831611@qq.com", "sthhxznovshqbbeg", "825943234@qq.com");

	setPortDomain(25, "smtp.qq.com");
	setMailContent("hello", "你好我是邮件正文");
	AddAttachment("./五行宫殿.txt");
	sendEmal();
		
	return 0;
}