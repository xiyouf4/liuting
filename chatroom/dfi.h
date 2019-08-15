/*************************************************************************
	> File Name: dfi.h
	> Author: 
	> Mail: 
	> Created Time: 2019年08月12日 星期一 17时36分59秒
 ************************************************************************/
#include <time.h>

#define EXIT       0
#define LOGIN      1
#define REGIST     2
#define REPASSWD   3
#define ADD_FRIEND 4
#define LIST_FRI   5
#define ONLINE_FRI 6
#define CHAT_FRI   7
#define STORE_CHAT 8
#define DELE_FRI   9

typedef struct package {
    int  type;
    int  account;
    char send_name[100];
    int  send_account;
    time_t time;
    char mes[1000];
    char mes2[1000];
} PACK;

typedef struct friends {
    int  account[200];
    char name[200][100];
    char online[200];
    int  len;
}fri;

typedef struct store_chat {
    int account[1000];
    int send_account[1000];
    char mes[1000][1000];
    int len;
}STR;
