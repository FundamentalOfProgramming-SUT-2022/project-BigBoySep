#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

bool forward = false,isarman=false;
int line,pos,size,d;
char command[50],dir[100],dir2[100],tmp[100],tmp2[100]
,str[10000],str2[10000],buff_str[10000],clipboard[10000],arman[10000],out[10000],linecounter=0;

bool existance(){
    for(int i =0;i<strlen(dir);i++){
        if(dir[i] == '/'){
            DIR* DIR = opendir(tmp);
            if(DIR){
            closedir(DIR);
            }else if(ENOENT == errno) {
                printf("Wrong Address\n");
                return false;
            }
        }
        tmp[i] = dir[i];
    }
    if (access(dir, F_OK) == 0) {
        return true;
    }else{
        printf("File Does not exist\n");
        return false;
    }
}
bool existance2(){
    for(int i =0;i<strlen(dir2);i++){
        if(dir2[i] == '/'){
            DIR* DIR = opendir(tmp);
            if(DIR){
            closedir(DIR);
            }else if(ENOENT == errno) {
                printf("Wrong second Address\n");
                return false;
            }
        }
        tmp[i] = dir2[i];
    }
    if (access(dir2, F_OK) == 0) {
        return true;
    }else{
        printf("Second file Does not exist\n");
        return false;
    }
}

// in --> insert find grep
// out -> cat grep compare tree find 

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
    FILE *fp,*fpu;
    fp = fopen(tmp,"w");
    memset(tmp,0,sizeof(tmp));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    fpu = fopen(tmp,"w");
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    fclose(fp);
    fclose(fpu);
}
//insertstr
void insertstr(const char *ins){
    FILE *fp,*tmpf;
    fp = fopen(dir,"r");
    FILE *fpu;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    fpu = fopen(tmp,"w");
    char c;
    while ((c=fgetc(fp))!=EOF){
        fputc(c,fpu);
    }
    rewind(fp);
    fclose(fpu);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
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
    memset(arman,0,sizeof(arman));
    FILE *fp;
    fp = fopen(dir,"r");
    char c = fgetc(fp);
    while (c!=EOF){
        if(isarman) strncat(arman,&c,1);
        else printf("%c",c);
        c = fgetc(fp);
    }
    if(!isarman) printf("\n");
    fclose(fp);
}

