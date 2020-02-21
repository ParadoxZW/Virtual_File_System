/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 关闭所有打开文件（对已经修改的文件，my_close会负责将数据块从内存写回虚拟硬盘区）
 *         讲内存中的fat表写回虚拟磁盘
 *         最后调用虚拟磁盘的关闭磁盘的接口
 */
void exitsys(){
    int fd = 10;
    while (fd--)
    {
        if(openfilelist[fd].topenfile){
            my_close(fd);
        }
    }
    memcpy(&buffer, memfat, sizeof(block));
    saveblock(1);
    memcpy(&buffer, memfat + 512, sizeof(block));
    saveblock(2);
    close_vdisk();
}