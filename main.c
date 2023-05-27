#include "header.h"
#include "server.h"
#include "thread_pool.h"
#include "getroute.h"

#define PORT 8080  //puerto de la conexion
#define SERV_ADRR "127.0.0.1" //ip


void *response(void* param)
{
  int clientfd =*((int*)param);
  
  char* content = get_info("/home");
  char* resp;
  if(content!=NULL)
  {
   resp=generate_response(content,200); 
  }else{
    resp=generate_response(read_file("./error_page.html"),404);
  }

 if(write(clientfd,resp,strlen(resp))<0)
 {
    printf("error enviando respuesta\n");
 }
  close(clientfd);
}

int main()
{

  ThreadPool* tp = create(3);

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

