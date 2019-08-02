/*************************************************************************
	> File Name: cteat_thread.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月02日 星期五 10时36分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *thread (void *arg)
{
    pthread_t newthid;
    printf("this is a new thread, thread ID=%u\n",newthid);
    pthread_exit(3);
}

int main()
{
    pthread_t thid;
    int status;

    printf("main thread,ID is %u\n",pthread_self());

    if (pthread_create(&thid, NULL, (void *)thread, NULL)) {
        printf("thread creation failed\n");
        exit(1);
    }
    pthread_join(thid,(void *)&status);
    printf("%d\n",status);
}
