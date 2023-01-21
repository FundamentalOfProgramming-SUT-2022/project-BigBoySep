#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

bool forward = false;
int line,pos,size;
char command[50],dir[100],tmp[100],str[1000],str2[1000],buff_str[1000],clipboard[1000],linecounter=0;


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
void insertstr(const char *ins){
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
                    fputs(ins,tmpf);
                fputc(buff_str[i],tmpf);
            }
            if(strlen(buff_str) == pos)
                fputs(ins,tmpf);
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
        fputs(ins,tmpf);
    }
    fclose(tmpf);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));
}

void cat(){
    if (access(dir, F_OK) != 0) {
        // file not exists
        printf("File doesn't exist\n");
        return;
    }
    FILE *fp;
    fp = fopen(dir,"r");
    char c = fgetc(fp);
    while (c!=EOF){
        printf("%c",c);
        c = fgetc(fp);
    }
    printf("\n");
    fclose(fp);
}

void removestr(bool fward){
    FILE *fp,*tmpf;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    fp = fopen(dir,"r");
    int t=0,p=0,l=1,ts;
    // 12345678910
    if(p==pos && l==line) ts=t;
    char c = fgetc(fp);
    t++;
    p++;
    if(c=='\n') l++;
    while (c!=EOF){
        if(c=='\n'){ 
            l++;
            p=0;
        }
        if(p==pos && l==line) ts=t;
        c = fgetc(fp);
        t++;
        p++;
    }
    // 1235678910
    fseek(fp,0,SEEK_SET);
    rewind(fp);
    c = fgetc(fp);
    t=1;
    while (c!=EOF){
        if(fward){
            if(!(t>ts && t<=ts+size)){
                fputc(c,tmpf);
            }
        }
        if(!fward){
            if(!(t<=ts && t>ts-size)){
                fputc(c,tmpf);
            }
        }
        c = fgetc(fp);
        t++;
    }
    fclose(tmpf);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));
}

void cutstr(bool fward){
    FILE *fp,*tmpf;
    memset(clipboard,0,sizeof(clipboard));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    fp = fopen(dir,"r");
    int t=0,p=0,l=1,ts;
    // 12345678910
    if(p==pos && l==line) ts=t;
    char c = fgetc(fp);
    t++;
    p++;
    if(c=='\n') l++;
    while (c!=EOF){
        if(c=='\n'){ 
            l++;
            p=0;
        }
        if(p==pos && l==line) ts=t;
        c = fgetc(fp);
        t++;
        p++;
    }
    // 1235678910
    fseek(fp,0,SEEK_SET);
    rewind(fp);
    c = fgetc(fp);
    t=1;
    int counter = 0;
    while (c!=EOF){
        if(fward){
            if(!(t>ts && t<=ts+size)){
                fputc(c,tmpf);
            }else{
                clipboard[counter] = c;
                counter++;
            }
        }
        if(!fward){
            if(!(t<=ts && t>ts-size)){
                fputc(c,tmpf);
            }else{
                clipboard[counter] = c;
                counter++;
            }
        }
        c = fgetc(fp);
        t++;
    }
    fclose(tmpf);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));
}

void copystr(bool fward){
    memset(clipboard,0,sizeof(clipboard));
    FILE *fp;
    fp = fopen(dir,"r");
    int t=0,p=0,l=1,ts;
    // 12345678910
    if(p==pos && l==line) ts=t;
    char c = fgetc(fp);
    t++;
    p++;
    if(c=='\n') l++;
    while (c!=EOF){
        if(c=='\n'){ 
            l++;
            p=0;
        }
        if(p==pos && l==line) ts=t;
        c = fgetc(fp);
        t++;
        p++;
    }
    // 1235678910
    fseek(fp,0,SEEK_SET);
    rewind(fp);
    c = fgetc(fp);
    t=1;
    int counter=0;
    while (c!=EOF){
        if(fward){
            if((t>ts && t<=ts+size)){
                clipboard[counter] = c;
                counter++;
            }
        }
        if(!fward){
            if((t<=ts && t>ts-size)){
                clipboard[counter] = c;
                counter++;
            }
        }
        c = fgetc(fp);
        t++;
    }
    fclose(fp);
    memset(tmp,0,sizeof(tmp));
}
// 
void find(int state,int n){

    FILE *fp = fopen(dir,"r");
    int len = strlen(str),i=0,pos_bychar[100],pos_byword[100],cnt=0,t=0,wcnt=1,ti,wi;
    char c = fgetc(fp);
    bool found = false;
    t++;
    while (c!=EOF){
        while (c!=EOF && c==str[i] && ++i<len){
            c=fgetc(fp);
            if(i==1){
                wi = wcnt;
                ti = t;
            }
            if(c==' ' || c=='\n') wcnt++;
            t++;
        }
        if(i==len){
            pos_bychar[cnt] = (ti-1)*(len!=1) + (t-1)*(len==1);
            pos_byword[cnt] = wi*(len!=1) + (wcnt)*(len==1);
            cnt++;
            found = true;
        }
        i=0;
        c=fgetc(fp);
        if(c==' ' || c=='\n') wcnt++;
        t++;
    }
    fclose(fp);
    // all byword at cnt
    // *1100-0001-*1000-0110-0010-*0100-*0000
    switch (state)
    {
    case 0:
        if(found) printf("%d\n",pos_bychar[0]);
        else printf("-1\n");
        break;
    case 1000:
        if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0) printf(", %d",pos_bychar[i]);
                else printf("%d",pos_bychar[i]);
            }
            printf("\n");
        }else printf("-1");
        break;
    case 100:
        if(found) printf("%d\n",pos_byword[0]);
        else printf("-1\n");
        break;
    case 1100:
        if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0) printf(", %d",pos_byword[i]);
                else printf("%d",pos_byword[i]);
            }
            printf("\n");
        }else printf("-1");
        break; 
    case 1:
        if(found)printf("%d\n",cnt);
        else printf("0\n");
        break;
    case 10:
        if(found && n<=cnt) printf("%d\n",pos_bychar[n-1]);
        else printf("-1\n");
        break;
    case 110:   
        if(found && n<=cnt) printf("%d\n",pos_byword[n-1]);
        else printf("-1\n");
        break;
    default:
        break;
    }
}

