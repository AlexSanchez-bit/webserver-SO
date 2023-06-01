#include "header.h"
#include "server.h"
#include "thread_pool.h"
#include "getroute.h"
#include <signal.h>

#define PORT 8080  //puerto de la conexion
#define SERV_ADRR "127.0.0.1" //ip


 int server_socket;
 ThreadPool* tp;

void *response(void* param) //metodo para responder las peticiones
{
  int clientfd =*((int*)param);//file descriptor del usuario

  char buff[200];//buffer de lectura / escritura
  memset(buff,0,200);//limpia el buffer
  char* route;// variable para la ruta 
  int def=1;//para saber si es el home (/) u otra ruta la pedida

  int size=0;
  while((size=read(clientfd,&buff,200))>0){//lee todo el pedido http
    if(buff[size-1]=='\n')//si encuentra el salto de linea es q ya se leyo hasta el final
    {
      break;
    }
    if(strncmp("GET",buff,3)==0)//si hay un get significa que se pidio data al server
    {
      char* petition=malloc(120);//variable para guardar la peticion
      memset(petition,'\0',120);//limpio la variable
      *petition=buff[4];//le guardo la pos 4 (las e primeras son GET)
      int cant=1;//cantidad de char leidos
      int actual=5;//posicion actual en el buffer
      while(cant<size && buff[actual]!='H') //en los pedidos http
                                            //desde Get a HTTP va la ruta que se pide 
                                            //ejemplo GET / HTTP 1.1 ...
      {      
        if(buff[actual]=='%')//si encuentra esto es que habia un espacio y en el buffer
                             //se escriben %20 por eso me salto 3 posiciones
        {
        *(petition+cant)=' ';
          actual+=3;
          cant++;
          continue;
        }
        *(petition+cant)=buff[actual];//guardo el caracter 
        cant++;
        actual++;
      }      
      if(strncmp(petition,"/static",7)==0)// si se pide de static es el css/js por
                                          // tanto lo busco en la ruta del server
      {
        def=0;
      route=malloc(strlen(petition)+1);
      memset(route,0,strlen(petition));
      strcat(route,dirname());
      strncat(route,petition,strlen(petition)-1);
      strcat(route,"\0");
      }else
      if(strncmp(petition,"/ ",2)!=0){// si no es / entonces no es el home
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
  if(def)//si se hizo GET / entonces devuelvo la ruta por defecto
  {
    content = get_info(__dirname,clientfd);
  }else{// caso contrario la que me piden
   content = get_info(route,clientfd);
   free(route);//limpiar memoria
  }
  if(size<0)
  {
    printf("error leyendo del cliente\n");
  }
  close(clientfd);//cierro la conexion con el cliente
}



void sign_handler(int sign_num)
{
  finish(tp);//limpia el thread pool
  close(server_socket);//cierra la conexion  
     signal(SIGINT,SIG_DFL);
}

int main(int argc,char** args)
{
   
   if(argc == 1){// si argc es 1 no pasaron ruta
    __dirname="/home";//pongo una por defecto
   }else
   {
    __dirname=*(args+1);//caso contrario asigno la ruta default
   }


  init_files_mutex();//crea un mutex (para que no de bateo los hilos)

   tp = create(4);//crea un thread pool con 4 hilos

  init(tp);//inicializa el thread pool


  signal(SIGINT,sign_handler); //para manejar las senales

  server_socket = create_conection(SERV_ADRR,PORT);//crea la conexion
  
  if(server_socket<0)
  {
    printf("error al abrir el servidor\n");
  }else{

    printf(" servidor escuchando en %s:%d\n",SERV_ADRR,PORT);
  }

  while(1){
  int client_fd = wait_client(server_socket);  //forever espera a q un usuario haga un request
  send_job(tp,response,client_fd);//manda a responder al thread pool
  }
  finish(tp);//limpia el thread pool
  close(server_socket);//cierra la conexion  
  return 0;
}

