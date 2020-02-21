/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 回溯关闭打开文件
 */
void chainclose(int sfd, int tfd){
    // traceback style
    int nfd = sfd;
    while (nfd != tfd) {
        if(openfilelist[nfd].fcbstate){
            int i = 0;
            unsigned short id = getfirst(nfd);
            while(id != END){
                if (openfilelist[nfd].mem[i++] != NULL){
                    buffer = *(openfilelist[nfd].mem[i - 1]);
                    saveblock(id);
                    free(openfilelist[nfd].mem[i - 1]);
                }
                id = memfat[id];
            }
        }
        memset(&openfilelist[nfd], 0, sizeof(useropen));
        nfd = openmanage[nfd];
        left++;
    }
}
