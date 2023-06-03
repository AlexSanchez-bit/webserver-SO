#include "header.h"
#include "server.h"
#include "handler.h"
#include "threads/thread_pool.h"
#include <signal.h>
#include "global.h"

#define SERV_ADRR "127.0.0.1"
#define PORT 8081



ThreadPool *tp;
int socketfd;
int end=0;

void sign_handler(int sign_num)
{
     finish(tp);  
     free(tp);
     close(socketfd);
     end=1;
     signal(SIGINT,SIG_DFL);
}


int main(int argc ,char** args)
{


  init_files_mutex();
//asignar directorios
char wcd[1024];//working directory
 memset(wcd,0,1024);
 getcwd(wcd,1024);//ruta desde donde se llamo el server
__dirname=malloc(strlen(wcd));
strcpy(__dirname,wcd);

if(argc<=1)
{
  default_dir=__dirname;
}else
{
default_dir=*(args+1);
}


  signal(SIGINT,sign_handler); //para manejar las senales

  tp = create(4);
  init(tp);

  socketfd = create_conection(SERV_ADRR,PORT);
  if(socketfd<0)
  {
    printf("error al iniciar el servidor puerto %d ocupado\n ",PORT);
    return -1;
  }
  printf("servidor escuchando en %s:%d\n",SERV_ADRR,PORT);

   while(!end){
    int clientfd = wait_client(socketfd);
    send_job(tp,handle_conection,clientfd);//manda a responder al thread pool
   }  

   free(__dirname);
  return 0;
}
