//
// Created by sudo-li on 2021/8/5.
//

#ifndef FILE_TRANSFER_COMMON_H
#define FILE_TRANSFER_COMMON_H

int make_nonblock(int fd);

int make_block(int fd);

int socket_create(int port);

int socket_connect(const char *ip, int port);

int recv_file_from_socket(int sockfd, char *name);

int send_file_to_socket(char *name, int sockfd);

char *get_conf_value(const char *filename, const char *key);
#define DEBUG 1
#ifdef DEBUG
#define log(frm, args...) {\
printf(frm,##args);\
}
#else
#define log(frm, args...)
#endif

#endif //FILE_TRANSFER_COMMON_H
