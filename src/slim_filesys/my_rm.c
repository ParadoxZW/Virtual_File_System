/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 rm x/s // s必须为data类型
 *         程序基本流程与老师ppt的流程图一致
 */
void my_rm(char *filename){
    char *p[MAXOPENFILE * 2], *tmp;
    p[0] = strtok(filename, "/");
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

    // 打开父目录文件
    int lastfd;
    int ret = chainopen(p, j-1, 0, &lastfd); // 递归打开前j-1个文件夹，0是类型，将打开的文件号写入lastfd（具体查看该函数的定义文件）
    if(ret != 0){
        printf("Can not find %s\n", p[j - 1 - ret]);
        chainclose(lastfd, currfd);
        return;
    }

    // 文件存在否
    int tarfd, dirno;
    unsigned short off;
    if((tarfd = findopened(p[j-1], 1)) != -1){ // 文件已被打开
        dirno = openfilelist[tarfd].dirno;
        off = openfilelist[tarfd].diroff;
        my_close(tarfd);
    }else{
        if (!findsubfile(p[j - 1], lastfd, 1, &dirno, &off)){
            printf("the file (%s) is not existed\n", p[j - 1]);
            chainclose(lastfd, currfd);
            return;
        }
    }

    // 更新 fat 表
    fcb *fcbptr = ((fcb *)openfilelist[lastfd].mem[dirno]) + (off >> 6);
    unsigned short block_num = fcbptr->first;
    int nxt_num = 0;
    while (1)
    {
        nxt_num = memfat[block_num];
        memfat[block_num] = FREE;
        if (nxt_num != END)
            block_num = nxt_num;
        else
            break;
    }

    // 清空 fcb
    fcbptr->free = FREE;
    // openfilelist[lastfd].fcbstate = 1;

    // 更新lastfd的length 和 lastfd的父目录
    if ((dirno << 10) + off + BLOCKSIZE == getlen(lastfd))
        update_father(lastfd);
}