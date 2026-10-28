#include <stdio.h>
#include <stdlib.h> 
#include<string.h>

int read_4_byte_binary(char* compressed){
int current = compressed[3];
for(int i=2;i>=0;i--){
    current = (current << 8)|compressed[i];
}
return current;
}
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("wunzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }
    for(int i=1;i<argc;i++){
    FILE *fp=fopen(argv[i],"r");
     if(fp == NULL){
        printf("wunzip: cannot open file\n");
        exit(EXIT_FAILURE);
     }
     char compressed[5];
      while(fread(&compressed,5,1,fp)){
      int current=read_4_byte_binary(compressed);
      char c=compressed[4];
      for(int i=0;i<current;i++)
      fwrite(&c,1,1,stdout);   
      }
      fclose(fp);
 }
    return 0;
}
