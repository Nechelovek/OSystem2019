#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants

#include <sys/mman.h>
#include <semaphore.h>
#include <iostream>
#include <unistd.h>

int flag = 1;       //Флаг завершения потока
sem_t *semWrite;    //Идентификатор семафора записи
sem_t *semRead;     //Идентификатор семафора чтения
char* addr;         


void* funcOne(void* DataFunc){
    char var;
    while(flag == 1){
        var = 'a';
        std::cout << var << std::endl << std::flush;
        *addr = var;
        sem_post(semRead);
        sem_wait(semWrite);
        std::cout << "SemWait" << std::flush;
        sleep(1);
    }
}

int main(){
    pthread_t tide; //Идентификатор потока

    int fd = shm_open("Memore",O_CREAT | O_RDWR,0777); // Создать или открыть
    ftruncate(fd,sizeof(char));
    addr = (char*)mmap(NULL,sizeof(char),PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);

    semWrite = sem_open("/SemWrite",O_CREAT,0777,0);    //Создание или открытие семафора записи
    semRead = sem_open("/SemRead",O_CREAT,0777,0;      //Создание или открытие семафора чтения

    pthread_create(&tide,NULL,funcOne,NULL);            //Создание потока из функции

    getchar();  //Ождание нажатия клавиши
    flag = 0;   //Изменение значение флага

    pthread_join(tide,NULL);    //Ождаем завершение потока

    sem_close(semWrite);        //Закрытие семафора записи
    sem_unlink("/SemWrite");    //Удаление семафора записи
    sem_close(semRead);         //Закрытие семафора чтения
    sem_unlink("/SemRead");     //Удаление семафора чтения

    munmap(addr,sizeof(char));
    close(fd);                  //Закрытие отображения разделяемой памяти на локальную 
    shm_unlink("/Memore");      //Удаление разделяемой памяти
}