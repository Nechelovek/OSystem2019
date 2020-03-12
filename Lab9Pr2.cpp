#include <sys/stat.h>   // For mode constants
#include <fcntl.h>      // For O_ constants

#include <cstdio>
#include <string.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <iostream>
#include <unistd.h>

int flag = 1;
struct mq_attr attr;
mqd_t des;

void* funcOne(void* pthreadData){
    char* buf = new char[256];
    while(flag == 1){
        memset(buf,0x00,sizeof(buf));
        mq_receive(des,buf,256,NULL);
        if(mq >= 0) {
		    perror("mq_recieve");
		else 
            std::cout << *buf << std::flush;
        sleep(1);
    }
}

int main(){
    pthread_t tide;

    attr.mq_msgsize = 256;
    attr.mq_maxmsg = 5;

    des = mq_open("/QueueMessage",O_CREAT,0644,&attr);

    pthread_create(&tide,NULL,funcOne,NULL);

    getchar();
    flag = 0;

    pthread_join(tide,NULL);

    mq_close(des);
    mq_unlink("/QueueMessage");
}
