/**
 * @author: Zhenwei_Shao
 * @date: 2019/10/20
 * @brief: 数据结构定义和全局变量定义
 */
#ifndef INIT_H
#define INIT_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BLOCKSIZE 1024
#define SIZE 1024000
#define BLOCKNUM 1000
#define END 65535 // FFFF
#define FREE 0    // 0000
#define USED 1
#define ROOTBLOCKNUM 1
#define MAXOPENFILE 10 // 同时打开最大文件数
#define MAX_TEXT_SIZE 10000

typedef struct FCB{
    char filename[8];   // 文件名 或 . 或 ..
    char truename[8];   // 为 .或.. 时使用
    char dir[36];       // 完整路径
    char type;          // 类型，看71行
    unsigned short time;
    unsigned short date;
    unsigned short first; // 文件起始盘块号
    unsigned short length;
    char free;
} fcb;  // 64B, 一个block 16个表项

typedef unsigned short fat;

typedef struct BLOCK0{ //超级块中的内容，简化了。。。
    char magic_number[8];
    char information[200];
} block0;

typedef struct BLOCK{ // 磁盘块
    char unit[1024];
} block;

typedef struct USEROPEN{
    fcb thisfcb;              // 指向本文件的fcb
    int firno;                 // 父目录文件物理盘块号
    int dirno;                 // 父目录文件逻辑盘块号
    int diroff;                // 该文件对应的 fcb 在父目录中的逻辑序号
    int count;                 // 读写指针在文件中的位置
    char fcbstate;             // 是否修改 1是 0否
    char topenfile;            // 0: 空 openfile
    block *mem[1000];          // todo: 初始值
} useropen;

useropen openfilelist[MAXOPENFILE];
int left = MAXOPENFILE; // 全局变量，剩余useropen
int openmanage[MAXOPENFILE]; // 用于管理openfilelist
char cwd[MAX_TEXT_SIZE]; // 当前路径

block vdisk[1000]; // 模拟磁盘块，文件系统的代码不直接操作该数据
block buffer; // 文件系统只能从buffer拿到磁盘块数据
fat memfat[1024]; // 磁盘中fat表读入内存后放置于此

int currfd;         // 当前目录文件在打开文件列表中的序号 
char *typeset[2] = {"dir", "data"};
/* vdisk simulator */
int prepare_vdisk();
void close_vdisk();
void loadblock(unsigned short id);
void saveblock(unsigned short id);

/* sysfile func */
void startsys();
void my_format();
void curr_flush();
unsigned short getlen(int fd);
unsigned short getfirst(int fd);
char gettype(int fd);
fcb *fd_to_fcb(int fd);
void my_cd(char *dirname);
int do_read(int fd, int len, char *text);
int do_write(int fd, char *text, int len, char wstyle);
int my_write(int fd);
void exitsys();
void my_cd(char *dirname);
int my_open(char *filename);
int my_close(int fd);
void my_mkdir(char *dirname);
void my_rmdir(char *dirname);
void my_create(char *filename);
void my_rm(char *filename);
void my_ls();
int opensubfile(char *filename, int openfd, char type_);
int get_next_fd(int pre);
int findsubfile(char *filename, int openfd, char type_, int *dirno, unsigned short *off);
int chainopen(char *p[], int need, char lasttype, int *fd);
unsigned short nextnfat(unsigned short begin, unsigned short nth);
int getitem(int openfd,  int *lblk, int *noff);
void chainclose(int sfd, int tfd);
void update_father(int lastfd);
int findopened(char *name, char type);
int my_read(int fd, int len);
unsigned short get_free_block(unsigned short pre);

#include "vdisk/vdisk_api.c"
#include "slim_filesys/utils.c"
#include "slim_filesys/chainclose.c"
#include "slim_filesys/chainopen.c"
#include "slim_filesys/do_read.c"
#include "slim_filesys/do_write.c"
#include "slim_filesys/exitsys.c"
#include "slim_filesys/startsys.c"
#include "slim_filesys/findsubfile.c"
#include "slim_filesys/my_cd.c"
#include "slim_filesys/my_close.c"
#include "slim_filesys/my_create.c"
#include "slim_filesys/my_format.c"
#include "slim_filesys/my_ls.c"
#include "slim_filesys/my_mkdir.c"
#include "slim_filesys/my_open.c"
#include "slim_filesys/my_read.c"
#include "slim_filesys/my_rm.c"
#include "slim_filesys/my_rmdir.c"
#include "slim_filesys/my_write.c"
#include "slim_filesys/opensubfile.c"
#endif