/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 mkdir x/s
 *         程序基本流程与老师ppt的流程图一致
 */
void my_mkdir(char *dirname) {
    // 先判断打开文件数
    char *p[MAXOPENFILE*2], *tmp;
    p[0] = strtok(dirname, "/");
    tmp = p[1] = strtok(NULL, "/");
    int j = 2;
    while (tmp){
        tmp = p[j++] = strtok(NULL, "/");
    }
    j -= 1; // 需打开文件数
    if (j > left){
        printf("The recursion of dirs are too large\n");
        return;
    }

    // 读取父目录信息
    int lastfd;
    int ret = chainopen(p, j - 1, 0, &lastfd); // 第二个参数： 打开几个
    if(ret != 0){
        printf("Can not find %s\n", p[j - 1 - ret]);
        chainclose(lastfd, currfd);
        return;
    }

    int tmpp;
    unsigned short tmps;
    if (findsubfile(p[j - 1], lastfd, 0, &tmpp, &tmps)){
        printf("the file is already existed\n");
        chainclose(lastfd, currfd);
        return;
    }

    // 申请空目录项
    int lblk, off;
    int iret = getitem(lastfd, &lblk, &off);
    if (iret == -1){
        printf("blocks are full, when enlarge father dirfile\n");
        chainclose(lastfd, currfd);
        return;
    }

    // 申请一个磁盘块
    unsigned short data_nblk = get_free_block(0);
    if (data_nblk == END){
        printf("blocks are full\n");
        chainclose(lastfd, currfd);
        return;
    }

    // 制作一个fcb
    fcb fcbtmp;
    time_t rawtime = time(NULL);
    struct tm *time = localtime(&rawtime);
    fcbtmp.date = (time->tm_year - 100) * 512 + (time->tm_mon + 1) * 32 + (time->tm_mday);
    fcbtmp.time = (time->tm_hour) * 2048 + (time->tm_min) * 32 + (time->tm_sec) / 2;
    strcpy(fcbtmp.filename, p[j - 1]);
    strcpy(fcbtmp.truename, p[j - 1]);
    strcpy(fcbtmp.dir, openfilelist[lastfd].thisfcb.dir);
    strcat(fcbtmp.dir, "/");
    strcat(fcbtmp.dir, p[j-1]);
    fcbtmp.type = 0;
    fcbtmp.first = data_nblk;
    fcbtmp.free = USED;
    fcbtmp.length = 2 * sizeof(fcb); // . and ..

    // 申请一个打开目录表项
    int sfd = get_next_fd(lastfd);
    openfilelist[sfd].firno = nextnfat(getfirst(lastfd), lblk);
    openfilelist[sfd].thisfcb = fcbtmp;
    openfilelist[sfd].dirno = lblk;
    openfilelist[sfd].diroff = off;
    openfilelist[sfd].count = 0;
    openfilelist[sfd].fcbstate = 1;
    openfilelist[sfd].topenfile = 1;
    left--;

    // 设置 . 和 .. 目录
    block *nbp = (block *)malloc(sizeof(block));
    fcb *fcbs = (fcb *)nbp;
    fcbs[0] = fcbtmp;
    strcpy(fcbs[0].filename, ".");
    fcbs[1] = openfilelist[lastfd].thisfcb;
    strcpy(fcbs[1].filename, "..");

    openfilelist[sfd].mem[0] = nbp;

    // 修改父目录 fcb
    int dirno_, diroff_, flag = 0, gfd;
    ((fcb *)openfilelist[lastfd].mem[lblk])[off >> 6] = fcbtmp;
    if (iret == 2 || iret == 3){ // 是否需要修改length
        ((fcb *)openfilelist[lastfd].mem[0])[0].length += sizeof(fcb);
        if (getfirst(lastfd) != 5){ // not root，需要更改祖父文件
            dirno_ = openfilelist[lastfd].dirno;
            diroff_ = openfilelist[lastfd].diroff;
            if (lastfd != currfd) { // 如果父目录文件不是当前路径文件夹，则祖父目录文件夹一定是打开状态
                gfd = openmanage[lastfd];
                ((fcb *)openfilelist[gfd].mem[dirno_])[diroff_ >> 6].length += sizeof(fcb);
                openfilelist[gfd].fcbstate = 1;        
            }else { // 祖父文件夹需要更改且不在内存
                flag = 1;
            }
        }else{ // .. of root 
            ((fcb *)openfilelist[lastfd].mem[0])[1].length += sizeof(fcb);
        }
    }
    openfilelist[lastfd].fcbstate = 1;

    // 祖父文件夹需要更改且不在内存
    if(flag){
        int firno = openfilelist[lastfd].firno;
        int diroff = openfilelist[lastfd].diroff;
        loadblock(firno);
        ((fcb *)(&buffer))[diroff >> 6].length += sizeof(fcb);
        saveblock(firno);
    }
    curr_flush();
    // 关闭文件
    chainclose(sfd, currfd);
}