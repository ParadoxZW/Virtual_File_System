/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 write 已打开文件号(即fd) 读取字节数
 *         程序基本流程与老师ppt的流程图一致
 */
int my_read(int fd, int len){
    if (fd < 0 || fd >= MAXOPENFILE)
    {
        printf("no such file\n");
        return -1;
    }
    if (openfilelist[fd].topenfile == 0)
    {
        printf("my_write: file didn't open\n");
        return -1;
    }

    if(len <= 0){
        printf("len must be positve number\n");
        return -1;
    }

    openfilelist[fd].count = 0;  // 总是从头开始读
    char text[MAX_TEXT_SIZE] = "\0";
    int num;
    if((num = do_read(fd, len, text)) < 0){
        return -1;
    }
    printf("%s\nSuccessfully read %dB\n", text, num);
    return 1;
}