#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;
const int maxCount = 100;
class TBuffer
{
    public:
    TBuffer();
    ~TBuffer();
    void Write(int m);
    int Read();
    private:
    int in, out, curCount;
    int buf[maxCount];
    pthread_cond_t read;
    pthread_cond_t write;
    pthread_mutex_t mutex;
};
TBuffer::TBuffer() : in(0), out(0), curCount(0) 
{
    pthread_cond_init(&read, NULL);
    pthread_cond_init(&write, NULL);
    pthread_mutex_init(&mutex, NULL);
}
TBuffer::~TBuffer()
{
    pthread_cond_destroy(&write);
    pthread_cond_destroy(&read);
    pthread_mutex_destroy(&mutex);
}
void TBuffer::Write(int m)
{
    timespec timerBr;
    pthread_mutex_lock(&mutex);
    if(curCount == maxCount)
    pthread_cond_wait(&write, &mutex);
    curCount++;
    buf[in++] = m;
    in %= maxCount;
    pthread_cond_signal(&read);
    pthread_mutex_unlock(&mutex);
}
int TBuffer::Read()
{
    timespec timerBr;
    pthread_mutex_lock(&mutex);
    if(curCount == 0)
    pthread_cond_wait(&read, &mutex);
    curCount--;
    int m = buf[out++];
    out %= maxCount;
    pthread_cond_signal(&write);
    pthread_mutex_unlock(&mutex);
    return m;
}
struct pthr_struct
{
    bool status;
    TBuffer *buf;
};

void *thread_func_1(void *pthr_data)
{
    printf("%s", "First thread started\n");
    int number = 1;
    pthr_struct *data = (pthr_struct *)pthr_data;
    while(data->status)
    {
        cout << "To buffer: " << number << "\n" << flush;
        data->buf->Write(number);
        number++;
        sleep(1);
    }
    int *pthr_return = new int;
    *pthr_return = 0;
    printf("%s", "First thread finished\n");
    pthread_exit(pthr_return);
}

void *thread_func_2(void *pthr_data)
{
    printf("%s", "Second thread started\n");
    int number = 0;
    pthr_struct *data = (pthr_struct *)pthr_data;
    while((data->status) != 0)
    {
        number = data->buf->Read();
        cout << "From buffer: " << number << "\n" << flush;
       sleep(2);
    }
    int *pthr_return = new int;
    *pthr_return = 0;
    printf("%s", "Second thread finished\n");
    pthread_exit(pthr_return);
}

int main()
{
    printf("%s", "Main started\n");
    pthread_t thread_1, thread_2;
    pthr_struct pthr_data_first, pthr_data_second;
    TBuffer buf;
    pthr_data_first.status = true;
    pthr_data_second.status = true;
    pthr_data_first.buf = pthr_data_second.buf = &buf;
    pthread_create(&thread_1, NULL, &thread_func_1, &pthr_data_first);
    pthread_create(&thread_2, NULL, &thread_func_2, &pthr_data_second);
    getchar();
    pthr_data_first.status = false;
    pthr_data_second.status = false;
    int *pthr_return_1;
    int *pthr_return_2;
    pthread_join(thread_1, (void **)&pthr_return_1);
    pthread_join(thread_1, (void **)&pthr_return_2);
    printf("%s", "Main finished\n");
    delete pthr_return_1;
    delete pthr_return_2;
    return 0;
}