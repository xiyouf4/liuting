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

typedef struct package{
    int  type;
    int  account;
    char send_name[100];
    int  send_account;
    time_t time;
    char mes[1000];
    char mes2[1000];
} PACK;

