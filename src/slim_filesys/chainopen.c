/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 根据路径串递归打开文件
 */
int chainopen(char *p[], int need, char lasttype, int *fd){
    // 依次打开文件
    int k = 0;
    int nfd = currfd;
    int type_, ret;
    while (need--){
        type_ = (need == 0) ? 0 : lasttype;
        ret = opensubfile(p[k++], nfd, type_);
        if (ret == 10) { // 错误处理
            *fd = nfd;
            return need + 1; // 几个文件没打开
        }
        nfd = ret;
    }
    *fd = nfd;
    return 0;
}