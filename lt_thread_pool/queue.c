/*************************************************************************
	> File Name: queue.c
	> Author: 
	> Mail: 
	> Created Time: 2019年08月05日 星期一 10时57分03秒
 ************************************************************************/
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *head;
    Node *end;
    size_t size;
} Queue;

Queue *Queue_init(void)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = (Node *)malloc(sizeof(Node));
    queue->head->next = NULL;
    queue->end = queue->head;
    queue->size = 0;
    return queue;
}

void Queue_destory(Queue *queue, void(*dataDestory)(void *))
{
    Node *q;
    Node *p;
    q = queue->head->next;
    while(q)
    {
        p = q;
        q = q->next;
        if(dataDestory) {
            dataDestory(p->data);
        }
        free(p);
    }
    free(queue->head);
    queue->head = NULL;
}
void Queue_push(Queue *queue, void *data)
{
    Node *qnew = (Node*)malloc(sizeof(Node));
    qnew->data = data;
    qnew->next = NULL;
    queue->end->next = qnew;
    queue->end = qnew;
    queue->size++;
}
void *Queue_pop(Queue *queue)
{
    void *data = NULL;
    if (queue->size == 1) {
        data = queue->head->next->data;
        free(queue->head->next);
        queue->end = queue->head;
        queue->head->next = NULL;
    }
    if (queue->size > 1) {
        Node *p = queue->head->next;
        data = p->data;
        queue->head->next = p->next;
        free(p);
    }
    queue->size--;
    return data;

}
size_t Queue_size(Queue *queue)
{
    return queue->size;
}