void replace(int state,int n){

    FILE *fp = fopen(dir,"r");
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    FILE *ftmp = fopen(tmp,"w");
    int len = strlen(str),i=0,pos_bychar[100],pos_byword[100],cnt=0,t=0,wcnt=1,ti,wi;
    char c = fgetc(fp);
    bool found = false;
    t++;
    while (c!=EOF){
        while (c!=EOF && c==str[i] && ++i<len){
            c=fgetc(fp);
            if(i==1){
                wi = wcnt;
                ti = t;
            }
            if(c==' ' || c=='\n') wcnt++;
            t++;
        }
        if(i==len){
            pos_bychar[cnt] = (ti-1)*(len!=1) + (t-1)*(len==1);
            pos_byword[cnt] = wi*(len!=1) + (wcnt)*(len==1);
            cnt++;
            found = true;
        }
        i=0;
        c=fgetc(fp);
        if(c==' ' || c=='\n') wcnt++;
        t++;
    }
    rewind(fp);
    switch (state){
    case 0:
        if(found){
            i=0;
            while ((c=fgetc(fp))!=EOF){
                if(i==pos_bychar[0]) fputs(str2,ftmp);
                if(!(i>=pos_bychar[0] && i<pos_bychar[0]+len)) fputc(c,ftmp);  
                i++;  
            }
        }else printf("-1\n");
        break;
    case 1:
        if(found){
            i=0;
            while ((c=fgetc(fp))!=EOF){
                if(i==pos_bychar[n-1]) fputs(str2,ftmp);
                if(!(i>=pos_bychar[n-1] && i<pos_bychar[n-1]+len)) fputc(c,ftmp);  
                i++;  
            }
        }else printf("-1\n");
        break;
    case 10:
        t=0;
        if(found){
            i=0;
            while ((c=fgetc(fp))!=EOF){
                if(i==pos_bychar[t]+len) t++;
                if(i==pos_bychar[t]) fputs(str2,ftmp);
                if(!(i>=pos_bychar[t] && i<pos_bychar[t]+len)) fputc(c,ftmp);  
                i++;  
            }
        }else printf("-1\n");
        break;
        
        break;
    default:
        break;
    }
    fclose(ftmp);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));
    // all byword at cnt
    // *1100-0001-*1000-0110-0010-*0100-*0000
}

