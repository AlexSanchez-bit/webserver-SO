#include "generate.h"
#include "header.h"
#include "getroute.h"
#include "thread_pool.h"
#include <pthread.h>
#include <unistd.h>


void *hola()
{
  printf("hola mundo\n");
        fflush ( stdout ) ;
        printf("finish primer hilo");
}

void *aux()
{
  printf("holiii\n");
  while(1){}
}


int main()
{
  ThreadPool* tp = create(4);
  init(tp);
  send_job(tp,hola);
  send_job(tp,hola);
  send_job(tp,hola);
  send_job(tp,hola);
  send_job(tp,aux);
  finish(tp); 

  return 0;
}

/*
 *
  char* directory ="/dev/fb0";
  char** name=malloc(sizeof(char*)*3);
  char** route=malloc(sizeof(char*)*3);
  char** date=malloc(sizeof(char*)*3);
  char** size=malloc(sizeof(char*)*3);

  *(name)="alex";
  *(name+1)="christian";
  *(name+2)="barbaro";

  *(size)="4";
  *(size+1)="12";
  *(size+2)="43";

  *(date)="5/5/2001";
  *(date+1)="9/9/2019";
  *(date+2)="1/8/2018";

  *(route)="/etc/rpc";
  *(route+1)="/opt/yay";
  *(route+2)="/home/alex/pokemon";

  char* a =generate_file(directory,name,route,size,date,3);

  printf("%s\n",dirname());
 * */
