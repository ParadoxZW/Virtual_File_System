/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 open x/s
 *         程序基本流程与老师ppt的流程图一致
 */
int my_open(char *filename){
    char *p[MAXOPENFILE<<1], *tmp;
    p[0] = strtok(filename, "/");
    tmp = p[1] = strtok(NULL, "/");
    int j = 2;
    while (tmp){
        tmp = p[j++] = strtok(NULL, "/");
    }
    j -= 1; // 需打开文件数
    if (j > left) {
        printf("The recursion of path is larger than the amount of left useropen\n");
        return 10;
    }

    // 打开父目录文件
    int lastfd;
    int ret = chainopen(p, j - 1, 0, &lastfd); // 递归打开前j-1个文件夹（具体查看该函数的定义文件）
    if(ret != 0){
        printf("Can not find %s\n", p[j - 1 - ret]);
        chainclose(lastfd, currfd);
        return 10;
    }

    // 文件存在否
    int tarfd;
    if((tarfd = findopened(p[j-1], 1)) != -1){
        printf("You can use fd: %d\n", tarfd);
        return tarfd;
    }else{
        if ((tarfd = opensubfile(p[j - 1], lastfd, 1)) == 10){
            printf("the file is not existed\n");
            chainclose(lastfd, currfd);
            return 10;
        }
    }

    chainclose(lastfd, currfd); // 关闭临时打开的路径里的文件夹
    printf("You can use fd: %d\n", tarfd);
    return tarfd;
}
