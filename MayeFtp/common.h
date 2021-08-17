#pragma once
#include"dir.h"
typedef  _int64 socket_t;

/* 保存客户端的信息 */
typedef struct ClientInfo
{
	socket_t fd;			//命令连接
	socket_t datafd;		//数据连接
	
	const char* message;	//状态码和描述信息
	
	char cmd[5];			//客户端发过来的命令
	char arg[128];			//命令后面的参数

	Dir *dir;				//操作目录
}ClientInfo;

/* 命令列表 */
typedef enum CmdList
{
	C_ERROR = -1,
	C_ABOR,		//cmd ftp 显示无效命令
	C_CWD,		//cmd ftp 显示无效命令
	C_DELE, 
	C_LIST,		//->dir 显示远程目录中文件和子目录的目录列表。
	C_MDTM,		//cmd ftp 显示无效命令
	C_MKD, 
	C_NLST,		//name list  ->ls
	C_PASS,		//cmd ftp 显示无效命令
	C_PASV,		//LITERAL PASV	命令
	C_PORT,		//ls 等命令时，会自动发送
	C_XPWD,		//pwd命令
	C_QUIT,		//quit
	C_RETR,		//get
	C_RMD,		//rmd
	C_RNFR,		//
	C_RNTO,		//
	C_SITE,		//
	C_SIZE,		//
	C_STOR,		//put
	C_TYPE,		//type 查看传输模式
	C_USER,		//user 切换用户
	C_NOOP,		//
	C_OPTS		//连接ftp服务器会发送
}CmdList;

/* 命令列表的字符串映射 */
static const char* cmdlist_str[] =
{
  "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
  "PORT", "XPWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
  "STOR", "TYPE", "USER", "NOOP","OPTS"
};

ClientInfo* createClientInfo();

void send_state(ClientInfo*);

//给客户端发送欢迎消息
void welcome(ClientInfo*);

//解析客户端命令
CmdList  parseCmd(ClientInfo* info,const char* buf);

//执行命令
void exeCmd(ClientInfo*,CmdList cmd);



//处理命令
void ftp_opts(ClientInfo* info);
void ftp_user(ClientInfo*);
void ftp_pass(ClientInfo*);
void ftp_pwd(ClientInfo*);
void ftp_cwd(ClientInfo*);		//改变工作目录
void ftp_mkd(ClientInfo*);
void ftp_rmd(ClientInfo*);
void ftp_pasv(ClientInfo*);
void ftp_list(ClientInfo*);
void ftp_retr(ClientInfo*);		//下载文件
void ftp_stor(ClientInfo*);
void ftp_dele(ClientInfo*);
void ftp_size(ClientInfo*);
void ftp_quit(ClientInfo*);		//退出
void ftp_type(ClientInfo*);
void ftp_abor(ClientInfo*);

void ftp_mdtm(ClientInfo*);	//文件最后修改时间
void ftp_nlst(ClientInfo*);
void ftp_port(ClientInfo*);
void ftp_rnfr(ClientInfo*);
void ftp_rnto(ClientInfo*);
void ftp_noop(ClientInfo*);
void ftp_site(ClientInfo*);