// tcpsrv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

//阻塞函数采用线程来保证程序运行
DWORD WINAPI connectsucess(LPVOID lpParameter); //监听是否连接成功
DWORD WINAPI recvmessage(LPVOID lpParameter);   //监听是否收到消息

CRITICAL_SECTION g_cs;
HANDLE hMutex = NULL;

int main(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	HANDLE hThread1;
	HANDLE hThread2;

	hThread1 = CreateThread(NULL, 0, connectsucess, NULL, 0, NULL);
	hThread2 = CreateThread(NULL, 0, recvmessage, NULL, 0, NULL);
	CloseHandle(hThread2);
	CloseHandle(hThread1);

	hMutex = CreateMutex(NULL, FALSE, NULL);

	wVersionRequested = MAKEWORD(1, 1); //因为基于Intel的CPU是低位现先存，有的机器是高位先存，为了保证数据的正确性，使用MAKEWORD函数，该函数的作用是获得wVersionRequested的正确值
	err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	//创建用于 监听的套接字，要注意成员字节的存放顺序，如果需要调换，则要用到htonl函数
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0); // socket套接字接受三个参数，对于TCP/IP协议的套接字第一个只能是AF_INET。第二个是指定socket类型，流式或者是数据报。如果socket调用成功就会返回一个新的socket套接字描述符

	SOCKADDR_IN addrSrv; //bind函数中第二个参数本来要传入的是socketaddr这个指针变量，但是这里用的是TCP/IP协议的套接字，可以用sockaddr_in代替
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 为什么不需要写入地址？
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	//绑定套接字
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		printf("等待连接\n");
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
	//	SOCKET sockConn = accept(sockSrv, NULL, NULL);
		if (sockConn == INVALID_SOCKET)
		{
			printf("acceot faild with error : %ld\n",WSAGetLastError());
			return -1;
		}
		printf("客户端连接\n");
		char sendBuf[100];
		sprintf(sendBuf, "Welcome %s to http://www.sunxin.org", inet_ntoa(addrClient.sin_addr));
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		printf("发送数据：%s\n", sendBuf);

		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);
		printf("接收数据：%s\n", recvBuf);

		printf("%S\n", recvBuf);	
		closesocket(sockConn);
	}
	closesocket(sockSrv);
	system("pause");
	return 1;
}

DWORD WINAPI connectsucess(LPVOID lpParameter)
{
	while (TRUE)
	{
		WaitForSingleObject(hMutex, INFINITE);
		sockSrv=

	}
}

