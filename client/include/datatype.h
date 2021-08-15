/*************************************************************************
    > File Name: datatype.h
    > Author: LYX
    > Mail: 
    > Created Time: Thu 29 Jul 2021 09:37:06 AM CST
 ************************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H

#define SEND_FILE   1
#define RECV_FILE   2
#define CAT_FILE    3
#define EXIT_FTP    4

const char IP[] = "8.140.160.253";
const int PORT = 8888; //数据链接端口

struct command {
    char arg[512];
    char code[5];
};

struct response {
    int flag; //1表示ACK
};
#endif
