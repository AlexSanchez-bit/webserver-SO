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
  int sockfd;//el file descriptor del socket
  int aux; 

  struct sockaddr_in server;// el tipo de dato para la info del server
  sockfd=socket(AF_INET,SOCK_STREAM,0);//crea el socket con http stream

  if(sockfd==-1)
  {
    printf("error creando el socket\n");
    return -1;
  }else{
    printf("socket creado con exito\n");
  }

  int optval=1;
  if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &optval,sizeof(optval))<0)//pide el socket al sistema
  {
    printf("error en la funcion setoptv\n");
    return -1;
  }

  //guarda la informacion del socket
  server.sin_family=AF_INET;
  server.sin_addr.s_addr = inet_addr(SERV_ADRR);
  server.sin_port        = htons(PORT);
  //inicializa en 0 la info 
  bzero(&(server.sin_zero),8);

  aux=bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));//ubica el socket en el sistema
  if(aux<0)
  {
    printf("no se pudo bindear el socket\n");
    return -1;
  }
  aux=listen(sockfd,BACKLOG);//pone a escuchar el socket (backlog es la cantidad de clientes que pueden escuchar al mismo time)
  if(aux<0)
  {
  printf("error abriendo el servidor: %s \n",strerror(aux));
  return -1;
  }
  return sockfd;//devuelve el fd del socket
}

int wait_client(int sockfd)
{
  int clientfd;//file descriptor del cliente
  struct sockaddr_in client;//estructura para guardar las propiedaddes del cliente
  unsigned int size = sizeof(struct sockaddr_in);

  clientfd=accept(sockfd,(struct sockaddr*)&client,&size);//acepta una conexion del cliente

  if(clientfd<0)
  {
    printf("no se puede establecer la conexion: %s \n",strerror(clientfd));
    return -1;
  }

  return clientfd;//devuelve el file descriptor del cliente
}



#endif
