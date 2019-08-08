/*************************************************************************
	> File Name: thread_syan.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月03日 星期六 16时04分50秒
 ************************************************************************/

#include<stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

pthread_mutex_t mutex; 
char buff[100];

void *thread1()
{

    int len;

    char data[10];
    sleep(5);
    while(1)
    {
        scanf("%s",data);
        len = strlen(buff);
        if(len<100)
        {

            pthread_mutex_lock(&mutex);
            strcat(buff,data);
            printf("**%s\n",buff);
            pthread_mutex_unlock(&mutex);
        
        }
        else
        {
            sleep(20);
        }

        
    }
}

void *thread2()
{
    int d;
    while(1)
    {
        d = get(&buff);
        pthread_mutex_lock(&mutex);
        while(p->readpos == p->writepos);
        data = p->buf[p->readpos++];
        printf("&&%s\n",
        pthread_mutex_unlock(&mutex);

        else
        {
            sleep(10);
        }
        
    }
    
}

int main()
{
    pthread_t proer,cuser;

    pthread_mutex_init(&mutex,NULL);

    pthread_create(&proer,NULL,thread1,NULL);
    pthread_create(&cuser,NULL,thread2,NULL);
    
    pthread_join(proer,NULL);
    pthread_join(cuser,NULL);
    return 0;
}
