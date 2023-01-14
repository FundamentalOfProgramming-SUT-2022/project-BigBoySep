#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int line,pos;
char command[50],dir[100],tmp[100],str[1000],buff_str[1000];

void empty(char str[]){
    for(int i =0;i<strlen(str);i++)
        str[i] = '\0';
}

void create_file(){
    if (access(dir, F_OK) == 0) {
        // file exists
        printf("File exists\n");
        return;
    }
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
    }else if(strcmp(command,"insertstr") == 0){
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
        if (access(dir, F_OK) == 0) {
        }else{
            printf("file not found\n");
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        c = getchar();
        if(c!='"'){
            str[0] = c;
            scanf("%s",buff_str);
            strcat(str,buff_str);
        }
        else{
            int t =0;
            c = getchar();
            while ((c=getchar()) != '"' || str[t-1] == '\\'){
                str[t]=c;
                t++;
            }
            str[t] = '\0';
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
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