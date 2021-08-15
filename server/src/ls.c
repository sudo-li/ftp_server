/*************************************************************************
    > File Name: 2.ls.c
    > Author: LYX
    > Mail: 
    > Created Time: Sat 03 Jul 2021 06:37:40 PM CST
 ************************************************************************/

#include "head.h"
#include "ls.h"
//#include "datatype.h"
#include "common.h"

static int a_flag;
static int l_flag;
static int file_flag; //文件还是目录



void get_file_mode(int mode, char *buff) {
    // 第一字段
    char tmp_mode[3] = {'r', 'w', 'x'};
    switch (mode & S_IFMT) {
        case S_IFBLK: 
            strcat(buff, "b");
            break;
        case S_IFCHR:
            strcat(buff, "c");
            break;
        case S_IFDIR:
            strcat(buff, "d");
            break;
        case S_IFIFO:
            strcat(buff, "p");
            break;
        case S_IFLNK:
            strcat(buff, "l");
            break;
        case S_IFSOCK:
            strcat(buff, "s");
            break;
        default:
            strcat(buff, "-");
    }
    int cnt = -1;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            cnt++;
            if (!!(mode & (0x0100 >> cnt))) {
                strncat(buff, &tmp_mode[i], 1);    
            } else {
                strcat(buff, "-");
            }
        }
    }
    return ;
}

int get_digitlen (long int n) {
    char buf[BUFSIZ];
    snprintf(buf, BUFSIZ, "%ld", n);
    return strlen(buf);
}

int get_stringlen(char *str) {
    char buf[BUFSIZ];
    return sprintf(buf, "%s", str);
}

int cmpname(const void *s1, const void *s2) {
    char *name1 = ((file*)s1)->name;
    char *name2 = ((file*)s2)->name;
    if (strcmp(name1, ".") == 0 || strcmp(name2, ".") == 0) {
        return strcmp(name1, name2);
    }
    if (name1[0] == '.' && name2[0] == '.') {
        return strcasecmp(&name1[1], &name2[1]);
    } 
    if (name1[0] == '.') {
        return strcasecmp(&name1[1], name2);
    }
    if (name2[0] == '.') {
        return strcasecmp(name1, &name2[1]);
    }
    return strcasecmp(name1, name2);
}

int sort_dir(dir *dp, int (*cmp)(const void *, const void *)) {
    if (dp == NULL || dp->filep == NULL) return -1;
    for (int i = 0; i < dp->cnt; i++) { // 寻找可能存在的子目录
        if (dp->filep[i].dirp) {
            sort_dir(dp->filep[i].dirp, cmp);
        }
    }
    qsort(dp->filep, dp->cnt, sizeof(file), cmp);
    return 0;
}

int free_dirs(dir *dp) {
    if (dp == NULL || dp->filep == NULL) return -1;
    for (int i = 0; i < dp->cnt; i++) {
        free_dirs(dp->filep[i].dirp);
    }
    free(dp->filep);
    free(dp);
    return 0;
}

void copyinfo(file *fp, const struct stat *sp, const char *pathname) {
    fp->dirp = NULL;
    fp->ino = sp->st_ino;
    fp->uid = sp->st_uid;
    fp->gid = sp->st_gid;
    fp->dev = sp->st_rdev;
    fp->mode = sp->st_mode;
    fp->size = sp->st_size;
    fp->atime = sp->st_atime;
    fp->mtime = sp->st_mtime;
    fp->ctime = sp->st_ctime;
    fp->links = sp->st_nlink;
    fp->length = strlen(pathname);
    strcpy(fp->name, pathname);
}

int getfile_time(int atime, char *buff) {
    char time[40] = {0};
    char *sub = NULL;
    strcpy(time, ctime((time_t*)(&atime)));

    sub = strtok(time, " ");
    sub = strtok(NULL, " ");
    int rsize = sprintf(buff, "%4s", sub);
    sub = strtok(NULL, " ");
    rsize += sprintf(buff + rsize, "%3s", sub);
    sub = strtok(NULL, " ");
    rsize += sprintf(buff + rsize, "%9s", sub);
    sub = strtok(NULL, ":");
    rsize += sprintf(buff + rsize, "%s", " ");
    return rsize;
}

