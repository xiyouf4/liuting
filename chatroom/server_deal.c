/************************************************************************
	> File Name: server_deal.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月08日 星期四 16时18分30秒
 ************************************************************************/
#include "MYSQL.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>


static int cli_fd;

void send_PACK(PACK pack);
void recv_PACK(int conn_fd);
void deal_regist(PACK pack);
void deal_login(PACK pack);
void deal_repass(PACK pack);
void deal_exit(PACK pack);
void deal_addfriend(PACK pack);
void deal_list_fri(PACK pack);
void deal_online_fri(PACK pack);
void deal_chat_fri(PACK pack);
void deal_store_fri(PACK);
void deal_dele_fri(PACK pack);
void deal(PACK pack);
void pro_off(int , int);
void deal_off(PACK pack);
void recv_addfr_PACK(PACK);

void recv_PACK(int conn_fd)
{
    PACK pack;
    cli_fd = conn_fd;
    int ret;
    if((ret = recv(conn_fd, &pack, sizeof(struct package),0)) < 0){
        perror("recv");
        exit(1);
    }
    if (ret == 0)
    {
        return ;
    }
    deal(pack);
}

void send_PACK(PACK pack)
{
    int ret;
    if((ret =  send(cli_fd,&pack,sizeof(PACK),0))<0) {
        perror("server_deal ：send");
    }
}

void send_other_PACK(PACK pack,int send_cli_fd)
{
    int ret;
    if((ret =  send(send_cli_fd,&pack,sizeof(PACK),0))<0) {
        perror("server_deal ：send");
    }
}


void deal(PACK pack)
{
    int type = pack.type;
    MYSQL_init();
    switch (type)
    {
        case 0:
            deal_exit(pack);
            break;
        case 1:
            deal_login(pack);
            break;
        case 2:
            deal_regist(pack);
            break;
        case 3:
            deal_repass(pack);
            break;
        case 4:
            deal_addfriend(pack);
            break;
        case 41:
            recv_addfr_PACK(pack);
            break;
        case 5:
            deal_list_fri(pack);
            break;
        case 6:
            deal_online_fri(pack);
            break;
        case 7:
            deal_chat_fri(pack);
            break;
        case 8:
            deal_store_fri(pack);
            break;
        case 9:
            deal_dele_fri(pack);
            break;
        //case 10：
            
    }
}
void deal_login(PACK pack)
{
    PACK send_pack;
   
    int ret = MYSQL_login(pack.account, pack.mes,cli_fd);
    if(ret == 0) {
        send_pack.type = 0;
        strcpy(send_pack.mes,"登录成功");
    }
    else {
        send_pack.type = -1;
    }
    send_PACK(send_pack);
    pro_off(pack.account, cli_fd);        

}

void deal_regist(PACK pack)
{
    PACK send_pack;

    int ret = MYSQL_regist(pack.mes, pack.mes2);
    if(ret != -1) {
        send_pack.type = 0;
        send_pack.account = ret;
        printf("%d\n",ret);
        strcpy(send_pack.mes,"注册成功");

    }
    else {
        send_pack.type = -1;
    }
    send_PACK(send_pack);
}

void deal_repass(PACK pack)
{
    PACK send_pack;

    int ret = MYSQL_repass(pack.account, pack.mes, pack.mes2);
    if(ret != -1) {
        send_pack.type = 0;
        strcpy(send_pack.mes,"重置密码成功");
    }
    else {
        send_pack.type = -1;
    }
    send_PACK(send_pack);
}

void deal_exit(PACK pack)
{
    PACK send_pack;

    int ret = MYSQL_exit(pack.account);
    if(ret != -1) {
        send_pack.type = 0;
    }
    else {
        send_pack.type =-1;
    }
    send_PACK(send_pack);

}

void deal_list_fri(PACK pack)
{
    PACK send_pack;
    fri p = MYSQL_list_fri(pack.account);
    if(send(cli_fd, &pack, sizeof(PACK), 0)<0) {
        perror("deal_online_fri");
    }
    if((send(cli_fd,&p,sizeof(fri),0))<0) {
        perror("server_deal ：send");
    }

}

