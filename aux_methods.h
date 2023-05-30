#ifndef AUX_H
#define AUX_H
 

        DIR* dir = opendir(route);
        struct stat buff;
 
        if(!dir)
        {
          return NULL;
        }

        struct dirent* ent=readdir(dir);
         
        char* text,*contenido=fill_content("./plantilla_content.html",NULL,0);        
        int oldsize=0;

        while(ent==NULL)
        {
            if(ent->d_name[0]!='.'){

            int str_size=strlen(route)+strlen(ent->d_name)+1;
            char* direction=malloc(str_size);
            printf("%s\n",direction);

            stat(direction,&buff);

             char* size =size_to_str(buff.st_size);
             printf("size: %s\n",buff.st_size);

            // char* date = malloc(80);
            // memset(date,0,80);
            // time_t now =buff.st_mtime;
            // struct tm* ts=localtime(&now);
            // strftime(date,80,nl_langinfo(D_T_FMT),ts);          
                                        
            Tuple info[3];   
            //direction
            info[0]=new_t("Route",direction);          
//            //name
            info[1]=new_t("Name",ent->d_name);
//            //size
            info[2]=new_t("Size",size);
//            //date
            //info[3]=new_t("Date",date);
            
             //printf("%s\t%s\t%s\n",direction,ent->d_name,size);

            // free(date);
            free(size);
            free(direction);
            }
            ent=readdir(dir);
        }
          Tuple tp[2];
          tp[0] =new_t("directory_name",route);
          tp[1] =new_t("content",contenido);
          free(contenido);
         closedir(dir);
#endif
