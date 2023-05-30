#include "header.h"
#include "server.h"
#include "thread_pool.h"
#include "getroute.h"

#define PORT 8080  //puerto de la conexion
#define SERV_ADRR "127.0.0.1" //ip



void *response(void* param)
{
  int clientfd =*((int*)param);

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
      char* petition=malloc(120);
      memset(petition,'\0',120);
      *petition=buff[4];
      int cant=1;
      int actual=5;
      while(cant<size && buff[actual]!='H')
      {      
        if(buff[actual]=='%')
        {
        *(petition+cant)=' ';
          actual+=3;
          cant++;
          continue;
        }
        *(petition+cant)=buff[actual];
        cant++;
        actual++;
      }      
      if(strncmp(petition,"/static",7)==0)
      {
        def=0;
      route=malloc(strlen(petition)+1);
      memset(route,0,strlen(petition));
      strcat(route,dirname());
      strncat(route,petition,strlen(petition)-1);
      strcat(route,"\0");
      }else
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
   
  for(int i=0;i<argc;i++)
  {
    printf("%s\n",*(args+i));
  }
   if(argc == 1){
    __dirname="/home";
   }else
   {
    __dirname=*(args+1);
   }


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

