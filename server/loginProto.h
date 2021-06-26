// proto
#ifndef __PROTO_T
#define __PROTO_T

// register server

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	7788

#define CNTSIZE		32
#define PWDSIZE		128

enum {LOGIN_OK, LOGIN_CNT_ERROR, LOGIN_PWD_ERROR};

struct login_st {
	char cnt[CNTSIZE];
	char pwd[PWDSIZE];
	int8_t login_state;
}__attribute__((packed)); 

#endif

