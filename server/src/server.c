/*************************************************************************
    > File Name: server.c
    > Author: LYX
    > Mail: 
    > Created Time: Wed 28 Jul 2021 06:54:40 PM CST
 ************************************************************************/

#include "common.h"
#include "head.h"
#include "color.h"
#include "datatype.h"
#include "ls.h"
#include <ctype.h>

//realpath

int data_listener;

int ftp_server_put(int sock_control, int sock_data, char *path);
int ftp_server_get(int sock_control, int sock_data, char *path);
int ftp_server_list(int sock_control, int sock_data);
void ftpserver_process(int sock_control);
int send_response(int sockfd, int rc);
int ftp_server_login(int sock_control);
int recv_data(int sockfd, char *buffer, size_t buffer_size);
int ftp_server_check_user(char *user, char *pass);
void trimstr(char *str, int len);
int ftp_server_recv_cmd(int sock_control, char *cmd, char *arg);
int ftp_server_data_listen(int sock_control);
int socket_acccpt(int sock_listen);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("argc count\n");
        exit(0);
    }

    chdir("/home/ftp_server"); //change work directory

    int port = atoi(argv[1]);

    int server_listen, sock_control;
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    log(YELLOW"<Debug>"NONE" : server_listen is OK!\n");
    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        //获取到控制链接
        if ((sock_control = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
            perror("accept1");
            break;
        }
        //创建子进程处理用户的请求
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork");
            close(sock_control);
            break;
        } else if (pid == 0) { // 子进程
            close(server_listen); //子进程关闭父进程的监听套接字
            log(GREEN"<Accept>"NONE" : %s:%d connected!\n", inet_ntoa(client.sin_addr), ntohs (client.sin_port));
            ftpserver_process(sock_control);
            close(sock_control);//处理完成后关闭控制套接字
            exit(0);
        }
        close(sock_control); //父进程关闭子进程的控制套接字
    }
    close(server_listen);
    return 0;
}

/***
 * 处理用户请求
 * @param sock_control
 */
void ftpserver_process(int sock_control) {
    int sock_data;
    char cmd[5] = {0};
    char arg[256] = {0};

    send_response(sock_control, 220); // 发送欢迎应答

    //用户认证
    if (ftp_server_login(sock_control) == 1) { //认证成功
        send_response(sock_control, 230);
    } else {
        send_response(sock_control, 430);
        exit(0);
    }

    while (1) {
        //log(L_GREEN"<Con>"NONE" : Waiting for request!\n");
        memset(cmd, 0, sizeof(cmd));
        memset(arg, 0, sizeof(arg));
        int rc = ftp_server_recv_cmd(sock_control, cmd, arg);
        //printf("rc = %d\n", rc);
        if ((rc < 0) || (rc == 221)) { //QUITs
            break;
        }
        if (rc == 502) {
            log(RED"<Error>"NONE" : Invalid commend.\n");
            continue;
        }
        if (rc == 200) {
            //创建数据链接
            if ((sock_data = ftp_server_data_listen(sock_control)) < 0) {
                close(sock_control);
                exit(1);
            }
            //执行命令
            if (strcmp(cmd, "LIST") == 0) {
                ftp_server_list(sock_control, sock_data);
            } else if (strcmp(cmd, "GETS") == 0) {
                printf("get\n");
                ftp_server_get(sock_control, sock_data, arg);
            } else if (strcmp(cmd, "PUTS") == 0) {
                printf("put\n");
                if (ftp_server_put(sock_control, sock_data, arg) == 0) {
                    log(GREEN"<Success>"NONE" : file put success\n");
                }
            } else if (strcmp(cmd, "CHEN") == 0) {
                printf("cd\n");
            }
            close(sock_data);
        }
    }
}

int ftp_server_data_listen(int sock_control) {
    int sock_data;
    int sock_listen = socket_create(PORT);
    if (sock_listen < 0) {
        perror("socket_create");
        return -1;
    }
    sock_data = socket_acccpt(sock_listen);
    close(sock_listen);
    return sock_data;
}

int socket_acccpt(int sock_listen) {
    int sockfd;
    //struct sockaddr_in client_addr;
    //socklen_t len = sizeof(client_addr);
    //sockfd = accept(sock_listen, (struct sockaddr *)&client_addr, &len);
    sockfd = accept(sock_listen, NULL, NULL);
    if (sockfd < 0) {
        perror("accept2");
        return -1;
    }
    return sockfd;
}

/***
 * 接受命令并响应 返回响应吗
 * @param sock_control
 * @param cmd
 * @param arg
 * @return
 */
