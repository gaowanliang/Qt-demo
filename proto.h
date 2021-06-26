#ifndef PROTO_H
#define PROTO_H
#include <stdint.h>
#define SERVERIP	"127.0.0.1"
#define SERVERPORT	7788

#define CNTSIZE		32
#define PWDSIZE		128

enum {RGS_OK, RGS_EXISTS, PWD_NOTFORMAT};

struct rgs_st {
    char cnt[CNTSIZE];
    char pwd[PWDSIZE];
    int8_t rgsState;
}__attribute__((packed));


#endif // PROTO_H
