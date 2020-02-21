/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 格式化磁盘
 *         程序基本流程与老师ppt的流程图一致
 */
void my_format() {
    /**
     * 在init_buff中初始化磁盘内容
	 * 初始化前五个磁盘块
	 * 设定第六个磁盘块为根目录磁盘块
	 * 初始化 root 目录： 创建 . 和 .. 目录
	 * init_buff内容按块遍历写入vdisk
	 */

    block init_buff[BLOCKNUM];

    block0 *boot = (block0 *)&(init_buff[0]); // 指向block0区的指针
    strcpy(boot->magic_number, "10101010");
    strcpy(boot->information, "Hello, there! This is the slim file system!\n\
Version:\t 0.1.0 (2019/12/21)\n\
Author:   \t zweigshao@gmail.com\n\
System Type:\t FAT\n\
BLOCKSIZE:\t 1024B\n\
BLOCKNUM:\t 1000\n\
RootBlockNum:\t 1\n");
    printf("%s", boot->information);
    fat *fat1 = (fat *)&(init_buff[1]);         // 指向fat1
    fat *fat2 = (fat *)&(init_buff[3]);         // 指向fat2
    int i;
    for (i = 0; i < 6; i++) {
        fat1[i] = END;
        fat2[i] = END;
    }
    for (i = 6; i < BLOCKNUM; i++) {
        fat1[i] = FREE;
        fat2[i] = FREE;
    }

    // 5th block is root
    fcb *root = (fcb *)&(init_buff[5]);         // 指向first root block
    fcb *dot = &(root[0]);                      // .
    fcb *ddot = &(root[1]);                     // ..
    strcpy(dot->filename, ".");
    strcpy(dot->truename, "root");
    dot->type = 0;

    time_t rawTime = time(NULL);
    struct tm *time = localtime(&rawTime);
    dot->time = time->tm_hour * 2048 + time->tm_min * 32 + time->tm_sec / 2;
    dot->date = (time->tm_year - 100) * 512 + (time->tm_mon + 1) * 32 + (time->tm_mday);
    dot->first = 5;
    dot->free = USED;           // not free
    dot->length = 2 * sizeof(fcb);
    strcpy(dot->dir, "");

    *ddot = *dot;
    strcpy(ddot->filename, "..");

    // 写入磁盘
    unsigned short ii = 0;
    for (ii; ii < BLOCKNUM; ii++){
        buffer = init_buff[ii];
        saveblock(ii);
    }
}