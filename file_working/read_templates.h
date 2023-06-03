#ifndef RTEMPLATE_H
#define RTEMPLATE_H
#include<string.h>
#include <pthread.h>

#define BUFFSIZE 2048

pthread_mutex_t filemutex;

void init_files_mutex()
{
     pthread_mutex_init(&filemutex,NULL);
}

char* read_file(char* direction)
{
  pthread_mutex_lock(&filemutex);
   int file=open(direction,O_RDONLY); //abro el archivo en modo lectura 
    if (file==-1)
    {
      printf("error leyendo el archivo ");
      return NULL;
    }

    char buffer[BUFFSIZE];
    memset(buffer,0,BUFFSIZE);

    int readed = read(file,&buffer,BUFFSIZE);
    close(file);
    pthread_mutex_unlock(&filemutex);

    if(readed<=0)
    {
     printf("error leyendo%s\n",direction);
     return NULL;
    }


    char* ret_val = malloc(readed);
    ret_val=strcpy(ret_val,&buffer[0]);       
    return ret_val;

}


#endif
