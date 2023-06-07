#include "read_templates.h"
#include "send_file.h"
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <langinfo.h>
#ifndef HWORKING_H
#define HWORKING_H

char* size_to_str(int bytes);

void send_html(char*route ,int clientfd )
{
        DIR* dir = opendir(route);//abre el directorio
        char* header="HTTP/1.1 %d OK\r\n Content-Type: text/html \r\n\r\n\r\n %s";//cabecera http
        if(!dir)//si no es un directorio
        {
          if(send_file(route,clientfd)>0)//si es un archivo lo envia
          {
            return;
          }
          char * file = read_file("./templates/error_page.html");//si no devuelve una pagina de error
          int size=strlen(header)+strlen(file)+3;
          char * resp=malloc(size);
          sprintf(resp,header,404,file);
          send(clientfd,resp, strlen(resp), 0);//envio la cabecera
          free(resp);
          free(file);          
          return;
        }
   

        struct stat buff;//buffer para ver la informacion
        
          char* template_ = read_file("./templates/plantilla.html");            
          char* ret = malloc(strlen(template_)+strlen(route));

          sprintf(ret,template_,route,"$");//crea el htmp y el $ es para dividirlo a la mitad
                                           //
          char* token = strtok(ret,"$");//lo divido 
                                        //
        char* resp = malloc(strlen(header)+strlen(token)+10);
         sprintf(resp,header,200,token);

          send(clientfd,resp, strlen(resp), 0);//envio la mitad del html

          token=strtok(NULL,"$");
         
          free(template_);
          struct dirent* ent=readdir(dir); //leee las propiedades del directorio 

        while(ent!=NULL)//minetras haya cosas que leer
        {
            if(ent->d_name[0]!='.'){//si no es el mismo directorio (quitar . y .. , se ve feo) 
               

            int str_size=strlen(route)+strlen(ent->d_name)+2;
            char* direction=malloc(str_size);
            if(*(route+strlen(route)-1)=='/')
            {
            snprintf(direction,str_size,"%s%s",route,ent->d_name);
            }else{
            snprintf(direction,str_size,"%s/%s",route,ent->d_name);//guardo la ruta al archivo/directorio
            }
            
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
             char* tmp = read_file("./templates/plantilla_content.html");            
             int tmp_size=strlen(tmp)+strlen(ent->d_name)+strlen(date)+strlen(size)+strlen(direction);
             char* tmp_ = malloc(tmp_size);
             snprintf(tmp_,tmp_size,tmp,direction,ent->d_name,date,size);
              
             //escribo la informacion (html) al cliente
              send(clientfd,tmp_, strlen(tmp_), 0);     //envio la mitad del html
             //limpio la memoria 
             free(tmp);
             free(tmp_);
             free(date);
             free(size);
             free(direction);
            }
            ent=readdir(dir);
        }        
              closedir(dir);
        //escribo la otra mitad del html (el final)
                                        //
          send(clientfd,token, strlen(token), 0);//envio la mitad del html
}

char* size_to_str(int bytes)//transforma un entero con la cantidad de bytes en un string con 
                            //bytes mb kb gb etc
{
  char* ret_value=malloc(20);
  memset(ret_value,0,20);
  if(bytes<1024)
  {
    sprintf(ret_value,"%dB",bytes); 
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

#endif
