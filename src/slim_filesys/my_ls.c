/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 ls
 *         程序基本流程与老师ppt的流程图一致
 */
void my_ls() {
    unsigned short i = 0;
    int len = getlen(currfd);

    // 打印第一块，第一块在内存中
    unsigned short max = ((len <= 1024) ? len : 1024) >> 6;
    printf("FileName\tType\t        Large(B)\tTime\n");
    fcb *fcbs = fd_to_fcb(currfd);
    for (i = 0; i < max; i++){
        if (fcbs[i].free == USED)
            printf("%-8s\t%-8s\t%-8d\t%d/%d/%d %d:%d\n",
                   fcbs[i].filename,
                   typeset[fcbs[i].type],
                   fcbs[i].length,
                   (fcbs[i].date >> 9) + 2000,
                   (fcbs[i].date >> 5) & 0x000f,
                   (fcbs[i].date) & 0x001f,
                   (fcbs[i].time >> 11),
                   (fcbs[i].time >> 5) & 0x003f);
    }

    // 打印剩余目录项，剩余目录项可能在内存也可能不在
    len -= 1024;
    unsigned short id = getfirst(currfd);
    id = memfat[id];
    int k = 1;
    while (len > 0){
        if (openfilelist[currfd].mem[k++] == NULL){
            loadblock(id);
            block *p = (block*) malloc(sizeof(block));
            *p = buffer;
            fcb *fcbs = (fcb *)p;
            openfilelist[currfd].mem[k - 1] = p;
        }else{
            fcb *fcbs = (fcb *)openfilelist[currfd].mem[k - 1];
        }
        max = ((len <= 1024) ? len : 1024) >> 6;
        for (i=0; i < max; i++) {
            if (fcbs[i].free == USED)
                printf("%-8s\t%-8s\t%-8d\t%d/%d/%d %d:%d\n",
                       fcbs[i].filename,
                       typeset[fcbs[i].type],
                       fcbs[i].length,
                       (fcbs[i].date >> 9) + 2000,
                       (fcbs[i].date >> 5) & 0x000f,
                       (fcbs[i].date) & 0x001f,
                       (fcbs[i].time >> 11),
                       (fcbs[i].time >> 5) & 0x003f);
        }
        len -= 1024;
        id = memfat[id];
    }
}