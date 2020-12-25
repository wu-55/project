#include"pub.h"

void sqlinit()
{
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,"localhost","root","123","chatroom",0,NULL,0))
    {
        perror("connet error");
        exit(-1);
    }else{
        printf("数据库连接成功!\n");
    }

}
