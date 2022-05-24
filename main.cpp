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
#include <pthread.h>
#include "member.h"
using namespace std;

#define PORT 8888
#define QUEUE 20
char ip[] = "101.43.84.223";
enum Flag
{
    OUT = 0,
    IN = 1
};

void* f(void* arg);
int list(int* arg, int sClient, int flag);//1写入，0删除

int main(int argc, char* argv[])
{
    re:
    int m = 0;
    //创建套接字  
    int slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == -1)
    {
        printf("socket error !");
        exit(-1);
    }

    //绑定IP和端口  
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(slisten, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
        printf("bind error !");
        bool bReuseaddr = 1;
        setsockopt(slisten, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(bool));
        exit(-1);
    }

    //开始监听  
    if (listen(slisten, 5) == -1)
    {
        printf("listen error !");
        exit(-1);
    }

    //循环接收数据  
    int sClient;
    sockaddr_in remoteAddr;
    socklen_t nAddrlen = sizeof(remoteAddr);
    int arg[10] = {0}; 
    int uid = 1;
    pthread_t id = 1000;
    while (true)
    {
        cout<<"等待连接...";
        cout << endl;
        sClient = accept(slisten, (struct sockaddr*)&remoteAddr, &nAddrlen);
        if (sClient == -1)
        {
            printf("accept error !");
            cout << endl;
            continue;
        }
        cout << "接受到一个连接：";
        cout << inet_ntoa(remoteAddr.sin_addr);
        cout << "-----" << sClient;
        cout << " \r\n";
        arg[0] = sClient;
        if(list(arg, sClient, IN)== -1) 
            continue;
        for (int ii = 1; ii < 10; ii++) 
            cout << arg[ii] << "--" ;
        cout << endl;
        pthread_create(&id, NULL, &f, arg);
        id++;
        //接收数据
        //char temp[255];
        //int ret = recv(member[mag], temp, sizeof(temp), 0);
    //if (ret > 0)
    //{
    //    //revData[ret] = 0x00;
    //    //printf(revData);
    //    //char sendData[255] = {0};
    //    //发送数据  
    //    send(sClient, temp, strlen(temp), 0);
    //    //close(sClient);
    }
    close(slisten);
    return 0;
}