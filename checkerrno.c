/*************************************************************************
	> File Name: checkerrno.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月02日 星期五 15时31分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *stream;
    char *filename = "test";

    stream = fopen (filename, "r");
    if (stream == NULL)
    {
        printf("open file %s failed, erno is %s\n", filename,strerror(errno));
        perror("open");
    }
    else
    {
        printf("open file %s successfully\n",filename);
    }
}
