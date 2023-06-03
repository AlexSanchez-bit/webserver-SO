#ifndef RC_H
#define RC_H

typedef struct rc_pointer //estructura para mantener los fd de los archivos
{
  char* file_name;
  int fd;
  int cantidad;
} RC;

//metodos

RC create_rc(char* file_name,int fd,int cantidad)
{    
  return (RC){file_name,fd,cantidad};//creo el reference count
}

int drop(RC* self)//limpiar el reference count
{
  if(self->cantidad ==0)
  {  
    close(self->fd);
  }else
  { 
    self->cantidad--;
  }
  return 1;
}


//metodo para los array de RC

int contains(RC* list,int size,char* search)//para saber si ya esta guardada
{
  if(size==0)
  {
    return -1;
  }else{
    for(int i=0;i<size;i++)
    {
       if(strcmp((list+i)->file_name,search)==0)
       {
        if((list+i)->cantidad==0)
        {
          return -1;
        }
         return i;
       }
    }
    return -1;
  }

}


int save_fd(RC** list ,int* size ,int fd,char* filename)//guardar una referencia
{
  int pos=0;
  if((*size)==0)//la lista esta vacia
  {   
    *size=1;
    *(list)=(RC*)malloc(sizeof(RC));
     RC save = create_rc(filename,fd,1);
    **list=save;
  }else if((pos =contains((*list),*size,filename))>=0)
  {
    (*(list+pos))->fd=fd;
    (*(list+pos))->cantidad++;   
  }
  else{  
  RC save = create_rc(filename,fd,1);   
    *list=(RC*)realloc(*list,sizeof(RC)*((*size)+1));
    *(*list+(*size))=save;
     *size=((*size)+1);
  }
  return 1;
}

#endif
