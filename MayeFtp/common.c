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
		break;
	case C_CWD:
		break;
	case C_DELE:
		break;
	case C_LIST:
		break;
	case C_MDTM:
		break;
	case C_MKD:
		break;
	case C_NLST:
		break;
	case C_PASS:
		ftp_pass(info);
		break;
	case C_PASV:
		break;
	case C_PORT:
		break;
	case C_XPWD:
		ftp_pwd(info);
		break;
	case C_QUIT:
		break;
	case C_RETR:
		break;
	case C_RMD:
		break;
	case C_RNFR:
		break;
	case C_RNTO:
		break;
	case C_SITE:
		break;
	case C_SIZE:
		break;
	case C_STOR:
		break;
	case C_TYPE:
		break;
	case C_USER:
		ftp_user(info);
		printf("C_USER ");
		break;
	case C_NOOP:
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

