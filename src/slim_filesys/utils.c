/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 一些辅助工具函数
 */
void curr_flush(){
    openfilelist[currfd].thisfcb = fd_to_fcb(currfd)[0];
    fcb *tmp = &(openfilelist[currfd].thisfcb);
    strcpy(tmp->filename, tmp->truename);
}

unsigned short getlen(int fd){
    return openfilelist[fd].thisfcb.length;
}

unsigned short getfirst(int fd){
    return openfilelist[fd].thisfcb.first;
}


char gettype(int fd){
    return openfilelist[fd].thisfcb.type;
}

fcb *fd_to_fcb(int fd){
    /**
     * 从 . 文件获得文件的fcb指针
     */
    return (fcb *)(openfilelist[fd].mem[0]); // . item
}

int get_next_fd(int pre){
    int fd = 0, tarfd;
    while (fd < 10)
    {
        if(!openfilelist[fd].topenfile){
            tarfd = fd;
            break;
        }
        fd++;
    }
    openmanage[tarfd] = pre;
    return tarfd;
}

unsigned short nextnfat(unsigned short begin, unsigned short nth){
    /**
     * 获得从begin开始的第nth个block的id，begin算第0个
     */
    int id = begin;
    while(nth--){
        id = memfat[id];
        if (id == END){
            break;
        }
    }
    return id;
}

int getitem(int openfd,  int *lblk, int *noff){
    /**
     * 申请一个空目录项，如果目录项用完，申请新block
     */
    int len = getlen(openfd);
    int k = 0, max; // logical
    unsigned short id = END;
    fcb *fcbs;

    int i = 0;
    while (len > 0){
        id = (id != END) ? id = memfat[id] : getfirst(openfd);
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
        max = ((len <= 1024) ? len : 1024) >> 6;
        for (i = 0; i < max; i++){
            if (fcbs[i].free != USED){
                *noff = i<<6;
                *lblk = k - 1;
                return 1;
            }
        }
        len -= 1024;
    }
    if(len != 0){
        *noff = i << 6;
        *lblk = k - 1;
        return 2;
    }
    id = get_free_block(id);
    if (id == END) {
        return -1;
    }
    *noff = 0;
    *lblk = k;
    return 3;
}

void update_father(int lastfd){
    // 更新ffd的length 和 ffd的父目录
    int flen = getlen(lastfd), dirno_, diroff_, gfd, flag = 0;
    int ii = flen / 1024;
    int jj = ((flen % 1024)>>6) - 1;
    for(ii;ii>=0;ii--){
        for(jj;jj>=0;jj--){
            if(((fcb *)openfilelist[lastfd].mem[ii])[jj].free == 0){
                flen -= sizeof(fcb);
            }else{
                if(((fcb *)openfilelist[lastfd].mem[0])[0].length != flen){
                    ((fcb *)openfilelist[lastfd].mem[0])[0].length = flen;
                    if (getfirst(lastfd) != 5){
                        dirno_ = openfilelist[lastfd].dirno;
                        diroff_ = openfilelist[lastfd].diroff;
                        if (lastfd != currfd){
                            gfd = openmanage[lastfd];
                            ((fcb *)openfilelist[gfd].mem[dirno_])[diroff_ >> 6].length = flen;
                            openfilelist[gfd].fcbstate = 1;        
                        }else{
                            flag = 1;
                        }
                    }else{ // .. of root 
                        ((fcb *)openfilelist[lastfd].mem[0])[1].length = flen;
                    }
                }
                openfilelist[lastfd].fcbstate = 1;
                if(flag){
                    int firno = openfilelist[lastfd].firno;
                    int diroff = openfilelist[lastfd].diroff;
                    loadblock(firno);
                    ((fcb *)(&buffer))[diroff >> 6].length += sizeof(fcb);
                    saveblock(firno);
                }
                curr_flush();
                chainclose(lastfd, currfd);
                return;
            }
        }
        jj = 15;
        unsigned short id = nextnfat(getfirst(lastfd), ii - 1);
        memfat[memfat[id]] = FREE;
        memfat[id] = END;
    }
}

int findopened(char *name, char type) {
    int fd = 10;
    while (fd--){
        if(openfilelist[fd].topenfile && (strcmp(openfilelist[fd].thisfcb.truename, name) == 0)){
            return fd;
        }
    }
    return -1;
}

unsigned short get_free_block(unsigned short pre){ 
    int i;
    for (i = 0; i < BLOCKNUM; i++)
    {
        if (memfat[i] == FREE)
        {
            memfat[pre] = i;
            memfat[i] = END;
            return i;
        }
    }
    return END;
}