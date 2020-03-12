#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants

#include <sys/mman.h>
#include <semaphore.h>
#include <iostream>
#include <unistd.h>

int flag = 1;
int fd;

void* funcOne(void* dataPthread){
    char *bufW;
    while(flag == 1){
        read(fd,bufW,256);
        std::cout << *bufW << std::endl;
        sleep(1);
    }
}

int main(){
    pthread_t tide;

    mkfifo("/NamePipe", O_RDWR);
    fd = open("/NamePipe", O_RDWR);


    pthread_create(&tide,NULL,funcOne,NULL);

    getchar();
    flag = 0;

    pthread_join(tide,NULL);

    close(fd);
    unlink("/NamePipe");
}
