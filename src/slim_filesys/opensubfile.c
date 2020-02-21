/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 功能与findsubfile类似，不同点在于会打开filename，并返回fd
 */
int opensubfile(char *filename, int openfd, char type_){
    int len = getlen(openfd);
    unsigned short max = ((len <= 1024) ? len : 1024) >> 6;
    unsigned short id = getfirst(openfd);
    int k = 0, fd;
    fcb *fcbs;
    block *p, *sp;
    while (len > 0){
        if (openfilelist[openfd].mem[k++] == NULL){ // 块不在内存
            loadblock(id);
            p = (block *)malloc(sizeof(block));
            *p = buffer;
            fcbs = (fcb *)p;
            openfilelist[openfd].mem[k - 1] = p; // 载入内存
        }else{ // 块在内存
            fcbs = (fcb *)(openfilelist[openfd].mem[k - 1]);
        }
        int i = 0;
        for (i; i < max;i++){
            if (fcbs[i].free == USED && strcmp(fcbs[i].filename, filename) == 0 && fcbs[i].type == type_) {
                fd = get_next_fd(openfd);
                openfilelist[fd].firno = id;
                openfilelist[fd].dirno = k - 1;
                openfilelist[fd].diroff = i<<6;
                openfilelist[fd].count = 0;
                openfilelist[fd].fcbstate = 0;
                openfilelist[fd].topenfile = 1;
                loadblock(fcbs[i].first);
                sp = (block *)malloc(sizeof(block));
                *sp = buffer;
                openfilelist[fd].mem[0] = sp;
                if (fcbs[i].type == 0 && strcmp(filename, "..") != 0 && getlen(openfd) != ((fcb *)sp)[1].length){
                    ((fcb *)sp)[1].length = getlen(openfd);
                }

                openfilelist[fd].thisfcb = fcbs[i];
                left--;
                return fd;
            }
        }
        len -= 1024;
        id = memfat[id];
        max = ((len <= 1024) ? len : 1024) >> 6;
    }
    return 10; // not find
}