/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 模拟硬磁盘，向文件系统提供四个接口，模拟物理io过程
 */
int prepare_vdisk(){
    FILE *file;
    char buff[SIZE];
    if ((file = fopen("./vdisk/vdisk", "r")) != NULL)
    {
        fread(buff, SIZE, 1, file);
        fclose(file);
        if (memcmp(buff, "10101010", 8) == 0)
        {
            printf("SLIM File System prepare successfully\n");
            printf("%s", ((block0 *)buff)->information);
            memcpy(vdisk, buff, SIZE);
        } else
        {
            printf("invalid myfsys magic num, system exited!\0");
            exit(0);
        }
    }else{
        return -1;
    }
    return 0;
}

void close_vdisk(){
    FILE *fp = fopen("./vdisk/vdisk", "w");
    fwrite(vdisk, SIZE, 1, fp);
    fclose(fp);
}

void loadblock(unsigned short id){
    buffer = vdisk[id];
}

void saveblock(unsigned short id){
    vdisk[id] = buffer;
}