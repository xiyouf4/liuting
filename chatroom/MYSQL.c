/*************************************************************************
	> File Name: mysql.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月07日 星期三 09时02分57秒
 ************************************************************************/

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

MYSQL mysql;
MYSQL_RES  *result=NULL;
MYSQL_ROW  row;
MYSQL_FIELD *field;


void MYSQL_init()
{
    if (NULL == mysql_init(&mysql)) {
        perror("mysql_init");
    }
    mysql_real_connect(&mysql, "127.0.0.1", "root", "200713", "chatroom", 0, NULL, 0);
    if (mysql_set_character_set(&mysql, "utf8")<0){
        perror("mysql_set_character");
    }
}

void MYSQL_main_init()
{
    MYSQL_init();
    char buff[1000];
    sprintf(buff, "update user set online = 0");
    printf("%s\n",buff);
    if (mysql_query(&mysql,buff))
    {
        perror("main init :mysql_query");
    }

    mysql_close(&mysql);
}


int MYSQL_login(int account, char *password, int cli_fd)
{
    int ret;
    int flag = 1;
    char buff[1000];
    sprintf(buff, "select *from user where id = %d ", account);//连接两个字符串
    ret = mysql_query(&mysql,buff);
    if (!ret) {
        result = mysql_store_result(&mysql);    //返回查询结果
        if (!result) {
            perror("login:mysql_store_result");
        }
        //num_fields=mysql_num_fields(result);    

        while (row=mysql_fetch_row(result)) { 
            if (strcmp(password, row[2]) == 0) {
                printf("客户端%d 登录成功\n",account);
            
                sprintf(buff,"update user set online = 1 ,sock_fd = %d where id = %d",cli_fd, account);
                if (mysql_query(&mysql, buff)) {
                    perror("login:mysql_query");
                }
                mysql_free_result(result);
                mysql_close(&mysql);
                return 0;
            }
        }
    }
    return -1;
}
int MYSQL_regist(char *mes, char *mes2)
{
    int ret;
    int num_fields;

    char buff[1000];
    sprintf(buff, "insert into user (name,passwd)values (\"%s\",\"%s\")", mes,mes2);//连接两个字符串
    ret = mysql_query(&mysql,buff);

    if (!ret) {
        sprintf(buff, "select LAST_INSERT_ID()");//连接两个字符串
        ret = mysql_query(&mysql,buff);
        if (!ret) {
            result = mysql_store_result(&mysql);    //返回查询结果
            if (!result) {
                perror("login:mysql_store_result");
            }
            while (row=mysql_fetch_row(result)) {
                int id = atoi(row[0]);
                printf("客户端 %d 注册成功\n",id);
                mysql_close(&mysql);
                return id;
            }
        }
    }
    else {
        mysql_close(&mysql);
        return -1;
     }
}

int MYSQL_repass(int account,char *old_passwd, char *new_passwd)
{
    int ret;

    char buff[1000];
    sprintf(buff, "select *from user where id = %d ", account);//连接两个字符串
    ret = mysql_query(&mysql,buff);
    if (!ret) {
        result = mysql_store_result(&mysql);    //返回查询结果
        if (!result) {
            perror("login:mysql_store_result");
        }
        //num_fields=mysql_num_fields(result);    

        while (row=mysql_fetch_row(result)) { 
            if (strcmp(old_passwd, row[2]) == 0) {
            
                sprintf(buff,"update user set passwd = '%s' where id = %d",new_passwd , account);
                if(mysql_query(&mysql, buff)) {
                    perror("login:mysql_query");
                }

                printf("客户端%d 修改密码成功\n",account);
                mysql_free_result(result);
                mysql_close(&mysql);
                return 0;
            }
        }
    }
    return -1;
}

int MYSQL_exit(int account)
{
    char buff[1000];
    int flag = 0;
    sprintf(buff,"update user set online = %d  where id = %d",flag,account);
    if (mysql_query(&mysql, buff)) {
        perror("login:mysql_query");
    }
     
}
int MYSQL_addfriend(int sned_account, int account)
{
    int ret; 
    int send_cli_fd;
    char buff[1000];

    sprintf(buff,"select * from user where id = %d",account);
    ret = mysql_query(&mysql, buff);
    if (!ret) {
        result = mysql_store_result(&mysql);    //返回查询结果
        if (!result) {
            perror("addfriend:mysql_store_result");
        }
        while (row=mysql_fetch_row(result)) {
            char tmp[20];
            if(atoi(row[3])==0) {
                return -1;
            }
            else {
               send_cli_fd = atoi(row[4]);
            }
            
            mysql_free_result(result);
            mysql_close(&mysql);
            return send_cli_fd;
        }
    }
    return -1;
}

int MYSQL_addfriend_store(int account, int send_account)
{
    MYSQL_init();
    int ret;
    char buff[1000];
    sprintf(buff, "insert into friends (id1,id2)values (%d,%d)", account,send_account);//连接两个字符串
    ret = mysql_query(&mysql,buff);
    if (!ret) {
        sprintf(buff, "insert into friends (id1,id2)values (%d,%d)", send_account,account);//连接两个字符串
        ret = mysql_query(&mysql,buff);
        if (!ret) {
            mysql_close(&mysql);
            return 0;
        }
        else {
            mysql_close(&mysql);
             return -1;
        }
    }
    else {
        mysql_close(&mysql);
        return -1;
     }
}

int MYSQL_list_fri(int account)
{
    MYSQL_init();
     int ret; 
    int send_cli_fd;
    char buff[1000];

    sprintf(buff,"select * from friends where id1 = %d",account);
    ret = mysql_query(&mysql, buff);
    if (!ret) {
        result = mysql_store_result(&mysql);    //返回查询结果
        if (!result) {
            perror("addfriend:mysql_store_result");
        }
        while (row=mysql_fetch_row(result)) {
            char tmp[20];
            if(atoi(row[3])==0) {
                return -1;
            }
            else {
               send_cli_fd = atoi(row[4]);
            }
            
            mysql_free_result(result);
            mysql_close(&mysql);
            return send_cli_fd;
        }
    }
    return -1;
}
