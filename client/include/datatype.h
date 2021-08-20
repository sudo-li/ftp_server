/*************************************************************************
    > File Name: datatype.h
    > Author: LYX
    > Mail: 
    > Created Time: Thu 29 Jul 2021 09:37:06 AM CST
 ************************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H

const char IP[] = "0.0.0.0";//你要连接的IP
const int PORT = 8888; //数据链接端口

struct command {
    char arg[512];
    char code[5];
};

#endif
