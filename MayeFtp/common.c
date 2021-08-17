#include "common.h"
#include<stdio.h>
#include<tcpSocket.h>

ClientInfo* createClientInfo()
{
	ClientInfo* info = calloc(1, sizeof(ClientInfo));
	if (info == NULL)
		return NULL;
	info->dir = createDir("./files");
	return info;
}

void send_state(ClientInfo* info)
{
	if (SOCKET_ERROR == send(info->fd, info->message, strlen(info->message), 0))
	{
		err("send");
		return;
	}
}

void welcome(ClientInfo* info)
{
	info->message = "220 welcome maye ftp server.\r\n";
	send_state(info);
}

CmdList parseCmd(ClientInfo* info,const char* buf)
{
	sscanf_s(buf, "%s %s", info->cmd,sizeof(info->cmd),info->arg,sizeof(info->arg));
	for (size_t i = 0; i < sizeof(cmdlist_str)/sizeof(char*); i++)
	{
		if (_stricmp(info->cmd, cmdlist_str[i]) == 0)
		{
			return i;
		}
	}
	return C_ERROR;
}

void exeCmd(ClientInfo* info,CmdList cmd)
{
	switch (cmd)
	{
	case C_ABOR:
		ftp_abor(info);
		break;
	case C_CWD:
		ftp_cwd(info);
		break;
	case C_DELE:
		ftp_dele(info);
		break;
	case C_LIST:
		ftp_list(info);
		break;
	case C_MDTM:
		ftp_mdtm(info);
		break;
	case C_MKD:
		ftp_mkd(info);
		break;
	case C_NLST:
		ftp_nlst(info);
		break;
	case C_PASS:
		ftp_pass(info);
		break;
	case C_PASV:
		ftp_pasv(info);
		break;
	case C_PORT:
		ftp_port(info);
		break;
	case C_XPWD:
		ftp_pwd(info);
		break;
	case C_QUIT:
		ftp_quit(info);
		break;
	case C_RETR:
		ftp_retr(info);
		break;
	case C_RMD:
		ftp_rmd(info);
		break;
	case C_RNFR:
		ftp_rnfr(info);
		break;
	case C_RNTO:
		ftp_rnto(info);
		break;
	case C_SITE:
		ftp_site(info);	//1
		break;
	case C_SIZE:
		ftp_size(info);
		break;
	case C_STOR:
		ftp_stor(info);
		break;
	case C_TYPE:
		ftp_type(info);
		break;
	case C_USER:
		ftp_user(info);
		break;
	case C_NOOP:
		ftp_noop(info);
		break;
	case C_OPTS:
		ftp_opts(info);
		break;
	case C_ERROR:
	default:
		printf("无法解析的命令:%d\n", cmd);
		info->message = "500 Syntax error, command unrecognized. This may include errors such as command line too long.";
		send_state(info);
		break;
	}
}

void ftp_opts(ClientInfo* info)
{
	info->message = "200 OPTS command successful - uft8 encodeing now on.\r\n";
	send_state(info);
}

void ftp_user(ClientInfo* info)
{
	info->message = "331 User name okay, need password\r\n";
	//info->message = "530 Invalid username\r\n";	//error
	send_state(info);
}

void ftp_pass(ClientInfo* info)
{
	info->message = "230 Login successful\r\n";
	//info->message = "500 Invalid username or password\r\n";	//error
	send_state(info);
}

void ftp_pwd(ClientInfo* info)
{
	info->message = "257 maye/aaa\r\n";
	//info->message = "550 Failed to get pwd.\r\n";	//error
	send_state(info);
}

void ftp_cwd(ClientInfo* info)
{
	info->message = "250 Directory successfully changed.\r\n";
	//info->message = "550 Failed to change directory.\r\n";
	//info->message = "500 Login with USER and PASS.\r\n";	//未登录
	send_state(info);
}

void ftp_mkd(ClientInfo* info)
{


}

void ftp_rmd(ClientInfo* info)
{

}

void ftp_pasv(ClientInfo* info)
{

}

void ftp_list(ClientInfo* info)
{

}

void ftp_retr(ClientInfo* info)
{
	printf("fileName:%s\n", info->arg);

}

void ftp_stor(ClientInfo* info)
{

}

void ftp_dele(ClientInfo* info)
{

}

void ftp_size(ClientInfo* info)
{

}

void ftp_quit(ClientInfo* info)
{
	info->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
	send_state(info);
	closesocket(info->fd);
}

void ftp_type(ClientInfo* info)
{

}

void ftp_abor(ClientInfo* info)
{

}



//文件最后修改时间
void ftp_mdtm(ClientInfo* info)
{

}
static socket_t connectToClient(ClientInfo* info)
{
	char* str = info->arg;
	//把所有的,替换成.		//"127.0.0.1.56.89"
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == ',')
			str[i] = '.';
	}
	//把最后两个.替换成空格	//"127.0.0.1 56 89"
	for (size_t i = 0; i < 2; i++)
	{
		char* rfirstcomma = strrchr(str, '.');	//倒数第一个逗号
		*rfirstcomma = ' ';
	}
	char ip[16] = { 0 };
	UINT32 p1 = 0, p2 = 0;
	sscanf_s(str, "%s %u %u", ip, 16, &p1, &p2);

	//开始连接
	info->datafd = create_clientSocket(ip, p1 * 256 + p2);
	return info->datafd;
}

void ftp_nlst(ClientInfo* info)
{
	info->message = "125 Data connection already open; Transfer starting.\r\n";
	send_state(info);
	//建立连接
	connectToClient(info);

	//发送ls信息
	send(info->datafd, u8"01 学习C语言准备.pptx\r\n", 29, 0);

	//传完就关闭数据连接
	closesocket(info->datafd);

	//发送传输完成状态
	info->message = "226 Transfer complete.\r\n";
	send_state(info);
}

void ftp_port(ClientInfo* info)
{
	info->message = "200 PORT command successful.\r\n";
	send_state(info);
}

void ftp_rnfr(ClientInfo* info)
{

}

void ftp_rnto(ClientInfo* info)
{

}

void ftp_noop(ClientInfo* info)
{

}

void ftp_site(ClientInfo* info)
{

}

