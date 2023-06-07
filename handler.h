#include "global.h"
#include "file_working/html_work.h"
#include "file_working/RC.h"
#include<pthread.h>
#ifndef HANDLER_H
#define HANDLER_H

//seccion donde se manejan las peticiones http
RC* list; 
int list_size=0;

pthread_mutex_t client_mutex;

                               //
void initHandlerMutex()
{
  pthread_mutex_init(&client_mutex,NULL);
}

char* get_route(int cfd);//metodod que devuelve la ruta pedida por el navegador

void *handle_conection(void* cfd)//metodo que se ejecutara en los hilos
{
  int clientfd = *((int*)cfd); //obtengo el file descriptor del cliente
  int fdpos=0;                                                                       
  char* route = get_route(clientfd); //obtengo la ruta
   while(route!=NULL){                                     
     if(strcmp(route,"null")!=0){
       printf("%s\n",route);
    send_html(route,clientfd );
     }//envio el html
    route=get_route(clientfd);
    }
    close(clientfd);
    printf("cerrando %d\n",clientfd);
}

char* get_route(int cfd)
{
  char buffer[BUFF_SIZE];//buffer de lectura para el stream http
  memset(&buffer,0,BUFF_SIZE);
  int readed=0;//cantidad de bytes leidos
  char* route="null";//ruta a devolver
  int size=0;   

  readed=recv(cfd,&buffer,BUFF_SIZE,0);

  if(readed<=0){return NULL;}//si ocurre esto se finalizo la conexion con el cliente
                             //

    if(strncmp(buffer,"GET",3)==0)//busco una peticion GET
    {
      char *aux = &buffer[4];//auxiliar me sirve para obtener los caracteres a leer
      if(strncmp(aux,"/static",7)==0)//para los archivos css y javascript
      {
        route=malloc(strlen(__dirname));//asigno la memoria a la ruta para guardar __dirname
        strcpy(route,__dirname);//copio __dirname
        size=strlen(__dirname);//actualizo el tamano
      }

      for(int i=0;*(aux+i)!='H';i++)// si llego a la H es que ya tome la infomacion que me interesa
      {
        if(strncmp(aux+i," H",2)==0)//si llega al final de la peticion agrega un caracter nulo y termina
        {
        route=realloc(route,size+1);//espacio para un caracter mas
        *(route+size)='\0';//agrego el caracter nulo
          break;
        }

        if(*(aux+i)=='%')//para manejar caracteres raros
        {
                  route=realloc(route,size+1);//reservo mas memoria
                 if(strncmp(aux+i,"%20",3)==0)//los espacios en http se representan %20
                 {
                    i+=3;//corro 3 caracteres
                  *(route+size)=' ';//agrego el espacio
                 }

        size++;//actualizo el tamanno
        }

        if(size==0)
        {
         route=malloc(1);
        }else{
        route=realloc(route,size+1);
        }
        //guardo espacio para 1 caracter mas y le almaceno lo que esta en aux
        *(route+size)=*(aux+i);
        size++;//actualizo el tamanno
      }
    }
    memset(&buffer,0,BUFF_SIZE);//limpio el buffer

  if(size==1)//si el tamanno es 1 es porque se pidio "/" 
             //por tanto la ruta es la por defecto
  {
    free(route);//limpio la memoria
    route=malloc(strlen(default_dir));
    strcpy(route,default_dir);//almaceno la ruta por defecto
  }

  return route;
}

#endif
