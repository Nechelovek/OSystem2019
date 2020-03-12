//Client
#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants
#include <sys/types.h>  // See NOTES
//Стандартные библиотеки
#include <iostream>
#include<string.h>
#include <queue>
#include <unistd.h>
//Библиотеки для потоков
#include <sys/mman.h>
#include <pthread.h>
//Библиотека коетов и сервера
#include <sys/socket.h>
#include <netinet/in.h>

int socketClient; //Дескриптор сокета сервера   
struct sockaddr_in addr; //Структура хранящая адрес сокета

pthread_t tideConnect;

int flagConnection = 1;
/*
void* Connection(void* connectionData){
    int err;
    while(flagConnection){
        err = connect(socketClient,(sockaddr*)&addr,sizeof(addr));
        if(err < 0){
            perror("connect");
            sleep(1);
        }
        else{
            std::cout << "Successful connection client" << std::endl;
            break;
        }
    return 0;
    }
}
*/
int main(){

    socketClient = socket(AF_INET,SOCK_STREAM,0);//Создаем не именнованый сокет
    //Проверка на успешное открытие сокета
    if(socketClient < 0)
        perror("socket");
    //Задаем адрес сервера
    int port = 7000;
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = INADDR_ANY;
    /*
    pthread_create(&tideConnect,NULL,&Connection,NULL); //Создание потока подключения

    getchar();
    flagConnection = 0;

    pthread_join(tideConnect,NULL);


    close(socketClient);
*/
    
   int err = connect(socketClient,(sockaddr*)&addr,sizeof(addr));
        if(err < 0){
            perror("err");
            sleep(1);
        }

    char serverMessage[256];
    //time_t Realtime = time(NULL);
    struct tm *RealTime;// = localtime(&Realtime);
    err = recv(socketClient,RealTime,sizeof(serverMessage),0);
    //sleep(1);
    char clientMessage[256] = "Give me time";
    send(socketClient,&clientMessage,sizeof(clientMessage),0);
    std::cout << RealTime->tm_hour << std::endl;
    std::cout << RealTime->tm_min << std::endl;
    std::cout << RealTime->tm_sec << std::endl;
  
}