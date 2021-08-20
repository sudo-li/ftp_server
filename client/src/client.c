/*************************************************************************
    > File Name: client.c
    > Author: LYX
    > Mail: 
    > Created Time: Wed 28 Jul 2021 07:13:56 PM CST
 ************************************************************************/

#include "common.h"
#include "head.h"
#include "datatype.h"
#include "color.h"

int sock_control; //控制链接

/***
 * 接收服务器的响应
 * @return 返回响应代码
 */
int read_reply() {
    int retcode = 0;
    if (recv(sock_control, &retcode, sizeof(retcode), 0) < 0) {
        perror("client recv");
        return -1;
    }
    return ntohl(retcode);
}

/***
 * 打印响应信息
 * @param rc 响应码
 */
void print_reply(int rc) {
    switch (rc) {
        case 220: {
            log(GREEN"<Server>"NONE"220 Welcome, server ready.\n");
        }break;
        case 221: {
            log(GREEN"<Server>"NONE"221 Goodbye!\n");
        }break;
        case 226: {
            log(GREEN"<Server>"NONE"226 Closing data connection. Requested file action successful.\n");
        }break;
        case 550: {
            log(RED"<Server>"NONE"550 Requested action not taken. File unavailable.\n");
        }break;
        case 560: {
            log(RED"<Server>"NONE"560 Requested action not taken. Directory not exist.\n");
        }break;
    }
}
/***
 * 获取输入命令的参数
 * @param buffer 参数
 * @param size 参数大小
 */
void read_input(char *buffer, int size) {
    memset(buffer, 0, size);
    scanf("%[^\n]s", buffer);
    getchar();
}

/***
 * 含有CODE和ARG的COMMAND结构体 链接成一个字符串发送给服务器
 * @param cmd
 */
int ftpclient_send_cmd(struct command *cmd) {
    char buffer[256] = {0};
    sprintf(buffer, "%s %s", cmd->code, cmd->arg);
    int rsize = send(sock_control, buffer, strlen(buffer), 0);
    if (rsize < 0) {
        perror("send commend to server");
        return -1;
    }
    return 0;
}

/***
 * 用户登录并到服务器验证
 */
void ftpclient_login() {
    struct command cmd;
    char user[256] = {0};
    char pass[256] = {0};
    //获取用户名
    while (strlen(user) == 0) {
        printf("Name: ");
        fflush(stdout);
        read_input(user, sizeof(user));
    }
    //发送用户名到服务器
    strcpy(cmd.code, "USER");
    strcpy(cmd.arg, user);
    ftpclient_send_cmd(&cmd);

    //获取密码
    fflush(stdout);
    //while (strlen(pass) == 0) {
    //    printf("Password: ");
    //    fflush(stdout);
    //    read_input(pass, sizeof(pass));
    //}
    strcpy(pass, getpass("Password: "));
    //发送密码到服务器
    strcpy(cmd.code, "PASS");
    strcpy(cmd.arg, pass);
    ftpclient_send_cmd(&cmd);

    //等待服务器验证
    int retcode = read_reply();
    switch (retcode) {
        case 430: {
            log(YELLOW"<Server>"NONE" : Invalid username/password.\n");
            exit(0);
        }
        case 230: {
            log(GREEN"<Success>"NONE" : Successful login.\n");
        } break;
        default: {
            perror("reading message from server");
            exit(1);
        }
    }
}

/***
 * 解析输入的字符串到结构体
 * @param buf 输入的字符串
 * @param size
 * @param cmd
 * @return
 */
int ftpclient_read_cmd(char *buf, int size, struct command *cmd) {
    int arg_count = 0;
    memset((void *)cmd, 0, sizeof(struct command));
    printf("ftp> ");
    fflush(stdout);
    read_input(buf, size);
    char *arg;
    arg = strtok(buf, " ");
    arg = strtok(NULL, " ");
    if (arg != NULL) {
        arg_count++;
        strncpy(cmd->arg, arg, strlen(arg));
    }
    arg = strtok(NULL, " ");
    if (arg != NULL) {
        arg_count++;
        strcat(cmd->arg, " ");
        strncat(cmd->arg, arg, strlen(arg));
    }
    // 由于strtok分割后的字符串原字符串会变成第一个子串
    if (strcmp(buf, "list") == 0) {
        strcpy(cmd->code, "LIST");
    } else if (strcmp(buf, "get") == 0) {
        strcpy(cmd->code, "GETS");
    } else if (strcmp(buf, "put") == 0) {
        strcpy(cmd->code, "PUTS");
    } else if (strcmp(buf, "quit") == 0) {
        strcpy(cmd->code, "QUIT");
    } else if (strcmp(buf, "cd") == 0) {
        strcpy(cmd->code, "CHEN");
    } else {
        return -1; //不合法
    }
    memset(buf, 0, size);
    strcpy(buf, cmd->code);

    //入如果还有参数 追加
    if (arg_count != 0) {
        strcat(buf, " ");
        strncat(buf, cmd->arg, strlen(cmd->arg));
    }
    return 0;
}

