#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int line,pos;
char command[50],dir[100],tmp[100],str[1000],buff_str[1000];


void createfile(){
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
    memset(tmp,0,sizeof(tmp));
    fclose(fp);
}
//insertstr
void insertstr(){
    FILE *fp,*tmpf;
    fp = fopen(dir,"r");
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    // fputs(str,tmpf);
    bool keep_reading = true;
    int current_line = 1;
    while (keep_reading){
        fgets(buff_str,1000,fp);
        if(feof(fp)) keep_reading = false;
        // printf("%d",keep_reading);
        if(line == current_line){
            for(int i =0;i<strlen(buff_str);i++){
                if(i==pos)
                    fputs(str,tmpf);
                fputc(buff_str[i],tmpf);
            }
            if(strlen(buff_str) == pos)
                fputs(str,tmpf);
            // fprintf(tmpf,"chert\nchert");
            // printf("aa\n");
            // printf("%s",str);
            // printf("%s",buff_str);
        }else{
        fputs(buff_str,tmpf);
        }
        current_line++;
    }
    if(current_line == line){
        fputc('\n',tmpf);
        fputs(str,tmpf);
    }
    fclose(tmpf);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));
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
        createfile();
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
            bool not_found = true;
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        c = getchar();
        if(c!='"'){
            str[0] = c;
            int t= 1;
            while ((c=getchar()) != ' ')
            {
                str[t] = c;
                if(str[t] == 'n' && str[t-1]=='\\' && str[t-2]!='\\'){
                    str[t-1] = '\n';
                    str[t] = '\0';
                    t--;
                }
                if(str[t] == 'n' && str[t-1]=='\\' && str[t-2]=='\\'){
                    str[t-1] = 'n';
                    str[t] = '\0';
                    t--;
                }
                if(str[t] == '"' && str[t-1] == '\\'){
                    str[t-1] = '"';
                    str[t] = '\0';
                    t--;
                }
                t++;
            }
        }
        else{
            int t =0;
            // c = getchar();
            while ((c=getchar()) != '"' || str[t-1] == '\\'){
                str[t]=c;
                if(str[t] == 'n' && str[t-1]=='\\' && str[t-2]!='\\'){
                    str[t-1] = '\n';
                    str[t] = '\0';
                    t--;
                }
                if(str[t] == 'n' && str[t-1]=='\\' && str[t-2]=='\\'){
                    str[t-1] = 'n';
                    str[t] = '\0';
                    t--;
                }
                if(str[t] == '"' && str[t-1] == '\\'){
                    str[t-1] = '"';
                    str[t] = '\0';
                    t--;
                }
                t++;
            }
            str[t] = '\0';
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        // printf("%s",str);
        insertstr();
        memset(str,0,sizeof(str));
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