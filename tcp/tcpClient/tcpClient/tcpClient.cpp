// tcpClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
//#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, _TCHAR* argv[])
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
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
	//创建要连接的套接字库
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//向服务器发送请求
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	printf("连接成功\n");

	char recvBuf[100] = {0};
	recv(sockClient, recvBuf, 100, 0);
	printf("接收：%s \n", recvBuf);


	send(sockClient, "this is list", strlen("this is list") + 1, 0);
	printf("发送：this is list\n");
	
	closesocket(sockClient);
	system("pause");
	return 0;
}