void deal_online_fri(PACK pack)
{
    fri p = MYSQL_online_fri(pack.account);
    if(send(cli_fd, &pack, sizeof(PACK), 0)<0) {
        perror("deal_online_fri");
    }
    if((send(cli_fd,&p,sizeof(fri),0))<0) {
        perror("server_deal ：send");
    }

}

void deal_chat_fri(PACK pack)
{
    int send_cli_fd = MYSQL_find_fd(pack.send_account);
    if(MYSQL_store_chat(pack.account,pack.send_account,pack.mes) != 0) {
         printf("聊天记录存入失败\n");
    }
    if(send_cli_fd == -1) {
        deal_off(pack);
    }
    else {
        send_other_PACK(pack,send_cli_fd);
    }

}

void deal_store_fri(PACK pack)
{
    PACK send_pack;
    STR str = MYSQL_find_chat(pack.account, pack.send_account);
    if(str.account[0] == -1)
    {
       strcmp(send_pack.mes, "没有聊天记录或者查询失败");
    }
    send_pack.type = STORE_CHAT;
    if(send(cli_fd, &send_pack, sizeof(PACK), 0)<0) {
        perror("deal_store_fri");
    }
    int ret;
    if((ret=send(cli_fd, &str, sizeof(STR),0)) <=0 ) {
        perror("send_store_fri");
    }
}
void deal_dele_fri(PACK pack)
{
    MYSQL_dele_fri(pack.account, pack.send_account);
}

typedef struct off{
    char mes[1000];
    int  account;
    int  send_account;
    int  type;
    struct off *next;
}OFF;
OFF *phead = NULL;
OFF *pend = NULL;

void deal_off(PACK pack)
{
    if (phead == NULL) {
        phead = (OFF *)malloc(sizeof(OFF));
        phead->next =NULL;        
        pend = phead;
    }
    OFF *pnew;
    pnew = (OFF *)malloc(sizeof(OFF));
    strcpy(pnew->mes , pack.mes);
    pnew->account = pack.account;
    pnew->send_account = pack.send_account;
    pnew->type = pack.type;
    pnew->next = NULL;
    pend->next = pnew;
    pend = pnew;
}

void pro_off(int send_account, int send_cli_fd)
{
    if (phead == NULL) {
        return;
    }
    OFF *r = phead;
    OFF * p = NULL;
    while (r->next) {
        p = r->next;
        if (p->send_account == send_account) {

            PACK send_pack;
            strcpy(send_pack.mes , p->mes);
            send_pack.type = p->type;
            send_pack.account = p->account;
            send_pack.send_account = p->send_account;
            printf("%d\n",send_pack.send_account);
            send_other_PACK(send_pack,send_cli_fd);
            PACK pack;
            r->next = p->next;
            free(p);
            continue;
        }
        r = r->next;
    }
    pend = r;
}

void deal_addfriend(PACK pack)
{
    PACK send_pack;
    int send_cli_fd;

    send_cli_fd = MYSQL_find_fd(pack.send_account);
    sprintf(send_pack.mes,"账号%d申请加你为好友，是否同意",pack.account);  
    printf("%s\n",send_pack.mes);
    send_pack.account = pack.account;
    send_pack.send_account = pack.send_account;
    send_pack.type = ADD_FRIEND;
    
    if(send_cli_fd == -1) {
        deal_off(send_pack);
    }
    else {
        send_other_PACK(send_pack,send_cli_fd);      
    }
    
}

void recv_addfr_PACK(PACK recv_pack)
{
    if(recv_pack.type == 41) {
        printf("成功，为：%s\n",recv_pack.mes);
            if(strcmp(recv_pack.mes,"success") == 0) {
                printf("** %d   %d\n ",recv_pack.account, recv_pack.send_account);
                if(MYSQL_addfriend_store(recv_pack.account , recv_pack.send_account) == 0) {
                    printf("添加好友信息成功!!!\n");
                    }
                }
    }
    else {
            printf("好友信息返回失败\n");
    }
}
