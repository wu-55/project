#include"pub.h"

void menu(){
    struct msg *head=(struct msg*)malloc(2048);
    int key;
    printf("--------欢迎来到卓越班---------\n");
    printf("\n\n");
    printf("\t1.用户注册\n");
    printf("\t2.用户修改密码\n");
    printf("\t3.用户登陆\n");
    printf("\t4.用户退出\n");
    printf("请输入你的选择:");
    scanf("%d",&key);
    switch(key){
    case 1:
        printf("\n\n-----欢迎进入用户注册页面-----\n");
        while(1){
            head->type=1;
            printf("\n请输入注册的用户名:");
            scanf("%s",head->name);
            printf("请输入注册的密码:");
            scanf("%s",head->passwd);
            send(sockfd,head,2048,0);
            recv(sockfd,head,2048,0);
            if(head->type==5){
                printf("\n你的用户名已经被注册！\n");
                continue;
            }else{
                printf("恭喜用户注册成功!\n");
                break;
            }
            //printf("head->type=%d\n",head->type);
        }
        break;

    case 2:
        printf("\n----欢迎用户进入更改密码模块----\n");


        break;
    case 3:

        printf("\n----欢迎用户进入登陆模块----\n");
        break;
    case 4:
        printf("----用户退出成功----\n");
        flag=0;
        break;
    }


}
