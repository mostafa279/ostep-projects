#include <stdio.h>
#include <stdlib.h> 
#include<string.h>
void writeOut(char  buffer , int count){
fwrite(&count,sizeof(int),1,stdout);
fwrite(&buffer,sizeof(char),1,stdout);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    char oldChar = 0;
    char newChar;
    int count = 0;

    for(int i=1;i<argc;i++){
    FILE *fp=fopen(argv[i],"r");
     if(fp == NULL){
        printf("wzip: cannot open file\n");
        continue;
     }
     while(fread(&newChar,1,1,fp)){
     if(count ==0){
        oldChar=newChar;
        count=1;
      }
      else if (oldChar==newChar){
         count++; 
        }
        else{
        writeOut(oldChar , count);
        oldChar=newChar;
        count=1;  
        }
      }
      fclose(fp);
    }
    if (count > 0)
        writeOut(oldChar, count);

    return 0;
}
/*
2-types of command .
1-wzip and file -> read the file and compressed it .. input use fwrite .. out (stdout)
2-wzip file1 > file2 ->
*/