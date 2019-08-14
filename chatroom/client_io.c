#include "dfi.h"

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


int cli_fd;
int account;

void recv_ADDFR_mess(PACK);
void recv_ADDFR_();
int getch()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

int recv_deal_PACK()
{
    PACK pack;
    while(1) {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(cli_fd, &pack,sizeof(PACK), 0))>0) {
            if(pack.type == 0) {
                return 0;
            }
            else {
                return -1;
            }
        }
    }
}

int send_login_PACK(int conn_fd)
{

    PACK pack; 
    pack.type = LOGIN;
    cli_fd = conn_fd;

    printf("请输入你的账号：");
    scanf("%d",&pack.account);
    account = pack.account;
    printf("请输入你的密码：");
    scanf("%s",pack.mes);
    getchar();    
    if(send(conn_fd, &pack, sizeof(PACK),0)<0){
        perror("send");
    }
    recv_deal_PACK(); 
}  

int send_regist_PACK(int conn_fd)
{
    PACK pack;
    pack.type = REGIST;

    printf("请输入你的昵称：");
    scanf("%s",pack.mes);
    printf("请输入你的密码：");
    scanf("%s",pack.mes2);
    getchar();
    if(send(conn_fd, &pack, sizeof(PACK),0)<0){
        perror("send");
    }
    while(1)
    {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(cli_fd, &pack,sizeof(PACK), 0))>0) {
            if(pack.type == 0) {
                printf("申请账号成功，您的账号为：%d\n",pack.account);
                return 0;
            }
            else {
                printf("注册失败");
                return -1;
            }
        }
    }
}
int send_repasswd_PACK(int conn_fd)
{
    PACK pack;
    pack.type = REPASSWD ;

    printf("请输入你的账号：");
    scanf("%d",&pack.account);
    printf("请输入你的旧密码：");
    scanf("%s",pack.mes);
    printf("请输入你的新密码：");
    scanf("%s",pack.mes2);
    getchar();
    if(send(conn_fd, &pack, sizeof(PACK),0)<0){
        perror("send");
    }
    recv_deal_PACK();
}


void send_exit_PACK()
{
   PACK pack;
   pack.type = EXIT;
   pack.account = account;
   if(send(cli_fd, &pack, sizeof(PACK),0)<0) {
       perror("send_exit");
   }
}

void send_addfriend_PACK()
{
    PACK pack;
    pack.type = ADD_FRIEND;
    pack.account = account;
    printf("请输入要添加的账号：");
    scanf("%d",&pack.send_account);
    getchar();
    if(send(cli_fd,&pack,sizeof(PACK),0)<0) {
        perror("send_addfriend_PACK");
    }
   
}

void send_list_fri()
{
  PACK pack;
  pack.type = LIST_FRI;
  if(send(cli_fd, &pack, sizeof(PACK),0)<0) {
      perror("send_list_fri");
  }
}

//void send_online_fri()
void *recv_PACK()
{
    PACK pack;
    while (1) {
        if ((recv(cli_fd, &pack,sizeof(PACK), 0))>0) {
            switch (pack.type) {
                case ADD_FRIEND:
                        recv_ADDFR_mess(pack);
                
            }
        }
    }
}

typedef struct mess
{
    char mes[1000];
    struct mess *next;
}MESS, *mess;
mess phead,pend;


void recv_ADDFR_mess(PACK pack)   //bug,处理请求过程中，pthread_recv可能会存在丢包问题
{
    phead = (struct mess *)malloc(sizeof(struct mess));
    phead->next = NULL;
    pend = phead;
    mess pnew;
    pnew = (mess)malloc(sizeof(MESS));
    strcpy(pnew->mes , pack.mes);
    pnew->next = NULL;
    pend->next = pnew;
    pend = pnew;
    printf("你有好友申请,及时前往消息盒子处理\n");
    
}

void recv_ADDFR_()
{
    PACK pack;
    mess p;
    p = phead->next;
    while (p) {
        printf("%s\n",p->mes);
        printf("是否添加？？y/n\n ");
        char a = getch();
        memset(&pack, 0, sizeof(PACK)); 
        pack.type = 4;
        printf("%d\n",pack.type);
        if (a == 'y' || a == 'Y') {
            strcpy(pack.mes, "success");
            printf("%s\n",pack.mes);
            if(send(cli_fd, &pack, sizeof(PACK),0)<0){
                perror("send");
            }
        }
        else {
            strcpy(pack.mes, "fail");
            if(send(cli_fd, &pack, sizeof(PACK),0)<0){
                perror("send");
            }
        }
    p = p->next;

    }
}