int ftp_server_recv_cmd(int sock_control, char *cmd, char *arg) {
    int rc = 200;
    char buffer[256] = {0};
    memset(cmd, 0, 5);
    memset(arg, 0, 256);

    if (recv_data(sock_control, buffer, sizeof(buffer)) == -1) {
        perror("recv error");
        return -1;
    }
    printf("buffer = %s\n", buffer);
    strncpy(cmd, buffer, 4);
    char *tmp = buffer + 5;
    strcpy(arg, tmp);
    printf("cmd = %s\n", cmd);
    //sleep(10);
    if (strcmp(cmd, "QUIT") == 0) {
        rc = 221;
    } else if ((strcmp(cmd, "USER") == 0) || (strcmp(cmd, "PASS") == 0) || (strcmp(cmd, "LIST") == 0) || \
    (strcmp(cmd, "GETS") == 0) || (strcmp(cmd, "PUTS") == 0) || (strcmp(cmd, "CHEN") == 0)) {
        rc = 200;
    } else {
        rc = 502;
    }
    send_response(sock_control, rc);
    return rc;
}

/***
 * 用户登录
 * @param sock_control
 * @return
 */
int ftp_server_login(int sock_control) {
    char buffer[256] = {0};
    char user[256] = {0};
    char pass[256] = {0};
    //获取客户端发来的用户名
    if (recv_data(sock_control, buffer, sizeof(buffer)) == -1) {
        perror("recv error");
        exit(1);
    }
    char *ind = index(buffer, ' ');
    strcpy(user, buffer + (ind - buffer) + 1);
    printf("user = %s\n", user);
    // 获取密码
    if (recv_data(sock_control, buffer, sizeof(buffer)) == -1) {
        perror("recv error");
        exit(1);
    }
    ind = index(buffer, ' ');
    strcpy(pass, buffer + (ind - buffer) + 1);
    printf("password = %s\n", pass);
    return ftp_server_check_user(user, pass); // 用户名和密码的验证并返回
}

/***
 * 用户资格印证成功返回1
 * @param user
 * @param pass
 * @return
 */
int ftp_server_check_user(char *user, char *pass) {
    char username[256];
    char password[256];
    char buffer[256];
    char *line = NULL;
    char *ch;
    int len = 0;
    FILE *fp;

    if ((fp = fopen("server/.auth", "r")) == NULL) {
        perror("open");
        exit(1);
    }
    //读取文件验证合法性
    while (getline(&line, (size_t *) &len, fp) != -1) {
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, line);
        ch = strtok(buffer, " ");
        strcpy(username, ch);
        if (ch != NULL) {
            ch = strtok(NULL, " ");
            strcpy(password, ch);
        }

        trimstr(password, (int)strlen(password));

        if ((strcmp(user, username) == 0) && (strcmp(pass, password) == 0)) {
            free(line);
            fclose(fp);
            return 1;
        }
    }
    free(line);
    fclose(fp);
    return 0;
}

void trimstr(char *str, int len) {
    for (int i = 0; i < len; i++) {
        if (isspace(str[i])) str[i] = 0;
        if (str[i] == '\n') str[i] = 0;
    }
}

int recv_data(int sockfd, char *buffer, size_t buffer_size) {
    int ret;
    memset(buffer, 0, sizeof(buffer_size));
    ret = recv(sockfd, (void *)buffer, buffer_size, 0);
    if (ret < 0) {
        return -1;
    }
    return ret;
}

/***
 * 发送响应吗到sockfd
 * @param sockfd
 * @param rc
 * @return
 */
int send_response(int sockfd, int rc) {
    int conv = htonl(rc);
    if (send(sockfd, (void *)&conv, sizeof(conv), 0) < 0) {
        perror("error sending ");
        return -1;
    }
    return 0;
}

int ftp_server_put(int sock_control, int sock_data, char *path) {

    log(GREEN"<Accept>"NONE" : data connected!\n");
    printf("path = %s\n", path);
    int fd = open(path, O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        send_response(sock_control, 550);
        close(fd);
        return -1;
    } else {
        send_response(sock_control, 150);
    }
    close(fd);
    if (recv_file_from_socket(sock_data, path) < 0) {
        perror("recv_file_from_socket");
        send_response(sock_control, 550);
        return -1;
    }
    send_response(sock_control, 226);
    return 0;
}

int ftp_server_get(int sock_control, int sock_data, char *path) {
    if (access(path, F_OK | R_OK)) {
        printf("file is not exist\n");
        send_response(sock_control, 550); // 发送错误码
        return -1;
    }
    send_response(sock_control, 150); //发送OK
    if (send_file_to_socket(path, sock_data) < 0) {
        return -1;
    }
    log(GREEN"<Accept>"NONE" : file transfer success!\n");
    send_response(sock_control, 226);
    return 0;
}


int ftp_server_list(int sock_control, int sock_data) {
    if (ls_to_file() < 0) {
        perror("ls_to_file");
        return -1;
    }
    //开始发送
    send_response(sock_control, 1);
    send_file_to_socket("server/tmp.txt", sock_data);
    log(GREEN"<Accept>"NONE" : ls connected!\n");
    send_response(sock_control, 226); //完成发送
    return 0;
}
