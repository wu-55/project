#include"pub.h"
#include"sql.h"

int main()
{
    struct msg *head=(struct msg*)malloc(2048);
    int maxid;
    int epollfd;
    int j;
    int client[OPEN_MAX];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket create error");
        exit(-1);
    }

    sqlinit();
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    
    socklen_t serverlen=sizeof(serveraddr);
    socklen_t clientlen=sizeof(clientaddr);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);

    int bindfd;
    bindfd=bind(sockfd,(struct sockaddr*)&serveraddr,serverlen);
    if(bindfd<0)
    {
        perror("bind sockfd error");
        exit(-1);
    }

    int listenfd;
    listenfd=listen(sockfd,20);
    if(listenfd<0)
    {
        perror("listen sockfd error");
        exit(-1);
    }

    for(int i=0;i<OPEN_MAX;i++)
    {
        client[i]=-1;
    }
    maxid=-1;
    
    struct epoll_event tep,ep[OPEN_MAX];
    epollfd=epoll_create(OPEN_MAX);
    if(epollfd<0)
    {
        perror("epoll_create error");
        exit(-1);
    }
    tep.events=EPOLLIN;
    tep.data.fd=sockfd;

    int res;
    int ready;
    int tempfd;
    int count=0;
    res=epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&tep);
    if(res==-1)
    {
        perror("epoll_ctl error");
        exit(-1);
    }

    for(;;){
         ready=epoll_wait(epollfd,ep,OPEN_MAX,-1);
        if(ready==-1)
        {
            perror("epoll_wait");
        }
        for(int i=0;i<ready;i++){
            if(!ep[i].events&EPOLLIN)
                continue;
            if(ep[i].data.fd==sockfd){
                int newfd=accept(sockfd,(struct sockaddr*)&clientaddr,&clientlen);
                count++;
                printf("count=%d\n",count);
                
                for( j=0;j<OPEN_MAX;j++){
                    if(client[j]<0){
                        client[j]=newfd;
                        break;
                    }
                    if(j>maxid){
                        maxid=j;
                    }
                }
                tep.events=EPOLLIN;
                tep.data.fd=newfd;
                res=epoll_ctl(epollfd,EPOLL_CTL_ADD,newfd,&tep);
                if(res==-1){
                    perror("epoll_ctl");
                    exit(-1);
                }
            }else{
                tempfd=ep[i].data.fd;
                int n;
                n=recv(tempfd,head,2048,0);
                if(head->type==1)
                {

                    char a[100]="select * from userlist where user='";
                    char dest[100]={""};
                    strcat(dest,a);
                    strcat(dest,head->name);
                    strcat(dest,"'");
                    printf("%s\n",dest);
                    if(mysql_query(&mysql,dest)==0){
                         result=mysql_store_result(&mysql);
                         int rows=mysql_num_rows(result);
                         if(rows==0){
                             head->type=0;
                             send(tempfd,head,2048,0);
                             char aa[100]="insert into userlist(user,passwd) values('";
                             char dest1[100]={""};
                             strcat(dest1,aa);
                             strcat(dest1,head->name);
                             strcat(dest1,"','");
                             strcat(dest1,head->passwd);
                             strcat(dest1,"')");
                             printf("%s\n",dest1);
                             mysql_query(&mysql,dest1);
                         }
                         int fields=mysql_num_fields(result);
                         //printf("rows=%d field=%d\n",rows,fields);  
                         while(row=mysql_fetch_row(result)){
                                printf("%s\t%s\n",row[0],row[1]);
                                head->type=5;
                                send(tempfd,head,2048,0);
                         }     
                    }
                }else if(head->type==2){
                    char a[100]="select * from userlist where user='";
                    char dest2[100]={""};
                    strcat(dest2,a);
                    strcat(dest2,head->name);
                    strcat(dest2,"'");
                    printf("%s\n",dest2);
                    if(mysql_query(&mysql,dest2)==0){
                         result=mysql_store_result(&mysql);
                         int rows=mysql_num_rows(result);
                         if(rows==0){
                             head->type=0;
                             send(tempfd,head,2048,0);
                         }
                         int fields=mysql_num_fields(result);
                         //printf("rows=%d field=%d\n",rows,fields);  
                         while(row=mysql_fetch_row(result)){
                                printf("%s\t%s\n",row[0],row[1]);
                                if(strcmp(row[0],head->name)==0&&strcmp(row[1],head->passwd)==0){
                                    head->type=5;
                                    send(tempfd,head,2048,0);
                                    recv(tempfd,head,2048,0);
                                    printf("%s\n",head->passwd);
                                    
                                    char aaa[50]="update userlist set passwd='";
                                    char dest3[100]={""};
                                    strcat(dest3,aaa);
                                    strcat(dest3,head->passwd);
                                    strcat(dest3,"' ");
                                    strcat(dest3, "where user ='");
                                    strcat(dest3,row[0]);
                                    strcat(dest3,"'");
                                    printf("%s\n",dest3);
                                    mysql_query(&mysql,dest3);
                         }     
                    }
                }
                }else if (head->type==3){
                    printf("%s\n",head->name);
                    char a[100]="select * from userlist where user='";
                    char dest4[100]={""};
                    strcat(dest4,a);
                    strcat(dest4,head->name);
                    strcat(dest4,"'");
                    printf("%s\n",dest4);
                    if(mysql_query(&mysql,dest4)==0){
                         result=mysql_store_result(&mysql);
                         int rows=mysql_num_rows(result);
                         if(rows==0){
                             head->type=0;
                             send(tempfd,head,2048,0);
                         }
                         int fields=mysql_num_fields(result);
                         //printf("rows=%d field=%d\n",rows,fields);  
                         while(row=mysql_fetch_row(result)){
                                printf("%s\t%s\n",row[0],row[1]);
                                if(strcmp(row[0],head->name)==0&&strcmp(row[1],head->passwd)==0){
                                    head->type=5;
                                    send(tempfd,head,2048,0);
                                }else{
                                    head->type=6;
                                    send(tempfd,head,2048,0);
                                }
                         }
                    }
                }else if(head->type==7){
                    memset(head,2048,0);
                    head->count=count;
                    printf("当前在线人数为:%d\n",count);
                    send(tempfd,head,2048,0);
                }
                else if(head->type==8){
                    int temp;
                    for(j=0;j<OPEN_MAX;j++)
                    {
                        if(ep[j].data.fd==tempfd){
                            ep[j].data.fd=-1;
                            temp=j;
                            break;
                        }
                    }
                    res=epoll_ctl(epollfd,EPOLL_CTL_DEL,tempfd,NULL);
                    if(res==-1){
                        perror("epoll_ctl");
                    }
                    close(tempfd);
                    printf("client[%d] 已经注销\n",temp);
                }else if(head->type==9){
                    recv(tempfd,head,2048,0);
                    printf("recv  : %s",head->data);
                }
            }
        }
    }
        return 0;
    }

