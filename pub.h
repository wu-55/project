#ifndef  _PUB_H
#define _PUB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<mysql/mysql.h>
#include<unistd.h>

#define PORT 8500
#define OPEN_MAX 1000
#define TEMP 0
#define REGEDIT 1//注册
#define MODIFY 2//修改
#define LOAD 3//登陆
#define BREAK 4//退出
#define HLOAD 5 //已经注册
struct msg{
    int type;
    char data[100];
    char name[20];
    char passwd[20];
};

int sockfd;
MYSQL mysql;
MYSQL_ROW row;
MYSQL_RES *result;
int flag=1;//循环判断
#endif
