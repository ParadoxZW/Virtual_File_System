/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 rmdir x/s // s必须为dir类型
 *         程序基本流程与老师ppt的流程图一致
 */
void my_rmdir(char *dirname){
    // 排除 . 和 .. 目录
    if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
    {
        printf("can not remove . and .. special dir\n");
        return;
    }
    char *p[MAXOPENFILE*2], *tmp;
    p[0] = strtok(dirname, "/");
    tmp = p[1] = strtok(NULL, "/");
    int j = 2;
    while (tmp){
        tmp = p[j++] = strtok(NULL, "/");
    }
    j -= 1; // 需打开文件数
    if (j > left) {
        printf("The recursion of path is larger than the amount of left useropen.\n");
        return;
    }

    // 上一级
    int lastfd;
    int ret = chainopen(p, j-1, 0, &lastfd);
    if(ret != 0){
        printf("Can not find %s\n", p[j - 1 - ret]);
        chainclose(lastfd, currfd);
        return;
    }

    // 文件存在否
    int tarfd, dirno, diroff;
    if ((tarfd = opensubfile(p[j-1], lastfd, 0)) == 10){
        printf("the file is not existed\n");
        chainclose(lastfd, currfd);
        return;
    }

    // 检查是否为空
    if(getlen(tarfd) != 2 * sizeof(fcb)) {
        printf("dirfile is not empty\n");
        chainclose(tarfd, currfd);
        return;
    }

    dirno = openfilelist[tarfd].dirno;
    diroff = openfilelist[tarfd].diroff;
    int block_num = getfirst(tarfd);
    my_close(tarfd);

    // 更新 fat 表
    int nxt_num = 0;
    while (1) // TAT
    {
        nxt_num = memfat[block_num];
        memfat[block_num] = FREE;
        if (nxt_num != END)
        {
            block_num = nxt_num;
        }
        else
        {
            break;
        }
    }

    // 更新 fcb
    ((fcb *)openfilelist[lastfd].mem[dirno])[diroff >> 6].free = FREE;
    // openfilelist[lastfd].fcbstate = 1;

    if ((dirno << 10) + diroff + BLOCKSIZE == getlen(lastfd))
        update_father(lastfd); // 维护父目录length
}
