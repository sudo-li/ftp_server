//
// Created by sudo-li on 2021/8/5.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "common.h"

char conf_ans[512] ={0};

int make_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) return -1;
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int make_block(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) return -1;
    flags &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int socket_create(int port) {
    int sockfd;
    struct sockaddr_in addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) &reuse, sizeof(reuse));
    if (listen(sockfd, 8) < 0) {
        return -1;
    }
    return sockfd;
}


int socket_connect(const char *ip, int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0) {
        return -1;
    }
    return sockfd;
}


int recv_file_from_socket(int sockfd, char *name, char *dir) {
    char path[1024] = {0};
    sprintf(path, "%s/%s", dir, name);
    printf("path = %s\n", path);
    int fd = open(path, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        perror("open");
        close(fd);
        return -1;
    }
    while (1) {
        char buff[1024] = {0};
        int rsize = recv(sockfd, buff, sizeof(buff), 0);
        if (rsize <= 0) {
            close(fd);
            return rsize;
        }
        int nwrite = write(fd, buff, rsize);
        if (nwrite != rsize) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int send_file_to_socket(char *name, int sockfd) {
    int fd;
    if ((fd = open(name, O_RDONLY)) < 0) {
        perror("open");
        return -1;
    }
    while (1) {
        char buff[1024] = {0};
        int rsize = read(fd, buff, sizeof(buff));
        if (rsize <= 0) {
            close(fd);
            return rsize;
        }
        send(sockfd, (void *) buff, rsize, 0);
    }
    close(fd);
    return 0;
}

char *get_conf_value(const char *filename, const char *key) {
    bzero(&conf_ans, sizeof(conf_ans));
    FILE *fp;
    char *line = NULL, *sub = NULL;
    ssize_t len = 0, nread = 0;
    if (filename == NULL || key == NULL) {
        return NULL;
    }
    if ((fp = fopen(filename, "r")) == NULL) {
        perror("fopen");
        return NULL;
    }
    while ((nread = getline(&line, &len, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) continue;
        if (line[strlen(key)] == '=' && sub == line) {
            strcpy(conf_ans, line + strlen(key) + 1);
            if (conf_ans[strlen(conf_ans) - 1] == '\n') {
                conf_ans[strlen(conf_ans) - 1] = '\0';
            }
        }
    }
    free(line);
    fclose(fp);
    return conf_ans;
}
