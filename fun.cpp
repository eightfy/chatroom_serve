#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <time.h>
#include "member.h"
using namespace std;

int list(int arg[10], int sClient, int flag);
char a[] = "SYSTEM";
char b[] = "Hello!";
const bag<char> weclome(a, b);

enum Flag
{
    OUT = 0,
    IN = 1
};

template<class T>
bag<T>::bag(T* n, T* m)
{
    int i = 0;
    for(i=0;i<sizeof(n);i++)
        name[i] = n[i];
    name[i] = 0;
    for (i = 0; i < sizeof(m); i++)
        message[i] = m[i];
    message[i] = 0;
    time_t timep;
    time(&timep);
    t = timep;
}

template<class T>
bag<T>::~bag()
{
	cout << "delete" << endl;
}
template<class T>
void bag<T>::send()
{
	cout << name << ": " << message << endl;
}

void* f(void* arg)
{
	int* uid = (int*)arg;
    int sClient = uid[0];
    send(sClient, &weclome, sizeof(weclome), MSG_NOSIGNAL);
    char temp[1112] = {0};//缓冲区
    while (1)
    {
        //客户端CloseSocket时,服务端recv()返回-1
        int re = recv(sClient, temp, 1112, 0);
        if (re == -1)
        {
            cout<< sClient <<"断开连接" << endl;
            break;
        }
        if (re != 1112)
        {
            //丢包
            cout << "大小不足" << endl;
            continue;
        }
        cout << sClient << "发来信息" << endl;

        for (int i = 1; i < 10; i++)
        {
            //if (uid[i] != sClient && uid[i] != 0) 
            if (uid[i] != 0)
            {
                int zj;
                zj = send(uid[i], temp, sizeof(temp), MSG_NOSIGNAL);
                cout << "转发" << uid[i] <<","<<zj<<"字节"<< endl;
            }
        }
        cout << "转发完毕" << endl;
        //memset(temp, 0, sizeof(temp));  
        //重置缓冲区
    }
    //关闭监听套接字
    list(uid, sClient, 0);
    cout << "线程关闭" << endl;
    close(sClient);
}

int list(int* arg, int sClient, int flag)
{
    if (flag) {
        int i; int j = 0;
        for (i = 1; i < 10; i++)
        {
            if (arg[i] == 0)
            {
                arg[i] = sClient;
                break;
            }
        }
        for (int ii = 1; ii < 10; ii++) 
            if(arg[ii] != 0) 
                j++;
        cout << "当前连接数" << j << endl;
        if (i == 10) {
            cout << "连接已满" << endl;
            return -1;
        }
        return 0;
    }
    else {
        int i;
        for (i = 1; i < 10; i++)
        {
            if (arg[i] == sClient)
            {
                arg[i] = 0;
            }
            break;
        }
        return 0;
    }
}