void removestr(bool fward){
    FILE *fp,*tmpf;
    FILE *fpu;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    fpu = fopen(tmp,"w");
    char c;
    while ((c=fgetc(fp))!=EOF){
        fputc(c,fpu);
    }
    rewind(fp);
    fclose(fpu);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    int t=0,p=0,l=1,ts;
    // 12345678910
    if(p==pos && l==line) ts=t;
    c = fgetc(fp);
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
    FILE *fpu;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    fpu = fopen(tmp,"w");
    char c;
    while ((c=fgetc(fp))!=EOF){
        fputc(c,fpu);
    }
    rewind(fp);
    fclose(fpu);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    memset(clipboard,0,sizeof(clipboard));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    int t=0,p=0,l=1,ts;
    // 12345678910
    if(p==pos && l==line) ts=t;
    c = fgetc(fp);
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

void find(int state,int n,const char *fin){

    FILE *fp = fopen(dir,"r");
    int len = strlen(fin),i=0,pos_bychar[100],pos_byword[100],cnt=0,t=0,wcnt=1,ti,wi;
    char c = fgetc(fp);
    bool found = false;
    t++;
    while (c!=EOF){
        while (c!=EOF && c==fin[i] && ++i<len){
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
    memset(arman,0,sizeof(arman));
    switch (state)
    {
    case 0:
        if(isarman){
            if(found){
                char num[5];
                sprintf(num,"%d",pos_bychar[0]);
                strcat(arman,num);
                strcat(arman,"\n");
            }else strcat(arman,"-1\n");
        }else{
            if(found) printf("%d\n",pos_bychar[0]);
            else printf("-1\n");
        }
        break;
    case 1000:
        if(isarman){
            if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0){
                    char num[5];
                    sprintf(num,"%d",pos_bychar[i]);
                    strcat(arman,", ");
                    strcat(arman,num);
                }
                else{
                    char num[5];
                    sprintf(num,"%d",pos_bychar[i]);
                    strcat(arman,num);
                }
            }
            strcat(arman,"\n");
        }else strcat(arman,"-1");
        }else{ 
            if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0) printf(", %d",pos_bychar[i]);
                else printf("%d",pos_bychar[i]);
            }
            printf("\n");
        }else printf("-1");
        }
        break;
    case 100:
        if(isarman){
            if(found){
                char num[5];
                sprintf(num,"%d",pos_byword[0]);
                strcat(arman,num);
                strcat(arman,"\n");
            }else strcat(arman,"-1\n");
        }else{
            if(found) printf("%d\n",pos_byword[0]);
            else printf("-1\n");
        }
        break;
    case 1100:
        if(isarman){
            if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0){
                    char num[5];
                    sprintf(num,"%d",pos_byword[i]);
                    strcat(arman,", ");
                    strcat(arman,num);
                }
                else{
                    char num[5];
                    sprintf(num,"%d",pos_byword[i]);
                    strcat(arman,num);
                }
            }
            strcat(arman,"\n");
        }else strcat(arman,"-1");
        }else{ 
            if(found){
            for(int i =0;i<cnt;i++){
                if(i!=0) printf(", %d",pos_byword[i]);
                else printf("%d",pos_byword[i]);
            }
            printf("\n");
        }else printf("-1");
        }
        break; 
    case 1:
        if(isarman){
            if(found){
                char num[5];
                sprintf(num,"%d",cnt);
                strcat(arman,num);
                strcat(arman,"\n");
            }else strcat(arman,"0\n");
        }else{
            if(found)printf("%d\n",cnt);
            else printf("0\n");
        }
        break;
    case 10:
        if(isarman){
            if(found && n<=cnt){
                char num[5];
                sprintf(num,"%d",pos_bychar[n-1]);
                strcat(arman,num);
                strcat(arman,"\n");
            }
            else strcat(arman,"-1\n");
        }else{
            if(found && n<=cnt) printf("%d\n",pos_bychar[n-1]);
            else printf("-1\n");
        }
        break;
    case 110:   
        if(isarman){
            if(found && n<=cnt){
                char num[5];
                sprintf(num,"%d",pos_byword[n-1]);
                strcat(arman,num);
                strcat(arman,"\n");
            }
            else strcat(arman,"-1\n");
        }else{
            if(found && n<=cnt) printf("%d\n",pos_byword[n-1]);
            else printf("-1\n");
        }
        break;
    default:
        break;
    }
}

