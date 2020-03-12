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
int mq;

void* funcOne(void* pthreadData){
    char *buf = new char[256];
    while(flag == 1){
        *buf = 'a';
        mq = mq_send(des,buf,256,1);
        if(mq >= 0) {
			std::cout << buf << std::flush;
		} else {
			perror("mq_send");
		}
        sleep(1);
    }
}

int main(){
    pthread_t tide;

    attr.mq_msgsize = 256;
    attr.mq_maxmsg = 5;

    des = mq_open("/QueueMessage",O_RDWR | O_CREAT,0664,&attr);
    if(des < 0){
        perror("mq_open");
    }

    mq_getattr(des,attr);
    std::cout << attr.mq_maxmsg << std::endl;

    pthread_create(&tide,NULL,funcOne,NULL);

    getchar();
    flag = 0;

    pthread_join(tide,NULL);

    mq_close(des);
    mq_unlink("/QueueMessage");
}
