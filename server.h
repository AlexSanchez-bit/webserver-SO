#include "header.h"
#include "getroute.h"
#include "thread_pool.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define  BUFF_SIZE 4098 //tamano del buffer a leer
#define BACKLOG 5  //cantidad de clientes a atender

#ifndef SERVER_H
#define SERVER_H

char* generate_response(char* text,int);


int create_conection(char* SERV_ADRR,int PORT)
{
  int sockfd;
  int aux;

  struct sockaddr_in server;
  sockfd=socket(AF_INET,SOCK_STREAM,0);

  if(sockfd==-1)
  {
    printf("error creando el socket\n");
    return -1;
  }else{
    printf("socket creado con exito\n");
  }

  int optval=1;
  if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &optval,sizeof(optval))<0)
  {
    printf("error en la funcion setoptv\n");
    return -1;
  }

  server.sin_family=AF_INET;
  server.sin_addr.s_addr = inet_addr(SERV_ADRR);
  server.sin_port        = htons(PORT);

  bzero(&(server.sin_zero),8);
  aux=bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));
  if(aux<0)
  {
    printf("no se pudo bindear el socket\n");
    return -1;
  }
  aux=listen(sockfd,BACKLOG);
  if(aux<0)
  {
  printf("error abriendo el servidor: %s \n",strerror(aux));
  return -1;
  }
  return sockfd;
}

int wait_client(int sockfd)
{
  int clientfd;
  struct sockaddr_in client;
  unsigned int size = sizeof(struct sockaddr_in);

  clientfd=accept(sockfd,(struct sockaddr*)&client,&size);

  if(clientfd<0)
  {
    printf("no se puede establecer la conexion: %s \n",strerror(clientfd));
    return -1;
  }

  return clientfd;
}


char* generate_response(char* text,int status)
{
  char* header="HTTP/1.1 %d OK\r\n\r\n %s \r\n\r\n";
  char* ret_val = malloc(strlen(text)+strlen(header)+3);
  sprintf(ret_val,header,status,text);
  return ret_val;
}

#endif
