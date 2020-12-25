#include"pub.h"
#include"regedit.h"

int main(int argc,char *argv[])
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket create error");
        exit(-1);
    }
    
    struct sockaddr_in serveraddr;
    socklen_t serverlen=sizeof(serveraddr);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
    serveraddr.sin_port=htons(PORT);

    int connectfd;
    connectfd=connect(sockfd,(struct sockaddr*)&serveraddr,serverlen);
    if(connectfd<0){
            perror("connect error");
            exit(-1);
    }


    while(flag){
        menu();
    }



    close(sockfd);
    return 0;
}
