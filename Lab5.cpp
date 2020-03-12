#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>    /* константы O_*/
#include <sys/stat.h>  /*константы для mode*/ 
#include <semaphore.h>
#include <iostream>

#define SEM_NAME "/named_sem"
int main(int argc, char *argv[])
{

    printf("%s", "Main started\n");
    pid_t pid;
    int rv;
    bool flag = true;
    char sem_name[] = "/name";
    sem_t *sem;
    sem = sem_open("empty_sem", O_CREAT, 0777, 1);
    if (sem == 0)
    {
        perror("sem_open");
    }

    switch (pid = fork())
    {
    case -1:
        perror("fork"); /* произошла ошибка*/ 
        exit(1);        /*выход из родительского процесса*/
    case 0:
        while (flag)
        {
            if(sem_wait(sem) != 0)
            {
                perror("sem_wait");
                flag = false;
                exit(1);
            }
            for (int i = 0; i < 5; i++)
            {
                printf(" CHILD: 1\n");
                sleep(1);
            }
            if(sem_post(sem) != 0)
            {
                perror("sem_post");
                flag = false;
                exit(1);
            }
            sleep(1);
        }
    default:
        while (flag)
        {
            if(sem_wait(sem) != 0)
            {
                perror("sem_wait");
                flag = false;
                exit(1);
            }
            for (int i = 0; i < 5; i++)
            {
                printf("PARENT: 2\n");
                sleep(1);
            }
            if(sem_post(sem) != 0)
            {
                perror("sem_post");
                flag = false;
                exit(1);
            }
            sleep(1);
        }
        wait(&rv);
        printf("PARENT: Код возврата потомка:%d\n", WEXITSTATUS(rv));
    }


    /**/


    sem_close(sem);

    printf("%s", "Main finished\n");
    return 0;
}