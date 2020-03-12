#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* funcOne(void* pthreadDataFuncOne){

    int* flagOne = (int*)pthreadDataFuncOne;
    
    int exit = 1;
    int* ptrExit = &exit;

    while(*flagOne == 1){ 
        std::cout << " 1 " << std::endl;
        sleep(1);
    }
    pthread_exit(&ptrExit);
}

void* funcTwo(void* pthreadDataFuncTwo){
    
    int* flagTwo = (int*)pthreadDataFuncTwo;

    int exit = 2;
    int* ptrExit = &exit;

    while(*flagTwo == 1){
        std::cout << " 2 " << std::endl;
        sleep(1);
    }
    pthread_exit(&ptrExit);
}

int main(){
    char button;

    int flagOne = 1;
    int flagTwo = 1;

    int* pthreadDataFuncOne = NULL;
    int* pthreadDataFuncTwo = NULL;

    pthreadDataFuncOne = &flagOne;
    pthreadDataFuncTwo = &flagTwo;

    pthread_t tideOne;
    pthread_t tideTwo;

    pthread_create(&tideOne,NULL,funcOne,(void*) pthreadDataFuncOne);
    pthread_create(&tideOne,NULL,funcTwo,(void*) pthreadDataFuncTwo);

    std::cin >> button;

    int **exitTideOne;
    int **exitTideTwo;

    flagOne = 0;
    flagTwo = 0;

    pthread_join(tideOne,(void**)&exitTideOne);
    pthread_join(tideTwo,(void**)&exitTideTwo);
    
    std::cout << "Exit data tide one: " << (int*) *exitTideOne << std::endl;
    std::cout << "Exit data tide two: " << (int*) *exitTideOne << std::endl;
}
