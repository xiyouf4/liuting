/*************************************************************************
	> File Name: client_io.h
	> Author: 
	> Mail: 
	> Created Time: 2019年08月08日 星期四 15时55分56秒
 ************************************************************************/

#ifndef _CLIENT_IO_H
#define _CLIENT_IO_H
int send_login_PACK(int conn_fd);
int send_regist_PACK(int conn_fd);
int send_repasswd_PACK(int conn_fd);
void send_exit_PACK();
void send_addfriend_PACK();
void send_list_fri();
void send_online_fri();

void *recv_PACK();
void recv_ADDFR_();

int getch();
#endif
