/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 初始化文件系统（虚拟磁盘通过prepare_vdisk打开），程序基本流程与老师ppt的流程图一致
 */
void startsys(){
    int state = prepare_vdisk();
    if(state == -1) { // 文件不存在
        my_format();
    }

    openfilelist[0].firno = 5;
    openfilelist[0].dirno = 1;
    openfilelist[0].diroff = 0;
    openfilelist[0].count = 0;
    openfilelist[0].fcbstate = 0;
    openfilelist[0].topenfile = 1;
    left--;
    currfd = 0;

    // 第一块读入内存
    loadblock(5); // load first root block -> buff
    block *p = (block *)malloc(sizeof(block));
    *p = buffer;
    openfilelist[0].mem[0] = p;
    openfilelist[0].thisfcb = ((fcb *)p)[0];
    strcpy(openfilelist[0].thisfcb.filename,openfilelist[0].thisfcb.truename);

    // 将fat表载入内存
    loadblock(1);
    memcpy(memfat, &buffer, sizeof(block));
    loadblock(2);
    memcpy(memfat + 512, &buffer, sizeof(block));
    return;
}