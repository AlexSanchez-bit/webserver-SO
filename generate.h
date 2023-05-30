
#ifndef GENHTML_H
#define GENHTML_H
#include "header.h"
#include<string.h>
#include <pthread.h>


pthread_mutex_t filemutex;

typedef struct tuple
{
 char* key;
 char* value;
} Tuple;

void init_files_mutex()
{
     pthread_mutex_init(&filemutex,NULL);
}

Tuple new_t(char* k,char* v)
{
 char *key,*value;

 key = malloc(strlen(k));
 value = malloc(strlen(v));
 strcpy(key,k);
 strcpy(value,v);
 return (Tuple){key,value};
}

char* concat(char* original,char* nuevo);
char* generate_content(Tuple** elementos,int cantidad);
char* fill_content(char* template_name,Tuple* key_value_pairs,int);




char* read_file(char* direction)
{
  pthread_mutex_lock(&filemutex);
   int file=open(direction,O_RDONLY); //abro el archivo en modo lectura 
    if (file==-1)
    {
      printf("error leyendo el archivo ");
      return NULL;
    }

    char buffer[200];
    memset(buffer,0,200);

    int readed = read(file,&buffer,200);
    close(file);
    pthread_mutex_unlock(&filemutex);

    char* ret_val = malloc(readed);
    ret_val=strcpy(ret_val,buffer);       


    return ret_val;

}


char* fill_content(char* template_name,Tuple* key_value_pairs,int tuple_size)
{
   char* file = read_file(template_name);
   if(file==NULL)
   {
     printf("archivo no encontrado\n");
     return NULL;
   }

   char* token = strtok(file,"$");
   char* result=malloc(1);   
   memset(result,0,1);
   int oldsize=1;
   while(token!=NULL)
   { 
    for(int i=0;i<tuple_size;i++)
    {
     char* key =(key_value_pairs+i)->key;
     if(!strcmp(token,key))
     {
      token=malloc(strlen((key_value_pairs+i)->value));
      strcpy(token,(key_value_pairs+i)->value);
      free((key_value_pairs+i)->value);
       break;
     }
    }

     oldsize=strlen(token)+oldsize;   
     result=realloc(result,oldsize);
     strcat(result,token);
     token = strtok(NULL,"$");
     
   }
   free(file);
  return result;
  
}





char* concat(char* original,char* nuevo)
{
 int size =strlen(original)+strlen(nuevo);
 printf("left: %s right: %s newsize %d\n",original,nuevo,size);
 char* new_value=malloc(size);
 memset(new_value,0,size);
  new_value = strcat(new_value,original);
  new_value = strcat(new_value,nuevo);  
  return new_value;
}

char* generate_content(Tuple** elementos,int cantidad)
{
 char* ret_val ="";
 for(int i=0;i<cantidad;i++)
 {
   ret_val=concat(ret_val,fill_content("./plantilla_content.html",*(elementos+i),4));
 }
 return ret_val;
}

#endif // !GENHTML_H
