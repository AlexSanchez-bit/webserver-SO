#ifndef QUEUE_H
#define QUEUE_H
#include "message.h"

typedef struct queue
{
   Message* msg; 
   struct queue* next;
} Queue;

Queue new_q(Message* msg)
{
  return (Queue){msg,NULL};
}

Queue* new_empty()
{
  Queue* pointer = malloc(sizeof(Queue));
  Queue new_node = new_q(NULL);
  *pointer=new_node;
  return pointer;
}

void push(Queue* cola,Message* mess)
{
     if(cola->msg==NULL)
     {
        cola->msg=malloc(sizeof(Message));
        cola->msg=mess;
        return;
     }

     Queue* aux=cola;
     while(aux->next!=NULL)
     {
        aux=aux->next;
     }   
     aux->next=new_empty();
     push(aux->next,mess);
}

Message* pop(Queue* cola)
{
  if(cola->msg==NULL)
  {
    return NULL;
  }else if(cola->next==NULL)
  {
    Message* ret = malloc(sizeof(Message));
    *ret = *(cola->msg);    
    cola->msg=NULL;
    return ret;
  }
  else{
    Queue* aux = malloc(sizeof(Queue));
    *aux = * cola;
    *(cola)=*(cola->next);
    free(aux->next);
    return aux->msg;
  }
}


#endif

