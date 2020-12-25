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
                                memset(head,0,2048);
                                head->type=5;
                                send(tempfd,head,2048,0);
                         }     
                    }
                }
            }
        }
    }
    return 0;
}

