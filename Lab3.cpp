#include <iostream>
#include <pthread.h>
#include <unistd.h>

int flagOne = 1;
int flagTwo = 1;
int fd[2];

void* funcOne(void* pthreadDataFuncOne){

    size_t size = 256;
    char *buf = new char[size];

    while(flagOne == 1){
        std::cin.get(buf,size); // read string in buf
        write(fd[1],buf,size);
        sleep(1);
    }
    delete buf;
}

void* funcTwo(void* pthreadDataFuncTwo){

    size_t size = 256;
    char *buf = new char[size];

    while(flagTwo == 1){
    
        read(fd[0],buf,size);
        std::cout << buf << std::endl;
        
        sleep(1);
    }
    delete buf;
}

int main(){

    //char button;

    pthread_t tideOne;
    pthread_t tideTwo;

    pipe(fd);

    pthread_create(&tideOne,NULL,funcOne,NULL);
    pthread_create(&tideTwo,NULL,funcTwo,NULL);

    //std::cin >> button;
    getchar();
    flagOne = 0;
    flagTwo = 0;

    pthread_join(tideOne,NULL);
    pthread_join(tideTwo,NULL);

    close(fd[0]);
    close(fd[1]);
}