int get_file_info(dir *dp, char *buff) {
    int rsize = 0;
    char buf[10] = {0};
    char tmp[BUFSIZ] = {0};
    
    if (dp == NULL || dp->filep == NULL) {
        return -1;
    }
    if (dp->cnt == 0) {
        printf("\n");
        return 0;
    }
    for (int i = 0; i < dp->cnt; i++) {
        memset(buf, 0, sizeof(buf));
        get_file_mode(dp->filep[i].mode, buf);
        char *username = getpwuid(dp->filep[i].uid)->pw_name;
        char *groupname = getgrgid(dp->filep[i].gid)->gr_name;
        rsize += sprintf(buff + rsize, "%s %-*d %-*s %-*s ", buf, dp->links_max_wid, dp->filep[i].links, dp->user_max_wid, username, dp->group_max_wid, groupname);
        if (S_ISCHR(dp->filep[i].mode) || S_ISBLK(dp->filep[i].mode)) {
            snprintf(tmp, BUFSIZ, "%d,%d", major(dp->filep[i].dev), minor(dp->filep[i].dev));
            rsize += sprintf(buff + rsize, "%-*s ", dp->size_max_wid, tmp);
        } else {
            rsize += sprintf(buff + rsize, "%-*ld ", dp->size_max_wid, (long int)dp->filep[i].size);
        }
        rsize += getfile_time(dp->filep[i].atime, buff + rsize);
        rsize += sprintf(buff + rsize, "%s\n", dp->filep[i].name);
    }
    return rsize;
}

dir *open_dir(const char *pathname) {
    int w = 0;
    dir *dirp;
    file *filep;
    DIR *dp;

    struct stat stbuf;
    struct stat lstbuf;
    struct dirent *direntp;
    char name[MAX_NAME_LEN + 1] = {0};
    char abspath[500] = {0};
    char link_path[50] = {0};

    if ((lstat(pathname, &stbuf)) == -1) {
        perror("stat");
        return NULL;
    }
    if ((dirp = malloc(sizeof(dir))) == NULL) {
        perror("malloc");
        return NULL;
    }
    memset(dirp, 0, sizeof(dir));
    if ((dirp->filep = malloc(sizeof(file))) == NULL) {
        perror("malloc");
        free(dirp);
        return NULL;
    }
    if (!S_ISDIR(stbuf.st_mode)) { //如果是文件
        file_flag = 1;
        dirp->cnt = 1;
        copyinfo(dirp->filep, &stbuf, pathname);
        return dirp;
    }
    
    if ((dp = opendir(pathname)) == NULL) {
        free(dirp->filep);
        free(dirp);
        perror("opendir");
        return NULL;
    }
    for (dirp->cnt = 0; direntp = readdir(dp); dirp->cnt++) {
        if (!a_flag) {
            if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {
                dirp->cnt--;
                continue;
            }
        }
        if ((filep = realloc(dirp->filep, (dirp->cnt + 1) * sizeof(file))) == NULL) {
            perror("realloc");
            free_dirs(dirp);
            closedir(dp);
            return NULL;
        }
        dirp->filep = filep;
        sprintf(abspath, "%s/%s", pathname, direntp->d_name);
        if (lstat(abspath, &stbuf) == -1) {
            perror("lstat");
            free_dirs(dirp);
            closedir(dp);
            return NULL;
        }
        copyinfo(dirp->filep + dirp->cnt, &stbuf, direntp->d_name);
        if (S_ISLNK(stbuf.st_mode) && l_flag) {
            int n = readlink(abspath, link_path, sizeof(link_path) - 1);
            strcat((dirp->filep + (dirp->cnt))->name, " -> ");
            strncat((dirp->filep + (dirp->cnt))->name, link_path, n);
        }
        /* 获取最长的links*/
        if ((w = get_digitlen(dirp->filep[dirp->cnt].links)) > dirp->links_max_wid) {
            dirp->links_max_wid = w;
        }
        /* 获取最长的用户名 */
        if ((w = get_stringlen(getpwuid(dirp->filep[dirp->cnt].uid)->pw_name))\
            > dirp->user_max_wid) {
            dirp->user_max_wid = w;
        }
        /* 获取最长的组名 */
        if ((w = get_stringlen(getgrgid(dirp->filep[dirp->cnt].gid)->gr_name))\
            > dirp->group_max_wid) {
            dirp->group_max_wid = w;
        }
        if (S_ISCHR(stbuf.st_mode) || S_ISBLK(stbuf.st_mode)) {
            w = get_digitlen(major(dirp->filep[dirp->cnt].dev));
            w += get_digitlen(minor(dirp->filep[dirp->cnt].dev)) + 1; //+1因为中间有,
            if (w > dirp->size_max_wid) dirp->size_max_wid = w;
        } else {
            if ((w = get_digitlen(dirp->filep[dirp->cnt].size)) > dirp->size_max_wid) {
                dirp->size_max_wid = w;
            }
        }
    }
    closedir(dp);
    return dirp;
}

int ls_to_file() {
    dir *dp;
    l_flag = 1;
    chdir("/home/ftp_server");

    int fd;
    if ((fd = open("/home/ftp_server/server/tmp.txt", O_CREAT | O_WRONLY, 0644)) < 0) {
        perror("open");
        return -1;
    }

    if ((dp = open_dir("."))) {
        sort_dir(dp, cmpname);
        int rsize = get_file_info(dp, ls_buff);
        //printf("%s", ls_buff);
        ssize_t ret = write(fd, ls_buff, rsize);
        if (ret != rsize) {
            perror("write");
            return -1;
        }
        free_dirs(dp);
        close(fd);
    } else {
        close(fd);
        return -1;
    }
    return 0;
}


