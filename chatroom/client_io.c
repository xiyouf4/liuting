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
int send_account;

void recv_ADDFR_mess(PACK);
void recv_ADDFR_();
void recv_CHAT_FRI(PACK);
void recv_store_chat();
void recv_list_fri();
void recv_online_fri();

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
            printf("@@\n");
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
    while(1) {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(conn_fd, &pack,sizeof(PACK), 0))>0) {
            if(pack.type == 0) {
                return 0;
            }
            else {
                return -1;
            }
        }
    }
}  

void send_regist_PACK(int conn_fd)
{
    PACK pack;
    pack.type = REGIST;

    printf("请输入你的昵称：");
    scanf("%s",pack.mes);
    printf("请输入你的密码：");
    scanf("%s",pack.mes2);
    //getchar();
    if(send(conn_fd, &pack, sizeof(PACK),0)<0){
        perror("send");
    }
    while(1) {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(conn_fd, &pack,sizeof(PACK), 0))>0) {
            if(pack.type == 0) {
                printf("注册成功，您的账号为%d\n",pack.account);
                return;
            }
            else {
                return;
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
    while(1) {
        int ret;
        memset(&pack, 0, sizeof(PACK));
        if((ret = recv(conn_fd, &pack,sizeof(PACK), 0))>0) {
            if(pack.type == 0) {
                return 0;
            }
            else {
                return -1;
            }
        }
    }
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

int send_list_fri()
{
    PACK pack;
    pack.type = LIST_FRI;
    pack.account = account;
    if(send(cli_fd, &pack, sizeof(PACK),0)<0) {
        perror("send_list_fri");
    }
}

int send_online_fri()
{
    PACK pack;
    pack.type = ONLINE_FRI;
    pack.account = account;
    if(send(cli_fd, &pack, sizeof(PACK),0)<0) {
        perror("send_list_fri");
    }
   
}

int send_chat_fri()
{
    PACK pack;
    pack.type = CHAT_FRI ;
    pack.account = account;
    printf("请输入你要发送的好友账号：");
    scanf("%d",&pack.send_account);
    send_account = pack.send_account;
    do {
        scanf("%s",pack.mes);
        getchar();
        if (send(cli_fd, &pack, sizeof(PACK),0)<0){
            perror("send");
        }
    }while (strcmp(pack.mes, "Bye") !=0 );
    send_account = 0;
}

void send_dele_fri()
{
    PACK pack;
    pack.type = DELE_FRI;
    pack.account = account;
    printf("请输入你要删除的好友账号:");
    scanf("%d",&pack.send_account);
    if (send(cli_fd, &pack, sizeof(PACK), 0)<0) {
        perror("send_dele_fri");
    }
  
}

void send_store_chat()
{
    PACK pack;
    pack.type = STORE_CHAT;
    pack.account = account;
    printf("请输入你想要查询的账号信息：");
    scanf("%d",&pack.send_account);
    getchar();
    if (send(cli_fd, &pack, sizeof(PACK),0)<0) 
    {
        perror("send_store_chat");
    }
    
}


void *recv_PACK()
{
    PACK pack;
    while (1) {
        if ((recv(cli_fd, &pack,sizeof(PACK), 0))>0) {
            switch (pack.type) {
                case ADD_FRIEND:
                        recv_ADDFR_mess(pack);
                        break;
                case CHAT_FRI:
                        recv_CHAT_FRI(pack);
                        break;
                case STORE_CHAT:
                        recv_store_chat();
                        break;
                case LIST_FRI:
                        recv_list_fri();
                        break;
                case ONLINE_FRI:
                        recv_online_fri();
                        break;

            }
        }
    }
}

typedef struct mess {  //加好友
    char mes[1000];
    int send_account;
    struct mess *next;
}MESS, *mess;
mess phead = NULL;
mess pend;

void recv_ADDFR_mess(PACK pack)   //bug,处理请求过程中，pthread_recv可能会存在丢包问题
{
    if (phead == NULL) {
        phead = (struct mess *)malloc(sizeof(struct mess));
        phead->next = NULL;
        pend = phead;
    }
    mess pnew;
    pnew = (mess)malloc(sizeof(MESS));
    strcpy(pnew->mes , pack.mes);
    pnew->send_account = pack.account;
    pnew->next = NULL;
    pend->next = pnew;
    pend = pnew;
    printf("你有好友申请,及时前往好友申请盒子处理\n");
    
}


void recv_ADDFR_()
{
    PACK pack;
    mess p,r;
    r = phead;
    while (r->next) {
        p = r->next;
        printf("%s\n",p->mes);
        printf("是否添加？？y/n\n ");
        char a = getch();
        memset(&pack, 0, sizeof(PACK)); 
        pack.type = 41;
        if (a == 'y' || a == 'Y') {
            strcpy(pack.mes, "success");
            printf("%s\n",pack.mes);
            pack.account = account;
            pack.send_account = p->send_account;
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
    r->next = p->next;
    free(p); 
    }
    free(r);
    phead = NULL;
}

typedef struct chat_fri {  //好友聊天
    int account;
    char mes[1000];
    struct chat_fri *next;
}chatF;
chatF *fhead,*fend;

void recv_CHAT_FRI(PACK pack)
{
    if (pack.account == send_account) {
       printf("账号%d:%s\n",pack.account,pack.mes); 
    }
    else {
        if (fhead == NULL) {
            fhead = (chatF *)malloc(sizeof(chatF));
            fhead->next = NULL;
            fend = fhead;
        }
        chatF *fnew;
        fnew = (chatF *)malloc(sizeof(chatF));
        fnew->account = pack.account;
        strcpy(fnew->mes , pack.mes);
        fnew->next = NULL;
        fend->next = fnew;
        fend = fnew;
        printf("你有好友消息,请及时前往聊天盒子处理\n");
    }
}

void recv_chat_fri()
{
    if (fhead == NULL) {
        printf("暂无好友消息\n");
        return;
    }
    chatF *f = fhead;
    chatF *m;
    while (f->next) {
        m = f->next;
        printf("账号%d:%s\n",m->account,m->mes); 
        f->next = m->next;
        free(m); 
    }
    free(f);
    fhead = NULL;
}

void recv_store_chat()
{
    STR str;
    while(1) {
        if(recv(cli_fd, &str, sizeof(STR), 0)>0) {
            for(int i = 0; i<str.len; i++) {
                printf("send_account:%d\t", str.account[i]);
                printf("recv_account:%d\t", str.send_account[i]);
                printf("%s\n", str.mes[i]);
            }
            return ;
        }
    }
}

void recv_list_fri()
{
    fri p;
    while(1) {
        int ret;
        memset(&p, 0, sizeof(fri));
        if(recv(cli_fd, &p,sizeof(fri), 0)>0) {
            for(int i = 0; i < p.len; i++) {
                printf("account:%d  ",p.account[i]);
                printf("昵称:%s\n",p.name);
            }
            return ;
        }
    }
}

void recv_online_fri()
{
    fri p;
    while(1) {
        int ret;
        memset(&p, 0, sizeof(fri));
        if(recv(cli_fd, &p,sizeof(fri), 0)>0) {
            for(int i = 0; i < p.len; i++) {
                if(p.account[i] != 0) {
                    printf("account:%d  ",p.account[i]);
                    printf("昵称:%s\n",p.name);
                }
            }
            return ;
        }
    }
}
