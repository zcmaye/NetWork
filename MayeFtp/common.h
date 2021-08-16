#pragma once
typedef  _int64 socket_t;

/* 保存客户端的信息 */
typedef struct ClientInfo
{
	socket_t fd;
	const char* message;
	char cmd[10];
}ClientInfo;


/* 命令列表 */
typedef enum CmdList
{
	C_ERROR = -1,
	C_ABOR, 
	C_CWD, 
	C_DELE, 
	C_LIST,		//->dir
	C_MDTM, 
	C_MKD, 
	C_NLST,		//name list  ->ls
	C_PASS, 
	C_PASV,
	C_PORT, 
	C_XPWD, 
	C_QUIT, 
	C_RETR, 
	C_RMD, 
	C_RNFR, 
	C_RNTO, 
	C_SITE, 
	C_SIZE,
	C_STOR, 
	C_TYPE, 
	C_USER, 
	C_NOOP, 
	C_OPTS
}CmdList;

/* 命令列表的字符串映射 */
static const char* cmdlist_str[] =
{
  "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
  "PORT", "XPWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
  "STOR", "TYPE", "USER", "NOOP","OPTS"
};

void send_state(ClientInfo* ,const char* msg);

//给客户端发送欢迎消息
void welcome(ClientInfo*);

//解析客户端命令
CmdList  parseCmd(const char* buf);

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
void ftp_retr(ClientInfo*);
void ftp_stor(ClientInfo*);
void ftp_dele(ClientInfo*);
void ftp_size(ClientInfo*);
void ftp_quit(ClientInfo*);
void ftp_type(ClientInfo*);
void ftp_abor(ClientInfo*);

void ftp_mdtm(ClientInfo*);	//文件最后修改时间
void ftp_nlst(ClientInfo*);
void ftp_port(ClientInfo*);
void ftp_rnfr(ClientInfo*);
void ftp_rnto(ClientInfo*);
void ftp_noop(ClientInfo*);
void ftp_site(ClientInfo*);