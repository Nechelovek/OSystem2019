//server
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

#include <time.h>
time_t Realtime = time(NULL);
struct tm *RealTime = localtime(&Realtime);

int flagConnect = 1;

pthread_t tideConnect;

int socketClient; //Дескриптор сокета клиента
int socketServer; //Дескриптор сокета сервера   
struct sockaddr_in addr; //Структура хранящая адрес сокета
struct sockaddr_in addrClient; //Структура хранящая адрес клиента
/*
void* Connection(void* connectionData){
    socklen_t size = (socklen_t)sizeof(addrClient);
    while(flagConnect){
        socketClient = accept(socketServer,(sockaddr*)&addrClient,&size);
        if(socketClient < 0)
            perror("accept");
        else{
            std::cout << "Successful connection server" << std::endl;
            break;
        }
    }
    return 0;
}
*/
int main(){

    socketServer = socket(AF_INET,SOCK_STREAM,0);//Создаем не именнованый сокет
    //Проверка на успешное открытие сокета
    if(socketServer < 0)
        perror("socket");
    //Задаем адрес сервера
    int port = 7000;
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = INADDR_ANY;
    //Привязка адреса сокету
    int err;
    err = bind(socketServer,(sockaddr*)&addr,sizeof(addr));
    //Проверка на успешность привязки адреса
    if(err < 0)
        perror("bind");

    err = listen(socketServer,10);
    //Проверка на открытый для прослушивания сокет
    if(err < 0)
        perror("listen");

    socklen_t size = (socklen_t)sizeof(addrClient);
    socketClient = accept(socketServer,(sockaddr*)&addrClient,&size);

    pthread_create(&tideConnect,NULL,&Connection,NULL);

    getchar();
    flagConnect = 0;

    pthread_join(tideConnect,NULL);
    
    char serverMessage[256] = "Hello Client";
    send(socketClient,RealTime,sizeof(serverMessage),0);

    char clientMessage[256];
    recv(socketClient,&clientMessage,sizeof(clientMessage),0);
    std::cout << clientMessage;
    
    shutdown(socketServer,SHUT_RDWR);
    close(socketServer);  //Закрыть сокет
    return 0;
}