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
char* __dirname;


char* dirname()
{
     char wcd[1024];//working directory
     getcwd(wcd,1024);//ruta desde donde se llamo la shell
     char* ret_val = malloc(strlen(&wcd[0]));
     ret_val=strcpy(ret_val,&wcd[0]);
     return ret_val;
}

char* get_info(char* route,int clientfd)
{         
        printf("%s\n",route);
        DIR* dir = opendir(route);
        struct stat buff;
  char* header="HTTP/1.1 %d OK\r\n\r\n %s \r\n\r\n";
        if(!dir)
        {
          char * file = fill_content("./error_page.html",NULL,0);
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
          write(clientfd,resp,strlen(resp));

        
          char* template_ = fill_content("./plantilla.html",NULL,0);            
          char* ret = malloc(strlen(template_)+strlen(route)+4);
          sprintf(ret,template_,route,"$");

          char* token = strtok(ret,"$");

          free(template_);

          write(clientfd,token,strlen(token));
          token=strtok(NULL,"$");

          struct dirent* ent=readdir(dir);      

        while(ent!=NULL)
        {
            if(ent->d_name[0]!='.'){
               

            int str_size=strlen(route)+strlen(ent->d_name)+1;
            char* direction=malloc(str_size);
            sprintf(direction,"%s/%s",route,ent->d_name);

            stat(direction,&buff);

             char* size =size_to_str(buff.st_size);

             char* date = malloc(80);
             memset(date,0,80);
             time_t now =buff.st_mtime;
             struct tm* ts=localtime(&buff.st_mtime);
             strftime(date,80,nl_langinfo(D_T_FMT),ts);          


             char* tmp = fill_content("./plantilla_content.html",NULL,0);            
             char* tmp_ = malloc(strlen(tmp)+strlen(ent->d_name)+strlen(date)+strlen(size)+strlen(direction));
             sprintf(tmp_,tmp,direction,ent->d_name,date,size);

              write(clientfd,tmp_,strlen(tmp_));
              
             free(tmp);
             free(tmp_);
             free(date);
             free(size);
             free(direction);
            }
            ent=readdir(dir);
        }        

         write(clientfd,token,strlen(token));
          free(ret);
          free(dir);

        return ret;
}


char* size_to_str(int bytes)
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
