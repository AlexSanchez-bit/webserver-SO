#ifndef Worker_h
#define Worker_h

#include "quehue.h"
#include <pthread.h>

pthread_mutex_t mymutex;

//es una clase con los hilos que correran simultaneamente para ejecutar el programa

typedef struct worker
{
  int worker_id; 
  Queue* receptor;
} Worker;

void *worker_f(void* wk)//funcion que se ejecutara en cada hilo
{
  bool finish=false;   
  while(!finish)   // hasta que los mande a parar 
   {
    pthread_mutex_lock(&mymutex);//bloquea la cola
     Message* msg =pop(((Worker*)wk)->receptor);//saca un mensaje
    pthread_mutex_unlock(&mymutex);//desbloquea la cola
     if(msg!=NULL){
      if(msg->job!=NULL){
       msg->job(&msg->param);// ejecuta el metodo con el parametro enviado
      }
      finish=msg->finish;//revisa que no haya q finalizar
      free(msg);//libera la memoria
     }
   }
  printf("\n matando hilo %d\n",((Worker*)wk)->worker_id);
  return 0;
}
#endif
