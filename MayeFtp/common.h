#pragma once
typedef  _int64 socket_t;

/* ����ͻ��˵���Ϣ */
typedef struct ClientInfo
{
	socket_t fd;
	const char* message;
	char cmd[10];
}ClientInfo;


/* �����б� */
typedef enum CmdList
{
	C_ERROR = -1,
	C_ABOR, C_CWD, C_DELE, C_LIST, C_MDTM, C_MKD, C_NLST, C_PASS, C_PASV,
	C_PORT, C_XPWD, C_QUIT, C_RETR, C_RMD, C_RNFR, C_RNTO, C_SITE, C_SIZE,
	C_STOR, C_TYPE, C_USER, C_NOOP, C_OPTS
}CmdList;

/* �����б���ַ���ӳ�� */
static const char* cmdlist_str[] =
{
  "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
  "PORT", "XPWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
  "STOR", "TYPE", "USER", "NOOP","OPTS"
};

void send_state(ClientInfo* ,const char* msg);

//���ͻ��˷��ͻ�ӭ��Ϣ
void welcome(ClientInfo*);

//�����ͻ�������
CmdList  parseCmd(const char* buf);

//ִ������
void exeCmd(ClientInfo*,CmdList cmd);



//��������
void ftp_opts(ClientInfo* info);
void ftp_user(ClientInfo*);
void ftp_pass(ClientInfo*);
void ftp_pwd(ClientInfo*);
void ftp_cwd(ClientInfo*);		//�ı乤��Ŀ¼
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