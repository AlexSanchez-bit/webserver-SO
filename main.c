#include "header.h"
#include "server.h"
#include "thread_pool.h"
#include "getroute.h"

#define PORT 8080  //puerto de la conexion
#define SERV_ADRR "127.0.0.1" //ip



void *response(void* param)
{
  int clientfd =*((int*)param);
  printf("llego el fd %d\n",clientfd);

  char buff[200];
  memset(buff,0,200);
  char* route;
  int def=1;

  int size=0;
  while((size=read(clientfd,&buff,200))>0){
    if(buff[size-1]=='\n')
    {
      break;
    }
    if(strncmp("GET",buff,3)==0)
    {
      char* petition = malloc(1);
      *petition=buff[4];
      int cant=1;
      while(cant<size && buff[cant+4]!='H')
      {
        petition=realloc(petition,cant+1);
        if(buff[cant+4]=='%')
        {
          buff[cant+4]='\\';
          buff[cant+5]='/';
        }
        *(petition+cant)=buff[cant+4];
        cant++;
      }
      if(strncmp(petition,"/ ",2)!=0){
        def=0;
      route=malloc(strlen(petition));
      memset(route,0,strlen(petition));
      strncpy(route,petition,strlen(petition)-1);
      strcat(route,"\0");
      }
      free(petition);
    }
    memset(buff,0,200);
  }

  char* content;
  if(def)
  {
  content = get_info(__dirname,clientfd);
  }else{
   content = get_info(route,clientfd);
   free(route);
  }
  char* resp;
  if(size<0)
  {
    printf("error leyendo del cliente\n");
  }
  close(clientfd);
}




int main(int argc,char** args)
{
  __dirname=dirname();
  init_files_mutex();

  ThreadPool* tp = create(4);

  init(tp);

  int socket = create_conection(SERV_ADRR,PORT);
  
  if(socket<0)
  {
    printf("error al abrir el servidor\n");
  }

  while(1){
  int client_fd = wait_client(socket);  
  send_job(tp,response,client_fd);
  }
  finish(tp);
  close(socket);  
  return 0;
}

