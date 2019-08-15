/*************************************************************************
	> File Name: mysql.h
	> Author: 
	> Mail: 
	> Created Time: 2019年08月07日 星期三 09时03分15秒
 ************************************************************************/
#include "dfi.h"
void MYSQL_main_init();
void MYSQL_init();
int MYSQL_login(int, char *, int);
int MYSQL_regist(char *, char *);
int MYSQL_repass(int ,char *, char *);
int MYSQL_exit(int);
int MYSQL_find_fd(int);
int MYSQL_addfriend_store(int, int);
fri MYSQL_list_fri(int);
fri MYSQL_online_fri(int);
int MYSQL_chat_fri(int);
int MYSQL_store_chat(int, int, char *);
STR MYSQL_find_chat(int, int);
int MYSQL_dele_fri(int ,int);
