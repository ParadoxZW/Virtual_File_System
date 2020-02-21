/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 指令 write 已打开文件号(即fd)
 *         程序基本流程与老师ppt的流程图一致
 */
int my_write(int fd){
    if (fd < 0 || fd >= MAXOPENFILE)
    {
        printf("my_write: no such file\n");
        return -1;
    }
    if (openfilelist[fd].topenfile == 0)
    {
        printf("my_write: file didn't open\n");
        return -1;
    }
    int wstyle;
    while (1)
    {
        // 1: 截断写，清空全部内容，从头开始写
        // 2. 覆盖写，从文件指针处开始写
        // 3. 追加写，字面意思
        printf("1:Truncation  2:Coverage  3:Addition\n");
        scanf("%d", &wstyle);
        if (wstyle > 3 || wstyle < 1)
        {
            printf("input error\n");
        }
        else
        {
            break;
        }
    }
    if (wstyle == 2){ // 覆盖写 指定文件指针位置
        printf("input the position of file pointer\n");
        scanf("%d", &(openfilelist[fd].count));
        if (openfilelist[fd].count > getlen(fd) - 1) {
            printf("The file pointer has been out of file.\n");
            openfilelist[fd].count = 0;
            return -1;
        }
    }

    char text[MAX_TEXT_SIZE] = "\0";
    char texttmp[MAX_TEXT_SIZE] = "\0";
    printf("please input data, Enter twice to end file\n");
    getchar(); // 过滤回车
    int len, sum = 0;
    while (gets(texttmp))
    {
        if (strcmp(texttmp, "") == 0)
        {
            break;
        }
        len = strlen(texttmp);
        texttmp[len] = '\n';
        texttmp[len + 1] = '\0';
        strcat(text, texttmp);
        sum += len + 1;
    }

    text[sum] = '\0'; // maybe useless
    int num = do_write(fd, text, strlen(text) + 1, wstyle);
    printf("\nSuccessfully write %dB!\n", num);
    openfilelist[fd].fcbstate = 1;
    return 1;
}