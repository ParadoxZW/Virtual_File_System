/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: openfd为一打开文件夹的fd，检查该文件夹下是否存在文件名为filename，类型为type_的文件
 *         将逻辑块号和块内偏移量写入dirno和off
 */
int findsubfile(char *filename, int openfd, char type_, int *dirno, unsigned short *off) {
    int len = getlen(openfd);
    unsigned short max = ((len <= 1024) ? len : 1024) >> 6;
    unsigned short id = getfirst(openfd);
    int k = 0;
    fcb *fcbs;
    while (len > 0){
        if (openfilelist[openfd].mem[k++] == NULL){ // 块不在内存
            loadblock(id);
            block *p = (block *)malloc(sizeof(block));
            *p = buffer;
            fcbs = (fcb *)p;
            openfilelist[openfd].mem[k - 1] = p; // 载入内存
        }
        else{ // 块在内存
            fcbs = (fcb *)(openfilelist[openfd].mem[k - 1]);
        }
        int i = 0;
        for (i; i < max; i++){
            if (fcbs[i].free == USED && strcmp(fcbs[i].filename, filename) == 0 && fcbs[i].type == type_){
                *dirno = k - 1;               
                *off = i<<6;
                return 1;
            }
        }
        len -= 1024;
        id = memfat[id];
        max = ((len <= 1024) ? len : 1024) >> 6;
    }
    return 0; // not find
}