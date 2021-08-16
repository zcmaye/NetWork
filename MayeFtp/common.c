#include "common.h"
#include<stdio.h>
#include<tcpSocket.h>
void send_state(ClientInfo* info, const char* msg)
{
	if (msg == NULL)
		return;
	if (SOCKET_ERROR == send(info->fd, msg, strlen(msg), 0))
	{
		err("send");
		return;
	}
	
	//send(info->fd, "\r\n", 1, 0);
	//send(info->fd, "\r\n", 1, 0);
}

void welcome(ClientInfo* info)
{
	char* wel = "220 welcome maye ftp server.\r\n";
	send_state(info, wel);
}

CmdList parseCmd(const char* buf)
{
	char cmd[10] = { 0 };
	sscanf_s(buf, "%s", cmd,10);
	for (size_t i = 0; i < sizeof(cmdlist_str)/sizeof(char*); i++)
	{
		if (_stricmp(cmd, cmdlist_str[i]) == 0)
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
		ftp_quit(info);
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
		info->message = NULL;
		break;
	}
	send_state(info, info->message);
}

void ftp_opts(ClientInfo* info)
{
	info->message = "200 OPTS command successful - uft8 encodeing now on.\r\n";
}

void ftp_user(ClientInfo* info)
{
	info->message = "331 User name okay, need password\r\n";
	//info->message = "530 Invalid username\r\n";	//error
}
void ftp_pass(ClientInfo* info)
{
	info->message = "230 Login successful\r\n";
	//info->message = "500 Invalid username or password\r\n";	//error
}

void ftp_pwd(ClientInfo* info)
{
	info->message = "257 maye/aaa\r\n";
	//info->message = "550 Failed to get pwd.\r\n";	//error
}

void ftp_cwd(ClientInfo* info)
{
	info->message = "250 Directory successfully changed.\r\n";
	//info->message = "550 Failed to change directory.\r\n";
	//info->message = "500 Login with USER and PASS.\r\n";	//未登录
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

void ftp_nlst(ClientInfo* info)
{

}

void ftp_port(ClientInfo* info)
{

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