void replace(int state,int n){

    FILE *fp = fopen(dir,"r");
    FILE *fpu;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    fpu = fopen(tmp,"w");
    char c;
    while ((c=fgetc(fp))!=EOF){
        fputc(c,fpu);
    }
    rewind(fp);
    fclose(fpu);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    FILE *ftmp = fopen(tmp,"w");
    int len = strlen(str),i=0,pos_bychar[100],pos_byword[100],cnt=0,t=0,wcnt=1,ti,wi;
    c = fgetc(fp);
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

void grep(int state,const char *grp){
    FILE *fp = fopen(dir,"r");
    bool isvalid = false;
    while (!feof(fp)){
        fgets(buff_str,1000,fp);
        if(strstr(buff_str,grp)!=NULL){
            linecounter++;
            isvalid = true;
            if(state == 1){
                // printf("%s: %s",dir,buff_str);
                strcat(out,dir);
                strcat(out,": ");
                strcat(out,buff_str);
                if(buff_str[strlen(buff_str)-1]!='\n') strcat(out,"\n");
            }
        }
    }
    if(state == 3 && isvalid == true){
        strcat(out,dir);
        strcat(out,"\n");
    }
        // printf("%s\n",dir);
    fclose(fp);
}

void undo(){
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt");
    strncpy(tmp2,dir,strlen(dir)-4);
    strcat(tmp2,"____undo2.txt");
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    rename(tmp,tmp2);
    rename(dir,tmp);
    rename(tmp2,dir);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    memset(tmp2,0,sizeof(tmp2));
}

void auto_indent(){
    
    FILE *fp,*tmpf;
    fp = fopen(dir,"r");
    FILE *fpu;
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____undo.txt"); 
    SetFileAttributes(tmp,FILE_ATTRIBUTE_NORMAL);
    fpu = fopen(tmp,"w");
    char c;
    while ((c=fgetc(fp))!=EOF){
        fputc(c,fpu);
    }
    rewind(fp);
    fclose(fpu);
    SetFileAttributes(tmp,FILE_ATTRIBUTE_HIDDEN);
    memset(tmp,0,sizeof(tmp));
    strncpy(tmp,dir,strlen(dir)-4);
    strcat(tmp,"____temp.txt"); 
    tmpf = fopen(tmp,"w+");
    int tab=0,space=0;
    bool ispace = false,open=false;
    while((c=fgetc(fp)) != EOF){
        if(c!=' ' && c!='{' && c!='}'){
            if(open)
                for(int i =0;i<tab;i++)
                    fputc('\t',tmpf);
            open = false;
            if(ispace)
                for(int i =0;i<space;i++)
                    fputc(' ',tmpf);
            ispace = true;
            fputc(c,tmpf);
            space = 0;
        }
        if(c==' '){
            space++;
        }
        // {{}   }
        if(c=='{'){
            if(!ispace)
                for(int i =0;i<tab;i++)
                    fputc('\t',tmpf);
            tab++;
            space =0;
            if(ispace)
                fputc(' ',tmpf);
            fputs("{\n",tmpf);
            ispace = false;
            open = true;
        }
        if(c=='}'){
            tab--;
            space=0;
            if(ispace){
                fputc('\n',tmpf);
            }
            for(int i =0;i<tab;i++)
                fputc('\t',tmpf);
            fputs("}\n",tmpf);
            ispace = false;
        }
    }
    fclose(tmpf);
    fclose(fp);
    remove(dir);
    rename(tmp,dir);
    memset(tmp,0,sizeof(tmp));  
}

void compare(){

    memset(arman,0,sizeof(arman));
    FILE *fp1,*fp2;
    fp1 = fopen(dir,"r");
    fp2 = fopen(dir2,"r");
    int line1=0,line2=0;
    while(!feof(fp1)){
        fgets(str,1000,fp1);
        line1++;
    }
    while(!feof(fp2)){
        fgets(str2,1000,fp2);
        line2++;
    }
    rewind(fp1);
    rewind(fp2);
    if(line1>line2){
        for(int i =1;i<=line2;i++){
            fgets(str,1000,fp1);
            fgets(str2,1000,fp2);
            if(str[strlen(str)-1] == '\n')str[strlen(str)-1] = '\0';
            if(str2[strlen(str2)-1] == '\n')str2[strlen(str2)-1] = '\0';
            if(strcmp(str,str2) != 0){
                if(isarman){
                    strcat(arman,"============ #");
                    char num[5];
                    sprintf(num,"%d",i);
                    strcat(arman,num);
                    strcat(arman," ============\n");
                    strcat(arman,str);
                    strcat(arman,"\n");
                    strcat(arman,str2);
                    strcat(arman,"\n");
                }else{
                    printf("============ #%d ============\n",i);
                    printf("%s\n%s\n",str,str2);
                }                
            }
        }
        if(isarman){
            strcat(arman,">>>>>>>>>>>> #");
            char num[5];
            sprintf(num,"%d",line2+1);
            strcat(arman,num);
            strcat(arman," - #");
            sprintf(num,"%d",line1);
            strcat(arman,num);
            strcat(arman," >>>>>>>>>>>>\n");
        }
        else printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",line2+1,line1);
        for(int i = line2+1;i<=line1;i++){
            fgets(str,1000,fp1);
            if(isarman){
                strcat(arman,str);
                strcat(arman,"\n");
            }else printf("%s\n",str);
        }
    }
    if(line1<line2){
        for(int i =1;i<=line1;i++){
            fgets(str,1000,fp1);
            fgets(str2,1000,fp2);
            if(str[strlen(str)-1] == '\n')str[strlen(str)-1] = '\0';
            if(str2[strlen(str2)-1] == '\n')str2[strlen(str2)-1] = '\0';
            if(strcmp(str,str2) != 0){
                if(isarman){
                    strcat(arman,"============ #");
                    char num[5];
                    sprintf(num,"%d",i);
                    strcat(arman,num);
                    strcat(arman," ============\n");
                    strcat(arman,str);
                    strcat(arman,"\n");
                    strcat(arman,str2);
                    strcat(arman,"\n");
                }else{
                    printf("============ #%d ============\n",i);
                    printf("%s\n%s\n",str,str2);
                }
            }
        }
        if(isarman){
            strcat(arman,">>>>>>>>>>>> #");
            char num[5];
            sprintf(num,"%d",line1+1);
            strcat(arman,num);
            strcat(arman," - #");
            sprintf(num,"%d",line2);
            strcat(arman,num);
            strcat(arman," >>>>>>>>>>>>\n");
        }
        else printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",line1+1,line2);
        for(int i = line1+1;i<=line2;i++){
            fgets(str2,1000,fp2);
            if(isarman){
                strcat(arman,str2);
                strcat(arman,"\n");
            }else printf("%s\n",str2);
        }
    }
    if(line1==line2){
        for(int i =1;i<=line1;i++){
            fgets(str,1000,fp1);
            fgets(str2,1000,fp2);
            if(str[strlen(str)-1] == '\n')str[strlen(str)-1] = '\0';
            if(str2[strlen(str2)-1] == '\n')str2[strlen(str2)-1] = '\0';
            if(strcmp(str,str2) != 0){
                if(isarman){
                    strcat(arman,"============ #");
                    char num[5];
                    sprintf(num,"%d",i);
                    strcat(arman,num);
                    strcat(arman," ============\n");
                    strcat(arman,str);
                    strcat(arman,"\n");
                    strcat(arman,str2);
                    strcat(arman,"\n");
                }else{
                    printf("============ #%d ============\n",i);
                    printf("%s\n%s\n",str,str2);
                }                
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
}

void tree(char *basePath, const int root,int depth)
{
    
    if(depth>=d && d!=-1)
        return;
    int i;
    char path[1000],tpath[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        strcpy(tpath, basePath);
        strcat(tpath, "/");
        strcat(tpath, dp->d_name);
        if(GetFileAttributesA(tpath) != FILE_ATTRIBUTE_HIDDEN){
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            {
                for (i=0; i<root; i++) 
                {
                    if (i%2 == 0 ){
                        char c = 179;
                        if(isarman) strcat(arman,"|");
                        else printf("%c", 179);
                    }
                    else
                        if(isarman) strcat(arman," ");
                        else printf(" ");

                }
                if(isarman){
                    char c = 195;
                    strcat(arman,"|-");
                    c = 196;
                    strcat(arman,"-");
                    strcat(arman, (dp->d_name));
                    strcat(arman,"\n");
                }else printf("%c%c%s\n", 195, 196, dp->d_name);

                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);
                tree(path, root + 2,depth+1);
            }
        }
    }

    closedir(dir);
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
        if(!isarman){
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
        }
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        // printf("%s",str);
        if(isarman){
            if(found) insertstr(arman);
        }else{
            if(found) insertstr(str);
            memset(str,0,sizeof(str));
        }
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
        if((c=getchar()) == ' '){
            scanf("%s",command);
            scanf("%s",command);
            isarman = true;
            bool found = existance();
            memset(tmp,0,sizeof(tmp));
            if(found)cat();
            input();
            isarman = false;
        }else{
            bool found = existance();
            memset(tmp,0,sizeof(tmp));
            // printf("%s\n",dir);
            if(found)cat();
        }
        return;
    }else if(strcmp(command,"removestr") == 0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
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
        if(found) removestr(forward);
        return; 
    }else if(strcmp(command,"copystr")==0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
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
        if(found)copystr(forward);
        return; 
    }else if(strcmp(command,"cutstr")==0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
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
        if(found) cutstr(forward);
        return;
    }else if(strcmp(command,"pastestr")==0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
        c = getchar();
        scanf("%s",command);
        c = getchar();
        scanf("%d:%d",&line,&pos);
        if(found) insertstr(clipboard);
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
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
        if(found) replace(state,n);
        }else{
            printf("unable to combine options");
        }
        // printf("st:%d n:%d\n",state,n);
        // printf("%s--%s\n",str,str2);
        memset(str,0,sizeof(str));
        memset(str2,0,sizeof(str2));
        return;
    }else if(strcmp(command,"find")==0){
        bool isarman_inner = false;
        memset(out,0,sizeof(out));
        char c;
        if(!isarman){
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
        }
        c = getchar();
        scanf("%s",command);
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
        memset(command,0,sizeof(command));
        int state=0,n=0;
        int t=0;
        while((c=getchar())!='\n'){
            command[t] = c;
            if(c == 'D' && command[t-1] == '='){
                isarman_inner = true;
                break;
            }
            t++;
        }
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
        if(isarman_inner){
            getchar();
            scanf("%s",command);
            if(state==0 || state==100 || state==10 || state==110 || state==1000 || state==1 || state==1100){
                if(isarman){
                if(found) find(state,n,arman);
                }else if(found){
                    isarman = true;
                    find(state,n,str);
                    }
                }else{
                    memset(arman,0,sizeof(arman));
                    isarman = true;
                    strcat(arman,"unable to combine options\n");
                }
            memset(str,0,sizeof(str));
            input();
            isarman = false;
            return;
        }
        if(state==0 || state==100 || state==10 || state==110 || state==1000 || state==1 || state==1100){
        if(isarman){
        if(found) find(state,n,arman);
        }else if(found) find(state,n,str);
        }else{
            printf("unable to combine options\n");
        }
        memset(str,0,sizeof(str));
        return;
    }else if(strcmp(command,"grep")==0){
        memset(out,0,sizeof(out));
        int state;
        bool isarman_inner = false;
        linecounter=0;
        scanf("%s",command);
        if(strcmp(command,"-c")==0 || strcmp(command,"-l") == 0){ 
            if(command[1] == 'c')
                state = 2;
            if(command[1] == 'l')
                state = 3;
            scanf("%s",command);
        }else state = 1;
        char c;
        if(!isarman){
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
            // c = getchar();
            scanf("%s",command);
        }
        c = getchar();
        c = getchar();
        while (c!='\n'){
            if(c=='='){
                getchar();
                getchar();
                scanf("%s",command);
                isarman = true;
                if(state == 2){
                    // printf("%d\n",linecounter);
                    char d = '0' + linecounter;
                    strcat(out,&d);
                    strcat(out,"\n");
                }
                strcpy(arman,out);
                // printf("%s",command);
                input();
                memset(arman,0,sizeof(arman));
                isarman = false;
                isarman_inner = true;
                break;
            }
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
            bool found = existance();
            memset(tmp,0,sizeof(tmp));
            if(isarman){
            if(found) grep(state,arman);
            }else if(found) grep(state,str);
            c = getchar();
            if(c == '\n'){
                break;
            }
            memset(dir,0,sizeof(dir));
            c=getchar();
        }
        if(state == 2 && !isarman_inner){
            // printf("%d\n",linecounter);
            char d = '0' + linecounter;
            strcat(out,&d);
            strcat(out,"\n");
        }
        // printf("asas");
        if(!isarman_inner){
            printf("%s",out);
            memset(out,0,sizeof(out));
        }
        memset(str,0,sizeof(str));
        return;
    }else if(strcmp(command,"undo") == 0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
        // printf("%s\n",dir);
        if(found) undo();
        return;
    }else if(strcmp(command,"auto-indent") == 0){
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
        bool found = existance();
        memset(tmp,0,sizeof(tmp));
        // printf("%s\n",dir);
        if(found) auto_indent();
        return;
    }else if(strcmp(command,"compare") == 0){
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
        c = getchar();
        c = getchar();
        if(c=='/')
            scanf("%s",dir2);
        else{
            int t =0;
            c = getchar();
            while ((c=getchar()) != '"'){
                dir2[t]=c;
                t++;
            }
            dir2[t] = '\0';
        }
        if((c=getchar()) == ' '){
            scanf("%s",command);
            scanf("%s",command);
            isarman = true;
            bool found = existance();
            memset(tmp,0,sizeof(tmp));
            bool found2 = existance2();
            memset(tmp,0,sizeof(tmp));
            if(found && found2) compare();
            input();
            isarman = false;
        }else{
            bool found = existance();
            memset(tmp,0,sizeof(tmp));
            bool found2 = existance2();
            memset(tmp,0,sizeof(tmp));
            // printf("%s\n",dir);
            if(found && found2) compare();
        }
        return;
    }else if(strcmp(command,"tree") == 0){
        scanf("%d",&d);
        if(d<-1){
            printf("Invalid depth\n");
            return;
        }
        char c;
        if((c=getchar()) == ' '){
            scanf("%s",command);
            scanf("%s",command);
            isarman = true;
            memset(arman,0,sizeof(arman));
            strcat(arman,"root\n");
            tree("./root",0,0);
            input();
            isarman = false;
        }else{
        printf("root\n");
        tree("./root",0,0);
        }
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
