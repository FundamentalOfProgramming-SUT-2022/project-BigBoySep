#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char command[50],dir[100],tmp[100];

void empty(char str[]){
    for(int i =0;i<100;i++)
        str[i] = '\0';
}

void create_file(){
    for(int i =0;i<strlen(dir);i++){
        // printf("%s\n",tmp);
        if(dir[i] == '/'){
            // strncpy(tmp,dir,i);
            // printf("%s\n",tmp);
            // printf("dir:%s\n",dir);
            mkdir(tmp);
        }
        tmp[i] = dir[i];
    }
    FILE *fp;
    fp = fopen(tmp,"w");
    empty(tmp);
    fclose(fp);
}

void input(){
    if(strcmp(command,"createfile") == 0){
        scanf("%s",command);
        char c = getchar();
        c = getchar();
        if(c=='/')
            scanf("%s",dir);
        else{
            int t =0;
            c = getchar();
            while ((c=getchar()) != '"'){
                dir[t]=c;
                t++;
            }
            dir[t] = '\0';
        }
        // printf("%s\n",dir);
        create_file();
        return;
    }else{
        printf("invalid command\n");
        gets(command);
        return;
    }
}


int main(){

    scanf("%s",command);
    while (strcmp(command,"exit") != 0){
        input();
        scanf("%s",command);
    }
}
