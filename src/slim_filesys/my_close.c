/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 关闭打开文件夹fd，对已经修改的文件，将数据块从内存写回虚拟硬盘区
 */
int my_close(int fd){
    if (fd > MAXOPENFILE || fd < 0)
    {
        printf("my_close: fd error\n");
        return -1;
    }

    if(openfilelist[fd].fcbstate){
        int i = 0;
        unsigned short id = getfirst(fd);
        while(id != END){
            if (openfilelist[fd].mem[i++] != NULL){
                buffer = *(openfilelist[fd].mem[i - 1]);
                saveblock(id);
                free(openfilelist[fd].mem[i - 1]);
            }
            id = memfat[id];
        }
    }
    memset(&openfilelist[fd], 0, sizeof(useropen));
    left++;
    return 0;
}