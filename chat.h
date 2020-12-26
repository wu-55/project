#include"pub.h"

void chat(){
    struct msg *head=(struct msg*)malloc(2048);
    int key;
    printf("-----欢迎进入聊天室-----\n\n");
    printf("\t1.在线人数查询\n");
    printf("\t2.群聊\n");
    printf("\t3.私聊\n");
    printf("\t4.注销\n");
    printf("\t5.踢人\n");
    printf("\t6.禁言\n");
    printf("\t7.解禁\n");
    printf("\t8.文件传输\n");
    printf("输入你的选择:");
    scanf("%d",&key);
    switch(key){
    case 1:
        head->type=7;
        send(sockfd,head,2048,0);
        recv(sockfd,head,2048,0);
        printf("当前在线的人数为:%d\n",head->count);
        break;
    case 2:
        head->type=9;
        char buf[100];
        printf("----欢迎进入群聊-----\n");
        while(1){
            memset(buf,0,100);
            fgets(buf,sizeof(buf),stdin);
            strcpy(head->data,buf);
            if(strcmp(buf,"quit\n")==0){
                break;
            }else{
                send(sockfd,head,2048,0);
                continue;
            }
        }
        break;
    case 3:
        break;
    case 4:
        head->type=8;
        send(sockfd,head,2048,0);
        flag1=0;
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    }
}
