#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include "loginProto.h"

#define BUFFERSIZE 256

// init socket
static int initSocket(int *s)
{
	struct sockaddr_in localAddr;

	*s = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == *s)
	{
		perror("socket()");
		return -1;
	}

	// socket bind -->man 7 ip man 2 bind
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(SERVERPORT);
	inet_aton(SERVERIP, &localAddr.sin_addr);
	if (bind(*s, (void *)&localAddr, sizeof(localAddr)) == -1)
	{
		perror("bind()");
		close(*s);
		return -1;
	}

	return 0;
}

static int isLoginOk(struct login_st *st);

int main(void)
{
	// recv client data---> register?
	int sd;
	struct sockaddr_in remoteAddr;
	socklen_t remoteAddrLen;
	struct login_st rcvData;
	int cnt;
	pid_t pid;
	sqlite3 *db;
	char *sql;

	initSocket(&sd);
	// open db
	if (sqlite3_open("./netServer.db", &db) != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_open() failed\n");
		goto ERROR;
	}
	sql = "create table if not exists rgsTable(count text primary key, password text not null)";
	sqlite3_exec(db, sql, NULL, NULL, NULL);
	sqlite3_close(db);

	remoteAddrLen = sizeof(remoteAddr);
	while (1)
	{
		cnt = recvfrom(sd, &rcvData, sizeof(rcvData), 0,
					   (void *)&remoteAddr, &remoteAddrLen);
		if (cnt == -1)
		{
			perror("recvfrom()");
			goto ERROR;
		}
		// debug
		printf("client ip:%s, port:%d\n",
			   inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));
		printf("login cnt:%s, pwd:%s\n", rcvData.cnt, rcvData.pwd);
		pid = fork();
		if (pid == -1)
		{
			perror("fork()");
			goto ERROR;
		}
		if (pid == 0)
		{
			isLoginOk(&rcvData);
			sendto(sd, &rcvData, sizeof(rcvData), 0,
				   (void *)&remoteAddr, remoteAddrLen);
			exit(0);
		}
	}
	exit(0);
ERROR:
	close(sd);
	exit(1);
}

static int isLoginOk(struct login_st *rcv)
{
	sqlite3 *db;
	char *sql;
	sqlite3_stmt *st;
	int ret;
	char buf[BUFFERSIZE] = {};

	sqlite3_open("./netServer.db", &db);

	sql = "select * from rgsTable where count=?";
	sqlite3_prepare_v2(db, sql, -1, &st, NULL);
	//var--->bind
	sqlite3_bind_text(st, 1, rcv->cnt, -1, NULL);
	ret = sqlite3_step(st);
	if (ret == SQLITE_ROW)
	{
		char *pwd = (char *)sqlite3_column_text(st, 1);
		puts(pwd);
		if (strcmp(pwd, rcv->pwd) == 0)
			rcv->login_state = LOGIN_OK;
		else
			rcv->login_state = LOGIN_PWD_ERROR;
	}
	else if (ret == SQLITE_DONE)
		rcv->login_state = LOGIN_CNT_ERROR;

	sqlite3_finalize(st);
	sqlite3_close(db);

	//st->rgsState=RGS_EXISTS;
	return 0;
}
