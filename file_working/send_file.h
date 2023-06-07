#include "../header.h"
#include "read_templates.h"
#include "RC.h"
#ifndef SEND_FILE_H
#define SEND_FILE_H
#define SEND_SIZE 200


RC* list;
int size=0;

int send_file(char *route_to_file,int cfd)
{  
  int openned=0;
  int file;
  pthread_mutex_lock(&filemutex);//para que no de lio con los hilos
  openned = contains(list,size,route_to_file);//verifica que ya este abierto
  if(openned>0)
  {
     file=(list+openned)->fd;//si esta entonces toma su fd
  }else{
    file=open(route_to_file,O_RDONLY);//abrelo
    save_fd(&list ,&size ,file,route_to_file);//guarda el fd
  }
  pthread_mutex_unlock(&filemutex);

  if(file<0)
  {
    return -1;
  }

  char* header="HTTP/1.1 200 OK\r\n Content-Type: raw  \r\n\r\n\r\n";//cabecera para enviar
  send(cfd, header, strlen(header), 0);//envio la cabecera
  char buff[SEND_SIZE];//buffer de lectura/escritura
  memset(buff,0,SEND_SIZE);//limpio el buffer
  int size=0;
  int total_size=0;

  while((size=read(file,&buff,SEND_SIZE))>0)//minetras lea algo
  {
    send(cfd, buff, size, 0);//envio los bytes                                 
    memset(buff,0,SEND_SIZE);//limpio el buffer
    total_size+=size;                             
  }  


  pthread_mutex_lock(&filemutex);
   drop((list+openned));//limpio el fd 
  pthread_mutex_unlock(&filemutex);
  return total_size; 
}

#endif
