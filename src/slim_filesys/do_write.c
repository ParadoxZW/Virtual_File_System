/**
 * @author: Zhenwei_Shao
 * @date:   2019/12/27
 * @brief:  程序基本流程与老师ppt的流程图一致
 */
int do_write(int fd, char *text, int len, char wstyle){
    // 根据写入方式修改指针位置
    if (wstyle == 1){
        openfilelist[fd].count = 0;
    }else if(wstyle == 3){
        openfilelist[fd].count = (getlen(fd) - 1 > 0) ? getlen(fd) - 1 : 0;
    }
    // 计算写入所需信息
    int fp = openfilelist[fd].count;
    int end = fp + len;
    int len_ = end;
    int sdno = fp/1024;
    int soff = fp%1024;
    int id = nextnfat(getfirst(fd), sdno); // 磁盘块号
    len += soff;
    int k = sdno; // 逻辑磁盘块号
    int max, min, copylen; //逻辑值，赋值[min, max)，copylen = max - min
    char *cp; // 指向内存中数据块 
    char *txtp = text; // text中的写入位置

    while (len > 0){ // 开始写入
        if (openfilelist[fd].mem[k++] == NULL){ // 块不在内存
            loadblock(id);
            block *p = (block *)malloc(sizeof(block));
            *p = buffer;
            openfilelist[fd].mem[k - 1] = p; // 载入内存
            cp = (char *)p;
        }else{
            cp = (char *)(openfilelist[fd].mem[k - 1]);
        }
        // 计算写入内容的上下限
        min = (k - 1 != sdno) ? 0: soff;
        max = ((len + min) <= 1024) ? (len + min) : 1024;
        copylen = max - min;
        memcpy(cp + min, txtp, copylen);
        txtp += copylen;
        len -= 1024;
        if(id == END){
            id = get_free_block(id);
        }else{
            id = memfat[id];
        }
    }

    // 更新该文件的父文件夹的fcb信息
    openfilelist[fd].count = end;
    openfilelist[fd].thisfcb.length = len_;
    if(openmanage[fd] != currfd){ // 判断父文件夹是否在内存
        int firno = openfilelist[fd].firno;
        int diroff = openfilelist[fd].diroff;
        loadblock(firno);
        ((fcb *)(&buffer))[diroff >> 6].length = len_;
        saveblock(firno);
    }else{
        int dirno_ = openfilelist[fd].dirno;
        int diroff_ = openfilelist[fd].diroff;
        ((fcb *)(openfilelist[currfd].mem[dirno_]))[diroff_ >> 6].length = len_;
        openfilelist[currfd].fcbstate = 1;
    }
    return len_;
}
