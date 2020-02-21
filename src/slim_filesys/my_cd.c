/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 cd x/s // s必须为dir类型
 *         支持path中包含.. 如 ../..
 *         程序基本流程与老师ppt的流程图一致
 */
void my_cd(char *dirname){
    // 路径分段，并判断需要打开的文件个数，需小于剩余useropen数量
    char *p[MAXOPENFILE << 1], *tmp;
    p[0] = strtok(dirname, "/");
    tmp = p[1] = strtok(NULL, "/");
    int j = 2;
    while (tmp){
        tmp = p[j++] = strtok(NULL, "/");
    }
    j -= 1; // 需打开文件数
    if (j > left) {
        printf("The recursion of path is larger than the amount of left useropen\n");
        return;
    }

    // 打开父目录文件
    int lastfd;
    int ret = chainopen(p, j - 1, 0, &lastfd); // 递归打开前j-1个文件夹（具体查看该函数的定义文件）
    if(ret != 0){
        printf("Can not find %s\n", p[j - 1 - ret]);
        chainclose(lastfd, currfd);
        return;
    }

    // 文件存在否
    int tarfd;
    if ((tarfd = opensubfile(p[j - 1], lastfd, 0)) == 10){
        printf("the file is not existed\n");
        chainclose(lastfd, currfd);
        return;
    }

    int old = currfd;
    currfd = tarfd;
    strcpy(cwd, openfilelist[currfd].thisfcb.dir);

    chainclose(lastfd, old); // 关闭临时打开的路径里的文件夹
    my_close(old);
    return;
}