#ifndef GETR_H
#define GETR_H
#include "header.h"
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <langinfo.h>
#include "generate.h"

char* size_to_str(int bytes);
int is_file(char* route_tofile);
int send_file(char* route_tofile,int);

char* __dirname;//variable con el directorio base


char* dirname()//devuelve el directorio donde se ejecuto el programa
{
     char wcd[1024];//working directory
     memset(wcd,0,1024);
     getcwd(wcd,1024);//ruta desde donde se llamo la shell
     char* ret_val = malloc(strlen(&wcd[0]));
     ret_val=strcpy(ret_val,&wcd[0]);
     return ret_val;
}

char* get_info(char* route,int clientfd)//envia la informacion del directorio al cliente
{         

        DIR* dir = opendir(route);//abre el directorio
        struct stat buff;//buffer para ver la informacion
        char* header="HTTP/1.1 %d OK\r\n\r\n %s \r\n\r\n";//cabecera http
        if(!dir)//si no es un directorio
        {
          if(send_file(route,clientfd)>0)//si es un archivo lo envia
          {
            return NULL;
          }
          char * file = fill_content("./error_page.html",NULL,0);//si no devuelve una pagina de error
          int size=strlen(header)+strlen(file)+3;
          char * resp=malloc(size);
          sprintf(resp,header,404,file);
          write(clientfd,resp,size);
          free(resp);
          free(file);
          return NULL;
        }


        char* resp = malloc(strlen(header));
          sprintf(resp,header,200,"");
          write(clientfd,resp,strlen(resp));//envia la cabecera con status 200

        
          char* template_ = fill_content("./plantilla.html",NULL,0);            
          char* ret = malloc(strlen(template_)+strlen(route)+4);
          sprintf(ret,template_,route,"$");//crea el htmp y el $ es para dividirlo a la mitad

          char* token = strtok(ret,"$");//lo divido 
          free(template_);//limpio la memoria

          write(clientfd,token,strlen(token));//envio la mitad del html
          token=strtok(NULL,"$");

          struct dirent* ent=readdir(dir); //leee las propiedades del directorio     

        while(ent!=NULL)//minetras haya cosas que leer
        {
            if(ent->d_name[0]!='.'){//si no es el mismo directorio (quitar . y .. , se ve feo) 
               

            int str_size=strlen(route)+strlen(ent->d_name)+2;
            char* direction=malloc(str_size);
            snprintf(direction,str_size,"%s/%s",route,ent->d_name);//guardo la ruta al archivo/directorio
            
            //stat lee la informacion relevante del archivo (tamano fecha etc)
            stat(direction,&buff);
              
            //guardo el tamano
             char* size =size_to_str(buff.st_size);
             
             //guardo la fecha como string
             char* date = malloc(80);
             memset(date,0,80);
             time_t now =buff.st_mtime;
             struct tm* ts=localtime(&buff.st_mtime);
             strftime(date,80,nl_langinfo(D_T_FMT),ts);         

             //leo la plantilla de los elementos de la tabla
             //y guardo el nombre direccion etc 
             char* tmp = fill_content("./plantilla_content.html",NULL,0);            
             int tmp_size=strlen(tmp)+strlen(ent->d_name)+strlen(date)+strlen(size)+strlen(direction);
             char* tmp_ = malloc(tmp_size);
             snprintf(tmp_,tmp_size,tmp,direction,ent->d_name,date,size);
              
             //escribo la informacion (html) al cliente
              write(clientfd,tmp_,strlen(tmp_));
             //limpio la memoria 
             free(tmp);
             free(tmp_);
             free(date);
             free(size);
             free(direction);
            }
            ent=readdir(dir);
        }        
        //escribo la otra mitad del html (el final)
         write(clientfd,token,strlen(token));
          free(ret);
          free(dir);

        return ret;
}


int is_file(char* route_tofile)//para verificar si es un archivo
{
  int file = open(route_tofile,O_RDONLY);
  close(file);
  return file;
}

int send_file(char* route_tofile,int cfd)//enviar archivo al cliente
{
  int file = open(route_tofile,O_RDONLY);//abro el archivo en lectura
  if(file==-1)
  {
    return -1;
  }
  char* header="HTTP/1.1 200  \r\n\r\n  \r\n\r\n";//cabecera para enviar
  write(cfd,header,strlen(header));//envio la cabecera
  char buff[100];//buffer de lectura/escritura
  memset(buff,0,100);//limpio el buffer
  int size=0;

  while((size=read(file,&buff,100))>0)//minetras lea algo
  {
    write(cfd,&buff,size);//esribo lo que lei al cliente (100bytes)
    memset(buff,0,100);//limpio el buffer
  }
  close(file);//cierro el archivo
  return file;
}


char* size_to_str(int bytes)//transforma un entero con la cantidad de bytes en un string con 
                            //bytes mb kb gb etc
{
  char* ret_value=malloc(20);
  memset(ret_value,0,20);
  if(bytes<1024)
  {
    sprintf(ret_value,"%dB\0",bytes); 
    return ret_value;
  }
 
  int kb = bytes/1024;
  if(kb<1024)
  {
    sprintf(ret_value,"%dKB",kb); 
    return ret_value;
  }
  
  int mb = kb/1024;
  if(mb<1024)
  {
    sprintf(ret_value,"%dMB",mb); 
    return ret_value;
  }
  int gb = mb/1024;
    sprintf(ret_value,"%dGB",gb);     
    return ret_value;
}


#endif // !DEBUG