void grep(int state){
    FILE *fp = fopen(dir,"r");
    bool isvalid = false;
    while (!feof(fp)){
        fgets(buff_str,1000,fp);
        if(strstr(buff_str,str)!=NULL){
            linecounter++;
            isvalid = true;
            if(state == 1)
                printf("%s: %s",dir,buff_str);
            if(buff_str[strlen(buff_str)-1]!='\n') printf("\n");
        }
    }
    if(state == 3)
        printf("%s\n",dir);
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
        createfile();
        return;
    }else if(strcmp(command,"insertstr") == 0){
        scanf("%s",command);
        bool not_found = false;
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
            not_found = true;
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
        if(!not_found) insertstr(str);
        memset(str,0,sizeof(str));
        return;
        
    }else if(strcmp(command,"cat") == 0){
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
        cat();
        return;
    }else if(strcmp(command,"removestr") == 0){
        scanf("%s",command);
        bool not_found = false;
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
            not_found = true;
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        c = getchar();
        scanf("%s",command);
        scanf("%d",&size);
        // c = getchar();
        scanf("%s",command);
        if(command[1] == 'f') forward = true;
        else forward = false;
        removestr(forward);
        return; 
    }else if(strcmp(command,"copystr")==0){
        scanf("%s",command);
        bool not_found = false;
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
            not_found = true;
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        c = getchar();
        scanf("%s",command);
        scanf("%d",&size);
        // c = getchar();
        scanf("%s",command);
        if(command[1] == 'f') forward = true;
        else forward = false;
        copystr(forward);
        return; 
    }else if(strcmp(command,"cutstr")==0){
        scanf("%s",command);
        bool not_found = false;
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
            not_found = true;
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        c = getchar();
        scanf("%s",command);
        scanf("%d",&size);
        // c = getchar();
        scanf("%s",command);
        if(command[1] == 'f') forward = true;
        else forward = false;
        cutstr(forward);
        return;
    }else if(strcmp(command,"pastestr")==0){
        scanf("%s",command);
        bool not_found = false;
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
            not_found = true;
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        if(!not_found) insertstr(clipboard);
        return;
    }   
    else if(strcmp(command,"replace")==0){
        scanf("%s",command);
        char c = getchar();
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
        scanf("%s",command);
        c = getchar();
        c = getchar();
        if(c!='"'){
            str2[0] = c;
            int t= 1;
            while ((c=getchar()) != ' ')
            {
                str2[t] = c;
                if(str2[t] == 'n' && str2[t-1]=='\\' && str2[t-2]!='\\'){
                    str2[t-1] = '\n';
                    str2[t] = '\0';
                    t--;
                }
                if(str2[t] == 'n' && str2[t-1]=='\\' && str2[t-2]=='\\'){
                    str2[t-1] = 'n';
                    str2[t] = '\0';
                    t--;
                }
                if(str2[t] == '"' && str2[t-1] == '\\'){
                    str2[t-1] = '"';
                    str2[t] = '\0';
                    t--;
                }
                t++;
            }
        }
        else{
            int t =0;
            // c = getchar();
            while ((c=getchar()) != '"' || str2[t-1] == '\\'){
                str2[t]=c;
                if(str2[t] == 'n' && str2[t-1]=='\\' && str2[t-2]!='\\'){
                    str2[t-1] = '\n';
                    str2[t] = '\0';
                    t--;
                }
                if(str2[t] == 'n' && str2[t-1]=='\\' && str2[t-2]=='\\'){
                    str2[t-1] = 'n';
                    str2[t] = '\0';
                    t--;
                }
                if(str2[t] == '"' && str2[t-1] == '\\'){
                    str2[t-1] = '"';
                    str2[t] = '\0';
                    t--;
                }
                t++;
            }
            str2[t] = '\0';
        }
        c = getchar();
        scanf("%s",command);
        bool not_found = false;
        c = getchar();
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
            not_found = true;
        }
        fgets(command,50,stdin);
        int state=0,n=0;
        // all at
        for(int i =0;i<strlen(command);i++){
            if(command[i] == '-'){
                if(command[i+2] == 't')
                    state+=1;
                if(command[i+2] == 'l')
                    state+=10;
            }
            if(command[i]>='0' && command[i]<='9'){
                n*=10;
                n+=(int)(command[i]-'0');
            }
        }
        // all byword at cnt
        // salam *ash
        // 1100-0001-1000-0110-0010-0100-0000
        if(state==0 || state==10 || state==1){
        if(!not_found) replace(state,n);
        }else{
            printf("unable to combine options");
        }
        // printf("st:%d n:%d\n",state,n);
        // printf("%s--%s\n",str,str2);
        memset(str,0,sizeof(str));
        memset(str2,0,sizeof(str2));
        return;
    }else if(strcmp(command,"find")==0){
        scanf("%s",command);
        char c = getchar();
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
        bool not_found = false;
        c = getchar();
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
            not_found = true;
        }
        fgets(command,50,stdin);
        int state=0,n=0;
        for(int i =0;i<strlen(command);i++){
            if(command[i] == '-'){
                if(command[i+1] == 'c')
                    state+=1;
                if(command[i+1] == 'b')
                    state+=100;
                if(command[i+2] == 't')
                    state+=10;
                if(command[i+2] == 'l')
                    state+=1000;
            }
            if(command[i]>='0' && command[i]<='9'){
                n*=10;
                n+=(int)(command[i]-'0');
            }
        }
        // all byword at cnt
        // salam *ash
        // 1100-0001-1000-0110-0010-0100-0000
        if(state==0 || state==100 || state==10 || state==110 || state==1000 || state==1 || state==1100){
        if(!not_found) find(state,n);
        }else{
            printf("unable to combine options");
        }
        memset(str,0,sizeof(str));
        return;
    }else if(strcmp(command,"grep")==0){
        int state;
        linecounter=0;
        scanf("%s",command);
        if(strcmp(command,"-c")==0 || strcmp(command,"-l") == 0){ 
            if(command[1] == 'c')
                state = 2;
            if(command[1] == 'l')
                state = 3;
            scanf("%s",command);
        }else state = 1;
        char c = getchar();
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
        // c = getchar();
        scanf("%s",command);
        c = getchar();
        c = getchar();
        while (c!='\n'){
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
            grep(state);
            c = getchar();
            if(c == '\n'){
                break;
            }
            memset(dir,0,sizeof(dir));
            c=getchar();
        }
        if(state == 2)
            printf("%d\n",linecounter);
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

// How many roads must a man walk down
// before you call him a "man"?
