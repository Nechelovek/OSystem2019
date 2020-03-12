#include <sys/stat.h>   //For mode constants
#include <fcntl.h>      //For O_ constnants
#include "fstream"
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



int flag = 1;

sem_t *sem_n;


std::ofstream output_file;

void* funcOne(void* arg){
    char value = '2';
    while(flag == 1){
        sem_wait(sem_n);        
        for(int i = 0; i < 5; i++){
            std::cout << "2" << std::flush;// << std::endl;    
            output_file << "2" << std::flush;
            sleep(1);
        }
        sem_post(sem_n);
        sleep(1);
    }

}


int main(){
    pthread_t tide;

    sem_n = sem_open("/empty_sem", O_CREAT, 0777, 1); 
    output_file.open("file.txt",std::ios_base::app);

    pthread_create(&tide,NULL,funcOne,NULL);
    
    getchar();
    flag = 0;

    pthread_join(tide,NULL);
    
    output_file.close();
    sem_unlink("/empty_sem");
}