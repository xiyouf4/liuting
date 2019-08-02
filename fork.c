/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月02日 星期五 16时49分46秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    char  *msg;
    int   k;

    printf (" process is running\n ");
    pid = fork();
    switch(pid){
        case 0:
            msg = "Child process is running";
            k = 3;
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
            msg = "Parent process is running";
            k = 5;
            break;
    }

    while(k>0)
    {
        puts(msg);
        k--;
    }
}
