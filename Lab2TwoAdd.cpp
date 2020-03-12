#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


int flagOne = 1;
int flagTwo = 1;

pthread_mutex_t Mutex;

pthread_t tideOne;
pthread_t tideTwo;

timespec tp;
clockid_t clock_realtime;

void* funcOne(void* pthreadDataFuncOne){

    int rc;
    //clock_gettime(clock_realtime, tp);
    while(flagOne == 1){

        while(1){
            if(flagOne == 0){
               // pthread_cancel(tideOne);
                return 0;
            }
            clock_gettime(clock_realtime, &tp);
            tp.tv_sec += 1;
            rc = pthread_mutex_timedlock(&Mutex, &tp);
            if(rc == 0)
                break;
        }
        std::cout << "Begin Function One " << std::endl;
        for(int i = 0; i < 10; i++){
            std::cout << "1";
        }
        std::cout << "End Function One " << std::endl;
        pthread_mutex_unlock(&Mutex);
        sleep(1);
    }
    

}

void* funcTwo(void* pthreadDataFuncTwo){
    
    int rc;

    while(flagTwo == 1){
        
        while(1){
        
            if(flagTwo == 0){
                //pthread_cancel(tideTwo);
                return 0;
            }
            clock_gettime(clock_realtime, &tp);
            tp.tv_sec += 1;
            rc = pthread_mutex_timedlock(&Mutex, &tp);
            if(rc == 0)
                break;
        }

        std::cout << "Begin Function Two " << std::endl;
        for(int i = 0; i < 10; i++){
            std::cout << "2";
        }
        std::cout << "End Function Two " << std::endl;
        pthread_mutex_unlock(&Mutex);
        sleep(1);
    }
    
}

int main(){

    char button;

    std::cout << "Begin Main() "<< std::endl;
    
    pthread_mutex_init(&Mutex,NULL);

    pthread_create(&tideOne,NULL,funcOne,NULL);
    pthread_create(&tideTwo,NULL,funcTwo,NULL);


    std::cout << "Press button " << std::endl;
    std::cin >> button;

    flagOne = 0;
    flagTwo = 0;

    pthread_join(tideOne,NULL);
    pthread_join(tideTwo,NULL);

    pthread_mutex_destroy(&Mutex);
    std::cout << "End Main() " << std::endl;
}