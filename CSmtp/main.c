#include"CSmtp_c.h"
/*Version:V1  函数指针封装版*/
int main()
{
	CSmtp* smtp =  initSmtp("xx@qq.com", "xx", "xx@qq.com");
	
	smtp->setPortDomain(25, "smtp.qq.com");
	smtp->setMailContent("hello", "hello,本邮件来自Maye编写的C语言邮件发送系统，欢迎关注微信公众号<C语言Plus>,更多资料等着你哦~\n\
		本程序代码开源在GitHub:https://github.com/zcmaye/NetWork 欢迎Star\n\
		                                                        ----最后，感谢大家的支持~\
		扫描下方二维码，快速关注微信公众号QAQ~");

	//添加附件
	smtp->AddAttachment("./res/[C语言Plus]微信公众号.jpg");
	smtp->AddAttachment("C:\\Users\\Maye\\Desktop\\README.md");
	//移除上面的README.md
	smtp->DeleteAttachment("README.md");	

	//轰炸,发送100次邮件
	for (size_t i = 0; i < 100; i++)
	{
		//发送邮件
		smtp->sendEmal();
		printf("发送完成，第%d次\n", i);
		Sleep(5000);
	}



		
	return 0;
}