#include <stdio.h>
#include <stdlib.h> 
#include<string.h>
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

     char* buffer=NULL;
     size_t bufsize=0;
     char* search = argv[1];
    //  There is no file to read .. the grep works on the input  
    if(argc == 2){
          while(getline(&buffer, &bufsize, stdin) != -1){
            if(strstr(buffer, search) != NULL)
                printf("%s", buffer);
        }
        free(buffer);
    }
    else{
    for(int i=2;i<argc;i++){
     FILE *fp=fopen(argv[i],"r");
     if(fp == NULL){
        printf("wgrep: cannot open file\n");
        exit(1);
     }

       
     while(getline(&buffer,&bufsize,fp)!=-1){
        if(strstr(buffer,search) !=NULL)
        printf("%s",buffer);
     }
     fclose(fp);
     free(buffer); 
    }
}                                               
    return 0;
}