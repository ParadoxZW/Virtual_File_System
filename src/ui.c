/**
 * @author: Zhenwei_Shao
 * @date: 2019/12/27
 * @brief: 模拟终端界面，使用方式类似Linux Bash.
 */
#include "slim.h"

int main(void)
{
    char cmd[11][10] = {
        "mkdir", "rmdir", "ls", "cd", "create",
        "rm", "open", "close", "write", "read",
        "exit"};
    char command[30], *sp, *sp1, *sp2;
    strcpy(cwd, "/");
    int cmd_idx, i;
    memset(openfilelist, 0, sizeof(useropen));

    printf("************* SLIM File System ***************\n");
    startsys();
    printf("************* SLIM File System ***************\n");

    while (1)
    {
        if(strcmp(cwd, "") == 0|| strcmp(cwd, " ") == 0)
            strcpy(cwd, "/");
        printf("%s> ", cwd);
        gets(command);
        cmd_idx = -1;
        if (strcmp(command, "") == 0)
        {
            printf("\n");
            continue;
        }
        sp = strtok(command, " ");
        for (i = 0; i < 13; i++)
        {
            if (strcmp(sp, cmd[i]) == 0)
            {
                cmd_idx = i;
                break;
            }
        }
        switch (cmd_idx)
        {
        case 0: // mkdir
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_mkdir(sp);
            else
                printf("argument miss\n");
            break;
        case 1: // rmdir
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_rmdir(sp);
            else
                printf("argument miss\n");
            break;
        case 2: // ls
            my_ls();
            break;
        case 3: // cd
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_cd(sp);
            else
                printf("argument miss\n");
            break;
        case 4: // create
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_create(sp);
            else
                printf("argument miss\n");
            break;
        case 5: // rm
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_rm(sp);
            else
                printf("argument miss\n");
            break;
        case 6: // open
            sp = strtok(NULL, " ");
            if (sp != NULL)
                my_open(sp);
            else
                printf("argument miss\n");
            break;
        case 7: // close
            sp = strtok(NULL, " ");
            if (sp != NULL){
                int fd;
                sscanf(sp, "%d", &fd);
                my_close(fd);
            }
            else
                printf("argument miss\n");
            break;
        case 8: // write
            sp = strtok(NULL, " ");
            if (sp != NULL){
                int fd;
                sscanf(sp, "%d", &fd);
                my_write(fd);
            }
            else
                printf("argument miss\n");
            break;
        case 9: // read
            sp1 = strtok(NULL, " ");
            sp2 = strtok(NULL, " ");
            if (sp1 != NULL && sp2 != NULL){
                int fd, len;
                sscanf(sp1, "%d", &fd);
                sscanf(sp2, "%d", &len);
                my_read(fd, len);
            }
            else
                printf("argument miss\n");
            break;
        case 10: // exit
            exitsys();
            printf("************** Exit SLIM System ****************\n");
            return 0;
            break;
        default:
            printf("wrong command: %s\n", command);
            break;
        }
    }
    return 0;
}