#ifndef Worker_h
#define Worker_h

#include "quehue.h"
#include <pthread.h>

pthread_mutex_t mymutex;

typedef struct worker
{
  int worker_id; 
  Queue* receptor;
} Worker;

void *worker_f(void* wk)
{
  bool finish=false;   
  while(!finish)    
   {
    pthread_mutex_lock(&mymutex);
     Message* msg =pop(((Worker*)wk)->receptor);
    pthread_mutex_unlock(&mymutex);
     if(msg!=NULL){
      if(msg->job!=NULL){
       msg->job(&msg->param);
      }
      finish=msg->finish;
      free(msg);
     }
   }
  printf("\n matando hilo %d\n",((Worker*)wk)->worker_id);
  return 0;
}
#endif
