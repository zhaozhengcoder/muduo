#include "MyMutexLock.h"

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;


MyMutexLock mutex;

class Stu
{
private:
	int * id;
public:
	Stu()
	{
		id = new int;
		*id = 100;
	}

	~Stu()
	{
		cout<<"~Stu()"<<endl;
		delete id;
	}
	int get_id()
	{
		return *id;
	}
};


//线程1执行的函数
void* th1(void * arg)
{
    {
        MyMutexGuard lock(mutex);
        for(int i=0;i<5;i++)
        {
            sleep(1);
            cout<<"th1"<<endl;
        }
    }
}


//线程2执行的函数
void* th2(void * arg)
{
    {
        MyMutexGuard lock(mutex);
        for(int i=0;i<5;i++)
        {
            sleep(1);
            cout<<"th2"<<endl;
        }
    }
}



int main(){
    pthread_t p1,p2; //创建线程id

    Stu s1;
    Stu * ps =&s1;

    //创建线程p1 去执行 th1函数 (不带参数)
    if(pthread_create(&p1,NULL,th1,(void *) ps)!=0){
        cout<<"thread create error"<<endl;
    }

    //创建线程p2 去执行 th2函数 (带参数)
    if(pthread_create(&p2,NULL,th2,(void *) ps)!=0){
         cout<<"thread create error"<<endl;
    }

    //调用pthread_join 函数的时候，主线程会阻塞在这个地方
    //等待对应的子线程结束，并且回收资源。
    pthread_join(p1,NULL); 
    pthread_join(p2,NULL);

    return 0;
}