int ftp_client_list(int data_sock) {
    int rsize, tmp;
    char buffer[1024] = {0};
    //等待服务器启动信息
    if (recv(sock_control, (void *)&tmp, sizeof(tmp), 0) < 0) {
        perror("reading massage from server\n");
        return -1;
    }

    while ((rsize = recv(data_sock, buffer, sizeof(buffer), 0)) > 0) {
        printf("%s", buffer);
        bzero(buffer, sizeof(buffer));
    }
    if (rsize < 0) {
        perror("recv");
        return -1;
    }
    //等待服务器完成的消息
    if (recv(sock_control, (void *)&tmp, sizeof(tmp), 0) < 0) {
        perror("reading massage from server\n");
        return -1;
    }
    return 0;
}

int ftp_client_get(int data_sock, char *path) {
    char *ind = rindex(path, '/');
    char name[100] = {0};
    char dir[100] = ".";
    if (ind == NULL) {
        strcpy(name, path);
    } else {
        strcpy(name, ind + 1);
    }
    if (recv_file_from_socket(data_sock, name, dir) < 0) {
        return -1;
    }
    log(GREEN"<Success>"NONE" : file transfer success!\n");
    return 0;
}

int ftp_client_put(int data_sock, char *arg) {
    char path[100] = {0};
    char buffer[256] = {0};
    strcpy(buffer,arg);
    //对于客户端忽略PUT的第二个参数
    char *ind;
    ind = rindex(buffer, ' ');
    buffer[ind - buffer] = '\0';
    getcwd(path, sizeof(path));
    strcat(path, "/");
    strcat(path, buffer);
    //printf("path = %s\n", path);
    if (access(path, F_OK) != 0) {
        printf("文件不存在\n");
        return -1;
    }

    if (send_file_to_socket(path, data_sock) < 0) {
        perror("send_file_to_socket");
        return -1;
    }
    return 0;
}

int ftp_client_cd(int data_sock, char *path) {
    int tmp;
    if (recv(sock_control, (void *)&tmp, sizeof(tmp), 0) < 0) {
        perror("reading massage from server\n");
        return -1;
    }
    if (tmp == 1) {
        return 0;
    }
    print_reply(tmp);
}

int main(int argc, char **argv) {
    //if (argc != 3) {
    //    printf("argc count!\n");
    //    exit(0);
    //}
    int retcode, data_sock;
    //char *host = argv[1]; // 服务器主机名
    //int port = atoi(argv[2]); //端口
    char host[15] = "8.140.160.253"; // 服务器主机名
    int port = 65000; //端口

    if ((sock_control = socket_connect(host, port)) < 0) {
        perror("socket_connect");
        exit(1);
    }

    //链接成功
    log(GREEN"<Success>"NONE" : Connected to %d.\n", port);
    print_reply(read_reply());

    //获取用户名和密码 并验证登录
    ftpclient_login();

    while (1) {
        char buffer[512] = {0};
        char *ind;
        struct command cmd;
        bzero(&cmd, sizeof(cmd));
        memset(buffer, 0, sizeof(buffer));
        //获取用户输入的命令
        if (ftpclient_read_cmd(buffer, sizeof(buffer), &cmd) < 0) {
            log(RED"<Error>"NONE" : Invalid commend!\n");
            continue;
        }
        //printf("buffer = %s\n", buffer);
        //发送命令到服务器
        if (send(sock_control, buffer, strlen(buffer), 0) < 0) {
            perror("sned");
            fflush(stdout);
            close(sock_control);
            exit(1);
        }

        retcode = read_reply();
        if (retcode == 221) { //退出命令
            print_reply(retcode);
            break;
        }
        if (retcode == 502) {
            printf("%d Invalid commend.\n", retcode); //不合法的输入
            continue;
        } else {
            //命令合法 打开数据链接
            if ((data_sock = socket_connect(IP, PORT)) < 0) {
                perror("socket_connect");
                exit(1);
            }
            //执行命令
            if (strcmp(cmd.code, "LIST") == 0) {
                ftp_client_list(data_sock);
            } else if (strcmp(cmd.code, "GETS") == 0) {
                if (read_reply() == 550) {
                    print_reply(550);
                    close(data_sock);
                    continue;
                }
                ftp_client_get(data_sock, cmd.arg);
                print_reply(read_reply());
            } else if (strcmp(cmd.code, "PUTS") == 0) {
                if (read_reply() == 550) {
                    print_reply(550);
                    close(data_sock);
                    continue;
                }
                ftp_client_put(data_sock, cmd.arg);
                close(data_sock);
                print_reply(read_reply());
                continue;
            } else if (strcmp(cmd.code, "CHEN") == 0) {
                ftp_client_cd(data_sock, cmd.arg);
            }
            close(data_sock);
        }
    }
    close(sock_control);
    return 0;
}

