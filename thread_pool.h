#ifndef Thread_pool_h
#define Thread_pool_h
#include "worker.h"
#include "message.h"

typedef struct tp 
{
  Worker* workers;
  int cant_workers;
  pthread_t* threads;
  Queue* cola;
} ThreadPool;

ThreadPool* create(int cant)
{
     Queue* cola = new_empty();
     pthread_mutex_init(&mymutex,NULL);

     Worker* worker_list =malloc(sizeof(Worker)*cant);

     pthread_t* threads = malloc(sizeof(pthread_t)*cant);

     for(int i=0;i<cant;i++)
     {
        *(worker_list+i)=(Worker){i+1,cola};      
  }

     ThreadPool tp = {worker_list,cant,threads,cola};

     ThreadPool* ret_val= malloc(sizeof(ThreadPool));
     *ret_val = tp;
     return ret_val;
}

void init(ThreadPool* tp)
{
  for(int i=0;i<tp->cant_workers;i++)
  {
    pthread_create ((tp->threads+i) , NULL , worker_f , (tp->workers+i) );
  }
}

void send_job(ThreadPool* tp , func* job)
{
    Message a = {false,job};
    Message* pointer = malloc(sizeof(Message));
    *pointer=a;
    pthread_mutex_lock(&mymutex);
    push(tp->cola,pointer);
    pthread_mutex_unlock(&mymutex);
}

void finish(ThreadPool* tp)
{
  
    Message a = {true,NULL};
  for(int i=0;i<tp->cant_workers+1;i++)
  {
    Message* pointer = malloc(sizeof(Message));
    *pointer=a;
    pthread_mutex_lock(&mymutex);
    push(tp->cola,pointer);
    pthread_mutex_unlock(&mymutex);
  }

  for(int i=0;i<tp->cant_workers;i++)
  {
    pthread_join ( *(tp->threads+i) , NULL ) ;
  }
}
#endif
