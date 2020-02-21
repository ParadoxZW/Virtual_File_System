/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 程序基本流程与老师ppt的流程图一致
 */
int do_read(int fd, int len, char *text){
    int fp = openfilelist[fd].count;
    int end = fp + len;
    int flen = getlen(fd);
    if(flen < end){
        printf("file size is smaller than %dB, we print the whole file:\n\n", len);
        len = flen - fp;
        // return -1;
    }
    int len_ = len; // for return
    int sdno = fp / 1024;
    int soff = fp%1024;

    int id = nextnfat(getfirst(fd), sdno); // 磁盘块号
    len += soff;
    int k = sdno; // 逻辑磁盘块号
    int max, min, copylen; //逻辑值，赋值[min, max)，copylen = max - min
    char *cp; // 指向内存中数据块 
    char *txtp = text; // text中的写入位置

    while (len > 0){
        if (openfilelist[fd].mem[k++] == NULL){ // 块不在内存
            loadblock(id);
            block *p = (block *)malloc(sizeof(block));
            *p = buffer;
            // fcb *fcbs = (fcb *)p;
            openfilelist[fd].mem[k - 1] = p; // 载入内存
            cp = (char *)p;
        }else{
            cp = (char *)(openfilelist[fd].mem[k - 1]);
        }
        max = ((len <= 1024) ? len : 1024);
        min = (k - 1 != sdno) ? 0: soff;
        copylen = max - min;
        memcpy(txtp, cp + min, copylen);
        txtp += copylen;
        len -= 1024;
        id = memfat[id];
    }
    *txtp = '\0';
    openfilelist[fd].count = end;
    return len_;
}