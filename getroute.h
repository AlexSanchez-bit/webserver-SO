#ifndef GETR_H
#define GETR_H
#include "header.h"
#include "getroute.h"

char* route_info(char* route);

char* dirname()
{
     char wcd[1024];//working directory
     getcwd(wcd,1024);//ruta desde donde se llamo la shell
     char* ret_val = malloc(strlen(&wcd[0]));
     ret_val=strcpy(ret_val,&wcd[0]);
     return ret_val;
}

char* route_info(char* route){
  const char* command = "ls -l > route.txt  ";
  char* real_command = malloc(strlen(route)+strlen(command));
  real_command=strcat(real_command,command);
  real_command=strcat(real_command,route);
  system(real_command);   
   return  read_file("./route.txt");
}


char** separate_lines(char* directory_info)
{
  
}




#endif // !DEBUG
