
#ifndef GENHTML_H
#define GENHTML_H
#include "header.h"
#include<string.h>


typedef struct tuple
{
 char* key;
 char* value;
} Tuple;

Tuple new_t(char* k,char* v)
{
   return (Tuple){k,v};
}

char* concat(char* original,char* nuevo);
char* generate_content(Tuple** elementos,int cantidad);
char* fill_content(char* template_name,Tuple* key_value_pairs,int);



char* generate_file(char* directorio,char** name,char** route,char** size,char** date,int cantidad)
{
   Tuple tupla = new_t("directory_name",directorio);

   Tuple* list = malloc(sizeof(Tuple)*2);
    list=&tupla;

    Tuple** elements=malloc(sizeof(Tuple*)*cantidad);

    for(int i=0;i<cantidad;i++){
      *(elements+i)=malloc(sizeof(Tuple)*4);
    }


    for(int i=0;i<cantidad;i++)
    {
   Tuple tp1 = new_t("Route",*(route+i));
   Tuple tp2 = new_t("Name",*(name+i));
   Tuple tp3 = new_t("Size",*(size+i));
   Tuple tp4 = new_t("Date",*(date+i));

      *(*(elements+i))=tp1;
      *(*(elements+i)+1)=tp2;
      *(*(elements+i)+2)=tp3;
      *(*(elements+i)+3)=tp4;
    }

    char* content_text = generate_content(elements,cantidad);

    Tuple content = new_t("content",content_text);
    *(list+1)=content;


    return fill_content("./plantilla.html",list,2);
}

char* read_file(char* direction)
{
   int file=open(direction,O_RDONLY); //abro el archivo en modo lectura 
    if (file==-1)
    {
      printf("error leyendo el archivo ");
      return NULL;
    }

    char buffer[1024];
    for(int i =0;i<1024;i++)
    {
     buffer[i]='\0';
    }

    read(file,&buffer,1024);

    close(file);

    char* ret_val = malloc(strlen(&buffer[0]));
    ret_val=strcpy(ret_val,&buffer[0]);       


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
   char* result="";
   while(token!=NULL)
   { 
    for(int i=0;i<tuple_size;i++)
    {
     char* key =(key_value_pairs+i)->key;
     if(!strcmp(token,key))
     {
       token=(key_value_pairs+i)->value;
     }
    }
   result=concat(result,token);
   token = strtok(NULL,"$");
   }
  return result;
  
}





char* concat(char* original,char* nuevo)
{
 char* new_value=malloc(strlen(original)+strlen(nuevo));
  *new_value='\0';
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
