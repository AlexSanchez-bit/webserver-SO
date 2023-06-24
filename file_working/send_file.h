#include "../header.h"
#include "read_templates.h"
#include "RC.h"
#include <sys/sendfile.h>
#ifndef SEND_FILE_H
#define SEND_FILE_H
#define SEND_SIZE 200

char* get_filename(char* route_to_file);

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

  char* header="HTTP/1.1 200 OK\r\n Content-Disposition: attachment; filename=%s; Content-Type: application/zip  \r\n\r\n";//cabecera para enviar
  char *filename=get_filename(route_to_file);
  char* sent_header = malloc(strlen(header)+strlen(filename));
  sprintf(sent_header,header,filename);
  send(cfd, sent_header, strlen(sent_header), 0);//envio la cabecera
  char buff[SEND_SIZE];//buffer de lectura/escritura
  memset(buff,0,SEND_SIZE);//limpio el buffer
  long size=SEND_SIZE;
  off_t off=0;
  size_t readed=0;
  while((readed =sendfile(cfd,file,&off,size))>0){
  }

  pthread_mutex_lock(&filemutex);
   drop((list+openned));//limpio el fd 
  pthread_mutex_unlock(&filemutex);
  return file; 
}

char* get_filename(char* route_to_file)
{

  for(int i=strlen(route_to_file)-1; i>0 ;i--)
  {
    if(*(route_to_file+i)=='/')
    {
      return (route_to_file+i+1);
    }
  }
  return route_to_file;
}


#endif
