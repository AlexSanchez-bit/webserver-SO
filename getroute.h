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


char* dirname()
{
     char wcd[1024];//working directory
     getcwd(wcd,1024);//ruta desde donde se llamo la shell
     char* ret_val = malloc(strlen(&wcd[0]));
     ret_val=strcpy(ret_val,&wcd[0]);
     return ret_val;
}

char* get_info(char* route)
{         
        char** names;
        char** dates;
        char** directions;
        char** sizes;

         names=malloc(sizeof(char*));
         dates=malloc(sizeof(char*));
         directions=malloc(sizeof(char*));
         sizes=malloc(sizeof(char*));

        DIR* dir = opendir(route);
        struct stat buff;
 
        if(!dir)
        {
          return NULL;
        }

        struct dirent* ent=readdir(dir);
         
        int cantidad=0;

        while(ent!=NULL)
        {
            if(ent->d_name[0]!='.'){
            names=realloc(names,sizeof(char*)*(cantidad+1));
            dates=realloc(dates,sizeof(char*)*(cantidad+1));
            sizes=realloc(sizes,sizeof(char*)*(cantidad+1));
            directions=realloc(directions,sizeof(char*)*(cantidad+1));

            char* direction=malloc(strlen(route)+strlen(ent->d_name)+1);
            sprintf(direction,"%s/%s",route,ent->d_name);

            stat(direction,&buff);

            char* size =size_to_str(buff.st_size);

            char* date = malloc(80);
            time_t now =buff.st_mtime;
            struct tm* ts=localtime(&now);
            strftime(date,80,nl_langinfo(D_T_FMT),ts);          
                                        
               
            //direction
            *(directions+cantidad)=malloc(strlen(direction));
            *(directions+cantidad)=strcpy(*(directions+cantidad),direction);
            //name
            *(names+cantidad)=malloc(strlen(&ent->d_name[0]));
            *(names+cantidad)=strcpy(*(names+cantidad),&ent->d_name[0]);
            //size
            *(sizes+cantidad)=malloc(strlen(size));            
            *(sizes+cantidad)=strcpy(*(sizes+cantidad),size);
            //date
            *(dates+cantidad)=malloc(strlen(date));            
            *(dates+cantidad)=strcpy(*(dates+cantidad),date);

            cantidad++;
            }
            ent=readdir(dir);
        }
        char* text=generate_file(route,names,directions,sizes,dates,cantidad);
        return text;

}


char* size_to_str(int bytes)
{
  char* ret_value=malloc(6);
  if(bytes<1024)
  {
    sprintf(ret_value,"%dB ",bytes); 
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
